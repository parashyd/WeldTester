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
#include "settings.h"
#include <QSettings>
#include <QPixmap>

QString MachNo;
static const QString DARK_MAIN_THEME = R"(
QMainWindow {
    background-color: #070b10;
}

QWidget#centralwidget {
    background-color: #070b10;
}

QLabel#label_UnitNo {
    background-color: transparent;
    color: white;
}

QLabel#label_logo {
    background-color: transparent;
    border: none;
}

QLabel[pressed="true"] {
    background-color: #0f1720;
    border: 2px solid #1f6feb;
}
)";
static const QString LIGHT_MAIN_THEME = R"(
QMainWindow {
    background-color: #eef2f6;
}

QWidget#centralwidget {
    background-color: #eef2f6;
}

QLabel#label_UnitNo {
    background-color: transparent;
    color: #18212b;
}

QLabel#label_logo {
    background-color: transparent;
    border: none;
}

QLabel[pressed="true"] {
    background-color: #dceeff;
    border: 2px solid #3aa0ff;
}
)";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    ui->setupUi(this);
    applyMainWindowTheme();
    startSocketServer();

    ReadMachNo();

    int ret = GPS_Init();
    qDebug() << "GPS_Init =" << ret;


    ui->label_UnitNo->setStyleSheet(
        "background-color: transparent;"
        );
    ui->label_UnitNo->setText("WT-2311D - "+MachNo);

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
    else{
        MachNo="0000";
    }
    // BIN.BIN contains 2 bytes: 0x90 0x00
    QByteArray data = file.readAll();
    file.close();

    if (data.size() < 2)
    {
        qDebug() << "Invalid file";
        return;
    }
    else{
        MachNo="0000";
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
        if(settingsWindow && settingsWindow->isVisible()){
            settingsWindow->close();
            settingsWindow =nullptr;
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
    if(settingsWindow && settingsWindow->isVisible())
    {
        settingsWindow->handleSocketKey(static_cast<int>(key));
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
        "background-color: rgba(120,180,255,80);"
        "border: 2px solid #3aa0ff;"
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
    if(m_currentLogicalFocus == ui->label_settings)if (m_currentLogicalFocus == ui->label_settings)
        {
            settingsWindow = new settings(this);

            settingsWindow->setAttribute(Qt::WA_DeleteOnClose);

            connect(settingsWindow,
                    &settings::settingsChanged,
                    this,
                    [this]()
                    {
                        applyMainWindowTheme();
                        updateMainMenuImages();
                    });

            connect(settingsWindow, &QObject::destroyed, this, [this]()
                    {
                        settingsWindow = nullptr;
                    });

            settingsWindow->show();
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

void MainWindow::applyMainWindowTheme()
{
    const QString currentTheme = settings::theme();

    if (currentTheme.compare("Light", Qt::CaseInsensitive) == 0)
    {
        setStyleSheet(LIGHT_MAIN_THEME);
    }
    else
    {
        setStyleSheet(DARK_MAIN_THEME);
    }

    updateMainMenuImages();
}
void MainWindow::updateMainMenuImages()
{
    const bool light =
        settings::theme().compare("Light", Qt::CaseInsensitive) == 0;

    if (light)
    {
        ui->label_test->setPixmap(
            QPixmap(":/images/light_test.png"));

        ui->label_config->setPixmap(
            QPixmap(":/images/light_config.png"));

        ui->label_open->setPixmap(
            QPixmap(":/images/light_open.png"));

        ui->label_openlog->setPixmap(
            QPixmap(":/images/light_openlog.png"));

        ui->label_settings->setPixmap(
            QPixmap(":/images/light_settings.png"));
    }
    else
    {
        ui->label_test->setPixmap(
            QPixmap(":/images/dark_test.png"));

        ui->label_config->setPixmap(
            QPixmap(":/images/dark_config.png"));

        ui->label_open->setPixmap(
            QPixmap(":/images/dark_open.png"));

        ui->label_openlog->setPixmap(
            QPixmap(":/images/dark_openlog.png"));

        ui->label_settings->setPixmap(
            QPixmap(":/images/dark_settings.png"));
    }

    // Keep the images filling their existing QLabel areas.
    ui->label_test->setScaledContents(true);
    ui->label_config->setScaledContents(true);
    ui->label_open->setScaledContents(true);
    ui->label_openlog->setScaledContents(true);
    ui->label_settings->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
