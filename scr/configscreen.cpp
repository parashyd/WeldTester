#include "configscreen.h"
#include "ui_configscreen.h"

ConfigScreen::ConfigScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigScreen)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
}

ConfigScreen::~ConfigScreen()
{
    delete ui;
}
