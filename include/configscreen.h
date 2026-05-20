#ifndef CONFIGSCREEN_H
#define CONFIGSCREEN_H

#include <QDialog>
#include <QTableWidget>

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
    void loadConfigToTables();
    void setLogicalFocus();
    void moveFocus(int dRow, int dCol);

    QTableWidget* activeTable();

};

#endif // CONFIGSCREEN_H
