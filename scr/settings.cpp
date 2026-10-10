#include "settings.h"
#include "ui_settings.h"

#include <QApplication>
#include <QDate>
#include <QDateTime>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QPushButton>
#include <QSettings>
#include <QStyle>
#include <QTime>
#include <QWidget>
#include <QLineEdit>
#include <QProcess>
#include "matrix_keypad.h"

const char *ORG_NAME = "Paras Electronics";
const char *APP_NAME = "WT2311D";
namespace
{

const QString DARK_THEME =
    "QDialog, QWidget { background-color: #070b10; color: #f2f5f8; }"
    "QGroupBox { border: 1px solid #2a3542; border-radius: 8px; margin-top: 10px; padding: 12px 8px 8px 8px; font-weight: 600; }"
    "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 5px; }"
    "QLabel { color: #f2f5f8; }"
    "QComboBox, QSpinBox, QLineEdit { background: #121821; color: #f2f5f8; border: 1px solid #2a3542; border-radius: 5px; padding: 5px; }"
    "QComboBox QAbstractItemView { background: #121821; color: #f2f5f8; selection-background-color: #2d6cdf; }"
    "QPushButton { background: #121821; color: #f2f5f8; border: 1px solid #2a3542; border-radius: 6px; padding: 6px 12px; }"
    "QPushButton:hover { border: 1px solid #4b8cff; }"
    "QPushButton:focus, QComboBox:focus, QSpinBox:focus, QLineEdit:focus { border: 2px solid #78b4ff; }"
    "QSlider::groove:horizontal { height: 6px; background: #2a3542; border-radius: 3px; }"
    "QSlider::handle:horizontal { width: 16px; margin: -5px 0; border-radius: 8px; background: #78b4ff; }"
    "QCheckBox { spacing: 8px; }";

const QString LIGHT_THEME =
    "QDialog, QWidget { background-color: #eef2f6; color: #18212b; }"
    "QGroupBox { border: 1px solid #aeb8c4; border-radius: 8px; margin-top: 10px; padding: 12px 8px 8px 8px; font-weight: 600; }"
    "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 5px; }"
    "QLabel { color: #18212b; }"
    "QComboBox, QSpinBox, QLineEdit { background: white; color: #18212b; border: 1px solid #aeb8c4; border-radius: 5px; padding: 5px; }"
    "QComboBox QAbstractItemView { background: white; color: #18212b; selection-background-color: #9ec5ff; }"
    "QPushButton { background: white; color: #18212b; border: 1px solid #aeb8c4; border-radius: 6px; padding: 6px 12px; }"
    "QPushButton:hover { border: 1px solid #3976c9; }"
    "QPushButton:focus, QComboBox:focus, QSpinBox:focus, QLineEdit:focus { border: 2px solid #3976c9; }"
    "QSlider::groove:horizontal { height: 6px; background: #aeb8c4; border-radius: 3px; }"
    "QSlider::handle:horizontal { width: 16px; margin: -5px 0; border-radius: 8px; background: #3976c9; }";
}

settings::settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::settings)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    ui->setupUi(this);

    // Basic fixed color choices. No QColorDialog/palette window is used.
    const QStringList colorNames = {
        "Magenta", "Yellow", "Red", "Green", "Blue",
        "Cyan", "Orange", "Purple", "White", "Gray", "Black"
    };

    const QStringList colorValues = {
        "#FF00FF", "#FFFF00", "#FF0000", "#00FF00", "#0000FF",
        "#00FFFF", "#FFA500", "#800080", "#FFFFFF", "#808080", "#000000"
    };

    ui->comboChannel1->clear();
    ui->comboChannel2->clear();

    for (int i = 0; i < colorNames.size(); ++i)
    {
        ui->comboChannel1->addItem(colorNames[i], colorValues[i]);
        ui->comboChannel2->addItem(colorNames[i], colorValues[i]);
    }

    setWindowTitle("Settings");
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    // The settings page is designed for the 640x480 display used by the UI.
    setMinimumSize(640, 480);
    resize(640, 480);

    // Keep navigation predictable for the hardware keypad.
    ui->comboTheme->setFocusPolicy(Qt::StrongFocus);
    ui->comboChannel1->setFocusPolicy(Qt::StrongFocus);
    ui->comboChannel2->setFocusPolicy(Qt::StrongFocus);
    ui->sliderAudio->setFocusPolicy(Qt::StrongFocus);
    ui->lineEditDate->setFocusPolicy(Qt::StrongFocus);
    ui->lineEditHour->setFocusPolicy(Qt::StrongFocus);
    ui->lineEditMinute->setFocusPolicy(Qt::StrongFocus);
    ui->lineEditSecond->setFocusPolicy(Qt::StrongFocus);
    ui->buttonReset->setFocusPolicy(Qt::StrongFocus);
    ui->buttonApply->setFocusPolicy(Qt::StrongFocus);
    ui->buttonCancel->setFocusPolicy(Qt::StrongFocus);
    ui->buttonOk->setFocusPolicy(Qt::StrongFocus);

    connect(ui->comboChannel1, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &settings::updateChannel1Color);
    connect(ui->comboChannel2, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &settings::updateChannel2Color);

    connect(ui->buttonApply, &QPushButton::clicked,
            this, &settings::applySettings);

    connect(ui->buttonReset, &QPushButton::clicked,
            this, &settings::resetDefaults);

    connect(ui->buttonCancel, &QPushButton::clicked,
            this, &QDialog::reject);

    connect(ui->buttonOk, &QPushButton::clicked, this, [this]() {
        saveSettings();
        applyApplicationTheme(parentWidget());
        emit settingsChanged();
        accept();
    });

    connect(ui->comboTheme,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &settings::onThemeChanged);

    loadSettings();

    // First keypad selection is always clearly visible.
    setCurrentWidget(ui->comboTheme);
    activateWindow();
    raise();
}

settings::~settings()
{
    delete ui;
}

QString settings::theme()
{
    QSettings s(ORG_NAME, APP_NAME);
    return s.value("appearance/theme", "Dark").toString();
}

QColor settings::channel1Color()
{
    QSettings s(ORG_NAME, APP_NAME);
    return QColor(s.value("appearance/channel1Color", "#00ff00").toString());
}

QColor settings::channel2Color()
{
    QSettings s(ORG_NAME, APP_NAME);
    return QColor(s.value("appearance/channel2Color", "#00aaff").toString());
}

int settings::audioLevel()
{
    QSettings s(ORG_NAME, APP_NAME);
    return s.value("audio/level", 75).toInt();
}

QString settings::dateFormat()
{
    QSettings s(ORG_NAME, APP_NAME);
    return s.value("datetime/dateFormat", "dd-MM-yyyy").toString();
}

QString settings::timeFormat()
{
    QSettings s(ORG_NAME, APP_NAME);
    return s.value("datetime/timeFormat", "HH:mm:ss").toString();
}

QDate settings::selectedDate()
{
    QSettings s(ORG_NAME, APP_NAME);

    return s.value("datetime/date", QDate::currentDate()).toDate();
}
QTime settings::selectedTime()
{
    QSettings s(ORG_NAME, APP_NAME);

    return s.value("datetime/time", QTime::currentTime()).toTime();
}

bool settings::autoSave()
{
    return false;
}

int settings::brightness()
{
    return 100;
}

void settings::applyApplicationTheme(QWidget *window)
{
    if (!window)
        return;

    if (theme().compare("Light", Qt::CaseInsensitive) == 0)
        window->setStyleSheet(LIGHT_THEME);
    else
        window->setStyleSheet(DARK_THEME);
}

void settings::loadSettings()
{
    QSettings s(ORG_NAME, APP_NAME);

    const QString theme = s.value("appearance/theme", "Dark").toString();
    const QColor ch1 = QColor(s.value("appearance/channel1Color", "#00ff00").toString());
    const QColor ch2 = QColor(s.value("appearance/channel2Color", "#00aaff").toString());

    ui->comboTheme->setCurrentText(theme);
    const QColor defaultCh1("#FF00FF");   // Magenta
    const QColor defaultCh2("#FFFF00");   // Yellow

    int ch1Index = ui->comboChannel1->findData(
        ch1.isValid() ? ch1.name(QColor::HexRgb).toUpper()
                      : defaultCh1.name(QColor::HexRgb).toUpper());

    int ch2Index = ui->comboChannel2->findData(
        ch2.isValid() ? ch2.name(QColor::HexRgb).toUpper()
                      : defaultCh2.name(QColor::HexRgb).toUpper());

    if (ch1Index < 0) ch1Index = 0;
    if (ch2Index < 0) ch2Index = 1;

    ui->comboChannel1->setCurrentIndex(ch1Index);
    ui->comboChannel2->setCurrentIndex(ch2Index);

    updateChannel1Color(ch1Index);
    updateChannel2Color(ch2Index);

    ui->sliderAudio->setValue(s.value("audio/level", 70).toInt());

    const QDate savedDate =
        s.value("datetime/date", QDate::currentDate()).toDate();
    const QTime savedTime =
        s.value("datetime/time", QTime::currentTime()).toTime();

    ui->lineEditDate->setText(savedDate.toString("dd-MM-yyyy"));
    ui->lineEditHour->setText(savedTime.toString("HH"));
    ui->lineEditMinute->setText(savedTime.toString("mm"));
    ui->lineEditSecond->setText(savedTime.toString("ss"));

    // Clear keypad input buffers whenever settings are loaded.
    ui->lineEditDate->setProperty("inputBuffer", ui->lineEditDate->text());
    ui->lineEditHour->setProperty("inputBuffer", ui->lineEditHour->text());
    ui->lineEditMinute->setProperty("inputBuffer", ui->lineEditMinute->text());
    ui->lineEditSecond->setProperty("inputBuffer", ui->lineEditSecond->text());

    applyThemePreview(theme);
}

void settings::saveSettings()
{
    QSettings s(ORG_NAME, APP_NAME);

    s.setValue("appearance/theme", ui->comboTheme->currentText());

    s.setValue("appearance/channel1Color",
               ui->comboChannel1->currentData().toString());

    s.setValue("appearance/channel2Color",
               ui->comboChannel2->currentData().toString());

    s.setValue("audio/level", ui->sliderAudio->value());

    const QDate date =
        QDate::fromString(ui->lineEditDate->text().trimmed(),
                          "dd-MM-yyyy");

    bool okH = false;
    bool okM = false;
    bool okS = false;

    const int hour =
        ui->lineEditHour->text().toInt(&okH);

    const int minute =
        ui->lineEditMinute->text().toInt(&okM);

    const int second =
        ui->lineEditSecond->text().toInt(&okS);

    if (date.isValid())
        s.setValue("datetime/date", date);

    if (okH && okM && okS &&
        hour >= 0 && hour <= 23 &&
        minute >= 0 && minute <= 59 &&
        second >= 0 && second <= 59)
    {
        s.setValue("datetime/time",
                   QTime(hour, minute, second));
    }

    s.sync();
}

void settings::updateChannel1Color(int index)
{
    if (index < 0 || index >= ui->comboChannel1->count())
        return;

    const QColor color(ui->comboChannel1->itemData(index).toString());
    ui->comboChannel1->setStyleSheet(
        QString("QComboBox {"
                "background-color: %1;"
                "color: %2;"
                "border: 1px solid #2a3542;"
                "border-radius: 5px;"
                "padding: 5px;"
                "}"
                "QComboBox:focus {"
                "border: 2px solid #3aa0ff;"
                "}")
            .arg(color.name(),
                 color.lightnessF() > 0.55 ? "#000000" : "#ffffff"));
}

void settings::updateChannel2Color(int index)
{
    if (index < 0 || index >= ui->comboChannel2->count())
        return;

    const QColor color(ui->comboChannel2->itemData(index).toString());
    ui->comboChannel2->setStyleSheet(
        QString("QComboBox {"
                "background-color: %1;"
                "color: %2;"
                "border: 1px solid #2a3542;"
                "border-radius: 5px;"
                "padding: 5px;"
                "}"
                "QComboBox:focus {"
                "border: 2px solid #3aa0ff;"
                "}")
            .arg(color.name(),
                 color.lightnessF() > 0.55 ? "#000000" : "#ffffff"));
}

namespace
{
bool applySystemDateTime(QLineEdit *dateEdit,
                         QLineEdit *hourEdit,
                         QLineEdit *minuteEdit,
                         QLineEdit *secondEdit)
{
    const QString dateText = dateEdit->text().trimmed();
    const QDate date = QDate::fromString(dateText, "dd-MM-yyyy");

    bool okH = false, okM = false, okS = false;
    const int hour = hourEdit->text().toInt(&okH);
    const int minute = minuteEdit->text().toInt(&okM);
    const int second = secondEdit->text().toInt(&okS);

    if (!date.isValid() ||
        !okH || !okM || !okS ||
        hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 ||
        second < 0 || second > 59)
    {
        qWarning() << "[Settings] Invalid date/time:"
                   << dateText << hour << minute << second;
        return false;
    }

    // Linux date expects YYYY-MM-DD HH:MM:SS.
    const QString dateTime =
        QString("%1 %2:%3:%4")
            .arg(date.toString("yyyy-MM-dd"))
            .arg(hour, 2, 10, QLatin1Char('0'))
            .arg(minute, 2, 10, QLatin1Char('0'))
            .arg(second, 2, 10, QLatin1Char('0'));

    qDebug() << "[Settings] Setting system clock:" << dateTime;

    const int dateResult =
        QProcess::execute("date", QStringList() << "-s" << dateTime);

    if (dateResult != 0)
    {
        qWarning() << "[Settings] date -s failed, exit code:" << dateResult;
        return false;
    }

    const int rtcResult =
        QProcess::execute("hwclock", QStringList() << "-w");

    if (rtcResult != 0)
    {
        qWarning() << "[Settings] hwclock -w failed, exit code:" << rtcResult;
        return false;
    }

    qDebug() << "[Settings] System clock and RTC synchronized.";
    return true;
}
}

void settings::applySettings()
{
    if (!applySystemDateTime(ui->lineEditDate,
                             ui->lineEditHour,
                             ui->lineEditMinute,
                             ui->lineEditSecond))
    {
        qDebug() << "[Settings] Date/time was not applied.";
        return;
    }

    saveSettings();
    applyApplicationTheme(parentWidget());
    emit settingsChanged();
}

void settings::resetDefaults()
{
    ui->comboTheme->setCurrentText("Dark");

    ui->comboChannel1->setCurrentIndex(0); // Magenta
    ui->comboChannel2->setCurrentIndex(1); // Yellow
    updateChannel1Color(0);
    updateChannel2Color(1);

    ui->sliderAudio->setValue(70);

    const QDate nowDate = QDate::currentDate();
    const QTime nowTime = QTime::currentTime();

    ui->lineEditDate->setText(nowDate.toString("dd-MM-yyyy"));
    ui->lineEditHour->setText(nowTime.toString("HH"));
    ui->lineEditMinute->setText(nowTime.toString("mm"));
    ui->lineEditSecond->setText(nowTime.toString("ss"));

    ui->lineEditDate->setProperty("inputBuffer", ui->lineEditDate->text());
    ui->lineEditHour->setProperty("inputBuffer", ui->lineEditHour->text());
    ui->lineEditMinute->setProperty("inputBuffer", ui->lineEditMinute->text());
    ui->lineEditSecond->setProperty("inputBuffer", ui->lineEditSecond->text());

    applyThemePreview("Dark");
    applyApplicationTheme(parentWidget());
}

void settings::onThemeChanged(int index)
{
    Q_UNUSED(index);
    applyThemePreview(ui->comboTheme->currentText());
}

void settings::applyThemePreview(const QString &theme)
{
    if (theme.compare("Light", Qt::CaseInsensitive) == 0)
        setStyleSheet(LIGHT_THEME);
    else
        setStyleSheet(DARK_THEME);
}

QList<QWidget*> settings::navigationWidgets() const
{
    return {
        ui->comboTheme,
        ui->comboChannel1,
        ui->comboChannel2,
        ui->sliderAudio,
        ui->lineEditDate,
        ui->lineEditHour,
        ui->lineEditMinute,
        ui->lineEditSecond,
        ui->buttonReset,
        ui->buttonApply,
        ui->buttonCancel,
        ui->buttonOk
    };
}

void settings::setCurrentWidget(QWidget *widget)
{
    if (!widget)
        return;

    const QList<QWidget*> widgets = navigationWidgets();

    for (QWidget *w : widgets)
    {
        if (w)
            w->setStyleSheet("");
    }

    m_currentIndex = widgets.indexOf(widget);
    if (m_currentIndex < 0)
        return;

    const QString focusStyle =
        "QWidget {"
        "border: 2px solid #3aa0ff;"
        // "background-color: #dceeff;"
        "border-radius: 6px;"
        "}";

    widget->setStyleSheet(focusStyle);

    widget->setFocus(Qt::OtherFocusReason);
    widget->update();

    qDebug() << "[SettingsFocus]"
             << "index =" << m_currentIndex
             << "widget =" << widget->objectName()
             << "hasFocus =" << widget->hasFocus();
}

void settings::handleSocketKey(int key)
{
    const QList<QWidget*> widgets = navigationWidgets();

    if (key == UP)
    {
        m_currentIndex--;
        if (m_currentIndex < 0)
            m_currentIndex = widgets.size() - 1;

        setCurrentWidget(widgets[m_currentIndex]);
        return;
    }

    if (key == DOWN)
    {
        m_currentIndex++;
        if (m_currentIndex >= widgets.size())
            m_currentIndex = 0;

        setCurrentWidget(widgets[m_currentIndex]);
        return;
    }

    QWidget *current = widgets.value(m_currentIndex, nullptr);
    if (!current)
        return;

    /*
     * Same keypad mapping used by TestScreen:
     * 1 -> 1, A/B/C -> 1
     * 2 -> 2, D/E/F -> 2
     * ...
     * 8 -> 8, W/X/Y/Z -> 8
     * 9 or '-' -> 9
     *
     * The date field is the one exception: '-' is kept as a real
     * date separator because its required format is DD-MM-YYYY.
     */
    auto mapNumericKey = [](int k) -> QChar {
        if (k >= '0' && k <= '9')
            return QChar(static_cast<char>(k));

        switch (k)
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
        default: return QChar();
        }
    };

    auto isDateField = [this](QLineEdit *le) {
        return le == ui->lineEditDate;
    };

    auto isTimeField = [this](QLineEdit *le) {
        return le == ui->lineEditHour ||
               le == ui->lineEditMinute ||
               le == ui->lineEditSecond;
    };

    if (auto *lineEdit = qobject_cast<QLineEdit*>(current))
    {
        // BACKSPACE: same "remove last entered character" behavior as TestScreen.
        if (key == BACKSPACE)
        {
            QString buffer = lineEdit->property("inputBuffer").toString();
            if (buffer.isEmpty())
                buffer = lineEdit->text();

            if (!buffer.isEmpty())
            {
                buffer.chop(1);

                lineEdit->setText(buffer);
                lineEdit->setProperty("inputBuffer", buffer);
            }
            return;
        }

        if (isDateField(lineEdit))
        {
            if (key == '-')
            {
                QString buffer = lineEdit->property("inputBuffer").toString();
                if (buffer.isEmpty())
                    buffer = lineEdit->text();

                // Only allow separators after DD and MM.
                if ((buffer.length() == 2 || buffer.length() == 5) &&
                    !buffer.endsWith('-'))
                {
                    buffer += '-';
                    lineEdit->setText(buffer);
                    lineEdit->setProperty("inputBuffer", buffer);
                }
                return;
            }

            const QChar mapped = mapNumericKey(key);
            if (mapped.isNull())
                return;

            QString buffer = lineEdit->property("inputBuffer").toString();
            if (buffer.isEmpty())
                buffer = lineEdit->text();

            // Do not allow digits after the final 10-character date.
            if (buffer.length() >= 10)
                return;

            // Prevent the date from becoming longer than DD-MM-YYYY.
            if (buffer.length() == 2 || buffer.length() == 5)
            {
                if (!buffer.endsWith('-'))
                    buffer += '-';
            }

            if (buffer.length() < 10)
                buffer += mapped;

            lineEdit->setText(buffer);
            lineEdit->setProperty("inputBuffer", buffer);
            return;
        }

        if (isTimeField(lineEdit))
        {
            const QChar mapped = mapNumericKey(key);
            if (mapped.isNull())
                return;

            QString buffer = lineEdit->property("inputBuffer").toString();
            if (buffer.isEmpty())
                buffer = lineEdit->text();

            if (buffer.length() >= 2)
                return;

            buffer += mapped;
            lineEdit->setText(buffer);
            lineEdit->setProperty("inputBuffer", buffer);
            return;
        }
    }

    if (key == LEFT || key == RIGHT)
    {
        const int step = (key == RIGHT) ? 1 : -1;

        if (auto *combo = qobject_cast<QComboBox*>(current))
        {
            int index = combo->currentIndex() + step;
            if (index < 0)
                index = combo->count() - 1;
            if (index >= combo->count())
                index = 0;
            combo->setCurrentIndex(index);
            return;
        }

        if (auto *slider = qobject_cast<QSlider*>(current))
        {
            slider->setValue(slider->value() + step * slider->pageStep());
            return;
        }

        // For line edits, LEFT/RIGHT are not used for value changes.
        return;
    }

    if (key == OK)
    {
        if (auto *button = qobject_cast<QPushButton*>(current))
        {
            button->click();
            return;
        }
    }
}