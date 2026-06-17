#ifndef GPS_H
#define GPS_H
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
#define uart_dev "/dev/ttymxc0"

int GPS_Init(void);
char *GPS_GetLatitude(void);
char *GPS_GetLongitude(void);
static void nmeaToDMS(const char *nmea,
               char hemisphere,
               char *output,
               size_t size);
#ifdef __cplusplus
}
#endif

#endif
