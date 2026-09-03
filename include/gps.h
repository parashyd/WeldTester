#ifndef GPS_H
#define GPS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define uart_dev "/dev/ttymxc0"

/*
 * Starts a background GPS reader.
 *
 * The reader continuously consumes the UART and keeps the latest valid
 * position in memory. The getter functions only return the latest cached
 * value; they do not block waiting for the UART.
 */
int GPS_Init(void);

/* Return the latest formatted coordinate.
 * Examples:
 *   "12° 34' 56.78\" N"
 *   "Waiting..."
 *   "GPS OFF"
 */
char *GPS_GetLatitude(void);
char *GPS_GetLongitude(void);

/* Returns 1 when at least one valid position has been received, 0 otherwise. */
int GPS_HasFix(void);

/* Stop the background reader and close the UART. */
void GPS_Close(void);

#ifdef __cplusplus
}
#endif

#endif
