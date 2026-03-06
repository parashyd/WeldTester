#include "Ugpio.h"
#include <unistd.h>  // For usleep()
#include "DataFile.h"
#include <QDebug>


//void OperateGain(int g_val,int ch_no);
void delay_loop_1ms();

#define ADDRESS_LINES_SIZE 1
#define WRITE_ENABLE 1
#define DATA_LINES_SIZE 8

int address_line[ADDRESS_LINES_SIZE] = {72};
int Write_Enable[WRITE_ENABLE] = {128};
int Gain_data_lines[DATA_LINES_SIZE] = {71,84,70,132,65,64,89,86};

extern ConfigEntry entry;



void gain_init()
{
  gpio_export(address_line[0]);
  gpio_set_dir(address_line[0], "out");
  
  gpio_export(Write_Enable[0]);
  gpio_set_dir(Write_Enable[0], "out");

  for (int i = 0; i < DATA_LINES_SIZE; i++)
  {
      gpio_export(Gain_data_lines[i]);
      gpio_set_dir(Gain_data_lines[i], "out");
  }
  
  gpio_Write(Write_Enable[0],1);

}


void OperateGainDual(int gain_ch1, int gain_ch2)
{
    // gain_init();                        //Initilize GPIO's
    int bit;

    // ---- CH1 ----
    if (gain_ch1 < 0)   gain_ch1 = 0;
    if (gain_ch1 > 255) gain_ch1 = 255;

    gpio_Write(address_line[0], 0);         // CH1

    for (int i = 0; i < 8; i++)
    {
        bit = (gain_ch1 >> i) & 1;
        gpio_Write(Gain_data_lines[i], bit);
    }

    gpio_Write(Write_Enable[0], 0);
    usleep(1);
    gpio_Write(Write_Enable[0], 1);

    usleep(5); // small separation

    // ---- CH2 ----
    if (gain_ch2 < 0)   gain_ch2 = 0;
    if (gain_ch2 > 255) gain_ch2 = 255;

    gpio_Write(address_line[0], 1);             // CH2

    for (int i = 0; i < 8; i++)
    {
        bit = (gain_ch2 >> i) & 1;
        gpio_Write(Gain_data_lines[i], bit);
    }

    gpio_Write(Write_Enable[0], 0);
    usleep(1);
    gpio_Write(Write_Enable[0], 1);

    printf("Loaded gains: CH1=%d, CH2=%d\n", gain_ch1, gain_ch2);
}
void delay_loop_1ms()
{
    volatile int i;
    for (i = 0; i < 200000; i++)
    {
        // Empty loop, prevent compiler optimization
        __asm__ __volatile__("nop");
    }
}

