#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "testdetail0.h"
#include "testdetails.h"
#include "testscreen.h"
#include "configscreen.h"
#include "openlog.h"
#include "openlog1.h"
#include "viewlogdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setSharedPointer(SharedData* ptr) { shared = ptr; };


    ~MainWindow();

private slots:
    void onNewConnection();
    void onSocketReadyRead();
    void navWidgets(int del);
    void navScreen();
    void setLogicalFocus(QWidget* widget);


private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *client;


    testdetail0 *testdetails0 =nullptr;
    TestScreen *testscreen=nullptr;
    TestDetails *testdetails=nullptr;
    ConfigScreen *configscreen=nullptr;
    Openlog *openlogScreen=nullptr;
    OpenLog1 *openlog1=nullptr;
    viewLogData *viewlogdata=nullptr;

    QWidget *m_currentFocusedWidget = nullptr;
    QWidget* m_currentLogicalFocus = nullptr;
    void startSocketServer();
};

#endif // MAINWINDOW_H
