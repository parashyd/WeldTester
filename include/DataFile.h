#ifndef DATAFILE_H
#define DATAFILE_H


#include "shared_data.h"
#include <QVector>
#include <QPointF>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int calset;
    int channel;
    int velocity;
    double range;
    double delay;
    double reject;
    int g1_start;
    int g1_end;
    int th1;
    int g2_start;
    int g2_end;
    int th2;
    double Gain;
    float Angle;
} ConfigEntry;

#define GAIN_FACTOR  2.0
#define REJECT_FACTOR  2.55


extern float DELAY_FACTOR,RANGE_FACTOR;
extern float RANGE_FACTOR_GT30,RANGE_FACTOR_LT30;

//extern ConfigEntry entry;

void gain_init();
void OperateGainDual(int gain_ch1, int gain_ch2);
int getconfig(int userCalset, int userChannel, ConfigEntry* matchedConfig);
int getFilteredPoints(const ConfigEntry& cfg, QVector<QPointF>& output);


bool filterMatch(uint16_t addr, uint8_t value);


#ifdef __cplusplus
}
#endif


#endif // DATAFILE_H
