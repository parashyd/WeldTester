#include "openlog.h"
#include "ui_Openlog.h"
#include "openlog1.h"
#include <QDir>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QTimer>
#include "matrix_keypad.h"
#include "testscreen.h"
#include "mainwindow.h"
#include <algorithm>
#include <QDate>
#include <QMap>
/* ============================================================
 * FULLSCREEN IMAGE VIEWER
 * ============================================================ */
class FullscreenImageViewer : public QWidget
{
public:
    explicit FullscreenImageViewer(const QString &path, QWidget *parent = nullptr)
    {
        qDebug() << "[Fullscreen] Opening image:" << path;

        setWindowFlags(Qt::FramelessWindowHint | Qt::Window );
        setAttribute(Qt::WA_DeleteOnClose);
        setStyleSheet("background:black;");
        // setFocusPolicy(Qt::StrongFocus);

        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);
        layout->addWidget(label);

        QPixmap pix(path);
        if (!pix.isNull()) {
            label->setPixmap(
                pix.scaled(QGuiApplication::primaryScreen()->size(),
                           Qt::KeepAspectRatio,
                           Qt::SmoothTransformation));
        } else {
            qDebug() << "[Fullscreen] Failed to load image!";
        }

        showFullScreen();
    }


    void closeFullscreen() {
        if (!isHidden()) {
            qDebug() << "[Fullscreen] ESC pressed -> closing";
            close();
        }
    }

};

/* ============================================================
 * OPENLOG IMPLEMENTATION
 * ============================================================ */

Openlog::Openlog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Openlog),
    watcher(new QFileSystemWatcher(this)),
    currentViewer(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window );


    // /* ✅ Focus-aware styling */
    // QString style = R"(
    // QListWidget {
    // color: white;
    // background-color: #1e1e1e;
    // border: 2px solid #444;
    // }

    // QListWidget::item {
    // padding: 6px;
    // }

    // QListWidget::item:selected {
    // background-color: #0078d7;
    // color: white;
    // }

    // QListWidget:focus {
    // border: 2px solid #00ffcc;
    // }

    // QListWidget:!focus {
    // border: 2px solid #444;
    // }
    // )";

    // ui->listWidget_Folder->setStyleSheet(style);
    // ui->listWidget_Images->setStyleSheet(style);


    basePath = QCoreApplication::applicationDirPath() + "/SavedData";

    /* --- Watch folders --- */
    watcher->addPath(basePath);
    connect(watcher, &QFileSystemWatcher::directoryChanged,
            this, &Openlog::loadFolders);

    /* --- Folder changed --- */
    connect(ui->listWidget_Folder,
            &QListWidget::currentItemChanged,
            this,
            &Openlog::onFolderChanged);

    /* --- Image activated --- */
    connect(ui->listWidget_Images,
            &QListWidget::itemActivated,
            this,
            &Openlog::onImageActivated);

    loadFolders();
    StatusLabTimer = new QTimer(this);

    StatusLabTimer->setSingleShot(true);

    connect(StatusLabTimer,
            &QTimer::timeout,
            this,
            [this]()
            {
                ui->label->clear();
            });

    connect(ui->comboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            [this](int)
            {
                loadFolders();

                // ComboBox still has focus,
                // so no date folder should be selected.
                ui->listWidget_Folder->setCurrentRow(-1);

                // No file/image should be selected.
                ui->listWidget_Images->clear();
                ui->listWidget_Images->setCurrentRow(-1);
            });

    // Initial focus on comboBox
    // Initial focus on comboBox
    // Initial focus on comboBox
    currentFocus = 0;

    ui->comboBox->setFocus();
    ui->comboBox->setCurrentIndex(0);

    // IMPORTANT:
    // No date folder should be selected while
    // focus is on the ComboBox.
    ui->listWidget_Folder->setCurrentRow(-1);

    // Images should not be selected initially.
    ui->listWidget_Images->setCurrentRow(-1);

    updateFocusStyle();
}

Openlog::~Openlog()
{
    delete ui;
}

/* -------------------- LOAD FOLDERS -------------------- */
// void Openlog::loadFolders()
// {
//     ui->listWidget_Folder->clear();

//     QDir dir(basePath);
//     QFileInfoList list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

//     for (const auto &fi : list)
//         ui->listWidget_Folder->addItem(fi.fileName());

//     if (ui->listWidget_Folder->count())
//         ui->listWidget_Folder->setCurrentRow(0);
// }
void Openlog::loadFolders()
{
    ui->listWidget_Folder->clear();

    QDir dir(basePath);

    QFileInfoList folders =
        dir.entryInfoList(
            QDir::Dirs |
            QDir::NoDotAndDotDot);

    // -----------------------------------------
    // Filter according to ComboBox selection
    // -----------------------------------------

    bool calibSelected =
        (ui->comboBox->currentText() == "Calib info");

    QFileInfoList filteredFolders;

    for (const QFileInfo &folder : folders)
    {
        QString folderName = folder.fileName();

        if (calibSelected)
        {
            // Calib info -> only folders ending with "Calib"
            if (folderName.endsWith("Calib",
                                    Qt::CaseInsensitive))
            {
                filteredFolders.append(folder);
            }
        }
        else
        {
            // Envelopes -> exclude Calib folders
            if (!folderName.endsWith("Calib",
                                     Qt::CaseInsensitive))
            {
                filteredFolders.append(folder);
            }
        }
    }

    // -----------------------------------------
    // Sort folders
    // -----------------------------------------

    std::sort(filteredFolders.begin(),
              filteredFolders.end(),
              [](const QFileInfo &a,
                 const QFileInfo &b)
              {
                  QString aName = a.fileName();
                  QString bName = b.fileName();

                  // Extract date part: DD-MM-YYYY
                  QString aDateStr = aName.left(10);
                  QString bDateStr = bName.left(10);

                  QDate aDate =
                      QDate::fromString(
                          aDateStr,
                          "dd-MM-yyyy");

                  QDate bDate =
                      QDate::fromString(
                          bDateStr,
                          "dd-MM-yyyy");

                  // Latest date first
                  if (aDate != bDate)
                      return aDate > bDate;

                  // Same date: sort suffix
                  QString aSuffix =
                      aName.mid(10).toUpper();

                  QString bSuffix =
                      bName.mid(10).toUpper();

                  QMap<QString, int> priority =
                      {
                          {"UP", 0},
                          {"DN", 1},
                          {"UD", 2},
                          {"CALIB", 3}
                      };

                  int aPriority =
                      priority.value(aSuffix, 99);

                  int bPriority =
                      priority.value(bSuffix, 99);

                  return aPriority < bPriority;
              });

    // -----------------------------------------
    // Add filtered folders to list
    // -----------------------------------------

    for (const QFileInfo &fi : filteredFolders)
    {
        ui->listWidget_Folder->addItem(
            fi.fileName());
    }

    // -----------------------------------------
    // Select first folder
    // -----------------------------------------

    if (ui->listWidget_Folder->count() > 0)
    {
        ui->listWidget_Folder->setCurrentRow(0);
    }
}

/* -------------------- LOAD IMAGES -------------------- */
void Openlog::onFolderChanged(QListWidgetItem *item)
{
    if (!item)
        return;

    QString selectedFolder = item->text();

    // Clear previous contents
    ui->listWidget_Images->clear();

    QDir dir(basePath + "/" + selectedFolder);

    QStringList filters{"*.jpg", "*.jpeg", "*.png"};

    QFileInfoList files =
        dir.entryInfoList(filters,
                          QDir::Files,
                          QDir::Name);

    for (const auto &f : files)
    {
        QListWidgetItem *it =
            new QListWidgetItem(f.completeBaseName());

        it->setData(Qt::UserRole, f.filePath());

        ui->listWidget_Images->addItem(it);
    }

    // IMPORTANT:
    // Do not select/highlight the first image yet.
    ui->listWidget_Images->setCurrentRow(-1);
}

/* -------------------- IMAGE FULLSCREEN -------------------- */
void Openlog::onImageActivated(QListWidgetItem *item)
{
    if (!item)
        return;

    if (currentViewer) {
        currentViewer->closeFullscreen();
        currentViewer = nullptr;
    }

    currentViewer = new FullscreenImageViewer(item->data(Qt::UserRole).toString(), this);
    connect(currentViewer, &QObject::destroyed,
            this, [this] { currentViewer = nullptr; });

}

/* -------------------- REMOTE / KEYPAD -------------------- */
void Openlog::handleRemoteKey(int key)
{

    QListWidget *currentList = nullptr;

    switch(currentFocus)
    {
    case 1:
        currentList = ui->listWidget_Folder;
        break;

    case 2:
        currentList = ui->listWidget_Images;
        break;

    default:
        currentList = nullptr;
        break;
    }

    // if(!currentList)
    //     return;

    int row =0;
    if(currentList!=nullptr)
        row = currentList->currentRow();

    if(key==ESC){
        if(currentViewer)
            {
                currentViewer->closeFullscreen();
                currentViewer = nullptr;
            }
            else
            {
                emit closeopenlogscreen();
            }
            return;
    }

    if(currentViewer != nullptr){
        return;
    }

    switch(key)
    {
    // case ESC:

    //     if(currentViewer)
    //     {
    //         currentViewer->closeFullscreen();
    //         currentViewer = nullptr;
    //     }
    //     else
    //     {
    //         emit closeopenlogscreen();
    //     }

    //     break;

    case UP:

        if (currentFocus == 0)
        {
            int index = ui->comboBox->currentIndex();

            if (index > 0)
                ui->comboBox->setCurrentIndex(index - 1);
        }
        else if (currentList != nullptr)
        {
            if (row > 0)
                currentList->setCurrentRow(row - 1);
        }

        break;


    case DOWN:

        if (currentFocus == 0)
        {
            int index = ui->comboBox->currentIndex();

            if (index < ui->comboBox->count() - 1)
                ui->comboBox->setCurrentIndex(index + 1);
        }
        else if (currentList != nullptr)
        {
            if (row < currentList->count() - 1)
                currentList->setCurrentRow(row + 1);
        }

        break;

    case RIGHT:

        qDebug() << "RIGHT CurrentFocus =" << currentFocus;

        if (currentFocus < 6)
        {
            currentFocus++;

            // -----------------------------------------
            // ComboBox -> Date Folder
            // -----------------------------------------
            if (currentFocus == 1)
            {
                // Move focus to Date Folder list
                ui->listWidget_Folder->setFocus();

                if (ui->listWidget_Folder->count() > 0)
                {
                    // Select first date folder
                    ui->listWidget_Folder->setCurrentRow(0);

                    // Immediately load its files
                    QListWidgetItem *firstItem =
                        ui->listWidget_Folder->currentItem();

                    if (firstItem)
                    {
                        onFolderChanged(firstItem);
                    }
                }

                updateFocusStyle();
            }

            // -----------------------------------------
            // Date Folder -> Images
            // -----------------------------------------
            else if (currentFocus == 2)
            {
                ui->listWidget_Images->setFocus();

                if (ui->listWidget_Images->count() > 0)
                {
                    ui->listWidget_Images->setCurrentRow(0);
                }

                updateFocusStyle();
            }

            // -----------------------------------------
            // Buttons
            // -----------------------------------------
            else
            {
                updateFocusStyle();
            }
        }

        break;
    case LEFT:

        qDebug() << "LEFT CurrentFocus =" << currentFocus;

        if (currentFocus > 0)
        {
            currentFocus--;

            if (currentFocus == 0)
            {
                ui->comboBox->setFocus();
            }
            else if (currentFocus == 1)
            {
                ui->listWidget_Folder->setFocus();
            }
            else if (currentFocus == 2)
            {
                ui->listWidget_Images->setFocus();
            }

            updateFocusStyle();
        }

        break;

    case OK:

        if(currentFocus == 1)
        {
            QListWidgetItem *item =
                ui->listWidget_Folder->currentItem();

            if(item)
                onFolderChanged(item);
        }

        else if(currentFocus == 2)
        {
            QListWidgetItem *item =
                ui->listWidget_Images->currentItem();

            if(item)
                onImageActivated(item);
        }

        else if(currentFocus == 3) // Copy Action
        {
            QString usbRoot = findUsbPath();

            if(usbRoot.isEmpty())
            {
                ui->label->setText(" ⚠️ No Pendrive Detected");
                StatusLabTimer->start(2000);
                return;
            }

            // -------------------------------------------------
            // WT<UnitNo>/Envelopes
            // -------------------------------------------------
            QString machineFolder =
                usbRoot + "/WT" + MachNo;

            if(!QDir().exists(machineFolder))
            {
                QDir().mkpath(machineFolder);
            }

            QString envelopesFolder =
                machineFolder + "/Envelopes";

            if(!QDir().exists(envelopesFolder))
            {
                QDir().mkpath(envelopesFolder);
            }

            // -------------------------------------------------
            // Get selected date folder
            // -------------------------------------------------
            QListWidgetItem *selectedItem =
                ui->listWidget_Folder->currentItem();

            if(!selectedItem)
                return;

            QString selectedDateFolder =
                selectedItem->text();

            // -------------------------------------------------
            // Decide destination:
            //
            // xxxxCalib -> Calibinfo
            // xxxxUP/DN/UD -> Defects
            // -------------------------------------------------
            QString destinationFolder;

            if(selectedDateFolder.endsWith(
                    "Calib",
                    Qt::CaseInsensitive))
            {
                destinationFolder =
                    envelopesFolder + "/Calibinfo";
            }
            else
            {
                destinationFolder =
                    envelopesFolder + "/Defects";
            }

            // Create destination
            if(!QDir().exists(destinationFolder))
            {
                QDir().mkpath(destinationFolder);
            }

            // -------------------------------------------------
            // Source
            // -------------------------------------------------
            QString src =
                basePath + "/" + selectedDateFolder;

            // -------------------------------------------------
            // Final destination
            // -------------------------------------------------
            QString dst =
                destinationFolder + "/" + selectedDateFolder;

            qDebug() << "Copy source:" << src;
            qDebug() << "Copy destination:" << dst;

            // -------------------------------------------------
            // Copy
            // -------------------------------------------------
            if(copyFolderRecursively(src, dst))
            {
                ui->label->setStyleSheet(
                    "color: #00ff66;"
                    "font-size: 11px;"
                    "font-weight: bold;");

                ui->label->setText(
                    "Copied " + selectedDateFolder);

                StatusLabTimer->start(2000);
            }
            else
            {
                ui->label->setStyleSheet(
                    "color: RED;"
                    "font-size: 11px;"
                    "font-weight: bold;");

                ui->label->setText(
                    "Copying " +
                    selectedDateFolder +
                    " has failed");

                StatusLabTimer->start(2000);
            }

            break;
        }
        else if(currentFocus == 4) // Copy All
        {
            QString usbRoot = findUsbPath();

            if(usbRoot.isEmpty())
            {
                ui->label->setText(" ⚠️ No Pendrive Detected");
                StatusLabTimer->start(2000);
                return;
            }

            // -------------------------------------------------
            // WT<UnitNo>/Envelopes
            // -------------------------------------------------
            QString machineFolder =
                usbRoot + "/WT" + MachNo;

            if(!QDir().exists(machineFolder))
            {
                QDir().mkpath(machineFolder);
            }

            QString envelopesFolder =
                machineFolder + "/Envelopes";

            if(!QDir().exists(envelopesFolder))
            {
                QDir().mkpath(envelopesFolder);
            }

            // -------------------------------------------------
            // Create both destination folders
            // -------------------------------------------------
            QString defectsFolder =
                envelopesFolder + "/Defects";

            QString calibInfoFolder =
                envelopesFolder + "/Calibinfo";


            if(!QDir().exists(defectsFolder))
            {
                QDir().mkpath(defectsFolder);
            }

            if(!QDir().exists(calibInfoFolder))
            {
                QDir().mkpath(calibInfoFolder);
            }

            // -------------------------------------------------
            // Get ALL folders from SavedData
            // -------------------------------------------------
            QDir srcDir(basePath);

            QFileInfoList folders =
                srcDir.entryInfoList(
                    QDir::Dirs |
                    QDir::NoDotAndDotDot);

            bool allCopied = true;

            // -------------------------------------------------
            // Copy each folder according to its type
            // -------------------------------------------------
            for(const QFileInfo &folder : folders)
            {
                QString folderName =
                    folder.fileName();

                QString destinationFolder;

                if(folderName.endsWith(
                        "Calib",
                        Qt::CaseInsensitive))
                {
                    // Calibration folder
                    destinationFolder =
                        calibInfoFolder;
                }
                else
                {
                    // Normal date/defect folder
                    destinationFolder =
                        defectsFolder;
                }

                QString src =
                    folder.absoluteFilePath();

                QString dst =
                    destinationFolder + "/" + folderName;

                qDebug() << "Copy All source:" << src;
                qDebug() << "Copy All destination:" << dst;

                if(!copyFolderRecursively(src, dst))
                {
                    allCopied = false;

                    qDebug()
                        << "Failed to copy:"
                        << folderName;
                }
            }

            // -------------------------------------------------
            // Result
            // -------------------------------------------------
            if(allCopied)
            {
                ui->label->setStyleSheet(
                    "color: #00ff66;"
                    "font-size: 11px;"
                    "font-weight: bold;");

                ui->label->setText(
                    "All folders copied successfully");

                StatusLabTimer->start(2000);
            }
            else
            {
                ui->label->setStyleSheet(
                    "color: RED;"
                    "font-size: 11px;"
                    "font-weight: bold;");

                ui->label->setText(
                    "Some folders failed to copy");

                StatusLabTimer->start(2000);
            }

            break;
        }
        else if(currentFocus == 5){ //delete
            QString selectedDateFolder = ui->listWidget_Folder->currentItem()->text();

            QString path =
                basePath + "/" +
                selectedDateFolder;

            QDir(path).removeRecursively();

            ui->label->setStyleSheet(
                "color: RED;"
                "font-size: 11px;"
                "font-weight: bold;");
            ui->label->setText("Deleted "+selectedDateFolder);
            StatusLabTimer->start(2000);

            loadFolders();

            ui->listWidget_Images->clear();
            break;
        }
        else if(currentFocus == 6){ //delete all
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
            StatusLabTimer->start(2000);
            loadFolders();

            ui->listWidget_Folder->clear();
            ui->listWidget_Images->clear();
            break;
        }

        break;


    }
}
void Openlog::updateFocusStyle()
{
    ui->comboBox->setStyleSheet("");
    ui->listWidget_Folder->setStyleSheet("");
    ui->listWidget_Images->setStyleSheet("");
    ui->CopyButton->setStyleSheet("");
    ui->CopyAllButton->setStyleSheet("");
    ui->DeleteButton->setStyleSheet("");
    ui->DeleteAllButton->setStyleSheet("");

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

    QString comboFocusStyle =
        "QComboBox {"
        "border: 2px solid #3aa0ff;"
        "background-color: #dceeff;"
        "border-radius: 6px;"
        "}";

    switch(currentFocus)
    {
    case 0:
        ui->comboBox->setStyleSheet(comboFocusStyle);
        break;

    case 1:
        ui->listWidget_Folder->setStyleSheet(focusStyle);
        break;

    case 2:
        ui->listWidget_Images->setStyleSheet(focusStyle);
        break;

    case 3:
        ui->CopyButton->setStyleSheet(focusStyle2);
        break;

    case 4:
        ui->CopyAllButton->setStyleSheet(focusStyle2);
        break;

    case 5:
        ui->DeleteButton->setStyleSheet(focusStyle2);
        break;

    case 6:
        ui->DeleteAllButton->setStyleSheet(focusStyle2);
        break;
    }
}
