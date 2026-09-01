#ifndef TESTDETAILS_H
#define TESTDETAILS_H

#include <QDialog>
#include "previewscreen.h"
#include <QLineEdit>
#include <QElapsedTimer>
#include <QTextEdit>

extern QString Mode;
extern QString TP;
extern QString LocOfDefect ;
extern QString stations;
extern QString rollmark;
extern QString weld;
extern QString Probe;
extern QString classVal;
extern QString FPNFP;
extern QString OthersObservation;
extern QString PrevHMBP;
extern QString PrevPH;
extern QString PrevClass;
extern QString Time;
extern QString Date;




namespace Ui {
class TestDetails;
}

class TestDetails : public QDialog
{
    Q_OBJECT

public:
    explicit TestDetails(QWidget *parent = nullptr);
    void PreviewClick();
    void handleSocketKey(quint8 key);

    bool isPreviewOpen() const;
    void closePreview();


    ~TestDetails();

private slots:
    void handleMultiPressAlpha(quint8 key, QLineEdit *lineEdit);
    void navigateFocus(int direction);
    void adjustComboBoxIndex(int delta);
    void handleBackspaceInput();
    void saveDataToFile();
    void loadDataFromFile();
    void setLogicalFocus(QWidget* widget);


private:

    struct KeyPressState {
        QElapsedTimer timer;
        int pressCount = 0;
        QString inputBuffer;   // Full committed string
        quint8 lastKey = 0;   // last key pressed
        QString lastChar;      // Current character being cycled
    };



    QMap<QLineEdit*, KeyPressState> inputStates;

    enum class InputMode {
        Numeric,
        Alphabetic,
        Alphanumeric,
        None   // no input allowed
    };

    QMap<QLineEdit*, InputMode> inputModes;

    Ui::TestDetails *testdetails;
    PreviewScreen *previewscreen;
    QDialog* previewDialog = nullptr;
    bool previewScreenIsOpen = false;
    QWidget *m_currentFocusedWidget = nullptr;
    QWidget* m_currentLogicalFocus = nullptr;


signals:
    void requestPreview();   // ✅ Add this signal
    void enterPressed(QLineEdit *lineEdit);
    void ctrlTPressed(QLineEdit *lineEdit);


};

#endif // TESTDETAILS_H
