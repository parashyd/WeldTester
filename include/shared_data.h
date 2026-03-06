#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdint.h>
#include <pthread.h>

#define BUFFER_SIZE 65535
//#define SHM_KEY 0x1234

#define DBG_I1  10
#define DBG_I2  100
#define DBG_I3  1000

typedef struct {
    uint16_t address;
    uint8_t data;
} AddressData;

typedef struct {
    pthread_mutex_t lock;
    AddressData buffer[BUFFER_SIZE];
    int updated;

    uint32_t sliceStart;
    uint32_t sliceEnd;

} SharedData;

// Dpram thread declaration
#ifdef __cplusplus
extern "C" {
#endif

void* DpramThread(void* arg);
void nsleep(unsigned long ns);
void dpram_init(void);
void dpram_write(uint16_t addr, uint8_t data);
uint8_t dpram_read(int addr[]);

#ifdef __cplusplus
}
#endif

#endif // SHARED_MEMORY_H

