#include "configscreen.h"
#include "ui_configscreen.h"
#include "DataFile.h"
#include "qdebug.h"
#include "matrix_keypad.h"
#include <QDebug>
#include <QTableWidgetItem>
#include <QFile>
#include <stdio.h>
#include "QTimer"

#define MAX_LINE_LEN 256

ConfigScreen::ConfigScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigScreen)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    saveMessageTimer = new QTimer(this);

    saveMessageTimer->setSingleShot(true);

    connect(saveMessageTimer,
            &QTimer::timeout,
            this,
            [this]()
            {
                ui->statuslabel->clear();
            });

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

        return;

    case CH_B:

        currentTable = 1;

        currentRow = 0;
        currentCol = 0;

        setLogicalFocus();

        qDebug() << "Focused Channel 2 Table";

        return;

    case UP:
        moveFocus(-1, 0);
        return;

    case DOWN:
        moveFocus(1, 0);
        return;

    case LEFT:
        moveFocus(0, -1);
        return;

    case RIGHT:
        moveFocus(0, 1);
        return;

    case INC:
        adjustValue(+1);
        return;

    case DEC:
        adjustValue(-1);
        return;

    case OK:
    {
        QTableWidget *table = activeTable();

        table->editItem(
            table->item(currentRow, currentCol));

        return;
    }

    case SAVE:
        qDebug() << "SAVE CONFIG";
        saveConfigFile();
        return;
    case BACKSPACE:
        handleBackspaceInput();
        return;

    case ESC:
        close();
        return;

    default:
    {
        int mapped =
            normalizeKeyForNumeric(key);

        if(mapped != -1)
        {
            handleDigitInput(
                mapped - '0');
        }

        break;
        return;
    }
    }

}

void ConfigScreen::saveConfigFile()
{
    QFile file("Config.txt");

    if(!file.open(
            QIODevice::WriteOnly |
            QIODevice::Text))
    {
        qDebug() << "Cannot open Config.txt";
        return;
    }

    QTextStream out(&file);

    for(int row=0; row<10; row++)
    {
        QString gain =
            ui->configTable->item(row,0)->text();

        QString delay =
            ui->configTable->item(row,1)->text();

        QString range =
            ui->configTable->item(row,2)->text();

        QString reject =
            ui->configTable->item(row,3)->text();

        QString g1s =
            ui->configTable->item(row,4)->text();

        QString g1e =
            ui->configTable->item(row,5)->text();

        QString th1 =
            ui->configTable->item(row,6)->text();

        QString g2s =
            ui->configTable->item(row,7)->text();

        QString g2e =
            ui->configTable->item(row,8)->text();

        QString th2 =
            ui->configTable->item(row,9)->text();

        QString angle =
            ui->configTable->item(row,10)->text();

        out
            << row+1 << ","
            << 1 << ","
            << range << ","
            << delay << ","
            << reject << ","
            << g1s << ","
            << g1e << ","
            << th1 << ","
            << g2s << ","
            << g2e << ","
            << th2 << ","
            << gain << ","
            << angle << ",\n";
    }

    for(int row=0; row<10; row++)
    {
        QString gain =
            ui->configTable_2->item(row,0)->text();

        QString delay =
            ui->configTable_2->item(row,1)->text();

        QString range =
            ui->configTable_2->item(row,2)->text();

        QString reject =
            ui->configTable_2->item(row,3)->text();

        QString g1s =
            ui->configTable_2->item(row,4)->text();

        QString g1e =
            ui->configTable_2->item(row,5)->text();

        QString th1 =
            ui->configTable_2->item(row,6)->text();

        QString g2s =
            ui->configTable_2->item(row,7)->text();

        QString g2e =
            ui->configTable_2->item(row,8)->text();

        QString th2 =
            ui->configTable_2->item(row,9)->text();

        QString angle =
            ui->configTable_2->item(row,10)->text();

        out
            << row+1 << ","
            << 2 << ","
            << range << ","
            << delay << ","
            << reject << ","
            << g1s << ","
            << g1e << ","
            << th1 << ","
            << g2s << ","
            << g2e << ","
            << th2 << ","
            << gain << ","
            << angle << ",\n";
    }

    file.close();

    qDebug() << "Config.txt Updated";
    ui->statuslabel->setText("Config Data Saved Successfully");

    ui->statuslabel->setStyleSheet(
        "color: #00ff66;"
        "font-size: 11px;"
        "font-weight: bold;");
    saveMessageTimer->start(2000);
}

void ConfigScreen::adjustValue(int delta)
{
    QTableWidget *table =
        activeTable();

    QTableWidgetItem *item =
        table->item(
            currentRow,
            currentCol);

    if(!item)
        return;

    int value =
        item->text().toInt();

    value += delta;

    item->setText(
        QString::number(value));
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
int ConfigScreen::normalizeKeyForNumeric(quint8 key)
{
    if (key >= '0' && key <= '9')
        return key;

    switch(key)
    {
    case 'A': case 'B': case 'C': return '1';
    case 'D': case 'E': case 'F': return '2';
    case 'G': case 'H': case 'I': return '3';
    case 'J': case 'K': case 'L': return '4';
    case 'M': case 'N': case 'O': return '5';
    case 'P': case 'Q': case 'R': case 'S': return '6';
    case 'T': case 'U': case 'V': return '7';
    case 'W': case 'X': case 'Y': case 'Z': return '8';
    case '-': return '9';
    }

    return -1;
}
void ConfigScreen::handleDigitInput(int digit)
{
    QTableWidget *table = activeTable();

    QTableWidgetItem *item =
        table->item(currentRow,currentCol);

    if(!item)
    {
        item = new QTableWidgetItem("0");

        table->setItem(
            currentRow,
            currentCol,
            item);
    }

    if(!editState.timer.isValid() ||
        editState.timer.elapsed() > 3000)
    {
        editState.inputBuffer.clear();
    }

    editState.inputBuffer += QString::number(digit);

    item->setText(editState.inputBuffer);

    editState.timer.restart();
}
void ConfigScreen::handleBackspaceInput()
{
    QTableWidget *table = activeTable();

    QTableWidgetItem *item =
        table->item(currentRow, currentCol);

    if(!item)
        return;

    QString text = item->text();

    if(text.isEmpty())
        return;

    text.chop(1);

    item->setText(text);

    editState.inputBuffer = text;

    editState.timer.restart();
}
ConfigScreen::~ConfigScreen()
{
    delete ui;
}
