QT       += core gui network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport opengl
DEFINES += QCUSTOMPLOT_USE_OPENGL

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include/
VPATH += scr/

SOURCES += \
    scr/gps.c \
    scr/openlog1.cpp \
    scr/configscreen.cpp \
    main.cpp \
    scr/mainwindow.cpp \
    scr/Audio.cpp \
    scr/Battery.cpp \
    scr/DataFile.cpp \
    scr/Gain.cpp \
    scr/gpio_init.c \
    scr/mk_dm2.cpp \
    scr/openlog.cpp \
    scr/previewscreen.cpp \
    scr/qcustomplot.cpp \
    scr/sh_dpram.cpp \
    scr/testdetails.cpp \
    scr/testscreen.cpp \
    scr/testdetail0.cpp \
    scr/viewlogdata.cpp

HEADERS += \
    include/gps.h \
    include/configscreen.h \
    include/Audio.h \
    include/Battery.h \
    include/DataFile.h \
    include/Ugpio.h \
    include/matrix_keypad.h \
    include/openlog.h \
    include/previewscreen.h \
    include/qcustomplot.h \
    include/shared_data.h \
    include/testdetails.h \
    include/testscreen.h \
    include/mainwindow.h \
    include/testdetail0.h \
    include/openlog1.h \
    include/viewlogdata.h \
    include/wt_logger.h \
    include/preview_logger.h

FORMS += \
    Openlog.ui \
    configscreen.ui \
    mainwindow.ui \
    openlog1.ui \
    previewscreen.ui \
    testdetail0.ui \
    testdetails.ui \
    testscreen.ui \
    viewlogdata.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Config.txt

RESOURCES += \
    resources.qrc
