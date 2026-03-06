#include <stdio.h>
#include <stdlib.h>
#include "Battery.h"


#define IIO_PATH "/sys/bus/iio/devices/iio:device0"
#define RAW_FILE IIO_PATH "/in_voltage2_raw"     // ADC_2 → SODIMM4
#define SCALE_FILE IIO_PATH "/in_voltage2_scale" // scale for ADC_2

float Battery(void)
{
    FILE *f;
    int raw_value;

    // read raw ADC value
    f = fopen(RAW_FILE, "r");
    if (!f) { perror("open raw"); return 1; }
    if (fscanf(f, "%d", &raw_value) != 1) { fclose(f); fprintf(stderr,"read raw failed\n"); return 1; }
    fclose(f);

    // Convert raw to volts
    double Conversion_volts = raw_value;
    float Battery_value = 0;


    Battery_value = ((Conversion_volts - min_level)/(max_level - min_level)) * 100;

   // printf("Battery percentage :%f\n",Battery_value);

    return Battery_value;
}


