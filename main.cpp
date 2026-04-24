#include "testscreen.h"
#include "mainwindow.h"
#include <QApplication>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include "matrix_keypad.h"
#include "shared_data.h"
#include <pthread.h>
#include <sched.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pthread_t Dpram,Keypad;
SharedData* shared = nullptr;


int main(int argc, char *argv[])
{
    // Allocate one shared buffer in process memory
    SharedData shared;
    memset(&shared, 0, sizeof(shared));

    // Init mutex (normal, not process-shared)
    pthread_mutex_init(&shared.lock, NULL);

    gain_init();                        //Initilize GPIO's
    Keypad_init();


    // Launch DPRAM thread
    pthread_create(&Dpram, NULL, DpramThread, &shared);

    // Launch Keypad thread (if needed)
    pthread_create(&Keypad, NULL, KeypadThread, NULL);

    // Start Qt
    QApplication a(argc, argv);

    MainWindow x;
    //TestScreen w;

    // Pass pointer so GUI can read from shared
    //w.setSharedPointer(&shared);

    //w.show();
    x.show();
    return a.exec();
}

