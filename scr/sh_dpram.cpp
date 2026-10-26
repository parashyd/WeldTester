//#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "shared_data.h"
#include <QElapsedTimer>
#include <QDebug>


extern pthread_t Dpram;

#define DPRAM_WORDS             65535                   //65535
// #define DPRAM_PHYS_BASE         0x80010000U
#define DPRAM_PHYS_BASE         0x80010004U
//#define DPRAM_PHYS_BASE         0x7c010004U
#define FLAG_OFFSET   0x0    // adjust if flag is not at DPRAM[0]





/* -------- /dev/mem mapping helpers -------- */

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int fd;
    size_t map_len;         // total mapped bytes (page-aligned)
    off_t page_base;        // page-aligned base phys addr
    size_t page_off;        // offset of phys_addr into that page
    volatile uint8_t *vaddr8; // byte view at requested phys_addr
} DpramMap;

/* -------- open & map a physical region (R/W) -------- */
static int dpram_open_map(DpramMap *m, uintptr_t phys_addr, size_t bytes)
{
    memset(m, 0, sizeof(*m));

    long pagesz = sysconf(_SC_PAGESIZE);
    if (pagesz <= 0) {
        perror("sysconf(_SC_PAGESIZE)");
        return -1;
    }

    // open /dev/mem read+write since we will write FLAG
    m->fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
    if (m->fd < 0) {
        perror("open(/dev/mem)");
        return -1;
    }

    // page-align the requested physical address
    m->page_base = (off_t)(phys_addr & ~((uintptr_t)pagesz - 1));
    m->page_off  = (size_t)(phys_addr - (uintptr_t)m->page_base);

    // ensure we map enough pages to cover [phys_addr, phys_addr+bytes)
    size_t need = m->page_off + bytes;
    size_t rounded = (need + (pagesz - 1)) & ~(size_t)(pagesz - 1);
    m->map_len = rounded;

    void *map = mmap(NULL,
                     m->map_len,
                     PROT_READ | PROT_WRITE,   // we will write the flag
                     MAP_SHARED,
                     m->fd,
                     m->page_base);
    if (map == MAP_FAILED) {
        perror("mmap(/dev/mem)");
        close(m->fd);
        m->fd = -1;
        return -1;
    }

    m->vaddr8 = (volatile uint8_t *)((uint8_t *)map + m->page_off);

    // optional debug
    /*fprintf(stderr,
            "DPRAM mapped: phys=0x%lX len=%zu (page_base=0x%lX off=%zu) -> vaddr=%p\n",
            (unsigned long)phys_addr, bytes,
            (unsigned long)m->page_base, m->page_off,
            (void*)m->vaddr8);*/

    return 0;
}

/* -------- unmap & close -------- */
static void dpram_close_map(DpramMap *m)
{
    if (m && m->vaddr8) {
        void *base = (void *)((uint8_t *)m->vaddr8 - m->page_off);
        munmap(base, m->map_len);
        m->vaddr8 = NULL;
    }
    if (m && m->fd >= 0) {
        close(m->fd);
        m->fd = -1;
    }
}

/* -------- tiny sleep helper (ns) -------- */
void nsleep(unsigned long ns)
{
    struct timespec ts;
    ts.tv_sec  = ns / 1000000000UL;
    ts.tv_nsec = ns % 1000000000UL;
    nanosleep(&ts, NULL);
}




// void* DpramThread(void* arg)
// {
//     SharedData* shared = (SharedData*)arg;

//     DpramMap map = {0};
//     if (dpram_open_map(&map, (uintptr_t)DPRAM_PHYS_BASE, DPRAM_WORDS) != 0)
//     {
//         fprintf(stderr, "[DPRAM] map failed\n");
//         return NULL;
//     }

//     volatile uint8_t* flag = map.vaddr8 + FLAG_OFFSET;
//     AddressData temp[DPRAM_WORDS];

//     while (1)
//     {
//         while (*flag != 1)
//             nsleep(10 * 1000);

//         // -------- READ DPRAM --------
//         for (uint32_t i = 0; i < DPRAM_WORDS; i++)
//             temp[i].data = map.vaddr8[i];

//         // -------- DEBUG SOURCE --------
//         static int dbg = 0;
//         if ((dbg++ % 1000) == 0)
//         {
//             printf("[SRC ] %u:%u %u:%u %u:%u\n",
//                    DBG_I1, temp[DBG_I1].data,
//                    DBG_I2, temp[DBG_I2].data,
//                    DBG_I3, temp[DBG_I3].data);
//         }

//         // -------- COPY TO SHARED --------
//         pthread_mutex_lock(&shared->lock);
//         memcpy(shared->buffer, temp, sizeof(temp));
//         pthread_mutex_unlock(&shared->lock);

//         *flag = 2;
//     }
// }


static inline bool validateSlice(uint32_t start,
                                 uint32_t end,
                                 uint32_t bufSize)
{
    return (start < bufSize &&
            end   < bufSize &&
            end  >= start);
}

void* DpramThread(void* arg)
{
    SharedData* shared = (SharedData*)arg;

    DpramMap map = {0};
    if (dpram_open_map(&map, (uintptr_t)DPRAM_PHYS_BASE, DPRAM_WORDS) != 0)
    {
        fprintf(stderr, "[DPRAM] map failed\n");
        return NULL;
    }

    //volatile uint8_t* flag = map.vaddr8 + FLAG_OFFSET;
    AddressData temp[DPRAM_WORDS];

    while (1)
    {
        // QElapsedTimer timer;
        // timer.start();

        // while (*flag != 1)
        //     nsleep(10 * 1000);

        // -------- READ DPRAM --------
        for (uint32_t i = 0; i < DPRAM_WORDS; i++)
            temp[i].data = map.vaddr8[i];

        // // -------- DEBUG SOURCE --------
        // static int dbg = 0;
        // if ((dbg++ % 1000) == 0)
        // {
        //     printf("[SRC ] %u:%u %u:%u %u:%u\n",
        //            DBG_I1, temp[DBG_I1].data,
        //            DBG_I2, temp[DBG_I2].data,
        //            DBG_I3, temp[DBG_I3].data);
        // }

        // -------- COPY TO SHARED --------
        pthread_mutex_lock(&shared->lock);
        memcpy(shared->buffer, temp, sizeof(temp));
        pthread_mutex_unlock(&shared->lock);

        // qDebug() << "copying took"
        //          << timer.nsecsElapsed() / 1000000.0
        //          << "ms";
        //*flag = 2;
    }
}







