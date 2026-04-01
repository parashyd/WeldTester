#include "testscreen.h"
#include "ui_testscreen.h"
#include "DataFile.h"
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QRegularExpression>
#include <QMap>
#include <cmath>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sched.h>
#include <QTimer>
#include <QtGlobal>
#include "matrix_keypad.h"
#include "Battery.h"
#include <QSvgRenderer>
#include <QPainter>
#include "Audio.h"
#include "openlog.h"
#include "previewscreen.h"

#define MAX_LINE 200
#define LINE_LENGTH 70

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

InputMode currentInputMode = InputMode_None;
QString MachNo,DC_SC_mode,calsetStr;


double ph1Value = 0, ph2Value = 0,phValue = 0,xPeakIndex1=-1,maxX, maxY,th1, g1_start, g1_end,th2, g2_start, g2_end;
float RANGE_FACTOR, DELAY_FACTOR = 3.4, m_audioPercent = 0;
//float RANGE_FACTOR_LT30 = 3.4;
float RANGE_FACTOR_LT30 = 3.3;
float RANGE_FACTOR_GT30 = 6.212;
int peakIndex =-1,peakIndex1 = -1, peakIndex2 = -1,UserVelocity,userGainVal,SEC,CP=1;
static int ModeCnt = 0,AudioLevel = 0,DACCnt = 0,CalGateCnt = 0;

bool freeze;
bool zoom = false;
bool gate1_focus=false;
bool gate2_focus=false;

QString receivedChannel,receivedCalset;
QMap<int, KeyPressState> keyStates;

KeyPressState calsetState;
QVector<double> xDataFreeze , yDataFreeze,DACx, DACy;
QVector<double> xData, yData;

Openlog *openlogScreen = nullptr;


TestScreen::TestScreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestScreen)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);         // to disable the menu bar

    this->setStyleSheet(R"(

    /* MAIN WINDOW */
    QMainWindow{
        background-color:#f4f6f8;
    }

    /* LABELS */
    QLabel{
        color:#2b2f33;
        font-size:13px;
        font-weight:500;
    }

    /* DAC Highlight */
    #label_DAC{
        color:#d32f2f;
        font-weight:bold;
    }

    /* INPUT BOXES */
    QLineEdit{
        background-color:#ffffff;
        border:1px solid #c7ccd1;
        border-radius:4px;
        color:#1f2328;
        padding:3px;
    }

    QLineEdit:focus{
        border:1px solid #1976d2;
        background-color:#ffffff;
    }

    /* PLOT AREA */
    #Plot{
        background-color:#ffffff;
        border:2px solid #d0d7de;
        border-radius:6px;
    }

    /* BUTTON */
    QPushButton{
        background-color:#1976d2;
        border-radius:5px;
        color:white;
        font-weight:bold;
        padding:5px;
    }

    QPushButton:hover{
        background-color:#1e88e5;
    }

    QPushButton:pressed{
        background-color:#1565c0;
    }

    /* FREEZE INDICATOR */
    #label_freeze{
        background-color:#e0e0e0;
        color:#2b2f33;
        border:1px solid #b0b0b0;
        border-radius:3px;
    }

    #label_pause{
        background-color:#e0e0e0;
        color:#2b2f33;
        border:1px solid #b0b0b0;
        border-radius:3px;
    }

    /* GENERAL FONT */
    QWidget{
        font-family: "Segoe UI";
    }

    )");

    ui->setupUi(this);
    DACx.resize(10);
    DACy.resize(10);



    QSettings settings("MyCompany", "MyApp");
    AudioLevel = settings.value("AudioLevel", 1).toInt(); // default = 1



    // Compute initial duty
    int duty = 200000 * AudioLevel;
    if (duty > 1000000) duty = 1000000;

    BuzzerDuty(duty);

    m_audioPercent = (float)duty / 1000000 * 100.0f;
    ui->label_audioIndicator->update();
    ui->label_audioIndicator->installEventFilter(this);
    BatteryIndication();

    plotUpdateTimer = new QTimer(this);
    BatteryUpdateTimer = new QTimer(this);

    ui->Plot->setOpenGl(true);

    setupPlotAppearance();

    // Create graphs ONCE (no indexing mistakes later)
    g1border = ui->Plot->addGraph();
    g2border = ui->Plot->addGraph();
    g1Line      = ui->Plot->addGraph();
    g2Line      = ui->Plot->addGraph();
    waveformGraph = ui->Plot->addGraph();
    DACline = ui->Plot->addGraph();




    // Appearance
    g1Line->setPen(QPen(QColor("#219601"), 2));  // Green
    g2Line->setPen(QPen(QColor("#0818ff"), 2));  // Blue
    //waveformGraph->setPen(QPen(Qt::yellow, 2));
    g1border->setPen(QPen(Qt::white, 6));;
    g2border->setPen(QPen(Qt::white, 6));;
    g1border->setVisible(false);
    g2border->setVisible(false);

    loadSavedConfig();
    UserVelocity = Postlog.velocity;

    RANGE_FACTOR = 3.4;
    ui->lineEdit_Mode->setText("SC");
    ui->label_DAC->setVisible(false);
    ui->lineEdit_CP->setVisible(false);

    autoRunConfig();
    onApplyGainClicked();


    startSocketServer();

    //connect(plotUpdateTimer, &QTimer::timeout, this, &TestScreen::updateGraphWithData);
    connect(plotUpdateTimer, &QTimer::timeout, this,     [this]() {
        static bool lastFreezeState = false;
        freeze = ui->label_freeze->isVisible();
        if (freeze && !lastFreezeState) {
            xDataFreeze.clear();
            yDataFreeze.clear();
        }
        lastFreezeState = freeze;

        if (ui->label_freeze->isVisible()) {
            waveformGraph->setLineStyle(QCPGraph::lsImpulse);
            handleFreezeLogic();
        }else{
             waveformGraph->setLineStyle(QCPGraph::lsLine);
            updateGraphWithData();
        }
    });
    // plotUpdateTimer->start(100);
    plotUpdateTimer->start(20);      //100msec, 10msec

    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* now){
        if (now) {
            setLogicalFocus(now);  // auto-highlight current focus
        }
    });



    connect(BatteryUpdateTimer, &QTimer::timeout, this,&TestScreen::BatteryIndication);
    BatteryUpdateTimer->start(3000);          // 3sec

    // In constructor or initialization
    connect(ui->lineEdit_G1ST, &QLineEdit::textChanged, this, [this](const QString &){
    g1_start = ui->lineEdit_G1ST->text().toDouble();        // update value
        updateGraphWithData();                              // redraw graph
    });

    connect(ui->lineEdit_G1ED, &QLineEdit::textChanged, this, [this](const QString &){
        g1_end = ui->lineEdit_G1ED->text().toDouble();
        updateGraphWithData();
    });

    connect(ui->lineEdit_TH1, &QLineEdit::textChanged, this, [this](const QString &){
        th1 = ui->lineEdit_TH1->text().toDouble();
        updateGraphWithData();
    });

    connect(ui->lineEdit_G2ST, &QLineEdit::textChanged, this, [this](const QString &){
        g2_start = ui->lineEdit_G2ST->text().toDouble();
        updateGraphWithData();                              // redraw graph
    });

    connect(ui->lineEdit_G2ED, &QLineEdit::textChanged, this, [this](const QString &){
        g2_end = ui->lineEdit_G2ED->text().toDouble();
        updateGraphWithData();
    });

    connect(ui->lineEdit_TH2, &QLineEdit::textChanged, this, [this](const QString &){
        th2 = ui->lineEdit_TH2->text().toDouble();
        updateGraphWithData();
    });


    ui->label_freeze->setVisible(false);//Make Freeze disable initially
    ui->label_pause->setVisible(false);//Make pause disable initially

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
    inputStates[ui->lineEdit_Angle]  = KeyPressState();
    inputStates[ui->lineEdit_G1ST]   = KeyPressState();
    inputStates[ui->lineEdit_G1ED]   = KeyPressState();
    inputStates[ui->lineEdit_TH1]    = KeyPressState();
    inputStates[ui->lineEdit_G2ST]   = KeyPressState();
    inputStates[ui->lineEdit_G2ED]   = KeyPressState();
    inputStates[ui->lineEdit_TH2]    = KeyPressState();
}

// In TestScreen.cpp
void TestScreen::setLogicalFocus(QWidget* widget)
{
    if (!widget) return;

    // 1️ Remove previous highlight
    if (m_currentLogicalFocus && m_currentLogicalFocus != widget) {
        // Reset style to default
        m_currentLogicalFocus->setStyleSheet("");
    }

    // 2️⃣ Apply highlight to the new widget
    widget->setStyleSheet(
        "background-color: rgb(255,255,150);"
        "border: 2px solid black;"
        );

    m_currentLogicalFocus = widget;

    // 3️⃣ Optional: attempt to give real Qt focus
    if (widget->isVisible() && widget->isEnabled()) {
        // For Wayland, this may not guarantee OS focus, but it's harmless
        widget->setFocus(Qt::OtherFocusReason);

        // If it's a QLineEdit, put cursor at start
        if (auto le = qobject_cast<QLineEdit*>(widget)) {
            le->setCursorPosition(0);
        }
    }

    // 4️⃣ Debug output
    qDebug() << "[LogicalFocus] Widget:" << widget->objectName()
             << "Visible:" << widget->isVisible()
             << "Enabled:" << widget->isEnabled()
             << "hasFocus:" << widget->hasFocus();
}




void TestScreen::setupPlotAppearance()
{
    ui->Plot->setFixedSize(521, 400);
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
    ui->Plot->yAxis->setTickLabelSide(QCPAxis::lsOutside);

    // Connect range change signals
    connect(ui->Plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateGridInterval()));
    connect(ui->Plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateGridInterval()));

    ui->Plot->xAxis->setRange(0, 100);
    ui->Plot->yAxis->setRange(0, 100);

    ui->Plot->replot();
}

void TestScreen::startSocketServer()
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &TestScreen::onNewConnection);

    quint16 port = 8888;  // Use any free port
    if (!server->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(this, "Server Error", "Failed to start server: " + server->errorString());
    } else {
        qDebug() << "Server listening on port" << port;
    }
}

void TestScreen::onNewConnection()
{
    client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &TestScreen::onSocketReadyRead);
    qDebug() << "Client connected from" << client->peerAddress().toString();
}

void TestScreen::onSocketReadyRead()
{
    QByteArray data = client->readAll();
    if (data.isEmpty()) return;

    quint8 key = static_cast<quint8>(data.at(0));
    qDebug() << "Received key (hex):" << QString("0x%1").arg(key, 2, 16, QLatin1Char('0')).toUpper();

    /* -----------------------------------------
     * 1) ESC key — highest priority handling
     * ----------------------------------------- */
    if (key == ESC)
    {
        qDebug() << "ESC key pressed";

        if (previewscreen && previewscreen->isVisible()) {
            previewscreen->close();
            previewscreen = nullptr;
            return;
        }
        if (testdetails && testdetails->isVisible()) {
            testdetails->close();
            testdetails = nullptr;
            if(!plotUpdateTimer->isActive())
            {
                plotUpdateTimer->start(20);
            }
            setInputFieldsEnabled(true);
            return;
        }
        if (openlogScreen && openlogScreen->isVisible()) {
            openlogScreen->handleRemoteKey(key);  // ESC closes inside Openlog
            return;
        }

        qDebug() << "ESC pressed on TestScreen (no popup)";
        return;
    }

    /* -----------------------------------------
     * 2) Forward key to the screen currently open
     *    Priority: Preview → TestDetails → Openlog
     * ----------------------------------------- */
    if (previewscreen && previewscreen->isVisible()) {
        previewscreen->handleSocketKey(static_cast<int>(key));
        return;
    }

    if (testdetails && testdetails->isVisible()) {
        testdetails->handleSocketKey(static_cast<int>(key));
        return;
    }

    if (openlogScreen && openlogScreen->isVisible()) {
        openlogScreen->handleRemoteKey(static_cast<int>(key));
        return;
    }

    /* -----------------------------------------
     * 3) No popup active → TestScreen key operations
     * ----------------------------------------- */
    switch (key)
    {
    case CH_A:
        receivedChannel = "1";
        ui->lineEdit_ch->setText(receivedChannel);
        entry.channel = 1;
        config.channel =1;
        autoRunConfig();
        break;

    case CH_B:
        receivedChannel = "2";
        ui->lineEdit_ch->setText(receivedChannel);
        entry.channel = 2;
        config.channel =2;
        autoRunConfig();
        break;

    case FREEZE: // Freeze
        if(ui->label_freeze->isVisible())
        {
           ui->label_freeze->setVisible(false);
        }
        else{
            ui->label_freeze->setVisible(true);
        }
        handleFreezeLogic();  // extracted to avoid long code here
        break;

    case VELOCITY:
        prepareVelocityInput();
        break;

    case ZOOM:
        if(zoom)
        {
            zoom=false;
        }
        else{
            zoom =true;
        }
        break;

    case CALSET:
        CalGateCnt ++;
        if(CalGateCnt == 1)
        {
            gate1_focus = false;
            gate2_focus = false;
            g1border->setVisible(false);
            g2border->setVisible(false);
            qDebug() <<"calset Active";
            prepareCalsetInput();
        }
        else if(CalGateCnt == 2)
        {
            //Gate1Input();
            ui->lineEdit_calset->setStyleSheet("");
            gate1_focus = true;
            gate2_focus = false;
            g1border->setVisible(true);
            g2border->setVisible(false);
        }
        else if(CalGateCnt == 3)
        {
            //Gate2Input();
            ui->lineEdit_calset->setStyleSheet("");
            gate1_focus = false;
            gate2_focus = true;
            g1border->setVisible(false);
            g2border->setVisible(true);
            CalGateCnt = 0;
        }
        break;

    case AUDIO:
        adjustAudioLevel();
        break;

    case RUN:
        if (plotUpdateTimer->isActive() )
        {
            plotUpdateTimer->stop();
            ui->label_pause->setVisible(true);
        }
        else{
            plotUpdateTimer->start(20);
            ui->label_pause->setVisible(false);
        }
        break;

    case SAVE:
        handleSaveFlow();
        break;

    case UP: // UP
        if(gate1_focus || gate2_focus)
        {
            HandleGateUpDownLift(1);
        }
        else{
            navigateFocusVertical(-1);
        }
        break;

    case DOWN: // DOWN
        if(gate1_focus || gate2_focus)
        {
            HandleGateUpDownLift(-1);
        }
        else{
            navigateFocusVertical(1);
        }
        break;

    case DAC:
        DACCnt ++;

        if(DACCnt == 1)
        {
            ui->label_DAC->setVisible(true);
            ui->lineEdit_CP->setVisible(true);  // make CP visible too
            ui->lineEdit_CP->setText(QString::number(CP));
            ui->lineEdit_CP->setFocus();   // <-- put focus directly on CP
        }

        else if(DACCnt == 2)
        {
            ui->label_DAC->setVisible(false);
            ui->lineEdit_CP->setVisible(false);
            DACCnt = 0;
            CP = 1 ;
            DACx.clear();
            DACy.clear();
            DACx.resize(10);
            DACy.resize(10);
            DrawDACCurve();
        }

        break;

    case SD_MODE:

        ModeCnt ++;

        if(ModeCnt == 1)
        {
            ui->lineEdit_Mode->setText("SC");
        }
        else if(ModeCnt == 2)
        {
            ui->lineEdit_Mode->setText("DC");
            ModeCnt = 0;
        }

        break;

    case OK:  // OK key
    {
        // Use the currently focused widget
        QWidget* focused = m_currentFocusedWidget ? m_currentFocusedWidget : this->focusWidget();
        if (!focused) return;

        // 1️⃣ If it's a button, click it
        if (auto btn = qobject_cast<QPushButton*>(focused))
        {
            btn->click();

            QStackedWidget *stackedWidget = this->parent()->findChild<QStackedWidget*>("stackedWidget");
            if (!stackedWidget) {
                openlogScreen = new Openlog();
                openlogScreen->setAttribute(Qt::WA_DeleteOnClose);
                openlogScreen->show();
                connect(openlogScreen, &QObject::destroyed, this, [this]() {
                    openlogScreen = nullptr;
                    setInputFieldsEnabled(true);
                });
                return;
            }

            if (!openlogScreen) {
                openlogScreen = new Openlog(stackedWidget);
                stackedWidget->addWidget(openlogScreen);
                connect(openlogScreen, &QObject::destroyed, this, [this]() {
                    openlogScreen = nullptr;
                    setInputFieldsEnabled(true);
                });
            }

            stackedWidget->setCurrentWidget(openlogScreen);
            return;
        }

        // 2️⃣ For line edits and other widgets, just save / apply
        qDebug() << "OK Pressed";
        // saveTo_entry();
        // autoRunConfig();
        // onApplyGainClicked();
    }
    break;


    case BACKSPACE: // Backspace
        handleBackspaceInput();
        break;

    case DEC: // '-'
        adjustCurrentLineEdit(-1);
        break;

    case INC: // '+'
        adjustCurrentLineEdit(1);
        break;

    case LEFT:
        if(gate1_focus || gate2_focus)
        {
            HandleGateShift(-1);
        }
        else{
            FunctionLeftRight(false);
        }
        break;

    case RIGHT:
        if(gate1_focus || gate2_focus)
        {
            HandleGateShift(1);
        }
        else{
            FunctionLeftRight(true);    //increment
        }
        break;


    default:
        if (testdetails && testdetails->isVisible())
        {
            //qDebug() << "handle sockey key event" << key;
            testdetails->handleSocketKey(key);
        }

        // 🔹 Decide based on current screen whether we want digits or alphas
        QLineEdit* focused = qobject_cast<QLineEdit*>(this->focusWidget());

        if (focused)
        {
            // Example: numeric-only fields
            if (focused == ui->lineEdit_calset ||
                focused == ui->lineEdit_Velocity||
                focused == ui->lineEdit_Gain   ||
                focused == ui->lineEdit_Range  ||
                focused == ui->lineEdit_Delay  ||
                focused == ui->lineEdit_Reject ||
                focused == ui->lineEdit_Angle  ||
                focused == ui->lineEdit_G1ST   ||
                focused == ui->lineEdit_G1ED   ||
                focused == ui->lineEdit_TH1    ||
                focused == ui->lineEdit_G2ST   ||
                focused == ui->lineEdit_G2ED   ||
                focused == ui->lineEdit_TH2)
            {
                int mapped = normalizeKeyForNumeric(key);
                if (mapped != -1)
                {

                    handleDigitInput(mapped - '0');  // '1' -> 1
                    saveTo_entry();
                    autoRunConfig();
                    onApplyGainClicked();
                }
                else
                {
                    //qDebug() << "Ignored non-numeric key:" << (char)key;
                }
            }
            else
            {
                // Example: alphabet fields (like operator name)
                handleOtherScreenKeys(key);
            }
        }
        break;

    }
}




void TestScreen::handleFreezeLogic(void)//Varun added to handle freeze logic
{
    if (!shared) return;

    QVector<QPointF> filtered;
    if (getFilteredPoints(config, filtered) == 0)
        return;

    // // -------- DEBUG FINAL INPUT --------
    // static int dbg = 0;
    // if ((dbg++ % 200) == 0)
    // {
    //     printf("[PLOT] %u:%u %u:%u %u:%u\n",
    //            DBG_I1, (DBG_I1 < filtered.size()) ? int(filtered[DBG_I1].y()) : 0,
    //            DBG_I2, (DBG_I2 < filtered.size()) ? int(filtered[DBG_I2].y()) : 0,
    //            DBG_I3, (DBG_I3 < filtered.size()) ? int(filtered[DBG_I3].y()) : 0);
    // }

    //QVector<double> xData, yData;
    // xDataFreeze.reserve(filtered.size());
    // yDataFreeze.reserve(filtered.size());
    xDataFreeze.resize(filtered.size());
    yDataFreeze.resize(filtered.size());

    maxX = 0;
    maxY = 0;

    // -----------------------------
    // Convert raw points → normalized arrays
    // -----------------------------
    // for (const QPointF& p : filtered)

    for (int i =0; i< filtered.size();i++)
    {
        double addr = filtered[i].x();
        double val  = (filtered[i].y() / 255.0) * 100.0;

        xDataFreeze[i]=addr;
        if(yDataFreeze[i]<val)
        {
            yDataFreeze[i] = val;
        }


        // maxX = qMax(maxX, int(addr));
        // maxY = qMax(maxY, val);

        maxX = qMax<double>(maxX, double(addr));
        maxY = qMax<double>(maxY, val);
    }

    if (xDataFreeze.isEmpty()) return;

    // Normalize X values
    QVector<double> xNorm;
    xNorm.reserve(xDataFreeze.size());
    // for (double a : xDataFreeze)
    //     xNorm.append(a / RANGE_FACTOR);

    for (double a : xDataFreeze)
    {
        config.Angle<=30 ? xNorm.append(a / RANGE_FACTOR_LT30) : xNorm.append(a / RANGE_FACTOR_GT30);

    }
    // -----------------------------
    // Compute G1/G2 range windows
    // -----------------------------
    double g1Start = (g1_start * config.range) / 100.0;
    double g1End   = (g1_end   * config.range) / 100.0;

    double g2Start = (g2_start * config.range) / 100.0;
    double g2End   = (g2_end   * config.range) / 100.0;

    // -----------------------------
    // Peak detection
    // -----------------------------
    phValue = 0;
    peakIndex = -1;

    for (int i = 0; i < xNorm.size(); i++)
    {
        if (yDataFreeze[i] > phValue)
        {
            peakIndex = i;
            phValue = qMax(phValue, yDataFreeze[i]);
        }
    }

    // -----------------------------
    // Update UI
    // -----------------------------

    int Lm=0,Rm=0, count = (int)config.range / 20;
    for(int i = peakIndex;i>=0;i--)
    {
        if(yDataFreeze[i] == 0)
        {
            if(count == ((int)config.range /20))
            {
                Lm = xNorm[i] * 100 / config.range;
            }
            else if(count == 0){
                break;
            }
            count--;
        }
        else{
            count = config.range / 20;
        }
    }

    count = config.range /20;

    for(int i =peakIndex;i<xNorm.size();i++)
    {
        if(yDataFreeze[i] == 0)
        {
            if(count == ((int)config.range /20))
            {
                Rm = xNorm[i] * 100 / config.range;
            }
            else if(count == 0){
                break;
            }
            count--;
        }
        else{
            count = config.range / 20;
        }
    }
    ui->lineEdit_PH2->hide();
    ui->lineEdit_BP2->hide();
    ui->lineEdit_D2->hide();
    ui->lineEdit_BP2->hide();
    ui->label_P2P->setText("Hm");
    ui->lineEdit_P2P->setText(QString::number(Lm)+"-"+QString::number(Rm));

    ui->lineEdit_PH1->setText(QString::number(ph1Value, 'f', 0));
    ui->lineEdit_BP1->setText(QString::number(xNorm[peakIndex], 'f', 0));
    ui->lineEdit_D1->setText(QString::number(xNorm[peakIndex] * cos(config.Angle * M_PI / 180.0), 'd', 0));
    ui->lineEdit_SD1->setText(QString::number(xNorm[peakIndex] * sin(config.Angle * M_PI / 180.0), 'd', 0));

    g1Line->data()->clear();
    g2Line->data()->clear();
    waveformGraph->data()->clear();

    // -----------------------------
    // Plot Update (no crashes)
    // -----------------------------
    g1Line->setData(QVector<double>{g1Start, g1End},
                    QVector<double>{th1, th1});

    g2Line->setData(QVector<double>{g2Start, g2End},
                    QVector<double>{th2, th2});
    waveformGraph->setData(xNorm, yDataFreeze);

    ui->Plot->replot();
}


void TestScreen::prepareVelocityInput(void)
{
    setLogicalFocus(ui->lineEdit_Velocity);

    ui->lineEdit_Velocity->setEnabled(true);
    ui->lineEdit_Velocity->clear();

    inputStates[ui->lineEdit_Velocity].timer.start();
    QString buffer = inputStates[ui->lineEdit_Velocity].inputBuffer;
}

void TestScreen::prepareCalsetInput()
{
    qDebug() << "Entered into calset";

    // 🔹 Logical focus + highlight
    setLogicalFocus(ui->lineEdit_calset);

    // 🔹 Enable the widget
    ui->lineEdit_calset->setEnabled(true);

    // 🔹 Clear or reset your input tracking
    calsetStr.clear();
    calsetState.pressCount = 0;
    calsetState.timer.start(); // start timing from now

    inputStates[ui->lineEdit_calset].inputBuffer.clear();
    inputStates[ui->lineEdit_calset].timer.start();
}

void TestScreen::adjustAudioLevel(void)
{

    AudioLevel++;  // increment count
    //qDebug() << "AUD" << key;
    if (AudioLevel > 5) AudioLevel = 0; // optional max count

    int duty = 0;
    if (AudioLevel > 0) {
        duty = 200000 * AudioLevel;
        if (duty > 1000000) duty = 1000000;
    }

    QSettings settings("MyCompany", "MyApp");
    settings.setValue("AudioLevel", AudioLevel);

    BuzzerDuty(duty);

    m_audioPercent = (float)duty / 1000000 * 100.0f;
    ui->label_audioIndicator->update();
}

void TestScreen::handleSaveFlow()
{
    saveTo_entry();  // Always save key press
    updateConfigFile("Config.txt",config);
    qDebug() << "Save button triggered";

    // -------- CASE 1: PreviewScreen is open --------
    if (previewscreen && previewscreen->isVisible()) {
        qDebug() << "Saving from PreviewScreen and returning to TestScreen";

        // ⬅️ Add preview save logic if required

        previewscreen->close();
        previewscreen = nullptr;

        if (testdetails) {
            testdetails->close();
            testdetails = nullptr;
        }
        if (!plotUpdateTimer->isActive())
        {
            plotUpdateTimer->start(20);
            if(ui->label_pause->isVisible()) // if pause is enabled and he have saved from preview screen the label should not be visible
            {
                ui->label_pause->setVisible(false);
            }
        }

        setInputFieldsEnabled(true);
        return;
    }

    // -------- CASE 2: TestDetails is open --------
    if (testdetails && testdetails->isVisible()) {
        qDebug() << "Saving from TestDetails";
        // ⬅️ Add TestDetail save logic if required
        // testdetails->saveDetails();
        return;
    }

    // -------- CASE 3: No screens open → open TestDetails --------
    qDebug() << "Opening TestDetails from TestScreen";

    if (!testdetails) {
        testdetails = new TestDetails(this);
        testdetails->setAttribute(Qt::WA_DeleteOnClose);

        if(plotUpdateTimer->isActive())
        {
            plotUpdateTimer->stop();
        }

        setInputFieldsEnabled(false);

        connect(testdetails, &QObject::destroyed, this, [this]() {
            qDebug() << "TestDetails closed";
            setInputFieldsEnabled(true);
            testdetails = nullptr;
        });

        // ---- Connect TestDetails → open PreviewScreen ----
        connect(testdetails, &TestDetails::requestPreview, this, [this]() {
            if (!previewscreen) {
                previewscreen = new PreviewScreen(this);
                previewscreen->setAttribute(Qt::WA_DeleteOnClose);

                // ✅ ADD THIS CONNECTION
                connect(previewscreen,&PreviewScreen::requestTestScreen,                            // fall back to Test screen
                        this,
                        &TestScreen::handleSaveFlow);

                connect(previewscreen, &QObject::destroyed, this, [this]() {
                    qDebug() << "PreviewScreen closed";
                    previewscreen = nullptr;
                });
            }
            previewscreen->show();
            // previewscreen->raise();
            // previewscreen->activateWindow(); //Commented bcoz of warning by wayland
            previewscreen->setDetails(MachNo, "XYZ");      // sample update
        });
    }

    testdetails->show();
    // testdetails->raise();
    // testdetails->activateWindow();
}

int TestScreen::normalizeKeyForNumeric(uint8_t key)
{
    //qDebug() << "normalfun\n";
    if (key >= '0' && key <= '9') {
        return key;  // already digit
    }

    switch (key)
    {

    case 'A': case 'B': case 'C': return '1';
    case 'D': case 'E': case 'F': return '2';
    case 'G': case 'H': case 'I': return '3';
    case 'J': case 'K': case 'L': return '4';
    case 'M': case 'N': case 'O': return '5';
    case 'P': case 'Q': case 'R': case 'S': return '6';
    case 'T': case 'U': case 'V': return '7';
    case 'W': case 'X': case 'Y': case 'Z': return '8';
    case '-': return '9';  // if you want '-' as 9

    }

    return -1;  // invalid
}



void TestScreen::handleOtherScreenKeys(uint8_t key)
{
    // Map letters and '-' to digits
    if (key == 0x31 || (key >= 0x41 && key <= 0x43))       key = 0x31;
    else if (key == 0x32 || (key >= 0x44 && key <= 0x46))  key = 0x32;
    else if (key == 0x33 || (key >= 0x47 && key <= 0x49))  key = 0x33;
    else if (key == 0x34 || (key >= 0x4A && key <= 0x4C))  key = 0x34;
    else if (key == 0x35 || (key >= 0x4D && key <= 0x4F))  key = 0x35;
    else if (key == 0x36 || (key >= 0x50 && key <= 0x53))  key = 0x36;
    else if (key == 0x37 || (key >= 0x54 && key <= 0x56))  key = 0x37;
    else if (key == 0x38 || (key >= 0x57 && key <= 0x5A))  key = 0x38;
    else if (key == 0x39 || (key == 0x2D))                 key = 0x39;

    //qDebug() << "calset keypress (mapped to digit):" << (char)key;
    handleDigitInput(key - 0x30);
}



void TestScreen::autoRunConfig()
{
    //qDebug() << "Entered into auto config";

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


    //config.reject = config.reject / REJECT_FACTOR;
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
    ui->lineEdit_Velocity->setText(QString::number(UserVelocity, 'd', 0));

    th1      = ui->lineEdit_TH1->text().toDouble();
    g1_start = ui->lineEdit_G1ST->text().toDouble();
    g1_end   = ui->lineEdit_G1ED->text().toDouble();

    th2     = ui->lineEdit_TH2->text().toDouble();
    g2_start = ui->lineEdit_G2ST->text().toDouble();
    g2_end   = ui->lineEdit_G2ED->text().toDouble();

    //qDebug() << "autoconf" << config.range;

    ui->Plot->xAxis->setRange(0, config.range);  // optional
    ui->Plot->yAxis->setRange(0, 100);

    saveConfigToFile();

}




void TestScreen::updateGraphWithData()
{
    if (!shared) return;

    QVector<QPointF> filtered;
    if (getFilteredPoints(config, filtered) == 0)
        return;

    // // -------- DEBUG FINAL INPUT --------
    // static int dbg = 0;
    // if ((dbg++ % 200) == 0)
    // {
    //     printf("[PLOT] %u:%u %u:%u %u:%u\n",
    //            DBG_I1, (DBG_I1 < filtered.size()) ? int(filtered[DBG_I1].y()) : 0,
    //            DBG_I2, (DBG_I2 < filtered.size()) ? int(filtered[DBG_I2].y()) : 0,
    //            DBG_I3, (DBG_I3 < filtered.size()) ? int(filtered[DBG_I3].y()) : 0);
    // }
    xData={0};
    yData={0};

    xData.resize(filtered.size());
    yData.resize(filtered.size());

    maxX = 0;
    maxY = 0;

    // -----------------------------
    // Convert raw points → normalized arrays
    // -----------------------------
    for (const QPointF& p : filtered)
    {
        double addr = p.x();
        double val  = (p.y() / 255.0) * 100.0;

        xData.append(addr);
        yData.append(val);

        // maxX = qMax(maxX, int(addr));
        // maxY = qMax(maxY, val);

        maxX = qMax<double>(maxX, double(addr));
        maxY = qMax<double>(maxY, val);
    }

    if (xData.isEmpty()) return;

    // Normalize X values
    QVector<double> xNorm;
    xNorm.reserve(xData.size());

    // for (double a : xData)
    //     xNorm.append(a / RANGE_FACTOR);

    for (double a : xData){
        config.Angle<=30 ? xNorm.append(a / RANGE_FACTOR_LT30) : xNorm.append(a / RANGE_FACTOR_GT30);
    }

    // -----------------------------
    // Compute G1/G2 range windows
    // -----------------------------
    double g1Start = (g1_start * config.range) / 100.0;
    double g1End   = (g1_end   * config.range) / 100.0;

    double g2Start = (g2_start * config.range) / 100.0;
    double g2End   = (g2_end   * config.range) / 100.0;

    // -----------------------------
    // Peak detection
    // -----------------------------
    ph1Value = ph2Value = 0;
    peakIndex1 = peakIndex2 = -1;

    for (int i = 0; i < xNorm.size(); i++)
    {
        double x = xNorm[i];

        if (x >= g1Start && x <= g1End)
        {
            if (yData[i] > ph1Value){
                peakIndex1 = i;
                xPeakIndex1=xNorm[i];
            }

            ph1Value = qMax(ph1Value, yData[i]);
        }

        if (x >= g2Start && x <= g2End)
        {
            if (yData[i] > ph2Value)
                peakIndex2 = i;

            ph2Value = qMax(ph2Value, yData[i]);
        }
    }

    // -----------------------------
    // BP detection
    // -----------------------------
    double bp1 = 0, bp2 = 0;

    if (peakIndex1 >= 0)
        for (int i = peakIndex1; i >= 0; i--)
            if (yData[i] == 0) { bp1 = xNorm[i]; break; }

    if (peakIndex2 >= 0)
        for (int i = peakIndex2; i >= 0; i--)
            if (yData[i] == 0) { bp2 = xNorm[i]; break; }

    // -----------------------------
    // Update UI
    // -----------------------------
    //VARUN Added since we are hiding these widgets while freezing
    if(!ui->lineEdit_PH2->isVisible())ui->lineEdit_PH2->setVisible(true);
    if(!ui->lineEdit_BP2->isVisible())ui->lineEdit_BP2->setVisible(true);
    if(!ui->lineEdit_D2->isVisible())ui->lineEdit_D2->setVisible(true);
    if(!ui->lineEdit_SD2->isVisible())ui->lineEdit_SD2->setVisible(true);
    ui->label_P2P->setText("P2P");
    //Varun ends

    ui->lineEdit_PH1->setText(QString::number(ph1Value, 'f', 0));
    ui->lineEdit_PH2->setText(QString::number(ph2Value, 'f', 0));
    ui->lineEdit_BP1->setText(QString::number(bp1, 'f', 0));
    ui->lineEdit_BP2->setText(QString::number(bp2, 'f', 0));

    double ang = config.Angle * M_PI / 180.0;

    double d1  = bp1 * cos(ang);
    double sd1 = bp1 * sin(ang);
    double d2  = bp2 * cos(ang);
    double sd2 = bp2 * sin(ang);

    ui->lineEdit_D1->setText(QString::number(d1, 'f', 0));
    ui->lineEdit_SD1->setText(QString::number(sd1, 'f', 0));
    ui->lineEdit_D2->setText(QString::number(d2, 'f', 0));
    ui->lineEdit_SD2->setText(QString::number(sd2, 'f', 0));
    ui->lineEdit_P2P->setText(QString::number(fabs(d1 - d2), 'f', 0));

    g1Line->data()->clear();
    g2Line->data()->clear();
    waveformGraph->data()->clear();

    // -----------------------------
    // Plot Update (no crashes)
    // -----------------------------
    if(zoom){
        ui->Plot->xAxis->setRange(g1Start,g1End);
        updateGridInterval();
        //ui->Plot->xAxis->setRange(g1_start*RANGE_FACTOR,g1_end*RANGE_FACTOR);
    }
    else{
        ui->Plot->xAxis->setRange(0,config.range);
        updateGridInterval();
    }


    g1Line->setData(QVector<double>{g1Start, g1End},
                    QVector<double>{th1, th1});

    g2Line->setData(QVector<double>{g2Start, g2End},
                    QVector<double>{th2, th2});

    if(gate1_focus){
        g1border->setData(QVector<double>{g1Start, g1End},
                      QVector<double>{th1, th1});
    }

    if(gate2_focus)
    {
        g2border->setData(QVector<double>{g2Start, g2End},
                    QVector<double>{th2, th2});
    }

    if (config.channel == 1){
        waveformGraph->setPen(QPen(Qt::magenta, 2));
    }
    else{
        waveformGraph->setPen(QPen(Qt::yellow, 2));
    }

    waveformGraph->setData(xNorm, yData);

    ui->Plot->replot();
}







bool TestScreen::updateConfigFile(const QString &filePath, const ConfigEntry &updatedConfig)
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

            // qDebug() << "Calset:" << calset << "Channel:" << channel;
            // qDebug() << "usercalset:" << updatedConfig.calset << "userChannel:" << updatedConfig.channel;

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

void TestScreen::saveTo_entry()
{
    auto clampDouble = [](double v, double min, double max)
    {
        return std::clamp(v, min, max);
    };

    auto clampInt = [](int v, int min, int max)
    {
        return std::clamp(v, min, max);
    };

    // ---------- Gain (0–80, step 0.5) ----------
    double gain = ui->lineEdit_Gain->text().toDouble();
    gain = qRound(gain * 2.0) / 2.0;     // round to nearest 0.5
    gain = clampDouble(gain, 0.0, 80.0);
    entry.Gain = gain * GAIN_FACTOR;

    // ---------- Range (50–1000) ----------
    double range = ui->lineEdit_Range->text().toDouble();
    range = clampDouble(range, 50.0, 1000.0);
    entry.range = range;

    // ---------- Delay (0–100) ----------
    double delay = ui->lineEdit_Delay->text().toDouble();
    delay = clampDouble(delay, 0.0, 100.0);
    entry.delay = delay;

    // ---------- Reject (0–80) ----------
    double reject = ui->lineEdit_Reject->text().toDouble();
    reject = clampDouble(reject, 0.0, 80.0);
    // entry.reject = reject * REJECT_FACTOR;
    entry.reject = reject;

    // ---------- Thresholds (5–99) ----------
    int th1 = clampInt(ui->lineEdit_TH1->text().toInt(), 5, 99);
    int th2 = clampInt(ui->lineEdit_TH2->text().toInt(), 5, 99);
    entry.th1 = th1;
    entry.th2 = th2;

    // ---------- Gate 1 (5–99) ----------
    int g1Start = clampInt(ui->lineEdit_G1ST->text().toInt(), 5, 99);
    int g1End   = clampInt(ui->lineEdit_G1ED->text().toInt(), 5, 99);

    if (g1Start <= g1End)
    {
        entry.g1_start = g1Start;
        entry.g1_end   = g1End;
    }

    // ---------- Gate 2 (5–99) ----------
    int g2Start = clampInt(ui->lineEdit_G2ST->text().toInt(), 5, 99);
    int g2End   = clampInt(ui->lineEdit_G2ED->text().toInt(), 5, 99);

    if (g2Start <= g2End)
    {
        entry.g2_start = g2Start;
        entry.g2_end   = g2End;
    }

    // ---------- Other fields ----------
    entry.Angle   = ui->lineEdit_Angle->text().toFloat();
    entry.calset  = ui->lineEdit_calset->text().toInt();
    entry.channel = ui->lineEdit_ch->text().toInt();

    MachNo       = ui->lineEdit_Machine->text();
    DC_SC_mode   = ui->lineEdit_Mode->text();

    config = entry;
}

void TestScreen::updateGridInterval()
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

void TestScreen::HandleGateUpDownLift(int lift){

    QLineEdit* Th;
    auto adjustlift = [&](auto& field, int minVal, int maxVal) {
        using T = std::decay_t<decltype(field)>;
        T current = static_cast<T>(Th->text().toDouble());
        T newValue = qBound(static_cast<T>(minVal), current + static_cast<T>(lift * 5), static_cast<T>(maxVal));
        field = newValue;
        Th->setText(QString::number(newValue, 'f', std::is_floating_point<T>::value ? 1 : 0));
    };

    if(gate1_focus)
    {
        Th = ui->lineEdit_TH1;
        adjustlift(entry.th1,5,99);
    }
    else
    {
        Th = ui->lineEdit_TH2;
        adjustlift(entry.th2,5,99);
    }
}
void TestScreen::HandleGateShift(int shift){

    auto adjustshift = [&](auto& field, int minVal, int maxVal,QLineEdit* L) {
        using T = std::decay_t<decltype(field)>;
        T current = static_cast<T>(L->text().toDouble());
        T newValue = qBound(static_cast<T>(minVal), current + static_cast<T>(shift * 5), static_cast<T>(maxVal));
        field = newValue;
        L->setText(QString::number(newValue, 'f', std::is_floating_point<T>::value ? 1 : 0));
    };
    // int width1 = g1_end-g1_start;
    // int width2 = g2_end-g2_start;
    if(gate1_focus)
    {
        if(shift == -1 && g1_start == 5) //if gate reaches starting point return
            return;

        if(shift == 1 && g1_end == 99)  //if gate reaches ending point return
            return;

        adjustshift(entry.g1_start,5,99,ui->lineEdit_G1ST);
        adjustshift(entry.g1_end,5,99,ui->lineEdit_G1ED);

    }
    else
    {
        if(shift == -1 && g2_start == 5) //if gate reaches starting point return
            return;

        if(shift == 1 && g2_end == 99)  //if gate reaches ending point return
            return;

        adjustshift(entry.g2_start,5,99,ui->lineEdit_G2ST);
        adjustshift(entry.g2_end,5,99,ui->lineEdit_G2ED);
    }

}

void TestScreen::navigateFocusVertical(int direction)
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
        ui->lineEdit_TH2,
        ui->pushButton
    };

    // 🔹 Trust Qt's real focus
    QWidget *current = focusWidget();

    // Fallback only if nothing focused
   if (!current || !navWidgets.contains(current)){
        current = ui->lineEdit_Gain;
        setLogicalFocus(current);
        return;
    }

    setLogicalFocus(current);
    int index = navWidgets.indexOf(current);

    int newIndex = index + direction;
    if (newIndex < 0)
        newIndex = navWidgets.size() - 1;
    else if (newIndex >= navWidgets.size())
        newIndex = 0;

    QWidget *newWidget = navWidgets[newIndex];

    setLogicalFocus(newWidget);
    // newWidget->setFocus(Qt::OtherFocusReason);

    // 🔹 Move cursor for LineEdit
    if (auto le = qobject_cast<QLineEdit*>(newWidget))
        le->setCursorPosition(le->text().length());

    qDebug() << "Moved focus from"
             << current->objectName()
             << "to"
             << newWidget->objectName();
}



void TestScreen::handleDigitInput(int digit)
{
    qDebug() << "handle Digit input:" << digit;

    QLineEdit* focused = qobject_cast<QLineEdit*>(this->focusWidget());
    if (!focused || !focused->isEnabled())
        return;

    if (!inputStates.contains(focused))
        return;

    KeyPressState &state = inputStates[focused];

    // Reset buffer if idle for more than 3 seconds
    if (!state.timer.isValid() || state.timer.elapsed() > 3000)
        state.inputBuffer.clear();

    // Append new digit
    state.inputBuffer += QString::number(digit);

    bool ok = false;
    int value = state.inputBuffer.toInt(&ok);
    if (!ok)
        return;

    // ---------- Determine limits ----------
    int min = 0, max = 999;

    if      (focused == ui->lineEdit_calset)   { min = 1;  max = 100; }
    else if (focused == ui->lineEdit_Velocity) { min = 0;  max = 9999; }
    else if (focused == ui->lineEdit_Gain)     { min = 0;  max = 80; }
    else if (focused == ui->lineEdit_Range)    { min = 50;  max = 1000; }
    else if (focused == ui->lineEdit_Delay)    { min = 0;  max = 100; }
    else if (focused == ui->lineEdit_Reject)   { min = 0;  max = 80; }
    else if (focused == ui->lineEdit_Angle)    { min = 0;  max = 90; }
    else if (focused == ui->lineEdit_G1ST ||
             focused == ui->lineEdit_G1ED ||
             focused == ui->lineEdit_TH1  ||
             focused == ui->lineEdit_G2ST ||
             focused == ui->lineEdit_G2ED ||
             focused == ui->lineEdit_TH2)
    {
        min = 5;
        max = 99;
    }

    // ---------- Allow partial typing ----------
    int maxDigits = QString::number(max).length();

    if (state.inputBuffer.length() > maxDigits || value > max)
    {
        state.inputBuffer.chop(1);
        return;
    }

    // Show exactly what user typed
    focused->setText(state.inputBuffer);
    state.timer.restart();

    // ---------- Update entry only if valid final range ----------

    if (value >max)
        value = max;


    if (value < min)
        value = min;

    if      (focused == ui->lineEdit_calset)   entry.calset   = value;
    else if (focused == ui->lineEdit_Velocity)
    {
        entry.velocity = value;
        UserVelocity = value;

        // if (value != 0)
        //     RANGE_FACTOR = 20000.0 / value;
    }
    else if (focused == ui->lineEdit_Gain)     entry.Gain     = value;
    else if (focused == ui->lineEdit_Range)    entry.range    = value;
    else if (focused == ui->lineEdit_Delay)    entry.delay    = value;
    else if (focused == ui->lineEdit_Reject)   entry.reject   = value;
    else if (focused == ui->lineEdit_Angle)    entry.Angle    = value;
    else if (focused == ui->lineEdit_G1ST)     entry.g1_start = value;
    else if (focused == ui->lineEdit_G1ED)     entry.g1_end   = value;
    else if (focused == ui->lineEdit_TH1)      entry.th1      = value;
    else if (focused == ui->lineEdit_G2ST)     entry.g2_start = value;
    else if (focused == ui->lineEdit_G2ED)     entry.g2_end   = value;
    else if (focused == ui->lineEdit_TH2)      entry.th2      = value;
}

void TestScreen::adjustCurrentLineEdit(int delta)
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

    if(gate1_focus){
        focused = ui->lineEdit_G1ED;
        if(g1_start==g1_end && delta ==-1){
            return;
        }
        if(g1_start<=g1_end)
            adjustValue(entry.g1_end, 5, 99);
        return;
    }
    else if(gate2_focus){
        focused=ui->lineEdit_G2ED;
        if(g2_start==g2_end && delta ==-1){
            return;
        }
        if(g2_start<=g2_end)
            adjustValue(entry.g2_end, 5, 99);
        return;
    }

    if (focused == ui->lineEdit_Gain)
    {       adjustValue(entry.Gain,   0, 80) ;
            onApplyGainClicked();
    }
    else if (focused == ui->lineEdit_Range)  {adjustValue(entry.range,  50, 1000);} // assume range is double
    else if (focused == ui->lineEdit_Delay)  adjustValue(entry.delay,  0, 100); // float or int
    else if (focused == ui->lineEdit_Reject) adjustValue(entry.reject, 0, 80);  // float or int
    else if (focused == ui->lineEdit_Angle)  adjustValue(entry.Angle,  0, 90);  // float
    else if (focused == ui->lineEdit_G1ST)   adjustValue(entry.g1_start, 5, 99); // int
    else if (focused == ui->lineEdit_G1ED)   adjustValue(entry.g1_end,   5, 99); // int
    else if (focused == ui->lineEdit_TH1)    adjustValue(entry.th1,      5, 99); // int
    else if (focused == ui->lineEdit_G2ST)   adjustValue(entry.g2_start, 5, 99); // int
    else if (focused == ui->lineEdit_G2ED)   adjustValue(entry.g2_end,   5, 99); // int
    else if (focused == ui->lineEdit_TH2)    adjustValue(entry.th2,      5, 99); // int

    else if (focused == ui->lineEdit_CP)
    {
        if(delta==1)
        {
            SaveDACPar();
            DrawDACCurve();
        }
        else if(delta == -1)
        {
            if(CP>=1 && CP<=10)
            {
                DACx[CP-1]=0;
                DACy[CP-1]=0;
                if(CP>1)CP=CP-1;
                ui->lineEdit_CP->setText(QString::number(CP));
                DrawDACCurve();
            }
        }
    }

}



void TestScreen::handleBackspaceInput()
{
    QLineEdit* focused = qobject_cast<QLineEdit*>(this->focusWidget());
    if (!focused || !focused->isEnabled() || !inputStates.contains(focused))
        return;

    KeyPressState &state = inputStates[focused];

    if (state.inputBuffer.isEmpty())
        return;

    // Remove last digit
    state.inputBuffer.chop(1);

    if (state.inputBuffer.isEmpty())
    {
        focused->clear();
        //return;
    }

    bool ok = false;
    int value = state.inputBuffer.toInt(&ok);
    if (!ok)
        return;

    // ---------- Limits ----------
    int min = 0, max = 999;

    if      (focused == ui->lineEdit_calset)   { min = 1;  max = 100; }
    else if (focused == ui->lineEdit_Velocity) { min = 0;  max = 9999; }
    else if (focused == ui->lineEdit_Gain)     { min = 0;  max = 80; }
    else if (focused == ui->lineEdit_Range)    { min = 50; max = 1000; }  // updated
    else if (focused == ui->lineEdit_Delay)    { min = 0;  max = 100; }
    else if (focused == ui->lineEdit_Reject)   { min = 0;  max = 80; }
    else if (focused == ui->lineEdit_Angle)    { min = 0;  max = 90; }
    else if (focused == ui->lineEdit_G1ST ||
             focused == ui->lineEdit_G1ED ||
             focused == ui->lineEdit_TH1  ||
             focused == ui->lineEdit_G2ST ||
             focused == ui->lineEdit_G2ED ||
             focused == ui->lineEdit_TH2)
    {
        min = 5;
        max = 99;
    }

    // ---------- Allow partial delete ----------
    if (value > max)
    {
        value = max;
        // focused->clear();
        // return;
    }

    // Show what remains
    focused->setText(state.inputBuffer);
    state.timer.restart();

    // Only update entry if within final valid range
    if (value < min)
    {
        value = min;
    }

    if      (focused == ui->lineEdit_calset)   entry.calset   = value;
    else if (focused == ui->lineEdit_Velocity) entry.velocity = value;
    else if (focused == ui->lineEdit_Gain)     entry.Gain     = value;
    else if (focused == ui->lineEdit_Range)    entry.range    = value;
    else if (focused == ui->lineEdit_Delay)    entry.delay    = value;
    else if (focused == ui->lineEdit_Reject)   entry.reject   = value;
    else if (focused == ui->lineEdit_Angle)    entry.Angle    = value;
    else if (focused == ui->lineEdit_G1ST)     entry.g1_start = value;
    else if (focused == ui->lineEdit_G1ED)     entry.g1_end   = value;
    else if (focused == ui->lineEdit_TH1)      entry.th1      = value;
    else if (focused == ui->lineEdit_G2ST)     entry.g2_start = value;
    else if (focused == ui->lineEdit_G2ED)     entry.g2_end   = value;
    else if (focused == ui->lineEdit_TH2)      entry.th2      = value;

}

void TestScreen::FunctionLeftRight(bool increment)
{
    QLineEdit* focused = qobject_cast<QLineEdit*>(focusWidget());
    if (focused != ui->lineEdit_Gain)
        return;

    double gain = focused->text().toDouble();

    if (increment)
        gain += 0.5;
    else
        gain -= 0.5;

    // Clamp
    gain = qBound(0.0, gain, 80.0);

    entry.Gain = gain;

    qDebug() <<  "decimalGain" << entry.Gain;

    focused->setText(QString::number(gain, 'f', 1));
}


void TestScreen::setInputFieldsEnabled(bool enabled)
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


void TestScreen::onApplyGainClicked()
{
    ConfigEntry updatedCfg;

    double userGain = ui->lineEdit_Gain->text().toDouble();   // 🔹 CHANGE
    int selectedCh  = ui->lineEdit_ch->text().toInt();
    int calset      = ui->lineEdit_calset->text().toInt();

    if (selectedCh != 1 && selectedCh != 2) {
        qWarning() << "Invalid channel selected";
        return;
    }

    int userGainVal = static_cast<int>(userGain * GAIN_FACTOR); // 🔹 SCALE AFTER DOUBLE

    int gain_ch1 = 0;
    int gain_ch2 = 0;

    ConfigEntry otherCfg;
    ConfigEntry selectedCfg;

    /* -------------------------------------------------
     * 1. Read config for both channels
     * ------------------------------------------------- */
    if (getconfig(calset, 1, &selectedCfg) != 0 ||
        getconfig(calset, 2, &otherCfg) != 0) {
        qWarning() << "Failed to read config file";
        return;
    }

    /* -------------------------------------------------
     * 2. Decide gains
     * ------------------------------------------------- */
    if (selectedCh == 1) {
        gain_ch1 = userGainVal;
        gain_ch2 = static_cast<int>(otherCfg.Gain);
    } else {
        gain_ch2 = userGainVal;
        gain_ch1 = static_cast<int>(selectedCfg.Gain);
    }

    qDebug() << "Applying gains -> CH1:" << gain_ch1
             << "CH2:" << gain_ch2;

    /* -------------------------------------------------
     * 3. Apply to hardware
     * ------------------------------------------------- */
    OperateGainDual(gain_ch1, gain_ch2);

    /* -------------------------------------------------
     * 4. Update ONLY selected channel in Config.txt
     * ------------------------------------------------- */
    if (getconfig(calset, selectedCh, &updatedCfg) != 0) {
        qWarning() << "Failed to reload selected channel config";
        return;
    }

    updatedCfg.Gain = static_cast<float>(userGainVal);  // 🔹 keep stored value scaled

    if (!updateConfigFile("Config.txt", updatedCfg)) {
        qWarning() << "Failed to update config file";
        return;
    }

    qDebug() << "Gain updated successfully in config file";
}

void TestScreen::loadSavedConfig()
{
    // QFile file("/run/media/mmcblk1p1/config_saved.json");
    QFile file("config_saved.json");

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject config = doc.object();

    if((config["channel"].toInt() != 0) && (config["calset"].toInt() != 0) && (config["velocity"].toDouble() != 0))
    {
        Postlog.channel = config["channel"].toInt();
        Postlog.calset  = config["calset"].toInt();
        Postlog.velocity = config["velocity"].toDouble();
    }

    else
    {
        Postlog.channel = 2;
        Postlog.calset  = 1;
        Postlog.velocity = 3230;

    }

    // Update any internal fields or UI if necessary
    ui->lineEdit_ch->setText(QString::number(Postlog.channel));
    ui->lineEdit_calset->setText(QString::number(Postlog.calset));
    ui->lineEdit_Velocity ->setText(QString::number(Postlog.velocity));
}

void TestScreen::saveConfigToFile()
{
    QJsonObject config;
    config["channel"] = entry.channel;
    config["calset"]  = entry.calset;
    config["velocity"] = UserVelocity;

    QFile file(QCoreApplication::applicationDirPath() + "/config_saved.json");
    qDebug() << "Saving to:" << QCoreApplication::applicationDirPath() + "/config_saved.json";

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(config).toJson());
        file.close();
    }
}

void TestScreen::setupValidators()
{
    ui->lineEdit_calset->setValidator(new QIntValidator(1, 100, this));
    ui->lineEdit_Gain->setValidator(new QIntValidator(0, 80, this));
    ui->lineEdit_Range->setValidator(new QIntValidator(50, 1000, this));
    ui->lineEdit_Delay->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_Reject->setValidator(new QIntValidator(0, 80, this));
    ui->lineEdit_Angle->setValidator(new QIntValidator(0, 90, this));
    ui->lineEdit_G1ST->setValidator(new QIntValidator(5, 99, this));
    ui->lineEdit_G1ED->setValidator(new QIntValidator(5, 99, this));
    ui->lineEdit_TH1->setValidator(new QIntValidator(5, 99, this));
    ui->lineEdit_G2ST->setValidator(new QIntValidator(5, 99, this));
    ui->lineEdit_G2ED->setValidator(new QIntValidator(5, 99, this));
    ui->lineEdit_TH2->setValidator(new QIntValidator(5, 99, this));
}


void TestScreen::BatteryIndication()
{
    float Update_value = Battery();

    if (Update_value < 0) Update_value = 0;
    if (Update_value > 100) Update_value = 100;

    int roundedPercent = (int)std::round(Update_value);

    updateBatteryLevel(roundedPercent);

    // // Update progress bar
    // ui->progressBar->setValue(roundedPercent);

}



void TestScreen::updateBatteryLevel(int percent)
{

    // Only update if value changed
    if (percent == lastBatteryPercent)
        return;

    lastBatteryPercent = percent;

    // Load the SVG outline
    QSvgRenderer renderer(QString(":/images/battery_outline.svg"));

    // Create a pixmap same size as the QLabel
    QPixmap pix(ui->label_battery->size());
    pix.fill(Qt::transparent);

    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);

    // Margin to keep fill inside the SVG outline (adjust if needed)
    int margin = 3;
    int nobeWidth = 6;  // adjust to match your SVG’s nobe
    int maxFill = (pix.width() - nobeWidth) - 2 * margin;

    // Calculate maximum fill width safely inside margins
    //int maxFill = (pix.width()- 4) - 2 * margin;
    int fillWidth = qBound(0, (maxFill * percent) / 100, maxFill);

    QRect fillRect(margin, margin, fillWidth, pix.height() - 2 * margin);

    // Pick fill color dynamically
    QColor fillColor;
    if (percent < 20)
        fillColor = Qt::red;
    else if (percent < 40)
        fillColor = Qt::blue;
    else
        fillColor = Qt::green;

    // Draw battery fill (under the outline)
    p.fillRect(fillRect, fillColor);

    // Draw the SVG outline scaled to QLabel size
    renderer.render(&p, pix.rect());

    // Draw numeric percentage in the center
    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 8, QFont::Bold));
    QString text = QString::number(percent) + "%";
    p.drawText(pix.rect(), Qt::AlignCenter, text);

    p.end();

    // Set result into the QLabel
    ui->label_battery->setPixmap(pix);
}

void TestScreen::paintAudioIndicator(QPainter &p)
{
    QWidget* w = ui->label_audioIndicator;
    int hgt = w->height();  // e.g. 41

    p.setRenderHint(QPainter::Antialiasing);

    // --- Step 1: Draw the audio icon ---
    QPixmap icon(":/images/audio.jpeg");
    QSize iconSize = icon.size();

    QRect iconRect(0, (hgt - iconSize.height()) / 2,
                   iconSize.width(), iconSize.height());
    p.drawPixmap(iconRect.topLeft(), icon);

    // --- Step 2: Calculate arc count ---
    int arcCount = 0;
    if (m_audioPercent >= 20) arcCount = 1;
    if (m_audioPercent >= 40) arcCount = 2;
    if (m_audioPercent >= 60) arcCount = 3;
    if (m_audioPercent >= 80) arcCount = 4;
    if (m_audioPercent >= 100) arcCount = 5;

    QPoint center(iconRect.right() + 3, hgt / 2);  // center for arcs
    QPen pen(Qt::darkBlue, 2);
    p.setPen(pen);

    int baseRadius = iconRect.height()/4;
    int gap = 4;

    if (arcCount > 0)
    {
        // --- Draw arcs ---
        for (int i = 0; i < arcCount; i++)
        {
            int radius = baseRadius + i * gap;

            QRect arcRect(center.x() - radius, center.y() - radius,
                          radius * 2, radius * 2);

            int startAngle = 45 * 16;
            int spanAngle  = -90 * 16;
            p.drawArc(arcRect, startAngle, spanAngle);
        }
    }
    else
    {
        QPen crossPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap);
        p.setPen(crossPen);

        int radius = iconRect.height() / 3;  // size of the X
        QRect crossRect(center.x() - radius, center.y() - radius,
                        radius * 2, radius * 2);

        // Draw X inside crossRect
        p.drawLine(crossRect.topLeft(), crossRect.bottomRight());
        p.drawLine(crossRect.bottomLeft(), crossRect.topRight());
    }
}



bool TestScreen::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->label_audioIndicator && event->type() == QEvent::Paint)
    {
        QPainter p(ui->label_audioIndicator);
        paintAudioIndicator(p);
        return true;
    }
    return QWidget::eventFilter(obj, event);
}



void TestScreen::CloseDAC()
{
    ui->label_DAC->setVisible(false);
    CP=1;
}



void TestScreen::SaveDACPar()
{
    // Acceptable CP values are 1..10 (inclusive). Convert to zero-based index safely.
    const int minCP = 1;
    const int maxCP = 10;
    if (CP >= minCP && CP <= maxCP) {
        int idx = CP - 1;  // 0..9
        if (idx >= 0 && idx < DACx.size() && idx < DACy.size()) {
            DACx[idx] = peakIndex1;
            DACy[idx] = ph1Value;
            // If you want to auto-increment CP for the next save, do it here,
            // but be careful not to overflow:
            if (CP < maxCP) ++CP;
        } else {
            qWarning() << "SaveDACPar: index out of range" << idx;
        }
    } else {
        qWarning() << "SaveDACPar: CP out of allowed range:" << CP;
    }
    ui->lineEdit_CP->setText(QString::number(CP));
}



void TestScreen::UserDACPoints()
{
    int tmp_CP = ui->lineEdit_CP->text().toInt();
    if (tmp_CP < 1 )
    {
        CP=1;
        ui->lineEdit_CP->setText(QString::number(CP));
    }
    if (tmp_CP > 10 )
    {
        CP=10;
        ui->lineEdit_CP->setText(QString::number(CP));
    }
}


void TestScreen::DrawDACCurve()
{
    // ui->Plot->addGraph();
    // ui->Plot->graph(3)->setData(DACx,DACy);
    // ui->Plot->graph(3)->setLineStyle(QCPGraph::lsLine);
    // ui->Plot->graph(3)->setPen(QPen(Qt::yellow, 2));

        // Ensure DAC arrays exist and have the correct size.
        // Example assumes they are QVector<double> DACx, DACy and size is dacArraySize (10).
        const int dacArraySize = 10;
        if (DACx.size() != dacArraySize || DACy.size() != dacArraySize)
        {
            qWarning() << "DrawDACCurve: DAC arrays have wrong size:" << DACx.size() << DACy.size();
            return;
        }

        QVector<double> xFiltered;
        QVector<double> yFiltered;

        for (int i = 0; i < DACx.size(); ++i) {
            if (DACy[i] != 0 ) {
                xFiltered.append(DACx[i]);
                yFiltered.append(DACy[i]);
            }
        }

        DACline->data()->clear();
        DACline->setData(xFiltered, yFiltered);
        DACline->setLineStyle(QCPGraph::lsLine);
        DACline->setPen(QPen(Qt::darkGreen, 2));

        ui->Plot->replot();
}



void TestScreen::showBlankScreen()
{
    QWidget *blank = new QWidget();
    blank->setStyleSheet("background-color: pink;"); // or white
    blank->showFullScreen(); // full screen blank window
}

TestScreen::~TestScreen()
{
    delete ui;
}
void close_openlog(){
    openlogScreen->close();
    openlogScreen = nullptr;
}
