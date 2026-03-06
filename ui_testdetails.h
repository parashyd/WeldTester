/********************************************************************************
** Form generated from reading UI file 'testdetails.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTDETAILS_H
#define UI_TESTDETAILS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_TestDetails
{
public:
    QLabel *label_OperatorName;
    QLineEdit *lineEdit_OperatorName;
    QLabel *label_LocOfDefect;
    QComboBox *comboBox_LocOfDefect;
    QLabel *label_OthersObservations;
    QLabel *label_Date;
    QLabel *label_Probe;
    QPushButton *pushButton_Preview;
    QComboBox *comboBox_Probe;
    QLabel *label_Time;
    QLineEdit *lineEdit_Date;
    QLineEdit *lineEdit_Time;
    QLineEdit *lineEdit_OtherObservation;
    QLabel *label_TESTDETAILS;
    QLabel *label_UnitNO;
    QLineEdit *lineEdit_UnitNo;
    QLineEdit *lineEdit_Model;
    QLabel *label_Model;
    QComboBox *comboBox_Line;
    QLineEdit *lineEdit_Div;
    QLabel *label_Div;
    QLineEdit *lineEdit_Sec;
    QLabel *label_Sec;
    QLabel *label_Line;
    QLineEdit *lineEdit_Calset;
    QLabel *label_TestType;
    QLabel *label_Stations;
    QLabel *label_Calset;
    QComboBox *comboBox_TestType;
    QLineEdit *lineEdit_Stations;
    QLabel *label_DefectDetails;
    QLabel *label_TP;
    QLineEdit *lineEdit_Mtr;
    QLineEdit *lineEdit_TP;
    QLabel *label_KM;
    QLineEdit *lineEdit_Km;
    QLabel *label_Mtr;
    QComboBox *comboBox_WeldType;
    QLabel *label_WeldType;
    QComboBox *comboBox_Rollmark1;
    QComboBox *comboBox_Rail;
    QLineEdit *lineEdit_Weldno;
    QLabel *label_Rollingmark;
    QComboBox *comboBox_Month;
    QLabel *label_Weldno;
    QComboBox *comboBox_kg;
    QLabel *label_Rail;
    QLabel *label_CH;
    QLabel *label_Mode;
    QLineEdit *lineEdit_CH;
    QComboBox *comboBox_Mode;
    QComboBox *comboBox_Class;
    QLabel *label_Class;
    QComboBox *comboBox_Type;
    QLabel *label_FP;
    QLabel *label_Type;
    QComboBox *comboBox_FP;
    QLineEdit *lineEdit_Rollmarkyear;

    void setupUi(QDialog *TestDetails)
    {
        if (TestDetails->objectName().isEmpty())
            TestDetails->setObjectName(QString::fromUtf8("TestDetails"));
        TestDetails->resize(640, 480);
        TestDetails->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        TestDetails->setAcceptDrops(true);
        label_OperatorName = new QLabel(TestDetails);
        label_OperatorName->setObjectName(QString::fromUtf8("label_OperatorName"));
        label_OperatorName->setGeometry(QRect(10, 90, 81, 20));
        QFont font;
        font.setBold(true);
        label_OperatorName->setFont(font);
        lineEdit_OperatorName = new QLineEdit(TestDetails);
        lineEdit_OperatorName->setObjectName(QString::fromUtf8("lineEdit_OperatorName"));
        lineEdit_OperatorName->setGeometry(QRect(90, 90, 113, 20));
        lineEdit_OperatorName->setCursor(QCursor(Qt::IBeamCursor));
        lineEdit_OperatorName->setAutoFillBackground(true);
        label_LocOfDefect = new QLabel(TestDetails);
        label_LocOfDefect->setObjectName(QString::fromUtf8("label_LocOfDefect"));
        label_LocOfDefect->setGeometry(QRect(370, 290, 141, 20));
        comboBox_LocOfDefect = new QComboBox(TestDetails);
        comboBox_LocOfDefect->addItem(QString());
        comboBox_LocOfDefect->addItem(QString());
        comboBox_LocOfDefect->addItem(QString());
        comboBox_LocOfDefect->addItem(QString());
        comboBox_LocOfDefect->setObjectName(QString::fromUtf8("comboBox_LocOfDefect"));
        comboBox_LocOfDefect->setGeometry(QRect(520, 290, 111, 22));
        label_OthersObservations = new QLabel(TestDetails);
        label_OthersObservations->setObjectName(QString::fromUtf8("label_OthersObservations"));
        label_OthersObservations->setGeometry(QRect(100, 370, 161, 20));
        label_Date = new QLabel(TestDetails);
        label_Date->setObjectName(QString::fromUtf8("label_Date"));
        label_Date->setGeometry(QRect(300, 50, 41, 20));
        label_Date->setFont(font);
        label_Probe = new QLabel(TestDetails);
        label_Probe->setObjectName(QString::fromUtf8("label_Probe"));
        label_Probe->setGeometry(QRect(10, 290, 63, 20));
        label_Probe->setFont(font);
        pushButton_Preview = new QPushButton(TestDetails);
        pushButton_Preview->setObjectName(QString::fromUtf8("pushButton_Preview"));
        pushButton_Preview->setGeometry(QRect(520, 430, 93, 29));
        comboBox_Probe = new QComboBox(TestDetails);
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->setObjectName(QString::fromUtf8("comboBox_Probe"));
        comboBox_Probe->setGeometry(QRect(70, 290, 81, 22));
        label_Time = new QLabel(TestDetails);
        label_Time->setObjectName(QString::fromUtf8("label_Time"));
        label_Time->setGeometry(QRect(470, 50, 41, 20));
        label_Time->setFont(font);
        lineEdit_Date = new QLineEdit(TestDetails);
        lineEdit_Date->setObjectName(QString::fromUtf8("lineEdit_Date"));
        lineEdit_Date->setGeometry(QRect(350, 50, 113, 20));
        lineEdit_Time = new QLineEdit(TestDetails);
        lineEdit_Time->setObjectName(QString::fromUtf8("lineEdit_Time"));
        lineEdit_Time->setGeometry(QRect(520, 50, 113, 20));
        lineEdit_OtherObservation = new QLineEdit(TestDetails);
        lineEdit_OtherObservation->setObjectName(QString::fromUtf8("lineEdit_OtherObservation"));
        lineEdit_OtherObservation->setGeometry(QRect(270, 370, 231, 41));
        lineEdit_OtherObservation->setAcceptDrops(false);
        label_TESTDETAILS = new QLabel(TestDetails);
        label_TESTDETAILS->setObjectName(QString::fromUtf8("label_TESTDETAILS"));
        label_TESTDETAILS->setGeometry(QRect(250, 12, 131, 18));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        label_TESTDETAILS->setFont(font1);
        label_UnitNO = new QLabel(TestDetails);
        label_UnitNO->setObjectName(QString::fromUtf8("label_UnitNO"));
        label_UnitNO->setGeometry(QRect(155, 50, 66, 18));
        label_UnitNO->setFont(font);
        lineEdit_UnitNo = new QLineEdit(TestDetails);
        lineEdit_UnitNo->setObjectName(QString::fromUtf8("lineEdit_UnitNo"));
        lineEdit_UnitNo->setGeometry(QRect(220, 50, 71, 20));
        lineEdit_UnitNo->setReadOnly(true);
        lineEdit_Model = new QLineEdit(TestDetails);
        lineEdit_Model->setObjectName(QString::fromUtf8("lineEdit_Model"));
        lineEdit_Model->setGeometry(QRect(70, 50, 81, 20));
        lineEdit_Model->setReadOnly(true);
        label_Model = new QLabel(TestDetails);
        label_Model->setObjectName(QString::fromUtf8("label_Model"));
        label_Model->setGeometry(QRect(10, 50, 55, 18));
        label_Model->setFont(font);
        comboBox_Line = new QComboBox(TestDetails);
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->addItem(QString());
        comboBox_Line->setObjectName(QString::fromUtf8("comboBox_Line"));
        comboBox_Line->setGeometry(QRect(470, 88, 111, 22));
        comboBox_Line->setEditable(true);
        lineEdit_Div = new QLineEdit(TestDetails);
        lineEdit_Div->setObjectName(QString::fromUtf8("lineEdit_Div"));
        lineEdit_Div->setGeometry(QRect(250, 90, 61, 20));
        label_Div = new QLabel(TestDetails);
        label_Div->setObjectName(QString::fromUtf8("label_Div"));
        label_Div->setGeometry(QRect(220, 90, 41, 18));
        label_Div->setFont(font);
        lineEdit_Sec = new QLineEdit(TestDetails);
        lineEdit_Sec->setObjectName(QString::fromUtf8("lineEdit_Sec"));
        lineEdit_Sec->setGeometry(QRect(360, 90, 61, 20));
        label_Sec = new QLabel(TestDetails);
        label_Sec->setObjectName(QString::fromUtf8("label_Sec"));
        label_Sec->setGeometry(QRect(320, 90, 41, 18));
        label_Sec->setFont(font);
        label_Line = new QLabel(TestDetails);
        label_Line->setObjectName(QString::fromUtf8("label_Line"));
        label_Line->setGeometry(QRect(430, 90, 41, 18));
        label_Line->setFont(font);
        lineEdit_Calset = new QLineEdit(TestDetails);
        lineEdit_Calset->setObjectName(QString::fromUtf8("lineEdit_Calset"));
        lineEdit_Calset->setGeometry(QRect(490, 130, 51, 20));
        lineEdit_Calset->setReadOnly(true);
        label_TestType = new QLabel(TestDetails);
        label_TestType->setObjectName(QString::fromUtf8("label_TestType"));
        label_TestType->setGeometry(QRect(230, 130, 75, 18));
        label_TestType->setFont(font);
        label_Stations = new QLabel(TestDetails);
        label_Stations->setObjectName(QString::fromUtf8("label_Stations"));
        label_Stations->setGeometry(QRect(10, 130, 71, 18));
        label_Stations->setFont(font);
        label_Calset = new QLabel(TestDetails);
        label_Calset->setObjectName(QString::fromUtf8("label_Calset"));
        label_Calset->setGeometry(QRect(430, 130, 61, 18));
        label_Calset->setFont(font);
        comboBox_TestType = new QComboBox(TestDetails);
        comboBox_TestType->addItem(QString());
        comboBox_TestType->addItem(QString());
        comboBox_TestType->addItem(QString());
        comboBox_TestType->addItem(QString());
        comboBox_TestType->setObjectName(QString::fromUtf8("comboBox_TestType"));
        comboBox_TestType->setGeometry(QRect(310, 128, 111, 22));
        comboBox_TestType->setEditable(false);
        lineEdit_Stations = new QLineEdit(TestDetails);
        lineEdit_Stations->setObjectName(QString::fromUtf8("lineEdit_Stations"));
        lineEdit_Stations->setGeometry(QRect(100, 130, 113, 20));
        label_DefectDetails = new QLabel(TestDetails);
        label_DefectDetails->setObjectName(QString::fromUtf8("label_DefectDetails"));
        label_DefectDetails->setGeometry(QRect(240, 170, 161, 18));
        label_DefectDetails->setFont(font1);
        label_TP = new QLabel(TestDetails);
        label_TP->setObjectName(QString::fromUtf8("label_TP"));
        label_TP->setGeometry(QRect(293, 210, 31, 18));
        label_TP->setFont(font);
        lineEdit_Mtr = new QLineEdit(TestDetails);
        lineEdit_Mtr->setObjectName(QString::fromUtf8("lineEdit_Mtr"));
        lineEdit_Mtr->setGeometry(QRect(183, 210, 71, 20));
        lineEdit_TP = new QLineEdit(TestDetails);
        lineEdit_TP->setObjectName(QString::fromUtf8("lineEdit_TP"));
        lineEdit_TP->setGeometry(QRect(323, 210, 81, 20));
        label_KM = new QLabel(TestDetails);
        label_KM->setObjectName(QString::fromUtf8("label_KM"));
        label_KM->setGeometry(QRect(13, 210, 41, 18));
        label_KM->setFont(font);
        lineEdit_Km = new QLineEdit(TestDetails);
        lineEdit_Km->setObjectName(QString::fromUtf8("lineEdit_Km"));
        lineEdit_Km->setGeometry(QRect(53, 210, 61, 20));
        label_Mtr = new QLabel(TestDetails);
        label_Mtr->setObjectName(QString::fromUtf8("label_Mtr"));
        label_Mtr->setGeometry(QRect(143, 210, 41, 18));
        label_Mtr->setFont(font);
        comboBox_WeldType = new QComboBox(TestDetails);
        comboBox_WeldType->addItem(QString());
        comboBox_WeldType->addItem(QString());
        comboBox_WeldType->addItem(QString());
        comboBox_WeldType->addItem(QString());
        comboBox_WeldType->addItem(QString());
        comboBox_WeldType->setObjectName(QString::fromUtf8("comboBox_WeldType"));
        comboBox_WeldType->setGeometry(QRect(513, 208, 93, 22));
        label_WeldType = new QLabel(TestDetails);
        label_WeldType->setObjectName(QString::fromUtf8("label_WeldType"));
        label_WeldType->setGeometry(QRect(430, 210, 81, 18));
        label_WeldType->setFont(font);
        comboBox_Rollmark1 = new QComboBox(TestDetails);
        comboBox_Rollmark1->addItem(QString());
        comboBox_Rollmark1->addItem(QString());
        comboBox_Rollmark1->addItem(QString());
        comboBox_Rollmark1->addItem(QString());
        comboBox_Rollmark1->addItem(QString());
        comboBox_Rollmark1->addItem(QString());
        comboBox_Rollmark1->setObjectName(QString::fromUtf8("comboBox_Rollmark1"));
        comboBox_Rollmark1->setGeometry(QRect(350, 248, 111, 22));
        comboBox_Rail = new QComboBox(TestDetails);
        comboBox_Rail->addItem(QString());
        comboBox_Rail->addItem(QString());
        comboBox_Rail->setObjectName(QString::fromUtf8("comboBox_Rail"));
        comboBox_Rail->setGeometry(QRect(50, 248, 61, 22));
        lineEdit_Weldno = new QLineEdit(TestDetails);
        lineEdit_Weldno->setObjectName(QString::fromUtf8("lineEdit_Weldno"));
        lineEdit_Weldno->setGeometry(QRect(190, 250, 51, 20));
        label_Rollingmark = new QLabel(TestDetails);
        label_Rollingmark->setObjectName(QString::fromUtf8("label_Rollingmark"));
        label_Rollingmark->setGeometry(QRect(250, 250, 101, 18));
        label_Rollingmark->setFont(font);
        comboBox_Month = new QComboBox(TestDetails);
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->addItem(QString());
        comboBox_Month->setObjectName(QString::fromUtf8("comboBox_Month"));
        comboBox_Month->setGeometry(QRect(520, 248, 51, 22));
        label_Weldno = new QLabel(TestDetails);
        label_Weldno->setObjectName(QString::fromUtf8("label_Weldno"));
        label_Weldno->setGeometry(QRect(110, 250, 81, 18));
        label_Weldno->setFont(font);
        comboBox_kg = new QComboBox(TestDetails);
        comboBox_kg->addItem(QString());
        comboBox_kg->addItem(QString());
        comboBox_kg->addItem(QString());
        comboBox_kg->addItem(QString());
        comboBox_kg->setObjectName(QString::fromUtf8("comboBox_kg"));
        comboBox_kg->setGeometry(QRect(460, 248, 61, 22));
        label_Rail = new QLabel(TestDetails);
        label_Rail->setObjectName(QString::fromUtf8("label_Rail"));
        label_Rail->setGeometry(QRect(10, 250, 41, 18));
        label_Rail->setFont(font);
        label_CH = new QLabel(TestDetails);
        label_CH->setObjectName(QString::fromUtf8("label_CH"));
        label_CH->setGeometry(QRect(170, 292, 31, 18));
        label_CH->setFont(font);
        label_Mode = new QLabel(TestDetails);
        label_Mode->setObjectName(QString::fromUtf8("label_Mode"));
        label_Mode->setGeometry(QRect(250, 292, 51, 18));
        label_Mode->setFont(font);
        lineEdit_CH = new QLineEdit(TestDetails);
        lineEdit_CH->setObjectName(QString::fromUtf8("lineEdit_CH"));
        lineEdit_CH->setGeometry(QRect(210, 292, 31, 20));
        lineEdit_CH->setReadOnly(true);
        comboBox_Mode = new QComboBox(TestDetails);
        comboBox_Mode->addItem(QString());
        comboBox_Mode->addItem(QString());
        comboBox_Mode->setObjectName(QString::fromUtf8("comboBox_Mode"));
        comboBox_Mode->setGeometry(QRect(300, 290, 61, 22));
        comboBox_Class = new QComboBox(TestDetails);
        comboBox_Class->addItem(QString());
        comboBox_Class->addItem(QString());
        comboBox_Class->addItem(QString());
        comboBox_Class->addItem(QString());
        comboBox_Class->addItem(QString());
        comboBox_Class->addItem(QString());
        comboBox_Class->addItem(QString());
        comboBox_Class->addItem(QString());
        comboBox_Class->setObjectName(QString::fromUtf8("comboBox_Class"));
        comboBox_Class->setGeometry(QRect(280, 330, 93, 22));
        label_Class = new QLabel(TestDetails);
        label_Class->setObjectName(QString::fromUtf8("label_Class"));
        label_Class->setGeometry(QRect(230, 332, 66, 18));
        label_Class->setFont(font);
        comboBox_Type = new QComboBox(TestDetails);
        comboBox_Type->addItem(QString());
        comboBox_Type->addItem(QString());
        comboBox_Type->addItem(QString());
        comboBox_Type->setObjectName(QString::fromUtf8("comboBox_Type"));
        comboBox_Type->setGeometry(QRect(60, 330, 93, 22));
        label_FP = new QLabel(TestDetails);
        label_FP->setObjectName(QString::fromUtf8("label_FP"));
        label_FP->setGeometry(QRect(450, 332, 66, 18));
        label_FP->setFont(font);
        label_Type = new QLabel(TestDetails);
        label_Type->setObjectName(QString::fromUtf8("label_Type"));
        label_Type->setGeometry(QRect(10, 332, 41, 18));
        label_Type->setFont(font);
        comboBox_FP = new QComboBox(TestDetails);
        comboBox_FP->addItem(QString());
        comboBox_FP->addItem(QString());
        comboBox_FP->addItem(QString());
        comboBox_FP->setObjectName(QString::fromUtf8("comboBox_FP"));
        comboBox_FP->setGeometry(QRect(510, 330, 93, 22));
        lineEdit_Rollmarkyear = new QLineEdit(TestDetails);
        lineEdit_Rollmarkyear->setObjectName(QString::fromUtf8("lineEdit_Rollmarkyear"));
        lineEdit_Rollmarkyear->setGeometry(QRect(570, 250, 61, 20));

        retranslateUi(TestDetails);

        QMetaObject::connectSlotsByName(TestDetails);
    } // setupUi

    void retranslateUi(QDialog *TestDetails)
    {
        label_OperatorName->setText(QCoreApplication::translate("TestDetails", "O/p Name :", nullptr));
        lineEdit_OperatorName->setText(QString());
        label_LocOfDefect->setText(QCoreApplication::translate("TestDetails", "Location of Defect :", nullptr));
        comboBox_LocOfDefect->setItemText(0, QCoreApplication::translate("TestDetails", "Head", nullptr));
        comboBox_LocOfDefect->setItemText(1, QCoreApplication::translate("TestDetails", "Web", nullptr));
        comboBox_LocOfDefect->setItemText(2, QCoreApplication::translate("TestDetails", "Flange", nullptr));
        comboBox_LocOfDefect->setItemText(3, QCoreApplication::translate("TestDetails", "Foot", nullptr));

        label_OthersObservations->setText(QCoreApplication::translate("TestDetails", "Others / Observations :", nullptr));
        label_Date->setText(QCoreApplication::translate("TestDetails", "Date:", nullptr));
        label_Probe->setText(QCoreApplication::translate("TestDetails", "Probe :", nullptr));
        pushButton_Preview->setText(QCoreApplication::translate("TestDetails", "Preview", nullptr));
        comboBox_Probe->setItemText(0, QCoreApplication::translate("TestDetails", "0\302\260", nullptr));
        comboBox_Probe->setItemText(1, QCoreApplication::translate("TestDetails", "17.5\302\260", nullptr));
        comboBox_Probe->setItemText(2, QCoreApplication::translate("TestDetails", "37\302\260", nullptr));
        comboBox_Probe->setItemText(3, QCoreApplication::translate("TestDetails", "45\302\260", nullptr));
        comboBox_Probe->setItemText(4, QCoreApplication::translate("TestDetails", "70\302\260", nullptr));
        comboBox_Probe->setItemText(5, QCoreApplication::translate("TestDetails", "2x70", nullptr));
        comboBox_Probe->setItemText(6, QCoreApplication::translate("TestDetails", "2x45", nullptr));

        label_Time->setText(QCoreApplication::translate("TestDetails", "Time:", nullptr));
        lineEdit_Date->setText(QString());
        lineEdit_Time->setText(QString());
        lineEdit_OtherObservation->setText(QString());
        label_TESTDETAILS->setText(QCoreApplication::translate("TestDetails", "TEST DETAILS", nullptr));
        label_UnitNO->setText(QCoreApplication::translate("TestDetails", "Unit No :", nullptr));
        lineEdit_Model->setText(QCoreApplication::translate("TestDetails", "WT-2311D", nullptr));
        label_Model->setText(QCoreApplication::translate("TestDetails", "Model :", nullptr));
        comboBox_Line->setItemText(0, QCoreApplication::translate("TestDetails", "UP", nullptr));
        comboBox_Line->setItemText(1, QCoreApplication::translate("TestDetails", "DN", nullptr));
        comboBox_Line->setItemText(2, QCoreApplication::translate("TestDetails", "SL", nullptr));
        comboBox_Line->setItemText(3, QCoreApplication::translate("TestDetails", "UD", nullptr));
        comboBox_Line->setItemText(4, QCoreApplication::translate("TestDetails", "ByPass UP", nullptr));
        comboBox_Line->setItemText(5, QCoreApplication::translate("TestDetails", "ByPass DN", nullptr));
        comboBox_Line->setItemText(6, QCoreApplication::translate("TestDetails", "ByPass SL", nullptr));
        comboBox_Line->setItemText(7, QCoreApplication::translate("TestDetails", "L1", nullptr));
        comboBox_Line->setItemText(8, QCoreApplication::translate("TestDetails", "L2", nullptr));
        comboBox_Line->setItemText(9, QCoreApplication::translate("TestDetails", "L3", nullptr));
        comboBox_Line->setItemText(10, QCoreApplication::translate("TestDetails", "L4", nullptr));
        comboBox_Line->setItemText(11, QCoreApplication::translate("TestDetails", "L5", nullptr));
        comboBox_Line->setItemText(12, QCoreApplication::translate("TestDetails", "L6", nullptr));
        comboBox_Line->setItemText(13, QCoreApplication::translate("TestDetails", "L7", nullptr));
        comboBox_Line->setItemText(14, QCoreApplication::translate("TestDetails", "L8", nullptr));
        comboBox_Line->setItemText(15, QCoreApplication::translate("TestDetails", "UP L", nullptr));
        comboBox_Line->setItemText(16, QCoreApplication::translate("TestDetails", "DN L", nullptr));

        label_Div->setText(QCoreApplication::translate("TestDetails", "Div :", nullptr));
        label_Sec->setText(QCoreApplication::translate("TestDetails", "Sec :", nullptr));
        label_Line->setText(QCoreApplication::translate("TestDetails", "Line :", nullptr));
        label_TestType->setText(QCoreApplication::translate("TestDetails", "Test Type :", nullptr));
        label_Stations->setText(QCoreApplication::translate("TestDetails", "Stations :", nullptr));
        label_Calset->setText(QCoreApplication::translate("TestDetails", "Calset :", nullptr));
        comboBox_TestType->setItemText(0, QCoreApplication::translate("TestDetails", "Periodic", nullptr));
        comboBox_TestType->setItemText(1, QCoreApplication::translate("TestDetails", "Initial", nullptr));
        comboBox_TestType->setItemText(2, QCoreApplication::translate("TestDetails", "1st Periodic", nullptr));
        comboBox_TestType->setItemText(3, QCoreApplication::translate("TestDetails", "General", nullptr));

#if QT_CONFIG(accessibility)
        comboBox_TestType->setAccessibleName(QCoreApplication::translate("TestDetails", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(accessibility)
        label_DefectDetails->setText(QCoreApplication::translate("TestDetails", "DEFECT DETAILS", nullptr));
        label_TP->setText(QCoreApplication::translate("TestDetails", "TP :", nullptr));
        label_KM->setText(QCoreApplication::translate("TestDetails", "KM :", nullptr));
        lineEdit_Km->setText(QString());
        label_Mtr->setText(QCoreApplication::translate("TestDetails", "Mtr :", nullptr));
        comboBox_WeldType->setItemText(0, QCoreApplication::translate("TestDetails", "AT Weld", nullptr));
        comboBox_WeldType->setItemText(1, QCoreApplication::translate("TestDetails", "FB Weld", nullptr));
        comboBox_WeldType->setItemText(2, QCoreApplication::translate("TestDetails", "SEJ", nullptr));
        comboBox_WeldType->setItemText(3, QCoreApplication::translate("TestDetails", "Points", nullptr));
        comboBox_WeldType->setItemText(4, QCoreApplication::translate("TestDetails", "Rail", nullptr));

        label_WeldType->setText(QCoreApplication::translate("TestDetails", "Weld Type :", nullptr));
        comboBox_Rollmark1->setItemText(0, QCoreApplication::translate("TestDetails", "880 O SAIL", nullptr));
        comboBox_Rollmark1->setItemText(1, QCoreApplication::translate("TestDetails", "R260 O SAIL", nullptr));
        comboBox_Rollmark1->setItemText(2, QCoreApplication::translate("TestDetails", "710 O SAIL", nullptr));
        comboBox_Rollmark1->setItemText(3, QCoreApplication::translate("TestDetails", "British Steel", nullptr));
        comboBox_Rollmark1->setItemText(4, QCoreApplication::translate("TestDetails", "MM IRS HSB", nullptr));
        comboBox_Rollmark1->setItemText(5, QCoreApplication::translate("TestDetails", "UIC O SAIL", nullptr));

        comboBox_Rail->setItemText(0, QCoreApplication::translate("TestDetails", "LR", nullptr));
        comboBox_Rail->setItemText(1, QCoreApplication::translate("TestDetails", "RR", nullptr));

        label_Rollingmark->setText(QCoreApplication::translate("TestDetails", "Rolling Mark :", nullptr));
        comboBox_Month->setItemText(0, QCoreApplication::translate("TestDetails", "I", nullptr));
        comboBox_Month->setItemText(1, QCoreApplication::translate("TestDetails", "II", nullptr));
        comboBox_Month->setItemText(2, QCoreApplication::translate("TestDetails", "III", nullptr));
        comboBox_Month->setItemText(3, QCoreApplication::translate("TestDetails", "IV", nullptr));
        comboBox_Month->setItemText(4, QCoreApplication::translate("TestDetails", "V", nullptr));
        comboBox_Month->setItemText(5, QCoreApplication::translate("TestDetails", "VI", nullptr));
        comboBox_Month->setItemText(6, QCoreApplication::translate("TestDetails", "VII", nullptr));
        comboBox_Month->setItemText(7, QCoreApplication::translate("TestDetails", "VIII", nullptr));
        comboBox_Month->setItemText(8, QCoreApplication::translate("TestDetails", "IX", nullptr));
        comboBox_Month->setItemText(9, QCoreApplication::translate("TestDetails", "X", nullptr));
        comboBox_Month->setItemText(10, QCoreApplication::translate("TestDetails", "XI", nullptr));
        comboBox_Month->setItemText(11, QCoreApplication::translate("TestDetails", "XII", nullptr));

        label_Weldno->setText(QCoreApplication::translate("TestDetails", "Weld  No. :", nullptr));
        comboBox_kg->setItemText(0, QCoreApplication::translate("TestDetails", "60", nullptr));
        comboBox_kg->setItemText(1, QCoreApplication::translate("TestDetails", "52", nullptr));
        comboBox_kg->setItemText(2, QCoreApplication::translate("TestDetails", "90R", nullptr));
        comboBox_kg->setItemText(3, QCoreApplication::translate("TestDetails", "36", nullptr));

        label_Rail->setText(QCoreApplication::translate("TestDetails", "Rail :", nullptr));
        label_CH->setText(QCoreApplication::translate("TestDetails", "CH. :", nullptr));
        label_Mode->setText(QCoreApplication::translate("TestDetails", "Mode :", nullptr));
        comboBox_Mode->setItemText(0, QCoreApplication::translate("TestDetails", "SC", nullptr));
        comboBox_Mode->setItemText(1, QCoreApplication::translate("TestDetails", "DC", nullptr));

        comboBox_Class->setItemText(0, QCoreApplication::translate("TestDetails", "DFWO", nullptr));
        comboBox_Class->setItemText(1, QCoreApplication::translate("TestDetails", "DFWR", nullptr));
        comboBox_Class->setItemText(2, QCoreApplication::translate("TestDetails", "DFWN", nullptr));
        comboBox_Class->setItemText(3, QCoreApplication::translate("TestDetails", "GW", nullptr));
        comboBox_Class->setItemText(4, QCoreApplication::translate("TestDetails", "ND", nullptr));
        comboBox_Class->setItemText(5, QCoreApplication::translate("TestDetails", "NR", nullptr));
        comboBox_Class->setItemText(6, QCoreApplication::translate("TestDetails", "OBS", nullptr));
        comboBox_Class->setItemText(7, QCoreApplication::translate("TestDetails", "OBSW", nullptr));

        label_Class->setText(QCoreApplication::translate("TestDetails", "Class :", nullptr));
        comboBox_Type->setItemText(0, QCoreApplication::translate("TestDetails", "HLC", nullptr));
        comboBox_Type->setItemText(1, QCoreApplication::translate("TestDetails", "VTC", nullptr));
        comboBox_Type->setItemText(2, QCoreApplication::translate("TestDetails", "VLC", nullptr));

        label_FP->setText(QCoreApplication::translate("TestDetails", "FP/NFP :", nullptr));
        label_Type->setText(QCoreApplication::translate("TestDetails", "Type :", nullptr));
        comboBox_FP->setItemText(0, QCoreApplication::translate("TestDetails", "FP/B", nullptr));
        comboBox_FP->setItemText(1, QCoreApplication::translate("TestDetails", "FP/C", nullptr));
        comboBox_FP->setItemText(2, QCoreApplication::translate("TestDetails", "NFP", nullptr));

        (void)TestDetails;
    } // retranslateUi

};

namespace Ui {
    class TestDetails: public Ui_TestDetails {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTDETAILS_H
