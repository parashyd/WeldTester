#ifndef VIEWLOGDATA_H
#define VIEWLOGDATA_H

#include <QDialog>
#include "openlog1.h"
#include <QFile>
#include "qcustomplot.h"
namespace Ui {
class viewLogData;
}

class viewLogData : public QDialog
{
    Q_OBJECT

public:
    explicit viewLogData(QWidget *parent = nullptr);
    ~viewLogData();

private:
    Ui::viewLogData *ui;
    void setPlotAppearance();
    void updateGridInterval();

    void loadWaveform(QString wtFilePath);

    void updatePlaybackFrame();

    QFile playbackFile;
    QTimer *playbackTimer;
    int frameSize = 0;
    qint64 currentOffset = 0;
    QCPGraph *wave=nullptr;

signals:
    void closeviewlogdata();
};

#endif // VIEWLOGDATA_H
