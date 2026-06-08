#include "openlog.h"
#include "ui_Openlog.h"

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

    currentFocus = 0;

    ui->listWidget_Folder->setCurrentRow(0);

    updateFocusStyle();
}

Openlog::~Openlog()
{
    delete ui;
}

/* -------------------- LOAD FOLDERS -------------------- */
void Openlog::loadFolders()
{
    ui->listWidget_Folder->clear();

    QDir dir(basePath);
    QFileInfoList list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    for (const auto &fi : list)
        ui->listWidget_Folder->addItem(fi.fileName());

    if (ui->listWidget_Folder->count())
        ui->listWidget_Folder->setCurrentRow(0);
}

/* -------------------- LOAD IMAGES -------------------- */
void Openlog::onFolderChanged(QListWidgetItem *item)
{
    if (!item) return;

    ui->listWidget_Images->clear();

    QDir dir(basePath + "/" + item->text());
    QStringList filters{"*.jpg", "*.jpeg", "*.png"};
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Name);

    for (const auto &f : files) {
        QListWidgetItem *it = new QListWidgetItem(f.completeBaseName());
        it->setData(Qt::UserRole, f.filePath());
        ui->listWidget_Images->addItem(it);
    }

    if (ui->listWidget_Images->count())
        ui->listWidget_Images->setCurrentRow(0);
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
    case 0:
        currentList = ui->listWidget_Folder;
        break;

    case 1:
        currentList = ui->listWidget_Images;
        break;
    }

    if(!currentList)
        return;

    int row = currentList->currentRow();

    switch(key)
    {
    case UP:

        if(row > 0)
            currentList->setCurrentRow(row - 1);

        break;

    case DOWN:

        if(row < currentList->count() - 1)
            currentList->setCurrentRow(row + 1);

        break;

    case RIGHT:

        if(currentFocus == 0 &&
            ui->listWidget_Images->count() > 0)
        {
            currentFocus = 1;

            updateFocusStyle();

            ui->listWidget_Images->setCurrentRow(0);
        }

        break;

    case LEFT:

        if(currentFocus == 1)
        {
            currentFocus = 0;

            updateFocusStyle();
        }

        break;

    case OK:

        if(currentFocus == 0)
        {
            QListWidgetItem *item =
                ui->listWidget_Folder->currentItem();

            if(item)
                onFolderChanged(item);
        }

        else if(currentFocus == 1)
        {
            QListWidgetItem *item =
                ui->listWidget_Images->currentItem();

            if(item)
                onImageActivated(item);
        }

        break;

    case ESC:

        if(currentViewer)
        {
            currentViewer->closeFullscreen();
            currentViewer = nullptr;
        }
        else
        {
            emit closeopenlogscreen();
        }

        break;
    }
}
void Openlog::updateFocusStyle()
{
    ui->listWidget_Folder->setStyleSheet("");
    ui->listWidget_Images->setStyleSheet("");

    QString focusStyle =
        "QListWidget {"
        "border: 2px solid #3aa0ff;"
        "background-color: #dceeff;"
        "border-radius: 6px;"
        "}";

    switch(currentFocus)
    {
    case 0:
        ui->listWidget_Folder->setStyleSheet(focusStyle);
        break;

    case 1:
        ui->listWidget_Images->setStyleSheet(focusStyle);
        break;
    }
}
