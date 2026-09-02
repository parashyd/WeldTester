#include <stdio.h>
#include <stdlib.h>
#include "DataFile.h"
#include "qdebug.h"
#include "shared_data.h"
#include <stdbool.h>
#include <QVector>
#include <QPointF>

extern SharedData* shared;


#define MAX_LINE_LEN 200
ConfigEntry entry;
ConfigEntry temp;


int getconfig(int userCalset, int userChannel, ConfigEntry* matchedConfig)
{
    FILE *configFile;
    int matchFound = 0, fields;
    char configLine[MAX_LINE_LEN];

    // configFile = fopen("/run/media/mmcblk1p1/Config.txt", "r");
    configFile = fopen("Config.txt", "r");

    if (!configFile)
    {
        perror("Failed to open Config.txt");
        return 1;
    }

    while (fgets(configLine, sizeof(configLine), configFile))
    {
        fields = sscanf(configLine, "%d,%d,%lf,%lf,%lf,%d,%d,%d,%d,%d,%d,%lf,%f",
                        &temp.calset, &temp.channel,
                        &temp.range, &temp.delay, &temp.reject,
                        &temp.g1_start, &temp.g1_end, &temp.th1,
                        &temp.g2_start, &temp.g2_end, &temp.th2,
                        &temp.Gain, &temp.Angle);

        if (fields == 13 && temp.calset == userCalset && temp.channel == userChannel) {
            *matchedConfig = temp;
            matchFound = 1;
            break;
        }
    }
    fclose(configFile);

    return matchFound ? 0 : 1;
}


bool filterMatch(uint16_t addr, uint8_t value, const ConfigEntry& cfg)
{
    unsigned int startAddr = cfg.delay * DELAY_FACTOR;
    // unsigned int endAddr   = startAddr + (cfg.range * RANGE_FACTOR) - 1;
    unsigned int endAddr   = startAddr + (cfg.range * (cfg.Angle<30?RANGE_FACTOR_LT30:RANGE_FACTOR_GT30)) - 1;

    if (addr < startAddr || addr > endAddr)
        return false;

    if (value < cfg.reject)
        return false;

    return true;
}


QVector<AddressData> localBuffer;   // <-- persistent buffer



static inline bool calculateSlice(const ConfigEntry& cfg,
                                  uint32_t& startAddr,
                                  uint32_t& endAddr)
{
    uint32_t base = 0;

    if (cfg.channel == 1)
    {
        base = 0;
    }
    else if (cfg.channel == 2)
    {
        //base = 32778;
        base = 32768;
    }
    else
        return false;

    startAddr = base + uint32_t(cfg.delay * DELAY_FACTOR);
    // uint32_t len = uint32_t(cfg.range * RANGE_FACTOR);
    uint32_t len = uint32_t(cfg.range * (cfg.Angle<30?RANGE_FACTOR_LT30:RANGE_FACTOR_GT30));

    if (len == 0)
        return false;

    endAddr = startAddr + len - 1;

    // 🔴 REAL bounds check
    if (endAddr >= 65535)
        return false;

    return (endAddr > startAddr);
}




// int getFilteredPoints(const ConfigEntry& cfg, QVector<QPointF>& output)
// {
//     if (!shared)
//         return 0;

//     uint32_t startAddr = 0, endAddr = 0;
//     if (!calculateSlice(cfg, startAddr, endAddr))
//         return 0;

//     uint32_t sliceSize = endAddr - startAddr + 1;

//     if (localBuffer.size() < sliceSize)
//         localBuffer.resize(sliceSize);

//     // -------- COPY SLICE --------
//     pthread_mutex_lock(&shared->lock);
//     memcpy(localBuffer.data(),
//            &shared->buffer[startAddr],
//            sliceSize * sizeof(AddressData));
//     pthread_mutex_unlock(&shared->lock);

//     // -------- DEBUG AFTER SLICE --------
//     static int dbg = 0;
//     if ((dbg++ % 200) == 0)
//     {
//         printf("[SLCE] abs %u:%u %u:%u %u:%u\n",
//                startAddr + DBG_I1, localBuffer[DBG_I1].data,
//                startAddr + DBG_I2, localBuffer[DBG_I2].data,
//                startAddr + DBG_I3, localBuffer[DBG_I3].data);
//     }

//     // -------- BUILD OUTPUT --------
//     output.clear();
//     output.reserve(sliceSize);

//     for (uint32_t i = 0; i < sliceSize; i++)
//     {
//         uint8_t val = localBuffer[i].data;

//         // reject currently disabled or enabled — unchanged
//         if (val < (cfg.reject * 2.55))
//             val = 0;

//         output.append(QPointF(i, val));
//     }

//     return output.size();
// }

int getFilteredPoints(const ConfigEntry& cfg, QVector<QPointF>& output)
{
    if (!shared)
        return 0;

    uint32_t startAddr = 0, endAddr = 0;

    if (!calculateSlice(cfg, startAddr, endAddr))
        return 0;

    uint32_t sliceSize = endAddr - startAddr + 1;

    // Publish slice to DPRAM thread
    pthread_mutex_lock(&shared->lock);
    shared->sliceStart = startAddr;
    shared->sliceEnd   = endAddr;
    pthread_mutex_unlock(&shared->lock);

    if (localBuffer.size() < sliceSize)
        localBuffer.resize(sliceSize);

    // -------- COPY SLICE --------
    pthread_mutex_lock(&shared->lock);
    memcpy(localBuffer.data(),
           &shared->buffer[startAddr],
           sliceSize * sizeof(AddressData));
    pthread_mutex_unlock(&shared->lock);

    // // -------- DEBUG AFTER SLICE --------
    // static int dbg = 0;
    // if ((dbg++ % 200) == 0)
    // {
    //     printf("[SLICE] %u:%u | %u:%u %u:%u %u:%u\n",
    //            startAddr, endAddr,
    //            startAddr + DBG_I1, localBuffer[DBG_I1].data,
    //            startAddr + DBG_I2, localBuffer[DBG_I2].data,
    //            startAddr + DBG_I3, localBuffer[DBG_I3].data);
    // }

    // -------- BUILD OUTPUT --------
    output.clear();
    output.reserve(sliceSize);

    for (uint32_t i = 0; i < sliceSize; i++)
    {
        uint8_t val = localBuffer[i].data;
        if (val < (cfg.reject * 2.55))
            val = 0;
        //qDebug()<<i <<" "<< val << "\n";
        output.append(QPointF(i, val));
    }

    return output.size();
}






