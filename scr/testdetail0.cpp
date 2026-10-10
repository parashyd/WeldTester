#include "testdetail0.h"
#include "ui_testdetail0.h"
#include "matrix_keypad.h"
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "QTimer"
#include "mainwindow.h"
#include <QSettings>
#include <QStyle>
#include "settings.h"
enum class InputMode {
    Numeric,
    Alphabetic,
    Alphanumeric,
    None   // no input allowed
};

QMap<QLineEdit*, InputMode> inputModesTD0;

QString OperatorName;
QString Division ;
QString Section ;
QString Line ;
QString Testtype;
QString WeldNo;
QString km;
QString M;
QString Rail ;
QString LineOthers;


namespace
{

const QString DARK_TESTDETAIL_THEME = R"(
QWidget#testdetail0 {
    background-color: #070b10;
    color: #f2f5f8;
}

/* Header */
QFrame#frameHeader {
    background-color: #0c1116;
    border: 1px solid #1e2833;
    border-radius: 6px;
}

/* Main frame */
QFrame#frameMain {
    background-color: #0e141a;
    border: 1px solid #2a3542;
    border-radius: 6px;
}

/* Labels */
QLabel {
    background: transparent;
    color: #f2f5f8;
    border: none;
    font-size: 16px;
    font-weight: 600;
}

QLabel#label_Title {
    background: transparent;
    color: #f2f5f8;
    border: none;
    font-size: 16px;
    font-weight: bold;
}

/* Line edits */
QLineEdit {
    background-color: #46515D;
    color: #F2F5F8;
    border: 1px solid #667381;
    border-radius: 8px;
    min-height: 28px;
    padding-left: 8px;
}

QLineEdit:focus {
    background-color: #111821;
    color: #ffffff;
    border: 2px solid #3aa0ff;
}

/* Combo boxes */
QComboBox {
    background-color: #0b1015;
    color: #ffffff;
    border: 1px solid #2a3440;
    border-radius: 5px;
    padding: 4px;
    min-height: 28px;
}

QComboBox:focus {
    background-color: #111821;
    color: #ffffff;
    border: 2px solid #3aa0ff;
}

QComboBox QAbstractItemView {
    background-color: #121821;
    color: #ffffff;
    selection-background-color: #2d6cdf;
    selection-color: #ffffff;
    border: 1px solid #2a3542;
}

/* Test button */
QPushButton {
    background-color: #55606D;
    color: #ffffff;
    border: 1px solid #748191;
    border-radius: 8px;
    min-height: 20px;
    padding: 4px 10px;
}

QPushButton:hover {
    background-color: #627081;
    border: 1px solid #4DB3FF;
}

QPushButton:focus {
    background-color: #627081;
    border: 2px solid #3aa0ff;
    color: #ffffff;
}

QLineEdit[logicalFocus="true"],
QComboBox[logicalFocus="true"] {
    background-color: #78b4ff;
    color: #101820;
    border: 2px solid #ffffff;
}
QLineEdit:focus {
    background-color: #111821;
    color: #ffffff;
    border: 2px solid #3aa0ff;
}

QLineEdit[logicalFocus="true"] {
    background-color: #78b4ff;
    color: #101820;
    border: 2px solid #ffffff;
}
)";


const QString LIGHT_TESTDETAIL_THEME = R"(
QWidget#testdetail0 {
    background-color: #eef2f6;
    color: #18212b;
}

/* Header */
QFrame#frameHeader {
    background-color: #ffffff;
    border: 1px solid #c5ced8;
    border-radius: 6px;
}

/* Main frame */
QFrame#frameMain {
    background-color: #ffffff;
    border: 1px solid #c5ced8;
    border-radius: 6px;
}

/* Labels */
QLabel {
    background: transparent;
    color: #18212b;
    border: none;
    font-size: 16px;
    font-weight: 600;
}

QLabel#label_Title {
    background: transparent;
    color: #18212b;
    border: none;
    font-size: 16px;
    font-weight: bold;
}

/* Line edits */
QLineEdit {
    background-color: #ffffff;
    color: #18212b;
    border: 1px solid #aeb8c4;
    border-radius: 8px;
    min-height: 28px;
    padding-left: 8px;
}

QLineEdit:focus {
    background-color: #ffffff;
    color: #18212b;
    border: 2px solid #3976c9;
}

/* Combo boxes */
QComboBox {
    background-color: #ffffff;
    color: #18212b;
    border: 1px solid #aeb8c4;
    border-radius: 5px;
    padding: 4px;
    min-height: 28px;
}

QComboBox:focus {
    background-color: #ffffff;
    color: #18212b;
    border: 2px solid #3976c9;
}

QComboBox QAbstractItemView {
    background-color: #ffffff;
    color: #18212b;
    selection-background-color: #9ec5ff;
    selection-color: #18212b;
    border: 1px solid #aeb8c4;
}

/* Test button */
QPushButton {
    background-color: #ffffff;
    color: #18212b;
    border: 1px solid #aeb8c4;
    border-radius: 8px;
    min-height: 20px;
    padding: 4px 10px;
}

QPushButton:hover {
    background-color: #f3f7fb;
    border: 1px solid #3976c9;
}

QPushButton:focus {
    background-color: #dceeff;
    border: 2px solid #3976c9;
    color: #18212b;
}
QLineEdit[logicalFocus="true"],
QComboBox[logicalFocus="true"] {
    background-color: #dceeff;
    color: #18212b;
    border: 2px solid #3aa0ff;
}
QLineEdit:focus {
    background-color: #ffffff;
    color: #18212b;
    border: 2px solid #3976c9;
}

QLineEdit[logicalFocus="true"] {
    background-color: #dceeff;
    color: #18212b;
    border: 2px solid #3aa0ff;
}
)";

}
testdetail0::testdetail0(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::testdetail0)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    applyTheme();
    QPixmap pixmap = this->grab();

    // --- Adjust JPG output size ---
    QSize jpgTargetSize(640, 480); // You can change this (640x480, 1024x768, etc.)
    QPixmap scaledJpg = pixmap.scaled(jpgTargetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QString jpgFileName = QString("TestDeatails.jpg");
    scaledJpg.save(jpgFileName, "JPG", 100 );

    ui->lineothersEdit->setVisible(false);

    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]() {
        QDateTime now = QDateTime::currentDateTime();
        ui->dateEdit->setText(now.toString("dd-MM-yyyy"));
        ui->timeEdit->setText(now.toString("HH:mm:ss"));
    });

    timer->start(1000);

    inputModesTD0[ui->opEdit]=InputMode::Alphabetic;
    inputModesTD0[ui->divEdit]=InputMode::Alphanumeric;
    inputModesTD0[ui->secEdit]=InputMode::Alphanumeric;
    inputModesTD0[ui->lineothersEdit]=InputMode::Alphanumeric;
    inputModesTD0[ui->kmEdit]=InputMode::Alphanumeric;
    inputModesTD0[ui->mtrEdit]=InputMode::Numeric;
    inputModesTD0[ui->weldNoEdit]=InputMode::Alphanumeric;

    display_mc_no();
    LoadSavedData();
}

void testdetail0::display_mc_no(void){
    QFile file("BIN.BIN");

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open BIN.BIN";
        return;
    }

    // BIN.BIN contains 2 bytes: 0x90 0x00
    QByteArray data = file.readAll();
    file.close();

    if (data.size() < 2)
    {
        qDebug() << "Invalid file";
        return;
    }

    // Little-endian 16-bit integer
    quint16 value = static_cast<quint8>(data[0]) |
                    (static_cast<quint8>(data[1]) << 8);

    // Print as 4-digit number: 0144
    MachNo= QString("%1").arg(value, 4, 10, QChar('0'));

    ui->unitEdit->setText(MachNo);
    qDebug() << "Mach No : "<<MachNo;
}
void testdetail0::setLogicalFocus(QWidget *widget)
{
    if (!widget)
        return;

    // Remove focus state from previous widget
    if (m_currentLogicalFocus && m_currentLogicalFocus != widget)
    {
        m_currentLogicalFocus->setProperty("logicalFocus", false);
        m_currentLogicalFocus->style()->unpolish(m_currentLogicalFocus);
        m_currentLogicalFocus->style()->polish(m_currentLogicalFocus);
        m_currentLogicalFocus->update();
    }

    // Apply focus state
    widget->setProperty("logicalFocus", true);

    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
    widget->update();

    m_currentLogicalFocus = widget;

    if (widget->isVisible() && widget->isEnabled())
    {
        widget->setFocus(Qt::OtherFocusReason);

        if (auto le = qobject_cast<QLineEdit*>(widget))
            le->setCursorPosition(0);
    }

    qDebug() << "[LogicalFocus] Widget:"
             << widget->objectName()
             << "Visible:" << widget->isVisible()
             << "Enabled:" << widget->isEnabled()
             << "hasFocus:" << widget->hasFocus();
}



void testdetail0::navFocus(int direction)
{
    QList<QWidget*> navWidgets = {
        ui->opEdit,
        ui->divEdit,
        ui->secEdit,
        ui->lineCombo,
        ui->lineothersEdit,
        ui->weldTypeCombo,
        ui->testTypeCombo,
        ui->kmEdit,
        ui->mtrEdit,
        ui->RailcomboBox,
        ui->weldNoEdit
       //ui->TestButton
    };

    // 🔹 Trust Qt's real focus
    QWidget *current = focusWidget();

    // Fallback only if nothing focused
    if (!current || !navWidgets.contains(current)){
        current = ui->opEdit;
        setLogicalFocus(current);
        return;
    }

    setLogicalFocus(current);
    int index = navWidgets.indexOf(current);


    int newIndex = index + direction;
    if(current == ui->lineCombo)
    {
        if(ui->lineCombo->currentText()== "Others" )
        {
            ui->lineothersEdit->setVisible(true);
        }
        else if(ui->lineCombo->currentData()!="Others")
        {
            ui->lineothersEdit->setVisible(false);
            if(direction==1)
                newIndex+=1;
        }
    }
    if(current == ui->weldTypeCombo)
    {
        if(!ui->lineothersEdit->isVisible() && direction == -1)
        {
            if(direction == -1)
            {
                newIndex-=1;
            }
        }
    }

    if (newIndex < 0)
        newIndex = navWidgets.size() - 1;
    else if (newIndex >= navWidgets.size())
        newIndex = 0;

    QWidget *newWidget = navWidgets[newIndex];

    setLogicalFocus(newWidget);


    // newWidget->setFocus(Qt::OtherFocusReason);

    // 🔹 Move cursor for LineEdit
    qDebug() << "Moved focus from"
             << current->objectName()
             << "to"
             << newWidget->objectName();
}


void testdetail0::adjustComboBoxIndex(int delta)
{

    QWidget* focused = this->focusWidget();
    if (!focused) {
        qDebug() << "No focused widget.";
        return;
    }

    qDebug() << "Focused widget:" << focused->objectName()
             << "Type:" << focused->metaObject()->className();

    QComboBox* combo = qobject_cast<QComboBox*>(focused);
    if (!combo) {
        qDebug() << "Focused widget is not a QComboBox.";
        return;
    }

    int currentIndex = combo->currentIndex();
    int count = combo->count();

    if (count == 0) {
        qDebug() << "ComboBox is empty.";
        return;
    }

    int newIndex = qBound(0, currentIndex + delta, count - 1);
    combo->setCurrentIndex(newIndex);

    qDebug() << "ComboBox" << combo->objectName()
             << "changed to index" << newIndex
             << "value:" << combo->currentText();
}

void testdetail0::saveDataToFile()
{
    QFile file("testdetails.json");
    QJsonObject obj;

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);

        if(doc.isObject())
            obj = doc.object();
    }

    obj["OpName"] = ui->opEdit->text();
    obj["Div"] = ui->divEdit->text();
    obj["Sec"]=ui->secEdit->text();
    obj["Line"] = ui->lineCombo->currentText();
    if(ui->lineothersEdit->isVisible())
        obj["LineOthers"] = ui->lineothersEdit->text();
    obj["WeldType"]=ui->weldTypeCombo->currentText();
    obj["TestType"]=ui->testTypeCombo->currentText();
    obj["Km"]=ui->kmEdit->text();
    obj["Mtr"]=ui->mtrEdit->text();
    obj["Rail"]=ui->RailcomboBox->currentText();
    obj["WeldNo"]=ui->weldNoEdit->text();

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(obj).toJson());
        file.close();
        qDebug() << "[INFO] TestDetails saved to file.";
    }
    OperatorName = ui->opEdit->text();
    Division = ui->divEdit->text();
    Section = ui->secEdit->text();
    Line = ui->lineCombo->currentText();
    if(ui->lineothersEdit->isVisible()){
    LineOthers =ui->lineothersEdit->text();
    }
    Testtype = ui->testTypeCombo->currentText();
    WeldNo = ui->weldNoEdit->text();
    km = ui->kmEdit->text();
    M = ui->mtrEdit->text();
    Rail = ui->RailcomboBox->currentText();
}
void testdetail0::LoadSavedData()
{
    QFile file("testdetails.json");
    if (!file.exists()) {
        return;
    }

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();


        // Restore other fields from last session
        ui->opEdit->setText(obj["OpName"].toString());
        ui->divEdit->setText(obj["Div"].toString());
        ui->secEdit->setText(obj["Sec"].toString());
        ui->lineCombo->setCurrentText(obj["Line"].toString());
        if(ui->lineothersEdit->isVisible())
            ui->lineothersEdit->setText(obj["LineOthers"].toString());
        ui->weldTypeCombo->setCurrentText(obj["WeldType"].toString());
        ui->testTypeCombo->setCurrentText(obj["TestType"].toString());
        ui->kmEdit->setText(obj["Km"].toString());
        ui->mtrEdit->setText(obj["Mtr"].toString());
        ui->RailcomboBox->setCurrentText(obj["Rail"].toString());
        ui->weldNoEdit->setText(obj["WeldNo"].toString());
    }
}

void testdetail0::handleSocketKey(quint8 key)
{
    switch (key) {
    case LEFT:
        navFocus(-1);
        return;

    case RIGHT:
        navFocus(+1);
        return;

    case DOWN:
        adjustComboBoxIndex(+1);
        return;

    case UP:
        adjustComboBoxIndex(-1);
        return;

    case BACKSPACE:
        qDebug() << "Backspace" << key;
        handleBackspaceInput();
        return;

    case SAVE:
        saveDataToFile();
        emit requestTestscreen();
        return;

    case OK:
    {
        QWidget* focused = m_currentFocusedWidget ? m_currentFocusedWidget : this->focusWidget();
        if (!focused) return;
        if (auto btn = qobject_cast<QPushButton*>(focused))
        {
            btn->click();
            emit requestTestscreen();
            return;
        }
    }

    }

    QWidget* current = focusWidget();

    QLineEdit *lineEdit = nullptr;
    if (QLineEdit *le = qobject_cast<QLineEdit*>(current)) {
        lineEdit = le;
    }

    if (!lineEdit) {
        qDebug() << "Focused widget does not accept text input";
        return;
    }

    InputMode mode = inputModesTD0.value(lineEdit, InputMode::Alphanumeric);
    switch (mode)
    {
    case InputMode::None:
        qDebug() << "[BLOCKED] Input disabled for:" << lineEdit->objectName();
        return;

    case InputMode::Numeric:
        handleMultiPressAlpha(key, lineEdit);   // ✅ let alpha handler manage digits
        return;

    case InputMode::Alphabetic:
    case InputMode::Alphanumeric:
        handleMultiPressAlpha(key, lineEdit);
        return;
    }
}
void testdetail0::handleMultiPressAlpha(quint8 key, QLineEdit *lineEdit)
{
    if (!lineEdit) return;

    InputMode mode = inputModesTD0.value(lineEdit, InputMode::Alphanumeric);
    KeyPressState &state = inputStates[lineEdit];

    QChar keyChar(key); // Convert received int to QChar

    if (key == '.' || key == '*')
    {
        quint8 groupKey = 200;
        QString cycle = ".*";

        if (state.lastKey == groupKey &&
            state.timer.isValid() &&
            state.timer.elapsed() <= 1000)
        {
            // Same special key pressed again:
            // . <-> *
            state.pressCount =
                (state.pressCount + 1) % cycle.length();
        }
        else
        {
            // Commit previous pending character
            if (!state.lastChar.isEmpty())
                state.inputBuffer += state.lastChar;

            state.pressCount = 0;
        }

        state.lastKey = groupKey;
        state.lastChar =
            QString(cycle.at(state.pressCount));

        state.timer.restart();

        QString display =
            state.inputBuffer +
            state.lastChar;

        lineEdit->setText(display);
        lineEdit->setCursorPosition(display.length());

        return;
    }
    // ---------- '-' <-> '_' ----------
    if (key == '-' || key == '_')
    {
        quint8 groupKey = 201;
        QString cycle = "-_";

        if (state.lastKey == groupKey &&
            state.timer.isValid() &&
            state.timer.elapsed() <= 1000)
        {
            state.pressCount =
                (state.pressCount + 1) % cycle.length();
        }
        else
        {
            if (!state.lastChar.isEmpty())
                state.inputBuffer += state.lastChar;

            state.pressCount = 0;
        }

        state.lastKey = groupKey;
        state.lastChar = QString(cycle.at(state.pressCount));
        state.timer.restart();

        QString display = state.inputBuffer + state.lastChar;

        lineEdit->setText(display);
        lineEdit->setCursorPosition(display.length());

        return;
    }
    // ---------- SPACE ----------
    if (key == ' ')
    {
        // Commit previous pending character
        if (!state.lastChar.isEmpty())
        {
            state.inputBuffer += state.lastChar;
            state.lastChar.clear();
        }

        // Insert one space
        state.inputBuffer += ' ';

        // Reset multitap state
        state.lastKey = 0;
        state.pressCount = 0;
        state.timer.invalidate();

        lineEdit->setText(state.inputBuffer);
        lineEdit->setCursorPosition(state.inputBuffer.length());

        qDebug() << "Special: SPACE Buffer:" << state.inputBuffer;
        return;
    }
    /* ---------------- Numeric Mode ---------------- */
    /* ---------------- Numeric Mode ---------------- */
    if (mode == InputMode::Numeric)
    {
        QString digit;

        if (key >= '0' && key <= '9') {
            digit = QChar(key);
        }
        else if (key >= 'A' && key <= 'Z') {

            static QMap<QChar, QChar> letterToDigit = {
                {'A','1'},{'B','1'},{'C','1'},
                {'D','2'},{'E','2'},{'F','2'},
                {'G','3'},{'H','3'},{'I','3'},
                {'J','4'},{'K','4'},{'L','4'},
                {'M','5'},{'N','5'},{'O','5'},
                {'P','6'},{'Q','6'},{'R','6'},{'S','6'},
                {'T','7'},{'U','7'},{'V','7'},
                {'W','8'},{'X','8'},{'Y','8'},{'Z','8'}
            };

            digit = letterToDigit.value(QChar(key), QChar());
        }

        // Do NOT handle '.', '-', '*' here.
        // They are already handled above as special characters.

        if (digit.isEmpty()) {
            qDebug() << "[BLOCKED] Invalid numeric key:" << key;
            return;
        }

        /*
     * IMPORTANT:
     * If a special character such as '.' is currently
     * displayed as state.lastChar, commit it before
     * adding the numeric digit.
     *
     * Example:
     *     press '.'  -> lastChar = "."
     *     press '2'  -> inputBuffer becomes "."
     *                   then append "2"
     *                   result = ".2"
     */
        if (!state.lastChar.isEmpty())
        {
            qDebug() << "[Numeric] Committing pending character:"
                     << state.lastChar;

            state.inputBuffer += state.lastChar;
            state.lastChar.clear();
        }

        // Numeric digit is immediately committed
        state.inputBuffer += digit;

        // Reset multi-tap state
        state.lastKey = 0;
        state.pressCount = 0;
        state.timer.invalidate();

        lineEdit->setText(state.inputBuffer);
        lineEdit->setCursorPosition(state.inputBuffer.length());

        qDebug() << "[UI] Numeric updated:"
                 << lineEdit->objectName()
                 << "→"
                 << state.inputBuffer;

        return;
    }


    /* ---------------- Alphabetic Mode ---------------- */
    if (mode == InputMode::Alphabetic)
    {

        static QMap<quint8, QString> multiTapAlphaMap = {
            { '1', "ABC" }, { '2', "DEF" }, { '3', "GHI" },
            { '4', "JKL" }, { '5', "MNO" }, { '6', "PQRS" },
            { '7', "TUV" }, { '8', "WXYZ" }, { '9', " " }, { '0', "0" }
        };

        static QMap<QChar, QChar> letterToNumericKey = {
            {'A','1'},{'B','1'},{'C','1'},
            {'D','2'},{'E','2'},{'F','2'},
            {'G','3'},{'H','3'},{'I','3'},
            {'J','4'},{'K','4'},{'L','4'},
            {'M','5'},{'N','5'},{'O','5'},
            {'P','6'},{'Q','6'},{'R','6'},{'S','6'},
            {'T','7'},{'U','7'},{'V','7'},
            {'W','8'},{'X','8'},{'Y','8'},{'Z','8'}
        };

        static QMap<quint8, QString> keyMap = {
            { 'A',"A"},{'B',"B"},{'C',"C"},{'D',"D"},{'E',"E"},{'F',"F"},
            {'G',"G"},{'H',"H"},{'I',"I"},{'J',"J"},{'K',"K"},{'L',"L"},
            {'M',"M"},{'N',"N"},{'O',"O"},{'P',"P"},{'Q',"Q"},{'R',"R"},
            {'S',"S"},{'T',"T"},{'U',"U"},{'V',"V"},{'W',"W"},{'X',"X"},
            {'Y',"Y"},{'Z',"Z"}
        };

        // -------- Normalize key for multi-tap --------
        quint8 multiTapKey = key;
        if (letterToNumericKey.contains(QChar(key))) {
            multiTapKey = static_cast<quint8>(letterToNumericKey[QChar(key)].unicode());
        }

        QString filtered;
        if (multiTapAlphaMap.contains(multiTapKey)) {
            filtered = multiTapAlphaMap[multiTapKey]; // numeric key → letters
        } else if (keyMap.contains(key)) {
            filtered = keyMap[key]; // single letters
        } else {
            return; // unknown key
        }

        // -------- Multi-tap logic --------
        if (state.lastKey == multiTapKey && state.timer.isValid() && state.timer.elapsed() <= 1000) {
            state.pressCount = (state.pressCount + 1) % filtered.length();
        } else {
            if (!state.lastChar.isEmpty()) {
                state.inputBuffer += state.lastChar; // commit previous char
            }
            state.pressCount = 0;
        }

        state.lastChar = filtered[state.pressCount];
        state.lastKey = multiTapKey;
        state.timer.restart();

        QString combined = state.inputBuffer + state.lastChar;
        lineEdit->setText(combined);
        lineEdit->setCursorPosition(combined.length());

        qDebug() << "[UI] Key:" << char(key)
                 << "PressCount:" << state.pressCount
                 << "Output:" << combined;
        return;
    }


    /* ---------------- Alphanumeric Mode ---------------- */
    // Map letters to numeric groups
    static QMap<QChar, QChar> alphaToNum = {
        {'A','1'},{'B','1'},{'C','1'},
        {'D','2'},{'E','2'},{'F','2'},
        {'G','3'},{'H','3'},{'I','3'},
        {'J','4'},{'K','4'},{'L','4'},
        {'M','5'},{'N','5'},{'O','5'},
        {'P','6'},{'Q','6'},{'R','6'},{'S','6'},
        {'T','7'},{'U','7'},{'V','7'},
        {'W','8'},{'X','8'},{'Y','8'},{'Z','8'}
    };

    quint8 normalizedKey = key;
    if (key >= 'A' && key <= 'Z') {
        normalizedKey = static_cast<quint8>(alphaToNum.value(QChar(key), QChar(0)).unicode());
        if (normalizedKey == 0) {
            qDebug() << "[BLOCKED] Unknown alpha key:" << char(key);
            return;
        }
    }

    // Alphanumeric multi-tap cycle
    static QMap<quint8, QString> cycleMap = {
        { '1',"1ABC" }, { '2',"2DEF" }, { '3',"3GHI" },
        { '4',"4JKL" }, { '5',"5MNO" }, { '6',"6PQRS" },
        { '7',"7TUV" }, { '8',"8WXYZ" }, { '9',"9" },
        { '0',"0" }, { '-'," -" }
    };

    if (!cycleMap.contains(normalizedKey)) {
        qDebug() << "[BLOCKED] Unsupported key:" << char(key);
        return;
    }

    const QString &cycle = cycleMap[normalizedKey];

    if (state.lastKey == normalizedKey &&
        state.timer.isValid() &&
        state.timer.elapsed() <= 1000)
    {
        // Same numeric key within timeout:
        // continue multi-tap cycle
        state.pressCount =
            (state.pressCount + 1) % cycle.length();
    }
    else
    {
        // A different key was pressed.
        // Commit the previous pending character,
        // including '.' or '*'.
        if (!state.lastChar.isEmpty())
        {
            state.inputBuffer += state.lastChar;
            state.lastChar.clear();
        }

        state.pressCount = 0;
    }

    state.lastKey  = normalizedKey;
    state.lastChar = QString(cycle.at(state.pressCount));
    state.timer.restart();

    QString display = state.inputBuffer + state.lastChar;
    lineEdit->setText(display);
    lineEdit->setCursorPosition(display.length());

    qDebug() << "[UI] Key:" << char(key)
             << "Normalized:" << char(normalizedKey)
             << "Cycle:" << cycle
             << "Index:" << state.pressCount
             << "Text:" << display;
}

void testdetail0::handleBackspaceInput()
{
    QWidget* focusedWidget = this->focusWidget();

    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(focusedWidget)) {
        KeyPressState &state = inputStates[lineEdit];

        if (!state.lastChar.isEmpty()) {
            // 🔹 Remove only current multi-tap character
            state.lastChar.clear();
        }
        else if (!state.inputBuffer.isEmpty()) {
            // 🔹 Remove last committed character
            state.inputBuffer.chop(1);
        }

        //reset multi-tap state
        state.lastKey = 0;
        state.pressCount = 0;
        state.timer.invalidate();

        QString updated = state.inputBuffer + state.lastChar;
        lineEdit->setText(updated);
        lineEdit->setCursorPosition(updated.length());

        qDebug() << "[UI] Backspace fixed, buffer:" << updated;
        return;
    }

}
void testdetail0::applyTheme()
{
    const QString currentTheme = settings::theme();

    if (currentTheme.compare("Light", Qt::CaseInsensitive) == 0)
    {
        setStyleSheet(LIGHT_TESTDETAIL_THEME);
        qDebug() << "[TestDetails] Light theme applied";
    }
    else
    {
        setStyleSheet(DARK_TESTDETAIL_THEME);
        qDebug() << "[TestDetails] Dark theme applied";
    }
}

testdetail0::~testdetail0()
{
    delete ui;
}
