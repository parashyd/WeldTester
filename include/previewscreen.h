#ifndef PREVIEWSCREEN_H
#define PREVIEWSCREEN_H

#include <QDialog>

namespace Ui {
class PreviewScreen;
}


class PreviewScreen : public QDialog
{
    Q_OBJECT

public:

    explicit PreviewScreen(QWidget *parent = nullptr);
    void setDetails(const QString &ModelNo, const QString &FilePath);
    void setupPlotAppearance();
    void handleSocketKey(quint8 key);
    //void init();
    //void clear_fields();
    //void closeEvent(QCloseEvent *event);
    //void updateGridInterval();
    void plotGraphFromFilePS();


    ~PreviewScreen();

private slots:
    void on_pushButton_save_clicked();
    void LoadSaveFileHeader();

signals:
    void requestTestScreen();

private:
    Ui::PreviewScreen *ui;
};

#endif // PREVIEWSCREEN_H
