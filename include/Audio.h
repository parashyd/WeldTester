#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define PWM_CHIP "/sys/class/pwm/pwmchip0"
#define PWM_CHANNEL "0"
#define PWM_PERIOD_NS "1000000"  // 1ms = 1 kHz

int Buzzerinit();
int BuzzerDuty(int duty_ns);
int BuzzerOn(bool val);
bool isBuzzerOn();


#endif // AUDIO_H
