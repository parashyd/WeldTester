#include "testdetails.h"
#include "ui_testdetails.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "matrix_keypad.h"
#include "testscreen.h"
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "testdetail0.h"

 QString Mode;
 QString TP;
 QString LocOfDefect ;
 QString stations;
 QString rollmark;
 QString weld;
 QString Probe;
 QString classVal;
 QString FPNFP;
 QString OthersObservation;
 QString Time;
 QString Date;



enum class InputMode {
    Numeric,
    Alphabetic,
    Alphanumeric,
    None   // no input allowed
};

QMap<QLineEdit*, InputMode> inputModes;

QJsonObject obj;

extern QString DC_SC_mode;


TestDetails::TestDetails(QWidget *parent)
    : QDialog(parent)
    , testdetails(new Ui::TestDetails)
    , previewscreen(nullptr)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    testdetails->setupUi(this);
    loadDataFromFile();

    QTimer::singleShot(0, this, [this]() {
        testdetails->lineEdit_Km->setFocus();
        testdetails->lineEdit_Km->setCursorPosition(0);
        testdetails->lineEdit_Km->selectAll();
    });

    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]() {
        QDateTime now = QDateTime::currentDateTime();
        testdetails->lineEdit_Date->setText(now.toString("dd-MM-yyyy"));
        testdetails->lineEdit_Time->setText(now.toString("HH:mm:ss"));
    });

    timer->start(1000);

    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* now){
        if (now) {
            setLogicalFocus(now);  // auto-highlight current focus
        }
    });


    testdetails->lineEdit_UnitNo->setText(MachNo);                    //unitno: taken from bin file

    testdetails->lineEdit_CH->setText(QString::number(config.channel));
    testdetails->lineEdit_Calset->setText(QString::number(config.calset));

    testdetails->comboBox_Probe->setCurrentIndex(config.Angle);                  // Angle (correction)
    testdetails->comboBox_Mode->setCurrentText(DC_SC_mode);
    testdetails->lineEdit_Km->setText(km);
    testdetails->lineEdit_Mtr->setText(M);
    testdetails->comboBox_Rail->setCurrentText(Rail);

    testdetails->comboBox_LocOfDefect->setEditable(false);
    testdetails->comboBox_Probe->setEditable(false);

    inputModes[testdetails->lineEdit_Stations] = InputMode::Alphabetic;
    inputModes[testdetails->lineEdit_Km] = InputMode::Alphanumeric;
    inputModes[testdetails->lineEdit_Mtr] = InputMode::Numeric;
    inputModes[testdetails->lineEdit_TP] = InputMode::Alphanumeric;
    inputModes[testdetails->lineEdit_Weldno] = InputMode::Alphanumeric;
    inputModes[testdetails->lineEdit_Rollmarkyear] = InputMode::Alphanumeric;
    inputModes[testdetails->lineEdit_Date] = InputMode::Numeric;
    inputModes[testdetails->lineEdit_Time] = InputMode::Alphanumeric;
    inputModes[testdetails->lineEdit_OtherObservation] = InputMode::Alphabetic;

}

void TestDetails::setLogicalFocus(QWidget* widget)
{
    if (!widget) return;

    // 1️⃣ Remove previous highlight
    if (m_currentLogicalFocus && m_currentLogicalFocus != widget) {
        // Reset style to default
        m_currentLogicalFocus->setStyleSheet("");
    }

    // 2️⃣ Apply highlight to the new widget
    widget->setStyleSheet(
        "background-color: rgb(120,180,255);"
        "border: 2px solid black;"
        );

    m_currentLogicalFocus = widget;

    // 3️⃣ Optional: attempt to give real Qt focus
    if (widget->isVisible() && widget->isEnabled()) {
        // For Wayland, this may not guarantee OS focus, but it's harmless
        widget->setFocus(Qt::OtherFocusReason);

        // If it's a QLineEdit, put cursor at start
        if (auto le = qobject_cast<QLineEdit*>(widget)) {
            le->setCursorPosition(0);
        }
    }

    // 4️⃣ Debug output
    qDebug() << "[LogicalFocus] Widget:" << widget->objectName()
             << "Visible:" << widget->isVisible()
             << "Enabled:" << widget->isEnabled()
             << "hasFocus:" << widget->hasFocus();
}


TestDetails::~TestDetails()
{
    delete testdetails;
}

void TestDetails::PreviewClick()
{
    saveDataToFile();
    Rail = testdetails->comboBox_Rail->currentText();
    Mode = testdetails->comboBox_Mode->currentText();
    km = testdetails->lineEdit_Km->text();
    M = testdetails->lineEdit_Mtr->text();
    TP = testdetails->lineEdit_TP->text();
    LocOfDefect = testdetails->comboBox_LocOfDefect->currentText();
    stations = testdetails->lineEdit_Stations->text();
    rollmark = testdetails->comboBox_Rollmark1->currentText() + testdetails->comboBox_kg->currentText() + testdetails->comboBox_Month->currentText() + testdetails->lineEdit_Rollmarkyear->text();
    weld = testdetails->comboBox_WeldType->currentText() + "-" + testdetails->lineEdit_Weldno->text();
    Probe = testdetails->comboBox_Probe->currentText();
    classVal = testdetails->comboBox_Class->currentText();
    FPNFP = testdetails->comboBox_FP->currentText();
    OthersObservation = testdetails->lineEdit_OtherObservation->text();   //combo
    Date = testdetails->lineEdit_Date->text();
    Time = testdetails->lineEdit_Time->text();

    emit requestPreview();
}


void TestDetails::handleSocketKey(quint8 key)
{
    QWidget* current = focusWidget();
    qDebug() << "Received key of test details(int):" << static_cast<int>(key);

    switch (key)
    {

    case SAVE:
        PreviewClick();
        break;

    case UP:   // UP
        qDebug() << "UP ARROW" << key;
        navigateFocus(-1);
        return;

    case DOWN:   // DOWN
        qDebug() << "DOWN ARROW" << key;
        navigateFocus(1);
        return;

    case BACKSPACE:   // Backspace
        qDebug() << "Backspace" << key;
        handleBackspaceInput();
        return;

    case DEC:   // '-'
        qDebug() << "'-' arrow" << key;
        adjustComboBoxIndex(-1);
        return;

    case INC:   // '+'
        qDebug() << "'+' arrow" << key;
        adjustComboBoxIndex(1);
        return;
    }

    // 🔹 Text-capable widget
    QLineEdit *lineEdit = nullptr;
    if (QLineEdit *le = qobject_cast<QLineEdit*>(current)) {
        lineEdit = le;
    } else if (QComboBox *combo = qobject_cast<QComboBox*>(current)) {
        if (combo->isEditable()) {
            lineEdit = combo->lineEdit();
        } else {
            qDebug() << "ComboBox not editable — ignoring input.";
            return;
        }
    }

    if (!lineEdit) {
        qDebug() << "Focused widget does not accept text input";
        return;
    }

    // 🔹 Get mode for this field
    InputMode mode = inputModes.value(lineEdit, InputMode::Alphanumeric);

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





void TestDetails::handleMultiPressAlpha(quint8 key, QLineEdit *lineEdit)
{
    if (!lineEdit) return;

    InputMode mode = inputModes.value(lineEdit, InputMode::Alphanumeric);
    KeyPressState &state = inputStates[lineEdit];

    QChar keyChar(key); // Convert received int to QChar
    //quint8 keyVal = static_cast<quint8>(keyChar.unicode()); // For comparisons with lastKey

    //-------- Handle special characters (common to all modes) --------
        if (keyChar == '*' || keyChar == '-' || keyChar == '.' || keyChar == '\r' || keyChar == '\x14') {
            QChar c = (keyChar == '\x14') ? '-' : keyChar; // Ctrl-T (\x14) → dash
            state.inputBuffer += c;
            lineEdit->setText(state.inputBuffer);
            lineEdit->setCursorPosition(state.inputBuffer.length());
            qDebug() << "[UI] Special Key:" << c << "Text:" << state.inputBuffer;
            return;
        }

    /* ---------------- Numeric Mode ---------------- */
    if (mode == InputMode::Numeric)
    {
        QString digit;
        if (key >= '0' && key <= '9') {
            digit = QChar(key);
        } else if (key >= 'A' && key <= 'Z') {
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
        } else if (key == '.') digit = ".";
        else if (key == '-') digit = "-";
        else if (key == '*') digit = "*";

        if (digit.isEmpty()) {
            qDebug() << "[BLOCKED] Invalid numeric key:" << key;
            return;
        }

        state.inputBuffer += digit;
        lineEdit->setText(state.inputBuffer);
        lineEdit->setCursorPosition(state.inputBuffer.length());
        qDebug() << "[UI] Numeric updated:" << lineEdit->objectName() << "→" << state.inputBuffer;
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

    // Multi-tap logic
    if (state.lastKey == normalizedKey &&
        state.timer.isValid() &&
        state.timer.elapsed() <= 1000)
    {
        state.pressCount = (state.pressCount + 1) % cycle.length();
    } else {
        if (!state.lastChar.isEmpty()) state.inputBuffer += state.lastChar;
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



void TestDetails::navigateFocus(int direction)
{
    QList<QWidget*> navWidgets = {
        testdetails->lineEdit_Date,
        testdetails->lineEdit_Time,
        testdetails->lineEdit_Km,
        testdetails->lineEdit_Mtr,
        testdetails->lineEdit_TP,
        testdetails->comboBox_WeldType,
        testdetails->lineEdit_Stations,
        testdetails->comboBox_Rail,
        testdetails->lineEdit_Weldno,
        testdetails->comboBox_Probe,
        testdetails->comboBox_Rollmark1,
        testdetails->comboBox_kg,
        testdetails->comboBox_Month,
        testdetails->lineEdit_Rollmarkyear,
        testdetails->comboBox_Mode,
        testdetails->comboBox_LocOfDefect,
        testdetails->comboBox_Type,
        testdetails->comboBox_Class,
        testdetails->comboBox_FP,
        testdetails->lineEdit_OtherObservation,
    };

    QWidget *current = focusWidget();
    int index = navWidgets.indexOf(current);
    if (index == -1) index = 0;

    // Wrap-around navigation
    int newIndex = (index + direction + navWidgets.size()) % navWidgets.size();
    QWidget *nextWidget = navWidgets[newIndex];

    // 🔹 Reset previous widget (restore white background)
    if (index >= 0 && index < navWidgets.size())
    {
        QWidget *prev = navWidgets[index];
        // prev->setStyleSheet(
        //     "QLineEdit, QTextEdit, QComboBox {"
        //     "   background-color: white;"
        //     "   border: 1px solid gray;"
        //     "   outline: none;"
        //     "}"
        //     );
        prev->setStyleSheet("");
        QPalette p = prev->palette();
        p.setColor(QPalette::Base, Qt::white);
        prev->setPalette(p);
        prev->clearFocus();
        if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(prev)) {
            lineEdit->setSelection(0, 0);
        }
        // else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(prev)) {
        //     textEdit->setSelection(0, 0);
        // }
    }

    // 🔹 Prepare next widget to use custom style
    nextWidget->setAttribute(Qt::WA_StyledBackground, true);
    nextWidget->setFocus(Qt::TabFocusReason);

    // 🔹 Apply yellow highlight to the newly focused widget
    nextWidget->setStyleSheet(
        "QLineEdit, QTextEdit, QComboBox {"
        "   background-color: rgb(255, 255, 150);"
        "   border: 1px solid black;"
        "   outline: none;"
        "}"
        "QLineEdit:focus, QTextEdit:focus, QComboBox:focus {"
        "   background-color: rgb(255, 255, 150);"
        "   border: 1px solid black;"
        "   outline: none;"
        "}"
        );

    // 🔹 Ensure palette matches stylesheet and set selection color
    QPalette p = nextWidget->palette();
    p.setColor(QPalette::Base, QColor(255, 255, 150));   // field background
    p.setColor(QPalette::Highlight, QColor(255, 255, 180)); // selected text background (light yellow)
    p.setColor(QPalette::HighlightedText, Qt::black);       // selected text color
    nextWidget->setPalette(p);


    // 🔹 Handle text selection for line/text edits
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(nextWidget)) {
        lineEdit->setCursorPosition(0);
        lineEdit->selectAll();
    }
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(nextWidget)) {
        textEdit->moveCursor(QTextCursor::Start);
        textEdit->selectAll();
    }

    qDebug() << "Moved focus from"
             << (index >= 0 ? navWidgets[index]->objectName() : "None")
             << "to"
             << nextWidget->objectName();
}


void TestDetails::handleBackspaceInput()
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

    // QTextEdit case
    if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(focusedWidget)) {
        QString text = textEdit->toPlainText();
        if (!text.isEmpty()) {
            text.chop(1);
            textEdit->setPlainText(text);
            QTextCursor cursor = textEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            textEdit->setTextCursor(cursor);
        }
    }
}



void TestDetails::adjustComboBoxIndex(int delta)
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


bool TestDetails::isPreviewOpen() const
{
    return previewDialog != nullptr;
}


void TestDetails::closePreview()
{
    if (previewDialog)
    {
        previewDialog->close();
        delete previewDialog;
        previewDialog = nullptr;
        previewScreenIsOpen = false;
    }
}


void TestDetails::saveDataToFile()
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
    obj["Stations"] = testdetails->lineEdit_Stations->text();
    obj["Km"]=testdetails->lineEdit_Km->text();
    obj["Mtr"]=testdetails->lineEdit_Mtr->text();
    obj["TP"]=testdetails->lineEdit_TP->text();
    obj["WeldType"] = testdetails->comboBox_WeldType->currentText();
    obj["Rail"] = testdetails->comboBox_Rail->currentText();
    obj["WeldNo"] = testdetails->lineEdit_Weldno->text();
    obj["RollMark"] = testdetails->comboBox_Rollmark1->currentText();
    obj["kg"] = testdetails->comboBox_kg->currentText();
    obj["Month"] = testdetails->comboBox_Month->currentText();
    obj["RollMarkYear"]=testdetails->lineEdit_Rollmarkyear->text();
    obj["Probe"] = testdetails->comboBox_Probe->currentText();
    obj["Mode"] = testdetails->comboBox_Mode->currentText();
    obj["LocOfDefect"] = testdetails->comboBox_LocOfDefect->currentText();
    obj["Type"] = testdetails->comboBox_Type->currentText();
    obj["Class"] = testdetails->comboBox_Class->currentText();
    obj["FP"] = testdetails->comboBox_FP->currentText();
    obj["OthersObservation"] = testdetails->lineEdit_OtherObservation->text();
    obj["Date"] = testdetails->lineEdit_Date->text();
    obj["Time"] = testdetails->lineEdit_Time->text();

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(obj).toJson());
        file.close();
        qDebug() << "[INFO] TestDetails saved to file.";
    }
}


void TestDetails::loadDataFromFile()
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
        testdetails->lineEdit_Stations->setText(obj["Stations"].toString());
        testdetails->lineEdit_Km->setText(obj["Km"].toString());
        testdetails->lineEdit_Mtr->setText(obj["Mtr"].toString());
        testdetails->lineEdit_TP->setText(obj["TP"].toString());
        testdetails->comboBox_WeldType->setCurrentText(obj["WeldType"].toString());
        testdetails->comboBox_Rail->setCurrentText(obj["Rail"].toString());
        testdetails->lineEdit_Weldno->setText(obj["WeldNo"].toString());
        testdetails->comboBox_Rollmark1->setCurrentText(obj["RollMark"].toString());
        testdetails->comboBox_kg->setCurrentText(obj["kg"].toString());
        testdetails->comboBox_Month->setCurrentText(obj["Month"].toString());
        testdetails->lineEdit_Rollmarkyear->setText(obj["RollMarkYear"].toString());
        testdetails->comboBox_Probe->setCurrentText(obj["Probe"].toString());
        testdetails->comboBox_Mode->setCurrentText(obj["Mode"].toString());
        testdetails->comboBox_LocOfDefect->setCurrentText(obj["LocOfDefect"].toString());
        testdetails->comboBox_Type->setCurrentText(obj["Type"].toString());
        testdetails->comboBox_Class->setCurrentText(obj["Class"].toString());
        testdetails->comboBox_FP->setCurrentText(obj["FP"].toString());
        testdetails->lineEdit_OtherObservation->setText(obj["OthersObservation"].toString());



        testdetails->lineEdit_Date->setText(obj["Date"].toString());
        testdetails->lineEdit_Time->setText(obj["Time"].toString());


    }
}

