#ifndef UGPIO_H
#define UGPIO_H


#ifdef __cplusplus
extern "C" {
#endif

void gpio_export(int gpio);
void gpio_unexport(int gpio);
void gpio_set_dir(int gpio, const char *dir);
int gpio_Read(int gpio);
void gpio_Write(int gpio, int value);

#ifdef __cplusplus
}
#endif

#endif
