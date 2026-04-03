#ifndef TESTSCREEN_H
#define TESTSCREEN_H

#include <QMainWindow>
#include <QVector>
#include "DataFile.h"
#include "testdetails.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QLineEdit>
#include <QProcess>
#include "shared_data.h"
#include <QStackedWidget>
#include "qcustomplot.h"


struct PostSave {
    int channel = 0;
    int calset = 0;
    int velocity = 0;
};


struct KeyPressState {
    QElapsedTimer timer;
    int pressCount = 0;
    QString inputBuffer;   // Full committed string
    QString lastChar;      // Current character being cycled
};


extern PostSave Postlog;

extern ConfigEntry config;

extern int userChannel;
extern int userCalset;
extern QString DC_SC_mode;
extern QString MachNo;
extern double maxX, maxY;

extern SharedData* shared;
extern QVector<double> xDataFreeze , yDataFreeze;
extern QVector<double> xData, yData;

extern bool freeze;
void close_openlog();

QT_BEGIN_NAMESPACE
namespace Ui {class TestScreen;}
QT_END_NAMESPACE

class TestScreen : public QMainWindow
{
    Q_OBJECT

public:
    TestScreen(QWidget *parent = nullptr);
    void setSharedPointer(SharedData* ptr) { shared = ptr; };


    QMap<QLineEdit*, KeyPressState> inputStates;


    ~TestScreen();

private slots:
    bool updateConfigFile(const QString &filePath, const ConfigEntry &updatedConfig);
    void setupPlotAppearance();
    void updateGridInterval();
    void autoRunConfig();
    void handleDigitInput(int digit);
   // void navigateFocusVertical(int direction, bool forceStartFromGain);
     void navigateFocusVertical(int direction);

    void setInputFieldsEnabled(bool enabled);
    void adjustCurrentLineEdit(int delta);
    void updateGraphWithData();
    void onApplyGainClicked();
    void CloseDAC();
    void SaveDACPar();
    void UserDACPoints();
    void DrawDACCurve();


    // Socket slots
    void onNewConnection();
    void onSocketReadyRead();

    void loadSavedConfig();
    void saveConfigToFile();
    void setupValidators();
    void BatteryIndication();
    void updateBatteryLevel(int percent);
    void handleOtherScreenKeys(uint8_t key);
    void handleBackspaceInput();
    int normalizeKeyForNumeric(uint8_t key);
    void paintAudioIndicator(QPainter &p);
    bool eventFilter(QObject *obj, QEvent *event);
    void showBlankScreen();

    void prepareVelocityInput(void);
    void prepareCalsetInput(void);
    void adjustAudioLevel(void);
    void handleSaveFlow();
    void handleFreezeLogic(void);

    void FunctionLeftRight(bool increment);
    void setLogicalFocus(QWidget* widget);
    void HandleGateUpDownLift(int lift);
    void HandleGateShift(int shift);




private:
    Ui::TestScreen *ui;

    int lastBatteryPercent = -1;  // -1 so first update always happens




    TestDetails *testdetails;
    PreviewScreen *previewscreen;

    QCPGraph *g1Line;
    QCPGraph *g2Line;
    QCPGraph *waveformGraph;
    QCPGraph *DACline;
    QCPGraph *g1border;
    QCPGraph *g2border;


    //QMap<QLineEdit*, KeyPressState> inputStates;


    // Network components
    QTcpServer *server;

    QTcpSocket *client;
    uint8_t cnt = 0;
    int userinput_prv[3] = {0};  // store previous values persistently

    QTimer *plotUpdateTimer;
    QTimer *BatteryUpdateTimer;
    QWidget *m_currentFocusedWidget = nullptr;
    QWidget* m_currentLogicalFocus = nullptr;  // currently highlighted widget
    void startSocketServer();

    QStackedWidget *stackedWidget;

};
#endif // TESTSCREEN_H
