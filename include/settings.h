#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QColor>
#include <QDate>
#include <QTime>
#include <QKeyEvent>
#include <QString>
#include <QList>

extern const char *ORG_NAME;
extern const char *APP_NAME;

class QWidget;

namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();

    // Called by MainWindow when UP/DOWN/LEFT/RIGHT/OK are received
    // from the hardware keypad.
    void handleSocketKey(int key);

    // Application-wide values saved by this settings page.
    static QString theme();
    static QColor channel1Color();
    static QColor channel2Color();
    static int audioLevel();
    static QString dateFormat();
    static QString timeFormat();
    static QDate selectedDate();
    static QTime selectedTime();
    static bool autoSave();
    static int brightness();
    static void applyApplicationTheme(QWidget *window);

signals:
    void settingsChanged();

private slots:
    void updateChannel1Color(int index);
    void updateChannel2Color(int index);
    void applySettings();
    void resetDefaults();
    void onThemeChanged(int index);

private:
    Ui::settings *ui;

    void loadSettings();
    void saveSettings();
    void updateColorButton(class QPushButton *button, const QColor &color);
    void applyThemePreview(const QString &theme);
    void setCurrentWidget(class QWidget *widget);
    QList<class QWidget*> navigationWidgets() const;

    int m_currentIndex = 0;
};

#endif // SETTINGS_H
