#ifndef CONFIGSCREEN_H
#define CONFIGSCREEN_H

#include <QDialog>
#include <QTableWidget>
#include <QElapsedTimer>

namespace Ui {
class ConfigScreen;
}

class ConfigScreen : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigScreen(QWidget *parent = nullptr);
    void handleSocketKey(quint8 key);
    ~ConfigScreen();

private:
    Ui::ConfigScreen *ui;
    int currentTable = 0;   // 0 = CH1 , 1 = CH2
    int currentRow = 0;
    int currentCol = 0;
    QTimer *saveMessageTimer;

    void loadConfigToTables();
    void setLogicalFocus();
    void moveFocus(int dRow, int dCol);
    void handleDigitInput(int digit);
    void saveConfigFile();
    int normalizeKeyForNumeric(quint8 key);
    void adjustValue(int delta);
    void handleBackspaceInput();

    struct KeyPressState
    {
        QString inputBuffer;
        QElapsedTimer timer;
    };

    KeyPressState editState;

    QTableWidget* activeTable();

};

#endif // CONFIGSCREEN_H
