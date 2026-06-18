#ifndef VIEWLOGDATA_H
#define VIEWLOGDATA_H

#pragma once
#include <QDialog>
#include <QTimer>
#include <QVector>
#include "wt_logger.h"           // WtReader lives here
#include "qcustomplot.h"

// Forward declared in openlog1.cpp
extern QString selectedFilePath;

namespace Ui { class viewLogData; }

class viewLogData : public QDialog
{
    Q_OBJECT

public:
    explicit viewLogData(QWidget *parent = nullptr);
    ~viewLogData();

    // Called by OpenLog1 after it sets selectedFilePath
    void openFile(const QString &path);

    // Hardware key forwarding
    void handleSocketKey(quint8 key);

private slots:
    void onTimerTick();
    void onPlayPause();
    void onStop();
    void onScrubReleased();
    void onScrubMoved(int value);

private:
    void setupPlot();
    void setupControls();
    void startPlayback();
    void pausePlayback();
    void stopPlayback();
    void renderFrame(const QByteArray &frame);
    void updateGridInterval();
    void updateTimeLabel();
    void HandleGateShift(int shift);
    void HandleGateUpDownLift(int lift);
    void adjustGatewidth(int delta);
    void LoadTestDetails();
    inline void focusGate(int replotRequired);
    inline void plotGate(int replotRequired);

    Ui::viewLogData *ui;

    WtReader      m_reader;
    QTimer       *m_timer        = nullptr;
    QCPGraph     *m_wave         = nullptr;
    QCPGraph     *gborder         = nullptr;
    QCPGraph     *gate         = nullptr;
    QVector<double> m_xBuf;   // pre-allocated, reused every frame
    QVector<double> m_yBuf;

    bool m_paused       = true;
    bool m_userScrubbing = false;
signals:
    void closeviewlogdata();
};

#endif // VIEWLOGDATA_H
