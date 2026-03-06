#include <stdio.h>
#include <stdint.h>
#include "Ugpio.h"

void gpio_export(int gpio)
{
    FILE *fp = fopen("/sys/class/gpio/export", "w");
    if (fp)
    {
        fprintf(fp, "%d", gpio);
        fclose(fp);
    }
}
// Function to unexport a GPIO pin
void gpio_unexport(int gpio)
{
    FILE *fp = fopen("/sys/class/gpio/unexport", "w");
    if (fp)
    {
        fprintf(fp, "%d", gpio);
        fclose(fp);
    }
}
// Function to set GPIO direction
void gpio_set_dir(int gpio, const char *dir)
{
    char path[35];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", gpio);
    FILE *fp = fopen(path, "w");
    if (fp)
    {
        fprintf(fp, "%s", dir);
        fclose(fp);
    }
}

int gpio_Read(int gpio)
{
    char path[35];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    FILE *fp = fopen(path, "r");
    int value = -1;
    if (fp)
    {
        fscanf(fp, "%d", &value);
        fclose(fp);
    }
    return value;
}

void gpio_Write(int gpio, int value)
{
    char path[35];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    FILE *fp = fopen(path, "w");
    if (fp)
    {
        fprintf(fp, "%d", value);
        fclose(fp);
    }
}

