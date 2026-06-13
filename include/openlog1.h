#ifndef OPENLOG1_H
#define OPENLOG1_H

#include <QDialog>
#include <QWidget>
#include <QListWidgetItem>
extern QString selectedFilePath;
namespace Ui {
class OpenLog1;
}

class OpenLog1 : public QDialog
{
    Q_OBJECT

public:
    explicit OpenLog1(QWidget *parent = nullptr);
    void handleSocketKey(quint8 key);
    ~OpenLog1();

private:
    Ui::OpenLog1 *ui;
    QString basePath = "LogData";

    QString selectedDateFolder;

    QString selectedWeldFolder;
    QTimer *StatusLabelTimer;

    void loadDateFolders();

    void loadWeldFolders(const QString &dateFolder);

    void loadFiles(const QString &weldFolder);
    void updateFocusStyle();
    QString findUsbPath();
    QString uniqueFolderName(QString parent, QString folderName);
    bool copyFolderRecursively(const QString &src,const QString &dst);






private slots:
    void onDateFolderClicked(QListWidgetItem *item);
    void onWeldFolderClicked(QListWidgetItem *item);

signals:
    void requestViewLogData();
};

#endif // OPENLOG1_H
