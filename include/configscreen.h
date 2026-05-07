#ifndef CONFIGSCREEN_H
#define CONFIGSCREEN_H

#include <QDialog>

namespace Ui {
class ConfigScreen;
}

class ConfigScreen : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigScreen(QWidget *parent = nullptr);
    ~ConfigScreen();

private:
    Ui::ConfigScreen *ui;
};

#endif // CONFIGSCREEN_H
