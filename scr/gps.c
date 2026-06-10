#include "gps.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

static FILE *gps_fp = NULL;
static char latitude[32];
static char longitude[32];

int GPS_Init()
{
    int fd;
    struct termios tty;

    fd = open("/dev/ttymxc0",
          O_RDONLY | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    tcgetattr(fd, &tty);

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;


    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 10;   // 1 second timeout

    tcsetattr(fd, TCSANOW, &tty);

    gps_fp = fdopen(fd, "r");

    if (!gps_fp)
    {
        close(fd);
        return -1;
    }

    return 0;
}

// static void GPS_Update(void)
// {
//     char line[256];

//     if (gps_fp == NULL)
//         return;

//     if (fgets(line, sizeof(line), gps_fp) == NULL)
//         return;

//     if (strncmp(line, "$GNGGA", 6) != 0)
//         return;

//     while (fgets(line, sizeof(line), gps_fp))
//     {
//         if (strncmp(line, "$GNGGA", 6) == 0)
//         {
//             char *token;
//             int field = 0;

//             char lat[20] = "";
//             char ns[4] = "";
//             char lon[20] = "";
//             char ew[4] = "";

//             token = strtok(line, ",");

//             while (token)
//             {
//                 switch (field)
//                 {
//                     case 2:
//                         strcpy(lat, token);
//                         break;

//                     case 3:
//                         strcpy(ns, token);
//                         break;

//                     case 4:
//                         strcpy(lon, token);
//                         break;

//                     case 5:
//                         strcpy(ew, token);
//                         break;
//                 }

//                 token = strtok(NULL, ",");
//                 field++;
//             }

//             snprintf(latitude, sizeof(latitude), "%s %s", lat, ns);
//             snprintf(longitude, sizeof(longitude), "%s %s", lon, ew);

//             return;
//         }
//     }
// }
static void GPS_Update(void)
{
    char line[256];

    if (gps_fp == NULL)
        return;

    if (fgets(line, sizeof(line), gps_fp) == NULL)
        return;

    if (strncmp(line, "$GNGGA", 6) != 0)
        return;

    char *token;
    int field = 0;

    char lat[20] = "";
    char ns[4] = "";
    char lon[20] = "";
    char ew[4] = "";

    token = strtok(line, ",");

    while (token)
    {
        switch (field)
        {
            case 2: strcpy(lat, token); break;
            case 3: strcpy(ns, token); break;
            case 4: strcpy(lon, token); break;
            case 5: strcpy(ew, token); break;
        }

        token = strtok(NULL, ",");
        field++;
    }

    snprintf(latitude, sizeof(latitude), "%s %s", lat, ns);
    snprintf(longitude, sizeof(longitude), "%s %s", lon, ew);
}

char *GPS_GetLatitude(void)
{
    if (gps_fp == NULL)
        return "GPS OFF";

    GPS_Update();

    if (strlen(latitude) == 0)
        return "NO FIX";

    return latitude;
}

char *GPS_GetLongitude(void)
{
    return longitude;
}
