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
    : QWidget(parent),
    ui(new Ui::Openlog),
    watcher(new QFileSystemWatcher(this)),
    currentViewer(nullptr)
{
    ui->setupUi(this);

    /* --- Focus setup --- */
    setFocusPolicy(Qt::NoFocus); // Parent itself does not take focus

    ui->listWidget_Folder->setFocusPolicy(Qt::StrongFocus);
    ui->listWidget_Images->setFocusPolicy(Qt::StrongFocus);

    /* ✅ Focus-aware styling */
    QString style = R"(
    QListWidget {
    color: white;
    background-color: #1e1e1e;
    border: 2px solid #444;
    }

    QListWidget::item {
    padding: 6px;
    }

    QListWidget::item:selected {
    background-color: #0078d7;
    color: white;
    }

    QListWidget:focus {
    border: 2px solid #00ffcc;
    }

    QListWidget:!focus {
    border: 2px solid #444;
    }
    )";

    ui->listWidget_Folder->setStyleSheet(style);
    ui->listWidget_Images->setStyleSheet(style);

    connect(ui->listWidget_Folder, &QListWidget::itemSelectionChanged,
            this, [this]() {
                if (ui->listWidget_Folder->hasFocus()) {
                    ui->listWidget_Images->clearSelection();
                    currentList = ui->listWidget_Folder;
                }
            });

    connect(ui->listWidget_Images, &QListWidget::itemSelectionChanged,
            this, [this]() {
                if (ui->listWidget_Images->hasFocus()) {
                    ui->listWidget_Folder->clearSelection();
                    currentList = ui->listWidget_Images;
                }
            });


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

    /* --- Initial focus --- */
    currentList = ui->listWidget_Folder;
    currentList->setFocus();
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
void Openlog::handleRemoteKey(int keyCode)
{
    auto *folder = ui->listWidget_Folder;
    auto *image  = ui->listWidget_Images;

    switch (keyCode)
    {
    case UP:
    case DOWN: {
        int delta = (keyCode == UP) ? -1 : 1;
        int row = qBound(0, currentList->currentRow() + delta, currentList->count() - 1);
        currentList->setCurrentRow(row);
        break;
    }

    case LEFT:
        currentList = folder;
        folder->setFocus();
        break;

    case RIGHT:
        if (image->count()) {
            currentList = image;
            image->setFocus();
        }
        break;

    case OK:
        if (currentList == image)
            onImageActivated(image->currentItem());
        break;


    case ESC:
        if (currentViewer)
        {
            qDebug() << "ESC -> closing fullscreen image";
            // Forward ESC to the fullscreen window
            currentViewer->closeFullscreen();
            currentViewer = nullptr;
        } else {
            emit closeopenlogscreen();
            // close
            //close_openlog();
        }
        break;

    }
}

