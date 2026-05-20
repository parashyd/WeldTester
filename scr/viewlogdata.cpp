#include "viewlogdata.h"
#include "ui_viewlogdata.h"
#include "testscreen.h"
#include "QFile"

ConfigEntry recconfig;

QVector<double> xarr,yarr;

viewLogData::viewLogData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::viewLogData)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    ui->setupUi(this);
    setPlotAppearance();
    wave=ui->PLOT->addGraph();
    loadWaveform(selectedFilePath);

}
void viewLogData::loadWaveform(QString wtFilePath)
{
    int channel, calset;

    QString filename =
        QFileInfo(wtFilePath).baseName();

    QString configPath =
        QFileInfo(wtFilePath).absolutePath();

    configPath += "/Config.txt";

    QStringList parts =
        filename.split("_");

    if(parts.size() < 3)
    {
        qDebug() << "Invalid WT filename";
        return;
    }

    channel = parts[0].toInt();

    calset = parts[1].toInt();

    qDebug() << "Channel:" << channel;
    qDebug() << "Calset:" << calset;

    QFile configFile(configPath);

    if(!configFile.open(QIODevice::ReadOnly |
                         QIODevice::Text))
    {
        qDebug() << "Failed to open Config.txt";

        return;
    }

    QTextStream in(&configFile);

    bool matchFound = false;

    while(!in.atEnd())
    {
        QString line = in.readLine();

        // QStringList fields =
        //     line.split(",");
        QStringList fields =
            line.split(",", Qt::SkipEmptyParts);
        if(fields.size() != 13)
            continue;

        ConfigEntry temp;

        temp.calset = fields[0].toInt();

        temp.channel = fields[1].toInt();

        temp.range = fields[2].toDouble();

        temp.delay = fields[3].toDouble();

        temp.reject = fields[4].toDouble();

        temp.g1_start = fields[5].toInt();

        temp.g1_end = fields[6].toInt();

        temp.th1 = fields[7].toInt();

        temp.g2_start = fields[8].toInt();

        temp.g2_end = fields[9].toInt();

        temp.th2 = fields[10].toInt();

        temp.Gain = fields[11].toDouble();

        temp.Angle = fields[12].toFloat();

        if(temp.calset == calset &&
            temp.channel == channel)
        {
            recconfig = temp;

            matchFound = true;

            break;
        }
    }

    configFile.close();

    if(!matchFound)
    {
        qDebug() << "Matching config not found";

        return;
    }

    frameSize =
        recconfig.Angle < 30 ?
            (recconfig.range *
             RANGE_FACTOR_LT30)
                             :
            (recconfig.range *
             RANGE_FACTOR_GT30);

    qDebug() << "Frame Size:"
             << frameSize;

    qDebug() << selectedFilePath;
    qDebug() << wtFilePath;


    // QFile plybk(wtFilePath);
    // qDebug() << "plybk initialized";
    // plybk.open(QIODevice::ReadOnly);
    // qDebug() << "plybk opened";

    // if(!plybk.open(QIODevice::ReadOnly))
    // {
    //     qDebug() << "Failed to open plybk file";

    //     return;
    // }

    if (channel == 1){
        wave->setPen(QPen(Qt::magenta, 2));
    }
    else{
        wave->setPen(QPen(Qt::yellow, 2));
    }

    wave->setLineStyle(QCPGraph::lsLine);

    playbackFile.setFileName(wtFilePath);
    qDebug() << " playback file initialized";

    // playbackFile.open(QIODevice::ReadOnly);
    // qDebug() << "opened playback file";


    if(!playbackFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open playback file";

        return;
    }

    ui->PLOT->xAxis->setRange(0, recconfig.range);
    ui->PLOT->yAxis->setRange(0, 100);
    updateGridInterval();

    currentOffset = 0;

    playbackTimer = new QTimer(this);

        connect(playbackTimer,
                &QTimer::timeout,
                this,
                &viewLogData::updatePlaybackFrame);


    playbackTimer->start(20);

    qDebug() << "Playback started";
}

void viewLogData::updatePlaybackFrame()
{
    if(!playbackFile.isOpen())
        return;

    if(playbackFile.atEnd())
    {
        playbackTimer->stop();

        playbackFile.close();

        qDebug() << "Playback Finished";

        return;
    }
    playbackFile.seek(currentOffset);

    QByteArray frameData =
        playbackFile.read(frameSize);

    xarr={0};
    yarr={0};

    yarr.resize(frameSize);
    xarr.resize(frameSize);

    for(int i = 0; i < frameData.size(); i++)
    {
        xarr[i] = i/RANGE_FACTOR_LT30;
        yarr[i] = static_cast<quint8>(frameData[i]);
    }

    wave->data()->clear();

    wave->setData(xarr, yarr);

    ui->PLOT->replot();
    currentOffset+=frameSize;
}


void viewLogData::setPlotAppearance()
{
    ui->PLOT->setFixedSize(485, 335);
    ui->PLOT->setBackground(Qt::black);
    QSharedPointer<QCPAxisTickerFixed> xTicker(new QCPAxisTickerFixed);
    xTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->PLOT->xAxis->setTicker(xTicker);
    ui->PLOT->xAxis->setBasePen(QPen(Qt::white));
    ui->PLOT->xAxis->setTickPen(QPen(Qt::white));
    ui->PLOT->xAxis->setTickLabelColor(Qt::white);
    ui->PLOT->xAxis->grid()->setPen(QPen(Qt::gray));
    ui->PLOT->xAxis->grid()->setSubGridPen(QPen(Qt::darkGray));
    ui->PLOT->xAxis->setTickLabelSide(QCPAxis::lsOutside);

    QSharedPointer<QCPAxisTickerFixed> yTicker(new QCPAxisTickerFixed);
    yTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->PLOT->yAxis->setTicker(yTicker);
    ui->PLOT->yAxis->setBasePen(QPen(Qt::white));
    ui->PLOT->yAxis->setTickPen(QPen(Qt::white));
    ui->PLOT->yAxis->setTickLabelColor(Qt::white);
    ui->PLOT->yAxis->grid()->setPen(QPen(Qt::gray));
    ui->PLOT->yAxis->grid()->setSubGridPen(QPen(Qt::darkGray));
    ui->PLOT->yAxis->setTickLabelSide(QCPAxis::lsOutside);

    ui->PLOT->xAxis->setRange(0, 100);
    ui->PLOT->yAxis->setRange(0, 100);

    ui->PLOT->replot();
    updateGridInterval();
}
void viewLogData::updateGridInterval()
{
    // Update X-axis
    double xLower = ui->PLOT->xAxis->range().lower;
    double xUpper = ui->PLOT->xAxis->range().upper;
    double xStep = (xUpper - xLower) / 10; // 10 intervals

    QSharedPointer<QCPAxisTickerFixed> xTicker = ui->PLOT->xAxis->ticker().staticCast<QCPAxisTickerFixed>();
    xTicker->setTickStep(xStep);

    // Update Y-axis
    double yLower = ui->PLOT->yAxis->range().lower;
    double yUpper = ui->PLOT->yAxis->range().upper;
    double yStep = (yUpper - yLower) / 10; // 10 intervals

    QSharedPointer<QCPAxisTickerFixed> yTicker = ui->PLOT->yAxis->ticker().staticCast<QCPAxisTickerFixed>();
    yTicker->setTickStep(yStep);

    ui->PLOT->replot();
}

viewLogData::~viewLogData()
{
    delete ui;
}
