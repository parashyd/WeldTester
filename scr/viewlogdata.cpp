#include "viewlogdata.h"
#include "ui_viewlogdata.h"
#include "wt_logger.h"
#include "DataFile.h"
#include "matrix_keypad.h"

static constexpr int PLAYBACK_INTERVAL_MS = 20;  // 50 fps, same as live

// ──────────────────────────────────────────────────────────────
//  Constructor
// ──────────────────────────────────────────────────────────────
viewLogData::viewLogData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::viewLogData)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    ui->setupUi(this);

    setupPlot();
    setupControls();

    // Open file immediately using the global selectedFilePath
    if (!selectedFilePath.isEmpty())
        openFile(selectedFilePath);
    ui->label_chval->setText(QString::number(m_reader.config().channel));
    ui->label_calsetval->setText(QString::number(m_reader.config().calset));
    ui->label_gainval->setText(QString::number(m_reader.config().Gain,'f',0));
    ui->label_Rejectval->setText(QString::number(m_reader.config().reject,'f',0));
    ui->label_rangeval->setText(QString::number(m_reader.config().range,'f',0));
    ui->label_DelayVal->setText(QString::number(m_reader.config().delay,'f',0));
    ui->label_Angleval->setText(QString::number(m_reader.config().Angle));
    ui->lineEdit_GTST->setText(QString::number(5));
    ui->lineEdit_GTED->setText(QString::number(30));
    ui->lineEdit_TH->setText(QString::number(25));

}

// ──────────────────────────────────────────────────────────────
//  Plot setup – OpenGL enabled for smooth rendering
// ──────────────────────────────────────────────────────────────
void viewLogData::setupPlot()
{
    ui->PLOT->setOpenGl(true);                 // <── same as testscreen
    ui->PLOT->setFixedSize(485, 335);
    ui->PLOT->setBackground(Qt::black);

    auto makeFixedTicker = [](QCustomPlot *plot, QCPAxis *axis) {
        QSharedPointer<QCPAxisTickerFixed> t(new QCPAxisTickerFixed);
        t->setScaleStrategy(QCPAxisTickerFixed::ssNone);
        axis->setTicker(t);
        axis->setBasePen(QPen(Qt::white));
        axis->setTickPen(QPen(Qt::white));
        axis->setTickLabelColor(Qt::white);
        axis->grid()->setPen(QPen(Qt::gray));
        axis->grid()->setSubGridPen(QPen(Qt::darkGray));
        axis->setTickLabelSide(QCPAxis::lsOutside);
    };
    makeFixedTicker(ui->PLOT, ui->PLOT->xAxis);
    makeFixedTicker(ui->PLOT, ui->PLOT->yAxis);

    ui->PLOT->xAxis->setRange(0, 100);
    ui->PLOT->yAxis->setRange(0, 100);

    m_wave = ui->PLOT->addGraph();
    m_wave->setLineStyle(QCPGraph::lsLine);
    m_wave->setPen(QPen(Qt::yellow, 2));

    gborder = ui->PLOT->addGraph();
    gborder->setLineStyle(QCPGraph::lsLine);
    gborder->setPen(QPen(Qt::white, 6));

    gate = ui->PLOT->addGraph();
    gate->setLineStyle(QCPGraph::lsLine);
    gate->setPen(QPen(Qt::blue, 2));

    ui->PLOT->replot();
}

// ──────────────────────────────────────────────────────────────
//  Connect UI controls (buttons / slider expected in .ui)
// ──────────────────────────────────────────────────────────────
void viewLogData::setupControls()
{
    m_timer = new QTimer(this);
    m_timer->setInterval(PLAYBACK_INTERVAL_MS);
    m_timer->setTimerType(Qt::PreciseTimer);   // avoids OS jitter
    connect(m_timer, &QTimer::timeout, this, &viewLogData::onTimerTick);

    // Wire up buttons – these object names must match your .ui file.
    // Adjust names to whatever you have in viewlogdata.ui.
    if (ui->btn_PlayPause)
        connect(ui->btn_PlayPause, &QPushButton::clicked,
                this, &viewLogData::onPlayPause);

    // if (ui->btn_Stop)
    //     connect(ui->btn_Stop, &QPushButton::clicked,
    //             this, &viewLogData::onStop);

    // if (ui->btn_Close)
    //     connect(ui->btn_Close, &QPushButton::clicked,
    //             this, &viewLogData::close);

    // Scrub slider – user drags to jump to a frame
    if (ui->slider_Scrub)
    {
        ui->slider_Scrub->setRange(0, 0);
        connect(ui->slider_Scrub, &QSlider::sliderPressed,
                this, [this]{ m_userScrubbing = true; m_timer->stop(); });
        connect(ui->slider_Scrub, &QSlider::sliderReleased,
                this, &viewLogData::onScrubReleased);
        connect(ui->slider_Scrub, &QSlider::valueChanged,
                this, &viewLogData::onScrubMoved);
    }
}

// ──────────────────────────────────────────────────────────────
//  Open a .wt file and start playback
// ──────────────────────────────────────────────────────────────
void viewLogData::openFile(const QString &path)
{
    stopPlayback();

    if (!m_reader.open(path))
    {
        qWarning() << "[viewLogData] Failed to open" << path;
        return;
    }

    // Configure plot axes from header
    float range = m_reader.config().range;
    ui->PLOT->xAxis->setRange(0, range);
    ui->PLOT->yAxis->setRange(0, 100);
    updateGridInterval();

    // Configure wave colour by channel
    QPen wavePen(m_reader.config().channel == 1 ? Qt::magenta : Qt::yellow, 2);
    m_wave->setPen(wavePen);

    // Configure scrub slider
    if (ui->slider_Scrub)
    {
        ui->slider_Scrub->setRange(0, qMax(0, m_reader.totalFrames() - 1));
        ui->slider_Scrub->setValue(0);
    }

    updateTimeLabel();
    startPlayback();
}

// ──────────────────────────────────────────────────────────────
//  Playback control
// ──────────────────────────────────────────────────────────────
void viewLogData::startPlayback()
{
    if (!m_reader.isOpen()) return;
    m_paused = false;
    m_timer->start();
    if (ui->btn_PlayPause)
        ui->btn_PlayPause->setText("Pause");
}

void viewLogData::pausePlayback()
{
    m_paused = true;
    m_timer->stop();
    if (ui->btn_PlayPause)
        ui->btn_PlayPause->setText("Play");
}

void viewLogData::stopPlayback()
{
    m_timer->stop();
    m_paused = true;
    m_reader.close();
    if (ui->btn_PlayPause)
        ui->btn_PlayPause->setText("Play");
    if (ui->slider_Scrub)
        ui->slider_Scrub->setValue(0);
}

// ──────────────────────────────────────────────────────────────
//  Core timer tick – runs every 20 ms during playback
// ──────────────────────────────────────────────────────────────
void viewLogData::onTimerTick()
{
    if (!m_reader.isOpen()) return;

    if (m_reader.atEnd())
    {
        pausePlayback();
        qDebug() << "[viewLogData] Playback finished";
        return;
    }

    QByteArray frame = m_reader.readNextFrame();
    if (frame.isEmpty()) return;

    renderFrame(frame);

    // Sync scrub slider without triggering valueChanged→seek loop
    if (ui->slider_Scrub && !m_userScrubbing)
    {
        QSignalBlocker blocker(ui->slider_Scrub);
        ui->slider_Scrub->setValue(m_reader.currentFrame() - 1);
    }

    updateTimeLabel();
}

// ──────────────────────────────────────────────────────────────
//  Render a single frame to the plot
// ──────────────────────────────────────────────────────────────
void viewLogData::renderFrame(const QByteArray &frame)
{
    const int   n  = frame.size();
    const float rf = wtRangeFactor(m_reader.config());  // exact factor from header

    // Reuse pre-allocated vectors to avoid heap churn every 20 ms
    if (m_xBuf.size() != n) m_xBuf.resize(n);
    if (m_yBuf.size() != n) m_yBuf.resize(n);

    double phValue =0;
    double peakIndex =  -1;

    double st = ui->lineEdit_GTST->text().toDouble() * m_reader.config().range / 100;
    double ed = ui->lineEdit_GTED->text().toDouble()* m_reader.config().range / 100;
    double th = ui->lineEdit_TH->text().toDouble();
    for (int i = 0; i < n; ++i)
    {
        m_xBuf[i] = i / rf;
        m_yBuf[i] = static_cast<quint8>(frame[i]);

        //double x = m_reader.config().Angle<30?m_xBuf[i]/RANGE_FACTOR_LT30:m_xBuf[i]/RANGE_FACTOR_GT30;

        double x = m_xBuf[i];
        if (x >= st && x <= ed)
        {
            if (m_yBuf[i] > phValue){
                peakIndex = i;
            }

            phValue = qMax(phValue, m_yBuf[i]);
        }
    }

    double bp = 0;

    if (peakIndex >= 0)
        for (int i = peakIndex; i >= 0; i--)
            if (m_yBuf[i] == 0) { bp = m_xBuf[i]; break; }


    ui->lineEdit_PH->setText(QString::number(phValue, 'f', 0));
    ui->lineEdit->setText(QString::number(bp, 'f', 0));

    double ang = m_reader.config().Angle * M_PI / 180.0;

    double d  = bp * cos(ang);
    double sd = bp * sin(ang);


    ui->lineEdit_D->setText(QString::number(d, 'f', 0));
    ui->lineEdit_SD->setText(QString::number(sd, 'f', 0));

    gate->data()->clear();
    gate->setData(QVector<double>{st, ed},
                    QVector<double>{th, th});

    m_wave->data()->clear();
    m_wave->setData(m_xBuf, m_yBuf, true); // true = already sorted
    ui->PLOT->replot(QCustomPlot::rpQueuedReplot); // non-blocking replot
}

// ──────────────────────────────────────────────────────────────
//  UI slots
// ──────────────────────────────────────────────────────────────
void viewLogData::onPlayPause()
{
    if (!m_reader.isOpen()) return;

    if (m_paused)
        startPlayback();
    else
        pausePlayback();
}

void viewLogData::onStop()
{
    stopPlayback();
    // Clear the plot
    m_wave->data()->clear();
    ui->PLOT->replot();
    updateTimeLabel();
}

void viewLogData::onScrubReleased()
{
    m_userScrubbing = false;
    if (!m_reader.isOpen()) return;

    int frame = ui->slider_Scrub ? ui->slider_Scrub->value() : 0;
    m_reader.seekToFrame(frame);
    renderFrame(m_reader.readNextFrame());   // preview the seeked frame
    updateTimeLabel();

    // Resume playback if it was playing before scrub
    if (!m_paused)
        m_timer->start();
}

void viewLogData::onScrubMoved(int value)
{
    if (!m_userScrubbing || !m_reader.isOpen()) return;
    // Live preview while dragging – seek and render but don't advance
    m_reader.seekToFrame(value);
    QByteArray frame = m_reader.readNextFrame();
    if (!frame.isEmpty())
    {
        m_reader.seekToFrame(value); // rewind back so onScrubReleased starts here
        renderFrame(frame);
    }
    updateTimeLabel();
}

// ──────────────────────────────────────────────────────────────
//  Helpers
// ──────────────────────────────────────────────────────────────
void viewLogData::updateTimeLabel()
{
    if (!ui->label_Time) return;

    int cur   = m_reader.isOpen() ? m_reader.currentFrame() : 0;
    int total = m_reader.isOpen() ? m_reader.totalFrames()  : 0;

    double curSec   = cur   * 0.020;
    double totalSec = total * 0.020;

    ui->label_Time->setText(
        QString("%1s / %2s")
            .arg(curSec,   0, 'f', 1)
            .arg(totalSec, 0, 'f', 1));
}

void viewLogData::updateGridInterval()
{
    auto setStep = [](QCPAxis *axis) {
        double lo = axis->range().lower;
        double hi = axis->range().upper;
        QSharedPointer<QCPAxisTickerFixed> t =
            axis->ticker().staticCast<QCPAxisTickerFixed>();
        if (t) t->setTickStep((hi - lo) / 10.0);
    };
    setStep(ui->PLOT->xAxis);
    setStep(ui->PLOT->yAxis);
    ui->PLOT->replot(QCustomPlot::rpQueuedReplot);
}

// ──────────────────────────────────────────────────────────────
//  Key forwarding from OpenLog1 (your existing pattern)
// ──────────────────────────────────────────────────────────────
void viewLogData::handleSocketKey(quint8 key)
{
    // Map your hardware keys to actions
    // Adjust key codes to match matrix_keypad.h defines
    switch (key)
    {
    case RUN: // example: PLAY key
        onPlayPause();
        break;
    case 0x51: // example: STOP key
        onStop();
        break;

    case ESC: // ESC – close
        emit closeviewlogdata();
        break;

    default:
        break;
    }
}

viewLogData::~viewLogData()
{
    stopPlayback();
    delete ui;
}
