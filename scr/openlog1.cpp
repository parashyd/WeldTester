#include "openlog1.h"
#include "ui_openlog1.h"
#include "testdetail0.h"
#include <QDir>
#include <QFileInfoList>
#include <QTableWidgetItem>
#include <QDebug>
#include "matrix_keypad.h"
#include "viewlogdata.h"
#include "mainwindow.h"

int currentFocus = 0;
QString selectedFilePath;

OpenLog1::OpenLog1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OpenLog1)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window );
    loadDateFolders();

    connect(ui->dateFolderList,
            &QListWidget::itemClicked,
            this,
            &OpenLog1::onDateFolderClicked);

    connect(ui->weldFolderList,
            &QListWidget::itemClicked,
            this,
            &OpenLog1::onWeldFolderClicked);
    ui->dateFolderList->setCurrentRow(0);
    currentFocus = 0;
    updateFocusStyle();

    StatusLabelTimer = new QTimer(this);

    StatusLabelTimer->setSingleShot(true);

    connect(StatusLabelTimer,
            &QTimer::timeout,
            this,
            [this]()
            {
                ui->label->clear();
            });

}

void OpenLog1::handleSocketKey(quint8 key)
{
    QListWidget *currentList = nullptr;

    switch(currentFocus)
    {
    case 0:
        currentList = ui->dateFolderList;
        break;

    case 1:
        currentList = ui->weldFolderList;
        break;

    case 2:
        currentList = ui->fileList;
        break;
    default:
        break;
    }

    // if(!currentList)
    //     return;
    int row=0;
    if(currentList!=nullptr)
        row = currentList->currentRow();

    switch(key)
    {
    case UP:

        if(row > 0 && (currentList!=nullptr))
        {
            currentList->setCurrentRow(row - 1);
        }

        break;

    case DOWN:

        if(currentList!=nullptr){
            if(row < currentList->count() - 1)
            {
                currentList->setCurrentRow(row + 1);
            }
        }

        break;

    case RIGHT:
    qDebug() << "RIGHT CurrentFocus =" << currentFocus;
        if(currentFocus < 6)
        {
            currentFocus++;

            if(currentFocus == 1)
            {
                updateFocusStyle();
                if(ui->weldFolderList->count() > 0)
                    ui->weldFolderList->setCurrentRow(0);
            }

            else if(currentFocus == 2)
            {
                updateFocusStyle();
                if(ui->fileList->count() > 0)
                    ui->fileList->setCurrentRow(0);
            }

            else {
                updateFocusStyle();
            }
        }

        break;

    case LEFT:
    qDebug() << "LEFT CurrentFocus =" << currentFocus;
        if(currentFocus > 0)
        {
            currentFocus--;

            if(currentFocus == 0)
            {
                updateFocusStyle();
            }

            else if(currentFocus == 1)
            {
                updateFocusStyle();
            }
            else{
                updateFocusStyle();

        }

        break;

    case OK:

        // DATE FOLDER SELECTED
        if(currentFocus == 0)
        {
            QListWidgetItem *item =
                ui->dateFolderList->currentItem();

            if(item)
            {
                onDateFolderClicked(item);

                currentFocus = 1;

                updateFocusStyle();

                if(ui->weldFolderList->count() > 0)
                    ui->weldFolderList->setCurrentRow(0);
            }
            break;
        }

        // WELD FOLDER SELECTED
        else if(currentFocus == 1)
        {
            QListWidgetItem *item =
                ui->weldFolderList->currentItem();

            if(item)
            {
                onWeldFolderClicked(item);

                currentFocus = 2;

                updateFocusStyle();

                if(ui->fileList->count() > 0)
                    ui->fileList->setCurrentRow(0);
            }
            break;
        }

        // FILE SELECTED
        else if(currentFocus == 2)
        {
            QListWidgetItem *item =
                ui->fileList->currentItem();

            if(item)
            {
                selectedFilePath=QString("%1/%2/%3/%4")
                                       .arg(basePath)
                                       .arg(selectedDateFolder)
                                       .arg(selectedWeldFolder)
                                       .arg(item->text());


                emit requestViewLogData();

                qDebug() << "Selected File:"
                         << item->text();

                // preview logic here
            }
            break;
        }
        else if(currentFocus == 3)
        {
            QString usbRoot = findUsbPath();

            if(usbRoot.isEmpty()){
                ui->label->setText("No Pendrive Detected");
                return;
            }

            QString machineFolder =
                usbRoot + "/" + MachNo;

            if (!QDir().exists(machineFolder)){
                QDir().mkpath(machineFolder);
            }
            QString src =
                basePath + "/" +
                selectedDateFolder;

            QString dst =
                uniqueFolderName(
                    machineFolder,
                    selectedDateFolder);

            if(copyFolderRecursively(src,dst))
            {
                ui->label->setStyleSheet(
                    "color: #00ff66;"
                    "font-size: 11px;"
                    "font-weight: bold;");
                ui->label->setText("Copied "+ selectedDateFolder);
                StatusLabelTimer->start(2000);

            }
            else{
                ui->label->setStyleSheet(
                    "color: RED;"
                    "font-size: 11px;"
                    "font-weight: bold;");
                ui->label->setText("Copying "+selectedDateFolder+" has failed");
                StatusLabelTimer->start(2000);

            }
            break;
        }
        else if(currentFocus == 4)
        {
            QString usbRoot = findUsbPath();

            if(usbRoot.isEmpty())
                return;

            QString machineFolder =
                usbRoot + "/" + MachNo;

            if (!QDir().exists(machineFolder)){
                    QDir().mkpath(machineFolder);
            }

            QDir srcDir(basePath);

            QFileInfoList folders =
                srcDir.entryInfoList(
                    QDir::Dirs |
                    QDir::NoDotAndDotDot);

            foreach(QFileInfo folder, folders)
            {
                QString dst =
                    uniqueFolderName(
                        machineFolder,
                        folder.fileName());


                if(copyFolderRecursively(folder.absoluteFilePath(),dst))
                {
                    ui->label->setStyleSheet(
                        "color: #00ff66;"
                        "font-size: 11px;"
                        "font-weight: bold;");
                    ui->label->setText("Copied "+ selectedDateFolder);
                    StatusLabelTimer->start(2000);
                }
                else{
                    ui->label->setStyleSheet(
                        "color: RED;"
                        "font-size: 11px;"
                        "font-weight: bold;");
                    ui->label->setText("Copying "+selectedDateFolder+" has failed");
                    StatusLabelTimer->start(2000);
                }
            }
            break;
        }
        else if(currentFocus == 5)
        {
            QString path =
                basePath + "/" +
                selectedDateFolder;

            QDir(path).removeRecursively();

            ui->label->setStyleSheet(
                "color: RED;"
                "font-size: 11px;"
                "font-weight: bold;");
            ui->label->setText("Deleted "+selectedDateFolder);
            StatusLabelTimer->start(2000);

            loadDateFolders();

            ui->weldFolderList->clear();
            ui->fileList->clear();
            break;
        }
        else if(currentFocus == 6)
        {
            QDir dir(basePath);

            QFileInfoList folders =
                dir.entryInfoList(
                    QDir::Dirs |
                    QDir::NoDotAndDotDot);

            foreach(QFileInfo folder, folders)
            {
                QDir(folder.absoluteFilePath())
                .removeRecursively();
            }
            ui->label->setStyleSheet(
                "color: Red;"
                "font-size: 11px;"
                "font-weight: bold;");
            ui->label->setText("Deleted all Date folders successfully");
            StatusLabelTimer->start(2000);
            loadDateFolders();

            ui->weldFolderList->clear();
            ui->fileList->clear();
            break;
        }
        break;
    }
}
}
void OpenLog1::updateFocusStyle()
{
    ui->dateFolderList->setStyleSheet("");
    ui->weldFolderList->setStyleSheet("");
    ui->fileList->setStyleSheet("");
    ui->copyButton->setStyleSheet("");
    ui->copyAllButton->setStyleSheet("");
    ui->deleteButton->setStyleSheet("");
    ui->deleteAllButton->setStyleSheet("");
    QString focusStyle =
        "QListWidget {"
        "border: 2px solid #3aa0ff;"
        "background-color: #dceeff;"
        "border-radius: 6px;"
        "}";

    QString focusStyle2 =
        "QPushButton {"
        "border: 2px solid #3aa0ff;"
        "background-color: #dceeff;"
        "border-radius: 6px;"
        "}";
    switch(currentFocus)
    {
    case 0:
        ui->dateFolderList->setStyleSheet(focusStyle);
        break;

    case 1:
        ui->weldFolderList->setStyleSheet(focusStyle);
        break;

    case 2:
        ui->fileList->setStyleSheet(focusStyle);
        break;

    case 3:
        ui->copyButton->setStyleSheet(focusStyle2);
        break;

    case 4:
        ui->copyAllButton->setStyleSheet(focusStyle2);
        break;

    case 5:
        ui->deleteButton->setStyleSheet(focusStyle2);
        break;

    case 6:
        ui->deleteAllButton->setStyleSheet(focusStyle2);
        break;

    }
}
void OpenLog1::loadDateFolders()
{
    ui->dateFolderList->clear();

    QDir dir(basePath);

    QFileInfoList folderList =
        dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,
                          QDir::Name);

    for(const QFileInfo &info : folderList)
    {
        ui->dateFolderList->addItem(info.fileName());
    }
}

void OpenLog1::onDateFolderClicked(QListWidgetItem *item)
{
    selectedDateFolder = item->text();

    loadWeldFolders(selectedDateFolder);
}
void OpenLog1::loadWeldFolders(const QString &dateFolder)
{
    ui->weldFolderList->clear();

    ui->fileList->clear();

    QString path =
        QString("%1/%2")
            .arg(basePath)
            .arg(dateFolder);

    QDir dir(path);

    QFileInfoList folderList =
        dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,
                          QDir::Name);

    for(const QFileInfo &info : folderList)
    {
        ui->weldFolderList->addItem(info.fileName());
    }
}

void OpenLog1::onWeldFolderClicked(QListWidgetItem *item)
{
    selectedWeldFolder = item->text();

    loadFiles(selectedWeldFolder);
}

void OpenLog1::loadFiles(const QString &weldFolder)
{
    ui->fileList->clear();

    QString path =
        QString("%1/%2/%3")
            .arg(basePath)
            .arg(selectedDateFolder)
            .arg(weldFolder);

    QDir dir(path);

    QStringList filters;
    filters << "*.wt";

    QFileInfoList fileList =
        dir.entryInfoList(filters,
                          QDir::Files,
                          QDir::Name);

    for(const QFileInfo &info : fileList)
    {
        ui->fileList->addItem(info.fileName());
    }

    qDebug() << "Loaded WT files from:" << path;
}

QString OpenLog1::findUsbPath()
{
    QDir mediaDir("/media");

    QFileInfoList dirs =
        mediaDir.entryInfoList(
            QDir::Dirs | QDir::NoDotAndDotDot);

    for(const QFileInfo &dir : dirs)
    {
        QString name = dir.fileName();

        if(name.contains("mmcblk"))
            continue;

        return dir.absoluteFilePath();
    }

    return "";
}
QString OpenLog1::uniqueFolderName(QString parent,
                         QString folderName)
{
    QString candidate =
        parent + "/" + folderName;

    if(!QDir(candidate).exists())
        return candidate;

    int count = 1;

    while(true)
    {
        QString test =
            parent + "/" +
            folderName +
            "(" +
            QString::number(count) +
            ")";

        if(!QDir(test).exists())
            return test;

        count++;
    }
}
bool OpenLog1::copyFolderRecursively(
    const QString &src,
    const QString &dst)
{
    QDir sourceDir(src);

    if(!sourceDir.exists())
        return false;

    QDir().mkpath(dst);

    QFileInfoList entries =
        sourceDir.entryInfoList(
            QDir::NoDotAndDotDot |
            QDir::Files |
            QDir::Dirs);

    foreach(QFileInfo entry, entries)
    {
        QString srcPath =
            entry.absoluteFilePath();

        QString dstPath =
            dst + "/" + entry.fileName();

        if(entry.isDir())
        {
            copyFolderRecursively(
                srcPath,
                dstPath);
        }
        else
        {
            QFile::copy(
                srcPath,
                dstPath);
        }
    }

    return true;
}
OpenLog1::~OpenLog1()
{
    delete ui;
}
