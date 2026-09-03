#define _GNU_SOURCE
#include "gps.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

static int gps_fd = -1;
static pthread_t gps_thread;
static pthread_mutex_t gps_mutex = PTHREAD_MUTEX_INITIALIZER;
static volatile int gps_running = 0;
static int gps_thread_started = 0;

/* Cached display strings. They are changed only while gps_mutex is locked. */
static char latitude[32] = "Waiting...";
static char longitude[32] = "Waiting...";
static int gps_has_fix = 0;

/*
 * The L89HA-S90 is normally 9600 baud by default.
 *
 * Keep the reader at the currently configured module baud rate. If you
 * configure the module itself for 115200/10 Hz, change this to B115200 too.
 */
#define GPS_BAUD B9600

#define RX_BUFFER_SIZE 4096
#define NMEA_LINE_SIZE 512

static void nmeaToDMS(const char *nmea,
                      char hemisphere,
                      char *output,
                      size_t size)
{
    if (nmea == NULL || *nmea == '\0' || size == 0)
    {
        if (size > 0)
            snprintf(output, size, "Waiting...");
        return;
    }

    char *end = NULL;
    double val = strtod(nmea, &end);

    if (end == nmea || !isfinite(val))
    {
        snprintf(output, size, "Waiting...");
        return;
    }

    int degrees = (int)(val / 100.0);
    double minutesFull = val - (degrees * 100.0);

    if (minutesFull < 0.0 || minutesFull >= 60.0)
    {
        snprintf(output, size, "Waiting...");
        return;
    }

    int minutes = (int)minutesFull;
    double seconds = (minutesFull - minutes) * 60.0;

    snprintf(output,
             size,
             "%d° %d' %.2f\" %c",
             degrees,
             minutes,
             seconds,
             hemisphere);
}

/*
 * Validate NMEA checksum.
 *
 * Example:
 *   $GNGGA,...*hh
 *
 * Returns 1 for a valid checksum, 0 for invalid/missing checksum.
 */
static int nmea_checksum_ok(const char *line)
{
    if (line == NULL || line[0] != '$')
        return 0;

    const char *star = strchr(line, '*');
    if (star == NULL || star[1] == '\0' || star[2] == '\0')
        return 0;

    unsigned char checksum = 0;

    for (const char *p = line + 1; p < star; ++p)
        checksum ^= (unsigned char)*p;

    char hex[3] = { star[1], star[2], '\0' };
    char *end = NULL;
    long received = strtol(hex, &end, 16);

    if (end != hex + 2)
        return 0;

    return checksum == (unsigned char)received;
}

static int is_sentence(const char *line, const char *type)
{
    /*
     * Accept:
     *   $GNGGA
     *   $GPGGA
     *   $GLGGA
     *   $GAGGA
     * etc.
     *
     * This avoids depending on one particular talker ID.
     */
    if (line == NULL || line[0] != '$')
        return 0;

    size_t len = strlen(line);

    if (len < 7)
        return 0;

    return line[3] == type[0] &&
           line[4] == type[1] &&
           line[5] == type[2];
}

/*
 * Split a comma-separated NMEA sentence.
 *
 * strtok() is intentionally used on a private local copy of the sentence,
 * so it is safe with the GPS reader thread.
 */
static int parse_gga(char *line)
{
    char *fields[20] = {0};
    int count = 0;

    char *token = strtok(line, ",");

    while (token != NULL && count < 20)
    {
        fields[count++] = token;
        token = strtok(NULL, ",");
    }

    /*
     * GGA:
     * 0 $GNGGA
     * 1 UTC
     * 2 latitude
     * 3 N/S
     * 4 longitude
     * 5 E/W
     * 6 fix quality
     * 7 satellites
     * ...
     */
    if (count < 7)
        return 0;

    if (fields[2][0] == '\0' ||
        fields[3][0] == '\0' ||
        fields[4][0] == '\0' ||
        fields[5][0] == '\0')
    {
        return 0;
    }

    int fix_quality = atoi(fields[6]);

    /*
     * 0 = invalid
     * >0 = valid GNSS fix
     */
    if (fix_quality <= 0)
        return 0;

    char lat[32];
    char lon[32];

    nmeaToDMS(fields[2], fields[3][0], lat, sizeof(lat));
    nmeaToDMS(fields[4], fields[5][0], lon, sizeof(lon));

    if (strcmp(lat, "Waiting...") == 0 ||
        strcmp(lon, "Waiting...") == 0)
    {
        return 0;
    }

    pthread_mutex_lock(&gps_mutex);

    snprintf(latitude, sizeof(latitude), "%s", lat);
    snprintf(longitude, sizeof(longitude), "%s", lon);
    gps_has_fix = 1;

    pthread_mutex_unlock(&gps_mutex);

    return 1;
}

static int parse_rmc(char *line)
{
    char *fields[20] = {0};
    int count = 0;

    char *token = strtok(line, ",");

    while (token != NULL && count < 20)
    {
        fields[count++] = token;
        token = strtok(NULL, ",");
    }

    /*
     * RMC:
     * 0 $GNRMC
     * 1 UTC
     * 2 status (A = valid, V = invalid)
     * 3 latitude
     * 4 N/S
     * 5 longitude
     * 6 E/W
     * ...
     */
    if (count < 7)
        return 0;

    if (fields[2][0] != 'A')
        return 0;

    if (fields[3][0] == '\0' ||
        fields[4][0] == '\0' ||
        fields[5][0] == '\0' ||
        fields[6][0] == '\0')
    {
        return 0;
    }

    char lat[32];
    char lon[32];

    nmeaToDMS(fields[3], fields[4][0], lat, sizeof(lat));
    nmeaToDMS(fields[5], fields[6][0], lon, sizeof(lon));

    if (strcmp(lat, "Waiting...") == 0 ||
        strcmp(lon, "Waiting...") == 0)
    {
        return 0;
    }

    pthread_mutex_lock(&gps_mutex);

    /*
     * RMC is also a valid position source. This means we don't have to
     * wait for a GGA sentence if a valid RMC arrives first.
     */
    snprintf(latitude, sizeof(latitude), "%s", lat);
    snprintf(longitude, sizeof(longitude), "%s", lon);
    gps_has_fix = 1;

    pthread_mutex_unlock(&gps_mutex);

    return 1;
}

static void process_nmea_line(const char *line)
{
    if (line == NULL || line[0] != '$')
        return;

    /*
     * Ignore corrupted/incomplete NMEA data. This is important on UART
     * because a partial line must never overwrite a good coordinate.
     */
    if (!nmea_checksum_ok(line))
        return;

    char local[NMEA_LINE_SIZE];

    snprintf(local, sizeof(local), "%s", line);

    if (is_sentence(local, "GGA"))
    {
        parse_gga(local);
        return;
    }

    snprintf(local, sizeof(local), "%s", line);

    if (is_sentence(local, "RMC"))
    {
        parse_rmc(local);
        return;
    }
}

static void process_rx_buffer(char *rx_buffer, size_t *rx_len)
{
    while (1)
    {
        char *newline = memchr(rx_buffer, '\n', *rx_len);

        if (newline == NULL)
            break;

        size_t line_len = (size_t)(newline - rx_buffer);

        if (line_len > 0 && rx_buffer[line_len - 1] == '\r')
            line_len--;

        if (line_len > 0)
        {
            char line[NMEA_LINE_SIZE];

            size_t copy_len = line_len;

            if (copy_len >= sizeof(line))
                copy_len = sizeof(line) - 1;

            memcpy(line, rx_buffer, copy_len);
            line[copy_len] = '\0';

            process_nmea_line(line);
        }

        size_t consumed = (size_t)(newline - rx_buffer) + 1;

        memmove(rx_buffer,
                rx_buffer + consumed,
                *rx_len - consumed);

        *rx_len -= consumed;
    }

    /*
     * If garbage/overflow accumulates without a newline, recover by
     * keeping only the newest possible sentence beginning.
     */
    if (*rx_len >= RX_BUFFER_SIZE - 1)
    {
        char *dollar = memrchr(rx_buffer, '$', *rx_len);

        if (dollar != NULL)
        {
            size_t remaining = *rx_len - (size_t)(dollar - rx_buffer);

            memmove(rx_buffer, dollar, remaining);
            *rx_len = remaining;
        }
        else
        {
            *rx_len = 0;
        }
    }
}

static void *gps_reader_thread(void *arg)
{
    (void)arg;

    char rx_buffer[RX_BUFFER_SIZE];
    size_t rx_len = 0;

    while (gps_running)
    {
        char temp[1024];

        ssize_t bytes = read(gps_fd, temp, sizeof(temp));

        if (bytes > 0)
        {
            /*
             * Append everything received. A single read may contain:
             *   - part of one NMEA sentence
             *   - one complete sentence
             *   - many complete sentences
             */
            size_t available = sizeof(rx_buffer) - rx_len;

            if ((size_t)bytes > available)
            {
                /*
                 * Preserve the newest data rather than allowing the
                 * receive buffer to overflow. read() is limited to 1024
                 * bytes here, so bytes can never be larger than RX_BUFFER_SIZE.
                 */
                size_t drop = (size_t)bytes - available;

                if (drop > rx_len)
                    drop = rx_len;

                memmove(rx_buffer,
                        rx_buffer + drop,
                        rx_len - drop);

                rx_len -= drop;

                memcpy(rx_buffer + rx_len,
                       temp,
                       (size_t)bytes);

                rx_len += (size_t)bytes;
            }
            else
            {
                memcpy(rx_buffer + rx_len, temp, (size_t)bytes);
                rx_len += (size_t)bytes;
            }

            process_rx_buffer(rx_buffer, &rx_len);
        }
        else if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("GPS UART read");
            break;
        }
        else
        {
            /*
             * Avoid busy-looping while there is no UART data.
             * This does not delay processing when data arrives because
             * the sleep is only 1 ms.
             */
            struct timespec ts = {0, 1000000L};
            nanosleep(&ts, NULL);
        }
    }

    return NULL;
}

int GPS_Init(void)
{
    if (gps_thread_started)
        return 0;

    struct termios tty;

    gps_fd = open(uart_dev, O_RDONLY | O_NOCTTY | O_NONBLOCK);

    if (gps_fd < 0)
    {
        perror("GPS: open");
        return -1;
    }

    if (tcgetattr(gps_fd, &tty) != 0)
    {
        perror("GPS: tcgetattr");
        close(gps_fd);
        gps_fd = -1;
        return -1;
    }

    /*
     * Put UART into raw mode. We do not use stdio/fgets() because GPS
     * data is a continuous byte stream and a read can contain arbitrary
     * numbers of NMEA sentences.
     */
    cfmakeraw(&tty);

    cfsetispeed(&tty, GPS_BAUD);
    cfsetospeed(&tty, GPS_BAUD);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    /*
     * Non-blocking read. We process whatever bytes are immediately
     * available and keep partial sentences in our own buffer.
     */
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(gps_fd, TCSANOW, &tty) != 0)
    {
        perror("GPS: tcsetattr");
        close(gps_fd);
        gps_fd = -1;
        return -1;
    }

    tcflush(gps_fd, TCIFLUSH);

    pthread_mutex_lock(&gps_mutex);

    snprintf(latitude, sizeof(latitude), "Waiting...");
    snprintf(longitude, sizeof(longitude), "Waiting...");
    gps_has_fix = 0;

    pthread_mutex_unlock(&gps_mutex);

    gps_running = 1;

    if (pthread_create(&gps_thread, NULL, gps_reader_thread, NULL) != 0)
    {
        perror("GPS: pthread_create");
        gps_running = 0;
        close(gps_fd);
        gps_fd = -1;
        return -1;
    }

    gps_thread_started = 1;

    return 0;
}

char *GPS_GetLatitude(void)
{
    /*
     * Thread-local buffer prevents the GPS reader thread from modifying
     * the same string while the GUI is using the returned pointer.
     */
    static __thread char result[32];

    pthread_mutex_lock(&gps_mutex);

    if (gps_fd < 0)
        snprintf(result, sizeof(result), "GPS OFF");
    else
        snprintf(result, sizeof(result), "%s", latitude);

    pthread_mutex_unlock(&gps_mutex);

    return result;
}

char *GPS_GetLongitude(void)
{
    static __thread char result[32];

    pthread_mutex_lock(&gps_mutex);

    if (gps_fd < 0)
        snprintf(result, sizeof(result), "GPS OFF");
    else
        snprintf(result, sizeof(result), "%s", longitude);

    pthread_mutex_unlock(&gps_mutex);

    return result;
}

int GPS_HasFix(void)
{
    int result;

    pthread_mutex_lock(&gps_mutex);
    result = gps_has_fix;
    pthread_mutex_unlock(&gps_mutex);

    return result;
}

void GPS_Close(void)
{
    if (!gps_thread_started)
        return;

    gps_running = 0;

    pthread_join(gps_thread, NULL);
    gps_thread_started = 0;

    if (gps_fd >= 0)
    {
        close(gps_fd);
        gps_fd = -1;
    }
}
