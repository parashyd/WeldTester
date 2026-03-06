#include "testwindow.h"
#include "ui_testwindow.h"
#include "DataFile.h"
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QRegularExpression>
#include <QMap>
#include <cmath>
#include "keypadinputhelper.h"
#include "shared_data.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sched.h>
#include <QTimer>
#include <QtGlobal>

extern pthread_t Dpram,Keypad;
extern SharedData* shared;
ConfigEntry config;
PostSave Postlog;
extern ConfigEntry temp;


enum InputMode
{
    InputMode_None,
    InputMode_Calset,
    InputMode_TestDetails
};

static bool firstNav = true;
InputMode currentInputMode = InputMode_None;


int SEC,saveflg = 1;

QString MachNo;
QString mode;

double maxX, maxY;
double th1, g1_start, g1_end;
double th2, g2_start, g2_end;
double RANGE_FACTOR = 3.4, DELAY_FACTOR = 3.4;
double velocity=5920;
#define MAX_LINE 200
#define LINE_LENGTH 70

QString receivedChannel;
QString receivedCalset;
QMap<int, KeyPressState> keyStates;
const int doubleClickThresholdMs = 400; // Customize as needed
KeyPressState calsetState;
QString calsetStr;


TestWindow::TestWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestWindow)
    , testdetails(nullptr)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);         // to disable the menu bar

    ui->setupUi(this);

    plotUpdateTimer = new QTimer(this);

    setupPlotAppearance();
    loadSavedConfig();
    autoRunConfig();

    startSocketServer();

    connect(plotUpdateTimer, &QTimer::timeout, this, &TestWindow::updateGraphWithData);
    plotUpdateTimer->start(100);             //10msec


    connect(ui->lineEdit_Gain, &QLineEdit::editingFinished,this, &TestWindow::onApplyGainClicked);

    connect(ui->lineEdit_Gain, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_Range, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_Delay, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_Reject,&QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_Angle,&QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_G1ST, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_G1ED, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_TH1, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_G2ST, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_G2ED, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_TH2, &QLineEdit::editingFinished, this, &TestWindow::saveTo_entry);
    connect(ui->lineEdit_Velocity, &QLineEdit::editingFinished, this,[=](){
        velocity=ui->lineEdit_Velocity->text().toDouble();
        RANGE_FACTOR=20000/velocity;
        DELAY_FACTOR=20000/velocity;
        saveTo_entry();
    });
    ui->label_G1ST->setStyleSheet("QLabel { color : #219601; }");
    ui->label_G1ED->setStyleSheet("QLabel { color : #219601; }");
    ui->label_TH1->setStyleSheet("QLabel { color : #219601; }");

    ui->label_G2ST->setStyleSheet("QLabel { color : #0818ff; }");
    ui->label_G2ED->setStyleSheet("QLabel { color : #0818ff; }");
    ui->label_TH2->setStyleSheet("QLabel { color : #0818ff; }");

    inputStates[ui->lineEdit_calset] = KeyPressState();
    inputStates[ui->lineEdit_Gain]   = KeyPressState();
    inputStates[ui->lineEdit_Range]  = KeyPressState();
    inputStates[ui->lineEdit_Delay]  = KeyPressState();
    inputStates[ui->lineEdit_Reject] = KeyPressState();
    inputStates[ui->lineEdit_Angle] = KeyPressState();
    inputStates[ui->lineEdit_G1ST] = KeyPressState();
    inputStates[ui->lineEdit_G1ED] = KeyPressState();
    inputStates[ui->lineEdit_TH1] = KeyPressState();
    inputStates[ui->lineEdit_G2ST] = KeyPressState();
    inputStates[ui->lineEdit_G2ED] = KeyPressState();
    inputStates[ui->lineEdit_TH2] = KeyPressState();
}


void TestWindow::setupPlotAppearance()
{
    ui->Plot->setFixedSize(500, 400);
    ui->Plot->setBackground(Qt::black);
    QSharedPointer<QCPAxisTickerFixed> xTicker(new QCPAxisTickerFixed);
    xTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->Plot->xAxis->setTicker(xTicker);
    ui->Plot->xAxis->setBasePen(QPen(Qt::white));
    ui->Plot->xAxis->setTickPen(QPen(Qt::white));
    ui->Plot->xAxis->setTickLabelColor(Qt::white);
    ui->Plot->xAxis->grid()->setPen(QPen(Qt::gray));
    ui->Plot->xAxis->grid()->setSubGridPen(QPen(Qt::darkGray));
    ui->Plot->xAxis->setTickLabelSide(QCPAxis::lsOutside);

    QSharedPointer<QCPAxisTickerFixed> yTicker(new QCPAxisTickerFixed);
    yTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->Plot->yAxis->setTicker(yTicker);
    ui->Plot->yAxis->setBasePen(QPen(Qt::white));
    ui->Plot->yAxis->setTickPen(QPen(Qt::white));
    ui->Plot->yAxis->setTickLabelColor(Qt::white);
    ui->Plot->yAxis->grid()->setPen(QPen(Qt::gray));
    ui->Plot->yAxis->grid()->setSubGridPen(QPen(Qt::darkGray));
    ui->Plot->yAxis->setTickLabelSide(QCPAxis::lsInside);

    // Connect range change signals
    connect(ui->Plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateGridInterval()));
    connect(ui->Plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateGridInterval()));

    ui->Plot->xAxis->setRange(0, 100);
    ui->Plot->yAxis->setRange(0, 100);

    ui->Plot->replot();
}


void TestWindow::startSocketServer()
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &TestWindow::onNewConnection);

    quint16 port = 8888;  // Use any free port
    if (!server->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(this, "Server Error", "Failed to start server: " + server->errorString());
    } else {
        qDebug() << "Server listening on port" << port;
    }
}

void TestWindow::onNewConnection()
{
    client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &TestWindow::onSocketReadyRead);
    qDebug() << "Client connected from" << client->peerAddress().toString();
}

void TestWindow::onSocketReadyRead()
{
    QByteArray data = client->readAll();
    if (data.isEmpty()) return;

    quint8 key = static_cast<quint8>(data.at(0));

    qDebug() << "Received key over socket (hex):" << QString("0x%1").arg(key, 2, 16, QLatin1Char('0')).toUpper();

    switch (key)
    {
    case 0x01:
        receivedChannel = "1";
        ui->lineEdit_ch->setText(receivedChannel);
        entry.channel = receivedChannel.toInt();
        break;

    case 0x02:
        receivedChannel = "2";
        ui->lineEdit_ch->setText(receivedChannel);
        entry.channel= receivedChannel.toInt();
        break;

    case 0x03: //Freeze
        break;

    case 0x04: // Velocity
        handleSingleDoublePress(0x04, ui->lineEdit_Velocity, "5920", "3230");
        break;

    case 0x05: //Zoom
        break;

    case 6:
        qDebug() << "Entered into calset";
        ui->lineEdit_calset->setEnabled(true);
        ui->lineEdit_calset->clear();
        ui->lineEdit_calset->setFocus();
        calsetStr.clear();
        calsetState.pressCount = 0;
        calsetState.timer.start(); // start timing from now

        inputStates[ui->lineEdit_calset].inputBuffer.clear();
        inputStates[ui->lineEdit_calset].timer.start();
        break;

    case 0x30: case 0x31: case 0x32: case 0x33: case 0x34:
    case 0x35: case 0x36: case 0x37: case 0x38: case 0x39:
    case 0x41: case 0x42: case 0x43: case 0x44: case 0x45:
    case 0x46: case 0x47: case 0x48: case 0x49: case 0x4a:
    case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54:
    case 0x55: case 0x56: case 0x57: case 0x58: case 0x59:
    case 0x5a: case 0x2d:

        if (key == 0x31 || (key >= 0x41 && key <= 0x43))  // '1', 'A'-'C'
        {
            key = 0x31;  // Group as '1'
        }
        else if (key == 0x32 || (key >= 0x44 && key <= 0x46))  // '2', 'D'-'F'
        {
            key = 0x32;
        }
        else if (key == 0x33 || (key >= 0x47 && key <= 0x49))  // '3', 'G'-'I'
        {
            key = 0x33;
        }
        else if (key == 0x34 || (key >= 0x4a && key <= 0x4c))  // '4', 'j'-'l'
        {
            key = 0x34;
        }
        else if (key == 0x35 || (key >= 0x4d && key <= 0x4f))  // '5', 'm'-'o'
        {
            key = 0x35;
        }
        else if (key == 0x36 || (key >= 0x50 && key <= 0x53))  // '6', 'p'-'s'
        {
            key = 0x36;
        }
        else if (key == 0x37 || (key >= 0x54 && key <= 0x56))  // '7', 't'-'v'
        {
            key = 0x37;
        }
        else if (key == 0x38 || (key >= 0x57 && key <= 0x5a))  // '8', 'w'-'z'
        {
            key = 0x38;
        }
        else if (key == 0x39 || (key == 0x2d))  // '9', '-'
        {
            key = 0x39;
        }


        qDebug() << "calset keypress";
        handleDigitInput(key - 0x30);
        break;

    case 0x07:
        handleSingleDoublePress(0x07, ui->lineEdit_Mode, "SC", "DC");
        break;

    case 0x08: qDebug() << "AUD" << key;
        break;

    case 0x09 :  qDebug() << "RUN" << key;
        onApplyGainClicked();               //gain values implemented
        break;

    case 0xA: // SAVE key
        saveflg = 0;

        qDebug() << "SAVE key pressed. Count =" << cnt;

        if(!updateConfigFile("Config.txt",entry))
        {
            QMessageBox::warning(this,"Error","Failed to update the config");
        }

        if (!testdetails)
        {
            qDebug() << "Creating TestDetails screen.";
            testdetails = new TestDetails(this);

            // Disable input fields in TestWindow
            setInputFieldsEnabled(false);

            // Ensure TestWindow inputs re-enable after closing TestDetails
            connect(testdetails, &QObject::destroyed, this, [this]() {
                qDebug() << "TestDetails closed.";
                setInputFieldsEnabled(true);  // ✅ Re-enable main screen inputs
                testdetails = nullptr;
                cnt = 0;
            });
        }

        if (cnt == 0) {
            cnt++;
            int result = testdetails->exec();  // ✅ Modal
            qDebug() << "TestDetails closed with result:" << result;

            delete testdetails;
            testdetails = nullptr;
            cnt = 0;
        } else if (cnt == 1)
        {
            if (testdetails)
                testdetails->PreviewClick();
            cnt = 0;
        }
         break;

    case 0xB:
        if (testdetails && testdetails->isVisible()) {
            testdetails->handleSocketKey(key);
        }
        else {
            qDebug() << "UP ARROW" << key;
            navigateFocusVertical(-1, firstNav);
            firstNav = false;
        }
        break;

    case 0xC:
        if (testdetails && testdetails->isVisible()) {
            testdetails->handleSocketKey(key);
        } else {
            qDebug() << "DOWN ARROW" << key;
            navigateFocusVertical(1, firstNav);
            firstNav = false;
        }
        break;

    case 0xD: // Left Arrow
        break;

    case 0xE: // Ok
        break;

    case 0xF: //Back
        handleBackspaceInput();
        break;

    case 0x10: //ESC
        qDebug() << "ESC key pressed";

        if (testdetails)
        {
            // If preview is open inside testdetails
            if (testdetails->isPreviewOpen())
            {
                testdetails->closePreview(); // Your function to close preview
            }
            else
            {
                testdetails->reject();  // Close the TestDetails screen
                testdetails = nullptr; // Clean pointer
                setInputFieldsEnabled(true); // Restore main screen input
                cnt = 0;
            }
        }
        break;




    case 0x11: // '-'

        if (testdetails && testdetails->isVisible()) {
            testdetails->handleSocketKey(key);
        }
        else
        {
            adjustCurrentLineEdit(-1);
        }
        break;

    case 0x12: //'+'
        if (testdetails && testdetails->isVisible()) {
            testdetails->handleSocketKey(key);
        }
        else
        {
            adjustCurrentLineEdit(1);
        }
        break;

    case 0x13: // Right arrow
        break;

    default:
        if (testdetails && testdetails->isVisible())
        {
            qDebug() << "handle sockey key event" << key;
            testdetails->handleSocketKey(key);
        }

        break;
    }

}
void TestWindow::autoRunConfig()
{
    int Userinputs_pre[3];

    Userinputs_pre[0] = ui->lineEdit_ch->text().toInt();        // channel
    Userinputs_pre[1] = ui->lineEdit_calset->text().toInt();    // calset

    if (Userinputs_pre[0] != userinput_prv[0] || Userinputs_pre[1] != userinput_prv[1])
    {
        getconfig(Userinputs_pre[1], Userinputs_pre[0], &config);

        qDebug() << "Changed config inputs";
        qDebug() << "pre Ch" << Userinputs_pre[0] << "pre Cal" << Userinputs_pre[1];
        qDebug() << "prv Ch" << userinput_prv[0] << "prv Cal" << userinput_prv[1];

        // Update previous
        userinput_prv[0] = Userinputs_pre[0];
        userinput_prv[1] = Userinputs_pre[1];
    }


    config.reject = config.reject / REJECT_FACTOR;
    config.Gain   = config.Gain / GAIN_FACTOR;

    ui->lineEdit_Range->setText(QString::number(config.range, 'd', 0));
    ui->lineEdit_Delay->setText(QString::number(config.delay, 'd', 0));
    ui->lineEdit_Reject->setText(QString::number(config.reject, 'd', 0));
    ui->lineEdit_Gain->setText(QString::number(config.Gain));
    ui->lineEdit_Angle->setText(QString::number(config.Angle));

    ui->lineEdit_G1ST->setText(QString::number(config.g1_start));
    ui->lineEdit_G1ED->setText(QString::number(config.g1_end));
    ui->lineEdit_TH1->setText(QString::number(config.th1));

    ui->lineEdit_G2ST->setText(QString::number(config.g2_start));
    ui->lineEdit_G2ED->setText(QString::number(config.g2_end));
    ui->lineEdit_TH2->setText(QString::number(config.th2));
    ui->lineEdit_Velocity->setText(QString::number(velocity, 'd', 0));

    th1      = ui->lineEdit_TH1->text().toDouble();
    g1_start = ui->lineEdit_G1ST->text().toDouble();
    g1_end   = ui->lineEdit_G1ED->text().toDouble();

    th2     = ui->lineEdit_TH2->text().toDouble();
    g2_start = ui->lineEdit_G2ST->text().toDouble();
    g2_end   = ui->lineEdit_G2ED->text().toDouble();

    ui->Plot->xAxis->setRange(0, config.range);  // optional
    ui->Plot->yAxis->setRange(0, 100);

    saveConfigToFile();

}

void TestWindow::updateGraphWithData()
{

    if (!shared) return;

    QVector<QPointF> filteredPoints;
    if (getFilteredPoints(config, filteredPoints) == 0) return;

    QVector<double> xData, yData;
    maxX = 0;
    maxY = 0;

    for (const QPointF& point : filteredPoints)
    {
        double address = point.x();  // Already normalized address
        double scaledY = (point.y() / 255.0) * 100.0;

        xData.append(address);
        yData.append(scaledY);

        maxX = qMax<int>(maxX, static_cast<int>(address));
        maxY = qMax<double>(maxY, scaledY);

        //qDebug() << "x-axis" << address << "y-axis" << scaledY;
    }

    if (xData.isEmpty()) return;

    // No need to subtract delay * DELAY_FACTOR here, already done
    QVector<double> xDataNormalized;
    for (double addr : xData)
        xDataNormalized.append(addr / RANGE_FACTOR);

    double normG1Start = (g1_start * config.range)/100;
    double normG1End   = (g1_end * config.range)/100;

    double normG2Start = (g2_start * config.range) / 100.0;
    double normG2End   = (g2_end * config.range) / 100.0;

    double ph1Value = 0, ph2Value = 0;
    int peakIndex1 = -1, peakIndex2 = -1;

    for (int i = 0; i < xDataNormalized.size(); ++i)
    {
        double x = xDataNormalized[i];
        if (x >= normG1Start && x <= normG1End)
        {
            if (yData[i] > ph1Value) peakIndex1 = i;
            ph1Value = qMax(ph1Value, yData[i]);
        }
        if (x >= normG2Start && x <= normG2End)
        {
            if (yData[i] > ph2Value) peakIndex2 = i;
            ph2Value = qMax(ph2Value, yData[i]);
        }
    }

    double bp1Value = 0, bp2Value = 0;
    for (int i = peakIndex1; i >= 0; i--)
    {
        if (yData[i] == 0)
        {
            bp1Value = xDataNormalized[i];
            break;
        }
    }
    for (int i = peakIndex2; i >= 0; i--)
    {
        if (yData[i] == 0)
        {
            bp2Value = xDataNormalized[i];
            break;
        }
    }

    // Update UI
    ui->lineEdit_PH1->setText(QString::number(ph1Value, 'd', 0));
    ui->lineEdit_PH2->setText(QString::number(ph2Value, 'd', 0));
    ui->lineEdit_BP1->setText(QString::number(bp1Value, 'd', 0));
    ui->lineEdit_BP2->setText(QString::number(bp2Value, 'd', 0));

    ui->lineEdit_D1->setText(QString::number(bp1Value * cos(config.Angle * M_PI / 180.0), 'd', 0));
    ui->lineEdit_SD1->setText(QString::number(bp1Value * sin(config.Angle * M_PI / 180.0), 'd', 0));
    ui->lineEdit_D2->setText(QString::number(bp2Value * cos(config.Angle * M_PI / 180.0), 'd', 0));
    ui->lineEdit_SD2->setText(QString::number(bp2Value * sin(config.Angle * M_PI / 180.0), 'd', 0));

    ui->lineEdit_P2P->setText(QString::number(
        abs(ui->lineEdit_D1->text().toInt() - ui->lineEdit_D2->text().toInt()), 'd', 0));

    // ------------------ Plot Drawing ------------------
    ui->Plot->clearGraphs();

    // Threshold lines (no delay offset needed anymore)
    ui->Plot->addGraph();
    ui->Plot->graph(0)->setData(QVector<double>{normG1Start,normG1End},
    QVector<double>{th1, th1});
    //qDebug() <<"Gs"<<normG1Start <<"GE"<<normG1End;
    ui->Plot->graph(0)->setPen(QPen(QColor("#219601"), 2)); // Green

    ui->Plot->addGraph();
    ui->Plot->graph(1)->setData(QVector<double>{normG2Start , normG2End},
     QVector<double>{th2, th2});
    ui->Plot->graph(1)->setPen(QPen(QColor("#0818ff"), 2)); // Blue


    qDebug() <<"Plot\n ";
    // Actual waveform

    ui->Plot->addGraph();
    ui->Plot->graph(2)->setData(xDataNormalized, yData);
    ui->Plot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->Plot->graph(2)->setPen(QPen(Qt::yellow, 2));

    ui->Plot->replot();

}


bool TestWindow::updateConfigFile(const QString &filePath, const ConfigEntry &updatedConfig)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open config file for reading:" << file.errorString();
        return false;
    }

    QStringList lines;
    QTextStream in(&file);
    bool lineUpdated = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.trimmed().split(",", Qt::SkipEmptyParts);

        if (parts.size() == 13) {
            int calset = parts[0].toInt();
            int channel = parts[1].toInt();

            qDebug() << "Calset:" << calset << "Channel:" << channel;
            qDebug() << "usercalset:" << updatedConfig.calset << "userChannel:" << updatedConfig.channel;

            if (calset == updatedConfig.calset && channel == updatedConfig.channel) {
                qDebug() << "Matched line found for update:" << line;

                // Extract existing values
                double range =updatedConfig.range;
                double delay = updatedConfig.delay;
                double reject = updatedConfig.reject;
                double gain = updatedConfig.Gain;

                // Update config line
                line = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,")
                           .arg(calset)
                           .arg(channel)
                           .arg(static_cast<int>(range))
                           .arg(static_cast<int>(delay))
                           .arg(static_cast<int>(reject))
                           .arg(updatedConfig.g1_start)
                           .arg(updatedConfig.g1_end)
                           .arg(updatedConfig.th1)
                           .arg(updatedConfig.g2_start)
                           .arg(updatedConfig.g2_end)
                           .arg(updatedConfig.th2)
                           .arg(static_cast<int>(gain))
                           .arg(static_cast<int>(updatedConfig.Angle));
                lineUpdated = true;
            }
        }
        lines << line;
    }
    file.close();

    if (!lineUpdated) {
        qWarning() << "No matching calset/channel found in config to update";
        return false;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Cannot open config file for writing:" << file.errorString();
        return false;
    }

    QTextStream out(&file);
    for (const QString &line : lines)
    {
        out << line << "\n";
    }
    file.close();

    qDebug() << "Config file updated successfully.";
    return true;
}

void TestWindow::saveTo_entry()
{
    // bool ok;
    float value = ui->lineEdit_Gain->text().toFloat();
    //float value = text.toFloat();

    //if (ok) {
    // Round to nearest 0.5
    float roundedValue = qRound(value * 2) / 2.0;
    // Clamp to range [0.0, 80.0]
    if (roundedValue < 0.0) {
        roundedValue = 0.0;
    }
    else if (roundedValue > 80.0) {
        roundedValue = 80.0;
    }
    // Set the corrected value back to the line edit
    ui->lineEdit_Gain->setText(QString::number(roundedValue, 'f', 1));

    //  } else{
    // Handle invalid input
    //   ui->lineEdit_Gain->setText("0.0"); // Or retain the last valid value
    // }
    double rangeval= ui->lineEdit_Range->text().toDouble();
    if (rangeval<=50.0){
        rangeval=50;
    }
    else if(rangeval>=5000.0)
    {
        rangeval=5000;
    }
    // else{
    //     rangeval=50;
    // }

    double delayval= ui->lineEdit_Delay->text().toDouble();
    if (delayval<=0.0){
        delayval=0;
    }
    else if(delayval>=3000.0)
    {
        delayval=3000;
    }


    double rejectval=ui->lineEdit_Reject->text().toDouble();
    if (rejectval<=0.0){
        rejectval=0;
    }
    else if(rejectval>=80.0)
    {
        rejectval=80;
    }

    int th1val=ui->lineEdit_TH1->text().toInt();
    if (th1val<=5){
        th1val=5;
    }
    else if(th1val>=99)
    {
        th1val=99;
    }

    int g1stval=ui->lineEdit_G1ST->text().toInt();
    int g1edval=ui->lineEdit_G1ED->text().toInt();

    if (g1stval<0){
        g1stval=0;
    }
    else if(g1stval>100)
    {
        g1stval=100;
    }

    if (g1edval<0){
        g1edval=0;
    }
    else if(g1edval>100)
    {
        g1edval=100;
    }

    if(g1stval>g1edval)
    {
        //QMessageBox::warning(this, "Invalid Gate", "Gate 1 Start must be less than Gate 1 end");
    }
    else{
        entry.g1_start=g1stval;
        entry.g1_end=g1edval;
    }

    int g2stval=ui->lineEdit_G2ST->text().toInt();
    int g2edval=ui->lineEdit_G2ED->text().toInt();

    if (g2stval<0){
        g2stval=0;
    }
    else if(g2stval>100)
    {
        g2stval=100;
    }

    if (g2edval<0){
        g2edval=0;
    }
    else if(g2edval>100)
    {
        g2edval=100;
    }

    if(g2stval>g2edval)
    {
        //QMessageBox::warning(this, "Invalid Gate", "Gate 2 Start must be less than Gate 2 end");
    }
    else{
        entry.g2_start=g2stval;
        entry.g2_end=g2edval;
    }
    int th2val=ui->lineEdit_TH2->text().toInt();
    if (th2val<=5){
        th2val=5;
    }
    else if(th2val>=99)
    {
        th2val=99;
    }

    MachNo= ui->lineEdit_Machine->text();

    entry.calset=ui->lineEdit_calset->text().toInt();
    entry.channel=ui->lineEdit_ch->text().toInt();


    entry.th1=th1val;
    entry.th2=th2val;

    entry.Gain = roundedValue * GAIN_FACTOR;
    entry.reject=rejectval*REJECT_FACTOR;

    entry.Angle=ui->lineEdit_Angle->text().toFloat();
    entry.range=rangeval;
    entry.delay=delayval;


    mode=ui->lineEdit_Mode->text();
    config=entry;
    autoRunConfig();
 }


void TestWindow::updateGridInterval()
{
    // Update X-axis
    double xLower = ui->Plot->xAxis->range().lower;
    double xUpper = ui->Plot->xAxis->range().upper;
    double xStep = (xUpper - xLower) / 10; // 10 intervals

    QSharedPointer<QCPAxisTickerFixed> xTicker = ui->Plot->xAxis->ticker().staticCast<QCPAxisTickerFixed>();
    xTicker->setTickStep(xStep);

    // Update Y-axis
    double yLower = ui->Plot->yAxis->range().lower;
    double yUpper = ui->Plot->yAxis->range().upper;
    double yStep = (yUpper - yLower) / 10; // 10 intervals

    QSharedPointer<QCPAxisTickerFixed> yTicker = ui->Plot->yAxis->ticker().staticCast<QCPAxisTickerFixed>();
    yTicker->setTickStep(yStep);

    ui->Plot->replot();
}

void TestWindow::handleSingleDoublePress(int keyId, QLineEdit* lineEdit, const QString& singleVal, const QString& doubleVal)
{
    auto& state = keyStates[keyId];
    qint64 elapsed = state.timer.elapsed();

    if (!state.timer.isValid() || elapsed > doubleClickThresholdMs) {
        // First press or timed out
        state.pressCount = 1;
        state.timer.start();
        lineEdit->setText(singleVal);
    }
    else if (elapsed <= doubleClickThresholdMs && state.pressCount == 1) {
        // Second press within threshold
        lineEdit->setText(doubleVal);
        state.pressCount = 0;  // Reset
        state.timer.invalidate();
    }
}

void TestWindow::navigateFocusVertical(int direction, bool forceStartFromGain)
{
    QList<QWidget*> navWidgets = {
        ui->lineEdit_Gain,
        ui->lineEdit_Range,
        ui->lineEdit_Delay,
        ui->lineEdit_Reject,
        ui->lineEdit_Angle,
        ui->lineEdit_G1ST,
        ui->lineEdit_G1ED,
        ui->lineEdit_TH1,
        ui->lineEdit_G2ST,
        ui->lineEdit_G2ED,
        ui->lineEdit_TH2
    };

    QWidget *current = focusWidget();
    int index = navWidgets.indexOf(current);

    // If forced to start from Gain or current focus is not part of our list
    if (forceStartFromGain || index == -1) {
        index = 0;  // Start at Gain
    }

    int newIndex = index + direction;

    if (newIndex >= 0 && newIndex < navWidgets.size()) {
        navWidgets[newIndex]->setFocus();
        qDebug() << "Moved focus from" << navWidgets[index]->objectName()
                 << "to" << navWidgets[newIndex]->objectName();
    } else {
        navWidgets[index]->setFocus(); // Stay at the current valid index
        qDebug() << "Focus at boundary. Staying at" << navWidgets[index]->objectName();
    }
}


void TestWindow::handleDigitInput(int digit)
{
    qDebug() << "handle Digital input with:" << digit;

    QLineEdit* focused = qobject_cast<QLineEdit*>(this->focusWidget());
    if (!focused || !focused->isEnabled() )
    {
        qDebug() << "No valid focused line edit.";
        return;
    }
    // Ensure input state is initialized for this field
    if (!inputStates.contains(focused))
    {
        qDebug() << "Input state not found for focused widget.";
        return;
    }

    KeyPressState &state = inputStates[focused];

    // Start fresh if last key was more than 3s ago
    if (!state.timer.isValid() || state.timer.elapsed() > 3000)
    {
        state.inputBuffer.clear();
    }

    state.inputBuffer += QString::number(digit);
    bool ok = false;
    int value = state.inputBuffer.toInt(&ok);

    if (!ok)
    {
        qDebug() << "Failed to convert buffer to int:" << state.inputBuffer;
        return;
    }

    int min = 0, max = 999; // default
    if (focused == ui->lineEdit_calset) { min = 1; max = 100; }
    else if (focused == ui->lineEdit_Gain)   { min = 0; max = 80; }
    else if (focused == ui->lineEdit_Range)  { min = 0; max = 5000; }
    else if (focused == ui->lineEdit_Delay)  { min = 0; max = 3000; }
    else if (focused == ui->lineEdit_Reject) { min = 0; max = 100; }
    else if (focused == ui->lineEdit_Angle) { min = 0; max = 100; }
    else if (focused == ui->lineEdit_G1ST)  { min = 0; max = 100; }
    else if (focused == ui->lineEdit_G1ED) { min = 0; max = 100; }
    else if (focused == ui->lineEdit_TH1)  { min = 0; max = 100; }
    else if (focused == ui->lineEdit_G2ST)  { min = 0; max = 100; }
    else if (focused == ui->lineEdit_G2ED) { min = 0; max = 100; }
    else if (focused == ui->lineEdit_TH2)  { min = 0; max = 100; }

    if (value >= min && value <= max) {
        focused->setText(QString::number(value));
        state.timer.restart();
    }
    else
    {
        qDebug() << "Value out of range:" << value;
        state.inputBuffer.chop(1);  // Remove last digit
        return;
    }

    if (focused == ui->lineEdit_calset)
    {
        entry.calset = value;
    }

    if (focused == ui->lineEdit_Gain)
    {
        entry.Gain = value;
    }
    if (focused == ui->lineEdit_Range)
    {
        entry.range = value;
    }
    if (focused == ui->lineEdit_Delay)
    {
        entry.delay = value;
    }
    if (focused == ui->lineEdit_Reject)
    {
        receivedCalset = state.inputBuffer;
        entry.reject = value;
    }
    if (focused == ui->lineEdit_Angle)
    {
        entry.Angle = value;
    }
    if (focused == ui->lineEdit_G1ST)
    {
        entry.g1_start = value;
    }
    if (focused == ui->lineEdit_G1ED)
    {
        entry.g1_end = value;
    }
    if (focused == ui->lineEdit_TH1)
    {
        entry.th1 = value;
    }
    if (focused == ui->lineEdit_G2ST)
    {
        entry.g2_start = value;
    }
    if (focused == ui->lineEdit_G2ED)
    {
        entry.g2_end = value;
    }
    if (focused == ui->lineEdit_TH2)
    {
        entry.th2 = value;
    }
    else
    {
        qDebug() << "error";

    }

}

void TestWindow::adjustCurrentLineEdit(int delta)
{
    QLineEdit* focused = qobject_cast<QLineEdit*>(this->focusWidget());
    if (!focused || !focused->isEnabled())
        return;


    auto adjustValue = [&](auto& field, int minVal, int maxVal) {
        using T = std::decay_t<decltype(field)>;
        T current = static_cast<T>(focused->text().toDouble());
        T newValue = qBound(static_cast<T>(minVal), current + static_cast<T>(delta), static_cast<T>(maxVal));
        field = newValue;
        focused->setText(QString::number(newValue, 'f', std::is_floating_point<T>::value ? 1 : 0));
    };

    if (focused == ui->lineEdit_Gain)        adjustValue(entry.Gain,   0, 80);
    else if (focused == ui->lineEdit_Range)  adjustValue(entry.range,  0, 5000); // assume range is double
    else if (focused == ui->lineEdit_Delay)  adjustValue(entry.delay,  0, 100); // float or int
    else if (focused == ui->lineEdit_Reject) adjustValue(entry.reject, 0, 100);  // float or int
    else if (focused == ui->lineEdit_Angle)  adjustValue(entry.Angle,  0, 100);  // float
    else if (focused == ui->lineEdit_G1ST)   adjustValue(entry.g1_start, 0, 100); // int
    else if (focused == ui->lineEdit_G1ED)   adjustValue(entry.g1_end,   0, 100); // int
    else if (focused == ui->lineEdit_TH1)    adjustValue(entry.th1,      0, 100); // int
    else if (focused == ui->lineEdit_G2ST)   adjustValue(entry.g2_start, 0, 100); // int
    else if (focused == ui->lineEdit_G2ED)   adjustValue(entry.g2_end,   0, 100); // int
    else if (focused == ui->lineEdit_TH2)    adjustValue(entry.th2,      0, 100); // int


}


void TestWindow::handleBackspaceInput()
{
    QLineEdit* focused = qobject_cast<QLineEdit*>(this->focusWidget());
    if (!focused || !focused->isEnabled() || !inputStates.contains(focused))
        return;

    KeyPressState &state = inputStates[focused];

    if (state.inputBuffer.isEmpty())
        return;

    // Remove last digit
    state.inputBuffer.chop(1);

    // Apply value if valid or clear the box
    if (!state.inputBuffer.isEmpty()) {
        int value = state.inputBuffer.toInt();

        int min = 0, max = 999;
        if (focused == ui->lineEdit_calset) { min = 1; max = 100; }
        else if (focused == ui->lineEdit_Gain)   { min = 0; max = 80; }
        else if (focused == ui->lineEdit_Range)  { min = 0; max = 5000; }
        else if (focused == ui->lineEdit_Delay)  { min = 0; max = 100; }
        else if (focused == ui->lineEdit_Reject) { min = 0; max = 100; }
        else if (focused == ui->lineEdit_Angle)  { min = 0; max = 100; }
        else if (focused == ui->lineEdit_G1ST)   { min = 0; max = 100; }
        else if (focused == ui->lineEdit_G1ED)   { min = 0; max = 100; }
        else if (focused == ui->lineEdit_TH1)    { min = 0; max = 100; }
        else if (focused == ui->lineEdit_G2ST)   { min = 0; max = 100; }
        else if (focused == ui->lineEdit_G2ED)   { min = 0; max = 100; }
        else if (focused == ui->lineEdit_TH2)    { min = 0; max = 100; }

        if (value >= min && value <= max) {
            focused->setText(QString::number(value));
        } else {
            focused->clear();
        }

        // Restart timer and update data field
        state.timer.restart();
        if (focused == ui->lineEdit_calset) entry.calset = value;
        if (focused == ui->lineEdit_Gain)   entry.Gain = value;
        if (focused == ui->lineEdit_Range)  entry.range = value;
        if (focused == ui->lineEdit_Delay)  entry.delay = value;
        if (focused == ui->lineEdit_Reject) entry.reject = value;
        if (focused == ui->lineEdit_Angle)  entry.Angle = value;
        if (focused == ui->lineEdit_G1ST)   entry.g1_start = value;
        if (focused == ui->lineEdit_G1ED)   entry.g1_end = value;
        if (focused == ui->lineEdit_TH1)    entry.th1 = value;
        if (focused == ui->lineEdit_G2ST)   entry.g2_start = value;
        if (focused == ui->lineEdit_G2ED)   entry.g2_end = value;
        if (focused == ui->lineEdit_TH2)    entry.th2 = value;
    }
    else {
        focused->clear();
    }
}


void TestWindow::setInputFieldsEnabled(bool enabled)
{
    ui->lineEdit_calset->setEnabled(enabled);
    ui->lineEdit_Gain->setEnabled(enabled);
    ui->lineEdit_Range->setEnabled(enabled);
    ui->lineEdit_Delay->setEnabled(enabled);
    ui->lineEdit_Reject->setEnabled(enabled);
    ui->lineEdit_Angle->setEnabled(enabled);
    ui->lineEdit_G1ST->setEnabled(enabled);
    ui->lineEdit_G1ED->setEnabled(enabled);
    ui->lineEdit_TH1->setEnabled(enabled);
    ui->lineEdit_G2ST->setEnabled(enabled);
    ui->lineEdit_G2ED->setEnabled(enabled);
    ui->lineEdit_TH2->setEnabled(enabled);
    // add any others if needed
}

void TestWindow::onApplyGainClicked()
{
    ConfigEntry config_ch1, config_ch2;

    // Get calset and selected channel from UI
    int calset = ui->lineEdit_calset->text().toInt();
    bool ok1 = false, ok2 = false;

    int userGain = ui->lineEdit_Gain->text().toInt(&ok1);

    int selectedChannel = ui->lineEdit_ch->text().toInt(&ok2);

    int otherChannel = (selectedChannel == 1) ? 2 : 1;

    if (!ok1 || !ok2 || (selectedChannel != 1 && selectedChannel != 2)) {
        qDebug() << "Invalid gain or channel input!";
        return;
    }

    if (getconfig(calset, selectedChannel, &config_ch1) == 0)
    {
        int gain_val = userGain*GAIN_FACTOR;

        OperateGain(gain_val, selectedChannel);
        qDebug() << "user sel ch:" << gain_val<<selectedChannel;
    }

    if (getconfig(calset, otherChannel, &config_ch2) == 0)
    {
        int gain_val = static_cast<int>(temp.Gain);
        OperateGain(gain_val, otherChannel);
        qDebug() << "config file" << gain_val<< otherChannel;
    }

}

void TestWindow::loadSavedConfig()
{
    QFile file("/run/media/mmcblk1p1/config_saved.json");
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject config = doc.object();

    Postlog.channel = config["channel"].toInt();
    Postlog.calset  = config["calset"].toInt();


    // Update any internal fields or UI if necessary
    ui->lineEdit_ch->setText(QString::number(Postlog.channel));
    ui->lineEdit_calset->setText(QString::number(Postlog.calset));
}

void TestWindow::saveConfigToFile()
{
    QJsonObject config;
    config["channel"] = entry.channel;
    config["calset"]  = entry.calset;

    QFile file(QCoreApplication::applicationDirPath() + "/config_saved.json");
    qDebug() << "Saving to:" << QCoreApplication::applicationDirPath() + "/config_saved.json";

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(config).toJson());
        file.close();
    }
}

void TestWindow::setupValidators()
{
    ui->lineEdit_calset->setValidator(new QIntValidator(1, 100, this));
    ui->lineEdit_Gain->setValidator(new QIntValidator(0, 80, this));
    ui->lineEdit_Range->setValidator(new QIntValidator(0, 5000, this));
    ui->lineEdit_Delay->setValidator(new QIntValidator(0, 3000, this));
    ui->lineEdit_Reject->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_Angle->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_G1ST->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_G1ED->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_TH1->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_G2ST->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_G2ED->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_TH2->setValidator(new QIntValidator(0, 100, this));
}




TestWindow::~TestWindow()
{
    delete ui;
}

