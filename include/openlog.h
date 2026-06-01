#ifndef OPENLOG_H
#define OPENLOG_H

#include <QWidget>
#include <QFileSystemWatcher>
#include <QListWidgetItem>


QT_BEGIN_NAMESPACE
namespace Ui { class Openlog; }   // match your .ui filename
QT_END_NAMESPACE

class FullscreenImageViewer;   // <<< forward declaration

class Openlog : public QWidget
{
    Q_OBJECT
public:
    explicit Openlog(QWidget *parent = nullptr);
    ~Openlog();


    void handleRemoteKey(int keyCode);

    // signals:
    //     void requestClose();   // notify TestScreen

private slots:
    void loadFolders();
    void onFolderChanged(QListWidgetItem *item);
    void onImageActivated(QListWidgetItem *item);

    // protected:
    //     void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::Openlog *ui;
    QFileSystemWatcher *watcher;
    QString basePath;

    QListWidget *currentList = nullptr;
    FullscreenImageViewer *currentViewer = nullptr;
signals:
    void closeopenlogscreen();
};

#endif // OPENLOG_H
