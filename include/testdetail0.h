#ifndef TESTDETAIL__H
#define TESTDETAIL__H

#include <QDialog>
#include <QLineEdit>
#include <QElapsedTimer>



extern QString MachNo;
extern QString OperatorName;
extern QString Division ;
extern QString Section ;
extern QString Line ;
extern QString Testtype;
extern QString WeldNo;
extern QString km;
extern QString M;
extern QString Rail ;


namespace Ui {
class testdetail0;
}

class testdetail0 : public QDialog
{
    Q_OBJECT

public:
    explicit testdetail0(QWidget *parent = nullptr);
    void handleSocketKey(quint8 key);

    ~testdetail0();

private:
    Ui::testdetail0 *ui;
    QWidget *m_currentFocusedWidget = nullptr;
    QWidget* m_currentLogicalFocus = nullptr;

    struct KeyPressState {
        QElapsedTimer timer;
        int pressCount = 0;
        QString inputBuffer;   // Full committed string
        quint8 lastKey = 0;   // last key pressed
        QString lastChar;      // Current character being cycled
    };

    QMap<QLineEdit*, KeyPressState> inputStates;

    void navFocus(int delta);
    void setLogicalFocus(QWidget* widget);
    void adjustComboBoxIndex(int delta);
    void handleMultiPressAlpha(quint8 key, QLineEdit *lineEdit);
    void handleBackspaceInput();
    void display_mc_no(void);
    void saveDataToFile();
    void LoadSavedData();



signals:
    void requestTestscreen();
};

#endif // TESTDETAIL__H
