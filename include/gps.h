#ifndef GPS_H
#define GPS_H

#ifdef __cplusplus
extern "C" {
#endif
#define uart_dev "/dev/ttymxc0"

int GPS_Init(void);
char *GPS_GetLatitude(void);
char *GPS_GetLongitude(void);

#ifdef __cplusplus
}
#endif

#endif
