#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/ipc.h>
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
#include <QSvgRenderer>
#include "matrix_keypad.h"
#include <qobject.h>
#include "openlog.h"
#include "viewlogdata.h"
#include "Audio.h"
#include "gps.h"

QString MachNo;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    ui->setupUi(this);
    this->setStyleSheet(R"(

    /* ===== MAIN WINDOW ===== */
    QMainWindow {
        background-color: #070b10;
    }
    // QWidget {
    //     background-color: #2B3138;
    //     color: #F2F5F8;
    //     font-family: "Segoe UI";
    //     font-size: 11px;
    // }
    // QWidget {
    // background-color: #070b10;
    // }

    // QDialog {
    //     background-color: #070b10;
    // }

    /* ===== CENTRAL WIDGET ===== */
    QWidget#centralwidget {
        padding: 8px;
    }
    /* ===== BASE TILE STYLE (ALL LABELS) ===== */

    /* ===== LOGO ===== */
    QLabel#label_logo {
        background: transparent;
        border: 0px;
    }

    /* ===== PRESSED EFFECT (we simulate using dynamic property later) ===== */
    QLabel[pressed="true"] {
        background-color: #0f1720;
        border: 2px solid #1f6feb;
    }

    )");
    // QLabel {
    //     background-color: #121821;
    // color: #dfe7ef;
    // border: 2px solid #2a3542;
    //     border-radius: 10px;
    //     font-size: 16px;
    //     font-weight: 600;
    //     qproperty-alignment: AlignCenter;
    // }
    startSocketServer();

    ReadMachNo();

    int ret = GPS_Init();
    qDebug() << "GPS_Init =" << ret;

    // QPixmap pixmap = this->grab();

    // // --- Adjust JPG output size ---
    // QSize jpgTargetSize(640, 480); // You can change this (640x480, 1024x768, etc.)
    // QPixmap scaledJpg = pixmap.scaled(jpgTargetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // QString jpgFileName = QString("Homescreen.jpg");
    // scaledJpg.save(jpgFileName, "JPG", 100);

}
void MainWindow::ReadMachNo(){
    QFile file("BIN.BIN");

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open BIN.BIN";
        return;
    }
    // BIN.BIN contains 2 bytes: 0x90 0x00
    QByteArray data = file.readAll();
    file.close();

    if (data.size() < 2)
    {
        qDebug() << "Invalid file";
        return;
    }

    // Little-endian 16-bit integer
    quint16 value = static_cast<quint8>(data[0]) |
                    (static_cast<quint8>(data[1]) << 8);

    // Print as 4-digit number: 0144
    MachNo= QString("%1").arg(value, 4, 10, QChar('0'));

    qDebug() << "Mach No : "<<MachNo;
}
void MainWindow::startSocketServer()
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &MainWindow::onNewConnection);

    quint16 port = 8888;  // Use any free port
    if (!server->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(this, "Server Error", "Failed to start server: " + server->errorString());
    } else {
        qDebug() << "Server listening on port" << port;
    }
}

void MainWindow::onNewConnection()
{
    client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);
    qDebug() << "Client connected from" << client->peerAddress().toString();
}

void MainWindow::onSocketReadyRead()
{
    QByteArray data = client->readAll();
    if (data.isEmpty()) return;

    quint8 key = static_cast<quint8>(data.at(0));
    qDebug() << "Received key (hex):" << QString("0x%1").arg(key, 2, 16, QLatin1Char('0')).toUpper();

    if(key == ESC)
    {
        qDebug() << "ESC key pressed";
        if(testscreen && testscreen->isVisible())
        {
            testscreen->onSocketReadyRead(static_cast<int>(key));
            return;
        }

        if(testdetails0 && testdetails0->isVisible())
        {
            testdetails0->close();
            testdetails0=nullptr;
            return;
        }
        if(configscreen && configscreen->isVisible())
        {
            configscreen->close();
            configscreen=nullptr;
            return;
        }
        // if(openlogScreen && openlogScreen->isVisible())
        // {
        //     openlogScreen->close();
        //     openlogScreen=nullptr;
        //     return;
        // }
        if(viewlogdata && viewlogdata->isVisible())
        {
            viewlogdata->handleSocketKey(static_cast<int>(key));
            return;
        }
        if(openlog1 && openlog1->isVisible())
        {
            openlog1->close();
            openlog1=nullptr;
            return;
        }

    }
    if(testscreen && testscreen->isVisible())
    {
        testscreen->onSocketReadyRead(static_cast<int>(key));
        return;
    }

    if(testdetails0 && testdetails0->isVisible())
    {
        testdetails0->handleSocketKey(static_cast<int>(key));
        return;
    }

    if(configscreen && configscreen->isVisible())
    {
        configscreen->handleSocketKey(static_cast<int>(key));
        return;
    }

    if(openlogScreen && openlogScreen->isVisible())
    {
        openlogScreen->handleRemoteKey(static_cast<int>(key));
        return;
    }
    if(viewlogdata && viewlogdata->isVisible())
    {
        viewlogdata->handleSocketKey(static_cast<int>(key));
        return;
    }
    if(openlog1 && openlog1->isVisible())
    {
        openlog1->handleSocketKey(static_cast<int>(key));
        return;
    }
    switch (key)
    {
    case UP: case LEFT:
        navWidgets(-1);
        break;

    case DOWN: case RIGHT:
        navWidgets(1);
        break;

    case OK:
        navScreen();
        break;

    }
}
void MainWindow::setLogicalFocus(QWidget* widget)
{
    if (!widget) return;

    // 1️ Remove previous highlight
    if (m_currentLogicalFocus && m_currentLogicalFocus != widget) {
        // Reset style to default
        m_currentLogicalFocus->setStyleSheet("");
    }

    // 2️⃣ Apply highlight to the new widget\
    // "background-color: rgb(255,255,150);"
    widget->setStyleSheet(
        "background-color: rgb(120,180,255);"
        "border: 2px solid white;"
        );


    m_currentLogicalFocus = widget;

    // 3️⃣ Optional: attempt to give real Qt focus
    if (widget->isVisible() && widget->isEnabled()) {
        // For Wayland, this may not guarantee OS focus, but it's harmless
        widget->setFocus(Qt::OtherFocusReason);

    }

    // 4️⃣ Debug output
    qDebug() << "[LogicalFocus] Widget:" << widget->objectName()
             << "Visible:" << widget->isVisible()
             << "Enabled:" << widget->isEnabled()
             << "hasFocus:" << widget->hasFocus();
}
void MainWindow::navScreen(void)
{
    if (!m_currentLogicalFocus)
        return;

    if (m_currentLogicalFocus == ui->label_test)
    {
        testdetails0 = new testdetail0(this);
        testdetails0->setAttribute(Qt::WA_DeleteOnClose);
        testdetails0->show();
        connect(testdetails0, &testdetail0::requestTestscreen,this,[this]()
            {
                if(!testscreen){
                    testscreen = new TestScreen(this);
                    testscreen->setAttribute(Qt::WA_DeleteOnClose);
                    testscreen->show();

                    connect(testscreen,&TestScreen::closeTestScreen,this,[this]()
                            {
                        qDebug()<<"Closing TestScreen";
                        testscreen->close();
                        testscreen=nullptr;
                    });
                }
            });
    }

    if (m_currentLogicalFocus == ui->label_config)
    {
        configscreen = new ConfigScreen(this);
        configscreen->setAttribute(Qt::WA_DeleteOnClose);
        configscreen->show();
    }

    if (m_currentLogicalFocus == ui->label_open)
    {
        openlogScreen = new Openlog(this);
        openlogScreen->setAttribute(Qt::WA_DeleteOnClose);
        openlogScreen->show();
        connect(openlogScreen,&Openlog::closeopenlogscreen,this,[this]{
            qDebug()<<"Closing openlogscreen";
            openlogScreen->close();
            openlogScreen = nullptr;
        });
    }
    if (m_currentLogicalFocus == ui->label_openlog)
    {
        openlog1 = new OpenLog1(this);
        openlog1->setAttribute(Qt::WA_DeleteOnClose);
        openlog1->show();
        connect(openlog1, &OpenLog1::requestViewLogData,this,[this]
                {
            if(!viewlogdata)
                {
                viewlogdata = new viewLogData(this);
                viewlogdata->show();
                viewlogdata->setAttribute(Qt::WA_DeleteOnClose);
                connect(viewlogdata, &viewLogData::closeviewlogdata,this,[this](){
                    qDebug()<<"Closing viewlogdata";
                    viewlogdata->close();
                    viewlogdata = nullptr;
                });
            }
        });
    }

}
void MainWindow::navWidgets(int direction)
{
    QList<QWidget*> navWidgets = {
        ui->label_test,
        ui->label_config,
        ui->label_open,
        ui->label_openlog,
        ui->label_settings
    };

    // 🔹 Trust Qt's real focus
    QWidget *current = focusWidget();

    // Fallback only if nothing focused
    if (!current || !navWidgets.contains(current)){
        current = ui->label_test;
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
    qDebug() << "Moved focus from"
             << current->objectName()
             << "to"
             << newWidget->objectName();
}


MainWindow::~MainWindow()
{
    delete ui;
}
