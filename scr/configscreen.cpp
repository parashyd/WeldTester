#include "configscreen.h"
#include "ui_configscreen.h"
#include "DataFile.h"
#include "qdebug.h"
#include "matrix_keypad.h"
#include <QDebug>
#include <QTableWidgetItem>

#include <stdio.h>

#define MAX_LINE_LEN 256

ConfigScreen::ConfigScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigScreen)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    loadConfigToTables();
}

void ConfigScreen::setLogicalFocus()
{
    QTableWidget *table = activeTable();

    table->setCurrentCell(currentRow, currentCol);

    qDebug() << "Focused:"
             << "Table:" << currentTable
             << "Row:" << currentRow
             << "Col:" << currentCol;
}

QTableWidget* ConfigScreen::activeTable()
{
    if(currentTable == 0)
        return ui->configTable;

    return ui->configTable_2;
}

void ConfigScreen::moveFocus(int dRow, int dCol)
{
    currentRow += dRow;
    currentCol += dCol;

    if(currentRow < 0)
        currentRow = 9;

    if(currentRow > 9)
        currentRow = 0;

    if(currentCol < 0)
    {
        // if(currentTable == 1)
        // {
        //     currentTable = 0;
        //     currentCol = ui->configTable->columnCount() - 1;
        // }
        // else
        // {
            currentCol = activeTable()->columnCount()-1;
        // }
    }

    if(currentCol >= activeTable()->columnCount())
    {
        // if(currentTable == 0)
        // {
        //     currentTable = 1;
            currentCol = 0;
        // }
        // else
        // {
            // currentCol =
            //     activeTable()->columnCount() - 1;
        // }
    }

    setLogicalFocus();
}

void ConfigScreen::handleSocketKey(quint8 key)
{
    switch(key)
    {
    case CH_A:

        currentTable = 0;

        currentRow = 0;
        currentCol = 0;

        setLogicalFocus();

        qDebug() << "Focused Channel 1 Table";

        break;

    case CH_B:

        currentTable = 1;

        currentRow = 0;
        currentCol = 0;

        setLogicalFocus();

        qDebug() << "Focused Channel 2 Table";

        break;

    case UP:
        moveFocus(-1, 0);
        break;

    case DOWN:
        moveFocus(1, 0);
        break;

    case LEFT:
        moveFocus(0, -1);
        break;

    case RIGHT:
        moveFocus(0, 1);
        break;

    case INC:
       // adjustValue(+1);
        break;

    case DEC:
       // adjustValue(-1);
        break;

    case OK:
    {
        QTableWidget *table = activeTable();

        table->editItem(
            table->item(currentRow, currentCol));

        break;
    }

    case SAVE:
        qDebug() << "SAVE CONFIG";
        break;

    case ESC:
        close();
        break;
    }
}




void ConfigScreen::loadConfigToTables()
{
    FILE *configFile;

    char configLine[MAX_LINE_LEN];

    ConfigEntry temp;

    int fields;

    configFile = fopen("Config.txt", "r");

    if(!configFile)
    {
        qDebug() << "Failed to open Config.txt";
        return;
    }

    while(fgets(configLine, sizeof(configLine), configFile))
    {
        fields = sscanf(configLine,
                        "%d,%d,%lf,%lf,%lf,%d,%d,%d,%d,%d,%d,%lf,%f",

                        &temp.calset,
                        &temp.channel,

                        &temp.range,
                        &temp.delay,
                        &temp.reject,

                        &temp.g1_start,
                        &temp.g1_end,
                        &temp.th1,

                        &temp.g2_start,
                        &temp.g2_end,
                        &temp.th2,

                        &temp.Gain,
                        &temp.Angle);

        if(fields != 13)
            continue;

        int row = temp.calset - 1;

        QTableWidget *table = nullptr;

        // CHANNEL SELECTION
        if(temp.channel == 1)
            table = ui->configTable;

        else if(temp.channel == 2)
            table = ui->configTable_2;

        else
            continue;

        // COLUMN DATA
        table->setItem(row, 0,
                       new QTableWidgetItem(QString::number(temp.Gain)));

        table->setItem(row, 1,
                       new QTableWidgetItem(QString::number(temp.delay)));

        table->setItem(row, 2,
                       new QTableWidgetItem(QString::number(temp.range)));

        table->setItem(row, 3,
                       new QTableWidgetItem(QString::number(temp.reject)));

        table->setItem(row, 4,
                       new QTableWidgetItem(QString::number(temp.g1_start)));

        table->setItem(row, 5,
                       new QTableWidgetItem(QString::number(temp.g1_end)));

        table->setItem(row, 6,
                       new QTableWidgetItem(QString::number(temp.th1)));

        table->setItem(row, 7,
                       new QTableWidgetItem(QString::number(temp.g2_start)));

        table->setItem(row, 8,
                       new QTableWidgetItem(QString::number(temp.g2_end)));

        table->setItem(row, 9,
                       new QTableWidgetItem(QString::number(temp.th2)));

        table->setItem(row, 10,
                       new QTableWidgetItem(QString::number(temp.Angle)));
    }

    fclose(configFile);
}

ConfigScreen::~ConfigScreen()
{
    delete ui;
}
