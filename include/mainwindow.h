#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "testdetail0.h"
#include "testdetails.h"
#include "testscreen.h"
#include "configscreen.h"

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


    testdetail0 *testdetails0;
    TestScreen *testscreen;
    TestDetails *testdetails;
    ConfigScreen *configscreen;

    QWidget *m_currentFocusedWidget = nullptr;
    QWidget* m_currentLogicalFocus = nullptr;
    void startSocketServer();
};

#endif // MAINWINDOW_H
