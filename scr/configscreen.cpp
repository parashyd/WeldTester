#include "configscreen.h"
#include "ui_configscreen.h"
#include "DataFile.h"
#include "qdebug.h"
#include "matrix_keypad.h"
#include <QDebug>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QFile>
#include <stdio.h>
#include "QTimer"
#include "settings.h"

#define MAX_LINE_LEN 256
static int chanlCnt=0;
ConfigScreen::ConfigScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigScreen)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    // ---------------------------------------------------------
    // Apply Light / Dark theme
    // ---------------------------------------------------------
    const bool lightTheme =
        settings::theme().compare("Light", Qt::CaseInsensitive) == 0;

    if (lightTheme)
    {
        this->setStyleSheet(R"(
        QWidget {
            background-color: #F2F5F8;
            color: #1B2430;
            font-family: "Segoe UI";
            font-size: 11px;
        }

        QLabel {
            color: #1B2430;
            background: transparent;
        }

        QTableWidget {
            background-color: #FFFFFF;
            alternate-background-color: #F7F9FB;
            color: #1B2430;
            gridline-color: #C5CED8;
            border: 1px solid #AAB6C3;
            border-radius: 8px;
            selection-background-color: #DCEEFF;
            selection-color: #1B2430;
        }

        QTableWidget::item {
            background-color: #FFFFFF;
            color: #1B2430;
            padding: 3px;
        }

        QTableWidget::item:selected {
            background-color: #DCEEFF;
            color: #1B2430;
            border: 1px solid #3AA0FF;
        }

        QHeaderView::section {
            background-color: #E3EAF1;
            color: #1B2430;
            border: 1px solid #AAB6C3;
            padding: 4px;
            font-weight: bold;
        }

        QScrollBar:vertical {
            background: #E8EDF2;
            width: 12px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: #AAB6C3;
            border-radius: 5px;
            min-height: 25px;
        }

        QScrollBar::handle:vertical:hover {
            background: #7F91A3;
        }

        QScrollBar:horizontal {
            background: #E8EDF2;
            height: 12px;
        }

        QScrollBar::handle:horizontal {
            background: #AAB6C3;
            border-radius: 5px;
            min-width: 25px;
        }

        QPushButton {
            background-color: #E3EAF1;
            border: 1px solid #AAB6C3;
            border-radius: 8px;
            color: #1B2430;
            min-height: 20px;
            padding: 4px 10px;
        }

        QPushButton:hover {
            background-color: #D7E9FA;
            border: 1px solid #3AA0FF;
        }
    )");
    }
    else
    {
        this->setStyleSheet(R"(
        QWidget {
            background-color: #2B3138;
            color: #F2F5F8;
            font-family: "Segoe UI";
            font-size: 11px;
        }

        QLabel {
            color: #D5DDE5;
            background: transparent;
        }

        QTableWidget {
            background-color: #313941;
            alternate-background-color: #39424C;
            color: #F2F5F8;
            gridline-color: #596573;
            border: 1px solid #55606D;
            border-radius: 8px;
            selection-background-color: #4E5A67;
            selection-color: #FFFFFF;
        }

        QTableWidget::item {
            background-color: #313941;
            color: #F2F5F8;
            padding: 3px;
        }

        QTableWidget::item:selected {
            background-color: #4E5A67;
            color: #FFFFFF;
            border: 1px solid #4DB3FF;
        }

        QHeaderView::section {
            background-color: #39424C;
            color: #F2F5F8;
            border: 1px solid #55606D;
            padding: 4px;
            font-weight: bold;
        }

        QScrollBar:vertical {
            background: #2B3138;
            width: 12px;
        }

        QScrollBar::handle:vertical {
            background: #596573;
            border-radius: 5px;
            min-height: 25px;
        }

        QScrollBar::handle:vertical:hover {
            background: #748191;
        }

        QScrollBar:horizontal {
            background: #2B3138;
            height: 12px;
        }

        QScrollBar::handle:horizontal {
            background: #596573;
            border-radius: 5px;
            min-width: 25px;
        }

        QPushButton {
            background-color: #55606D;
            border: 1px solid #748191;
            border-radius: 8px;
            color: white;
            min-height: 20px;
            padding: 4px 10px;
        }

        QPushButton:hover {
            background-color: #627081;
            border: 1px solid #4DB3FF;
        }
    )");
    }
    saveMessageTimer = new QTimer(this);

    saveMessageTimer->setSingleShot(true);

    connect(saveMessageTimer,
            &QTimer::timeout,
            this,
            [this]()
            {
                ui->statuslabel->clear();
            });
    //  QPixmap pixmap = this->grab();

    // --- Adjust JPG output size ---
    // QSize jpgTargetSize(640, 480); // You can change this (640x480, 1024x768, etc.)
    // QPixmap scaledJpg = pixmap.scaled(jpgTargetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // QString jpgFileName = QString("ConfigScreen.jpg");
    // scaledJpg.save(jpgFileName, "JPG", 100 );
    // Position column: only allow Head, Web, or Flange.
    const QStringList positions = {"Head", "Web", "Flange"};
    for (QTableWidget *table : {ui->configTable, ui->configTable_2})
    {
        table->setColumnCount(12);
        table->setHorizontalHeaderItem(11, new QTableWidgetItem("POS"));
        table->setColumnWidth(11, 76);
        for (int row = 0; row < table->rowCount(); ++row)
        {
            QComboBox *positionCombo = new QComboBox(table);
            positionCombo->addItems(positions);
            positionCombo->setCurrentText("Head");
            positionCombo->setFocusPolicy(Qt::StrongFocus);
            positionCombo->setStyleSheet(
                "QComboBox { padding: 2px; border: 1px solid #AAB6C3; }"
                "QComboBox:focus { border: 2px solid #3AA0FF; }"
                "QComboBox QAbstractItemView { selection-background-color: #DCEEFF; }");
            table->setCellWidget(row, 11, positionCombo);
        }
    }

    loadConfigToTables();
}
void ConfigScreen::setLogicalFocus()
{
    QTableWidget *table = activeTable();

    if (!table)
        return;

    table->setCurrentCell(currentRow, currentCol);


    if (currentCol == 11)
    {
        // POS column: give focus to its combo box.
        QComboBox *positionCombo =
            qobject_cast<QComboBox *>(
                table->cellWidget(currentRow, 11));

        if (positionCombo)
        {
            positionCombo->setFocus(Qt::OtherFocusReason);

            qDebug() << "POS focused:"
                     << "Row:" << currentRow
                     << "Value:" << positionCombo->currentText();
        }
    }
    else
    {
        table->setFocus(Qt::OtherFocusReason);
    }

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

    // if(currentRow < 0)
    //     currentRow = 9;
    if(currentRow < 0)
        currentRow = 49;

    // if(currentRow > 9)
    //     currentRow = 0;
    if(currentRow > 49)
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
    case CHANNEL:
        chanlCnt++;
        if(chanlCnt == 1){
            currentTable = 0;

            currentRow = 0;
            currentCol = 0;

            setLogicalFocus();

            qDebug() << "Focused Channel 1 Table";
        }
        else if(chanlCnt==2){
            currentTable = 1;

            currentRow = 0;
            currentCol = 0;

            setLogicalFocus();
            chanlCnt=0;
            qDebug() << "Focused Channel 2 Table";
        }
        return;
        // case CH_A:

        //     currentTable = 0;

        //     currentRow = 0;
        //     currentCol = 0;

        //     setLogicalFocus();

        //     qDebug() << "Focused Channel 1 Table";

        //     return;

        // case CH_B:

        //     currentTable = 1;

        //     currentRow = 0;
        //     currentCol = 0;

        //     setLogicalFocus();

        //     qDebug() << "Focused Channel 2 Table";

        //     return;

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
    QPixmap pixmap = this->grab();

    //--- Adjust JPG output size ---
    QSize jpgTargetSize(640, 480); // You can change this (640x480, 1024x768, etc.)
    QPixmap scaledJpg = pixmap.scaled(jpgTargetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QString jpgFileName = QString("ConfigScreen.jpg");
    scaledJpg.save(jpgFileName, "JPG", 100 );
    QFile file("Config.txt");

    if(!file.open(
            QIODevice::WriteOnly |
            QIODevice::Text))
    {
        qDebug() << "Cannot open Config.txt";
        return;
    }

    QTextStream out(&file);

    // for(int row=0; row<10; row++)
    for(int row=0; row<50; row++)
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

        QComboBox *positionCombo =
            qobject_cast<QComboBox *>(ui->configTable->cellWidget(row, 11));
        QString position = positionCombo ? positionCombo->currentText() : "Head";

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
            << angle << ","
            << position << ",\n";
    }

    // for(int row=0; row<10; row++)
    for(int row=0; row<50; row++)
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

        QComboBox *positionCombo =
            qobject_cast<QComboBox *>(ui->configTable_2->cellWidget(row, 11));
        QString position = positionCombo ? positionCombo->currentText() : "Head";

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
            << angle << ","
            << position << ",\n";
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
    QTableWidget *table = activeTable();

    if (!table)
        return;


    // INC/DEC should change the position only when the Position
    // combo box itself currently has focus.
    // Handle POS combo box.
    if (currentCol == 11)
    {
        QComboBox *positionCombo =
            qobject_cast<QComboBox *>(
                table->cellWidget(currentRow, 11));

        if (positionCombo && positionCombo->count() > 0)
        {
            int nextIndex = positionCombo->currentIndex() + delta;

            // Wrap around at either end.
            if (nextIndex < 0)
                nextIndex = positionCombo->count() - 1;
            else if (nextIndex >= positionCombo->count())
                nextIndex = 0;

            positionCombo->setCurrentIndex(nextIndex);

            qDebug() << "POS INC/DEC:"
                     << "Table:" << currentTable
                     << "Row:" << currentRow
                     << "Index:" << positionCombo->currentIndex()
                     << "Text:" << positionCombo->currentText();
        }
        else
        {
            qDebug() << "POS combo not found at row:" << currentRow;
        }

        return;
    }

    // Existing numeric-cell behavior.
    QTableWidgetItem *item =
        table->item(currentRow, currentCol);

    if (!item)
        return;

    bool ok = false;
    int value = item->text().toInt(&ok);

    if (!ok)
        value = 0;

    value += delta;
    item->setText(QString::number(value));
}

void ConfigScreen::loadConfigToTables()
{
    FILE *configFile;

    char configLine[MAX_LINE_LEN];

    ConfigEntry temp;
    char position[16] = {};

    int fields;

    configFile = fopen("Config.txt", "r");

    if(!configFile)
    {
        qDebug() << "Failed to open Config.txt";
        return;
    }

    while(fgets(configLine, sizeof(configLine), configFile))
    {
        memset(position, 0, sizeof(position));
        fields = sscanf(configLine,
                        "%d,%d,%lf,%lf,%lf,%d,%d,%d,%d,%d,%d,%lf,%f,%15[^,\r\n]",

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
                        &temp.Angle,
                        position);

        if(fields < 13)
            continue;

        QString savedPosition = (fields >= 14)
                                    ? QString::fromLocal8Bit(position).trimmed()
                                    : QString("Head");
        if (savedPosition != "Head" &&
            savedPosition != "Web" &&
            savedPosition != "Flange")
        {
            savedPosition = "Head";
        }

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

        QComboBox *positionCombo =
            qobject_cast<QComboBox *>(table->cellWidget(row, 11));
        if (positionCombo)
            positionCombo->setCurrentText(savedPosition);
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