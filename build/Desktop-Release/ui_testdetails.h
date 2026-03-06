/********************************************************************************
** Form generated from reading UI file 'testdetails.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
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
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_TestDetails
{
public:
    QLabel *label_TestDetails;
    QLabel *label_OperatorName;
    QLineEdit *lineEdit_OperatorName;
    QLabel *label_Railway;
    QLineEdit *lineEdit_Railway;
    QLabel *label_AxleType;
    QLabel *label_AxleNo;
    QLineEdit *lineEdit_WheelNo;
    QLabel *label_LocOfDefect;
    QComboBox *comboBox_LocOfDefect;
    QLabel *label_TestResult;
    QLabel *label_OthersObservations;
    QTextEdit *textEdit_OthersObservations;
    QLabel *label_DateTime;
    QLineEdit *lineEdit_DateTime;
    QLabel *label_FrameNo;
    QLabel *label_Probe;
    QPushButton *pushButton_Preview;
    QComboBox *comboBox_FrameNo;
    QComboBox *comboBox_Probe;
    QComboBox *comboBox_AxleWheelType;
    QComboBox *comboBox_TestResult;

    void setupUi(QDialog *TestDetails)
    {
        if (TestDetails->objectName().isEmpty())
            TestDetails->setObjectName("TestDetails");
        TestDetails->resize(640, 480);
        TestDetails->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        TestDetails->setAcceptDrops(true);
        label_TestDetails = new QLabel(TestDetails);
        label_TestDetails->setObjectName("label_TestDetails");
        label_TestDetails->setGeometry(QRect(270, 10, 101, 21));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        label_TestDetails->setFont(font);
        label_OperatorName = new QLabel(TestDetails);
        label_OperatorName->setObjectName("label_OperatorName");
        label_OperatorName->setGeometry(QRect(10, 50, 121, 20));
        lineEdit_OperatorName = new QLineEdit(TestDetails);
        lineEdit_OperatorName->setObjectName("lineEdit_OperatorName");
        lineEdit_OperatorName->setGeometry(QRect(160, 50, 113, 26));
        label_Railway = new QLabel(TestDetails);
        label_Railway->setObjectName("label_Railway");
        label_Railway->setGeometry(QRect(10, 90, 141, 20));
        lineEdit_Railway = new QLineEdit(TestDetails);
        lineEdit_Railway->setObjectName("lineEdit_Railway");
        lineEdit_Railway->setGeometry(QRect(160, 90, 113, 26));
        label_AxleType = new QLabel(TestDetails);
        label_AxleType->setObjectName("label_AxleType");
        label_AxleType->setGeometry(QRect(10, 130, 131, 20));
        label_AxleNo = new QLabel(TestDetails);
        label_AxleNo->setObjectName("label_AxleNo");
        label_AxleNo->setGeometry(QRect(10, 170, 131, 20));
        lineEdit_WheelNo = new QLineEdit(TestDetails);
        lineEdit_WheelNo->setObjectName("lineEdit_WheelNo");
        lineEdit_WheelNo->setGeometry(QRect(160, 170, 113, 26));
        label_LocOfDefect = new QLabel(TestDetails);
        label_LocOfDefect->setObjectName("label_LocOfDefect");
        label_LocOfDefect->setGeometry(QRect(10, 210, 141, 20));
        comboBox_LocOfDefect = new QComboBox(TestDetails);
        comboBox_LocOfDefect->addItem(QString());
        comboBox_LocOfDefect->addItem(QString());
        comboBox_LocOfDefect->setObjectName("comboBox_LocOfDefect");
        comboBox_LocOfDefect->setGeometry(QRect(160, 210, 111, 26));
        label_TestResult = new QLabel(TestDetails);
        label_TestResult->setObjectName("label_TestResult");
        label_TestResult->setGeometry(QRect(10, 250, 81, 20));
        label_OthersObservations = new QLabel(TestDetails);
        label_OthersObservations->setObjectName("label_OthersObservations");
        label_OthersObservations->setGeometry(QRect(10, 300, 161, 20));
        textEdit_OthersObservations = new QTextEdit(TestDetails);
        textEdit_OthersObservations->setObjectName("textEdit_OthersObservations");
        textEdit_OthersObservations->setGeometry(QRect(160, 300, 441, 75));
        label_DateTime = new QLabel(TestDetails);
        label_DateTime->setObjectName("label_DateTime");
        label_DateTime->setGeometry(QRect(350, 50, 101, 20));
        lineEdit_DateTime = new QLineEdit(TestDetails);
        lineEdit_DateTime->setObjectName("lineEdit_DateTime");
        lineEdit_DateTime->setGeometry(QRect(450, 50, 181, 26));
        label_FrameNo = new QLabel(TestDetails);
        label_FrameNo->setObjectName("label_FrameNo");
        label_FrameNo->setGeometry(QRect(350, 130, 81, 20));
        label_Probe = new QLabel(TestDetails);
        label_Probe->setObjectName("label_Probe");
        label_Probe->setGeometry(QRect(350, 170, 63, 20));
        pushButton_Preview = new QPushButton(TestDetails);
        pushButton_Preview->setObjectName("pushButton_Preview");
        pushButton_Preview->setGeometry(QRect(520, 430, 93, 29));
        comboBox_FrameNo = new QComboBox(TestDetails);
        comboBox_FrameNo->addItem(QString());
        comboBox_FrameNo->addItem(QString());
        comboBox_FrameNo->addItem(QString());
        comboBox_FrameNo->addItem(QString());
        comboBox_FrameNo->addItem(QString());
        comboBox_FrameNo->addItem(QString());
        comboBox_FrameNo->setObjectName("comboBox_FrameNo");
        comboBox_FrameNo->setGeometry(QRect(450, 130, 111, 26));
        comboBox_Probe = new QComboBox(TestDetails);
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->addItem(QString());
        comboBox_Probe->setObjectName("comboBox_Probe");
        comboBox_Probe->setGeometry(QRect(450, 170, 111, 26));
        comboBox_AxleWheelType = new QComboBox(TestDetails);
        comboBox_AxleWheelType->addItem(QString());
        comboBox_AxleWheelType->addItem(QString());
        comboBox_AxleWheelType->addItem(QString());
        comboBox_AxleWheelType->addItem(QString());
        comboBox_AxleWheelType->setObjectName("comboBox_AxleWheelType");
        comboBox_AxleWheelType->setGeometry(QRect(160, 130, 111, 26));
        comboBox_TestResult = new QComboBox(TestDetails);
        comboBox_TestResult->addItem(QString());
        comboBox_TestResult->addItem(QString());
        comboBox_TestResult->addItem(QString());
        comboBox_TestResult->setObjectName("comboBox_TestResult");
        comboBox_TestResult->setGeometry(QRect(160, 250, 111, 26));

        retranslateUi(TestDetails);

        QMetaObject::connectSlotsByName(TestDetails);
    } // setupUi

    void retranslateUi(QDialog *TestDetails)
    {
        label_TestDetails->setText(QCoreApplication::translate("TestDetails", "Test Details", nullptr));
        label_OperatorName->setText(QCoreApplication::translate("TestDetails", "Operator Name :", nullptr));
        lineEdit_OperatorName->setText(QString());
        label_Railway->setText(QCoreApplication::translate("TestDetails", "Railway / Workshop :", nullptr));
        label_AxleType->setText(QCoreApplication::translate("TestDetails", "Axle / Wheel Type :", nullptr));
        label_AxleNo->setText(QCoreApplication::translate("TestDetails", "Axle / Wheel No :", nullptr));
        label_LocOfDefect->setText(QCoreApplication::translate("TestDetails", "Location of Defect :", nullptr));
        comboBox_LocOfDefect->setItemText(0, QCoreApplication::translate("TestDetails", "Head", nullptr));
        comboBox_LocOfDefect->setItemText(1, QCoreApplication::translate("TestDetails", "Foot", nullptr));

        label_TestResult->setText(QCoreApplication::translate("TestDetails", "Test Result :", nullptr));
        label_OthersObservations->setText(QCoreApplication::translate("TestDetails", "Others / Observations :", nullptr));
        label_DateTime->setText(QCoreApplication::translate("TestDetails", "Date & Time :", nullptr));
        label_FrameNo->setText(QCoreApplication::translate("TestDetails", "Frame No. :", nullptr));
        label_Probe->setText(QCoreApplication::translate("TestDetails", "Probe :", nullptr));
        pushButton_Preview->setText(QCoreApplication::translate("TestDetails", "Preview", nullptr));
        comboBox_FrameNo->setItemText(0, QCoreApplication::translate("TestDetails", "1", nullptr));
        comboBox_FrameNo->setItemText(1, QCoreApplication::translate("TestDetails", "2", nullptr));
        comboBox_FrameNo->setItemText(2, QCoreApplication::translate("TestDetails", "3", nullptr));
        comboBox_FrameNo->setItemText(3, QCoreApplication::translate("TestDetails", "4", nullptr));
        comboBox_FrameNo->setItemText(4, QCoreApplication::translate("TestDetails", "5", nullptr));
        comboBox_FrameNo->setItemText(5, QCoreApplication::translate("TestDetails", "6", nullptr));

        comboBox_Probe->setItemText(0, QCoreApplication::translate("TestDetails", "0\302\260", nullptr));
        comboBox_Probe->setItemText(1, QCoreApplication::translate("TestDetails", "17.5\302\260", nullptr));
        comboBox_Probe->setItemText(2, QCoreApplication::translate("TestDetails", "37\302\260", nullptr));
        comboBox_Probe->setItemText(3, QCoreApplication::translate("TestDetails", "45\302\260", nullptr));
        comboBox_Probe->setItemText(4, QCoreApplication::translate("TestDetails", "70\302\260", nullptr));

        comboBox_AxleWheelType->setItemText(0, QCoreApplication::translate("TestDetails", "Locomotive ", nullptr));
        comboBox_AxleWheelType->setItemText(1, QCoreApplication::translate("TestDetails", "Freight Car", nullptr));
        comboBox_AxleWheelType->setItemText(2, QCoreApplication::translate("TestDetails", "Passenger Car", nullptr));
        comboBox_AxleWheelType->setItemText(3, QCoreApplication::translate("TestDetails", "Wagon", nullptr));

        comboBox_TestResult->setItemText(0, QCoreApplication::translate("TestDetails", "Pass", nullptr));
        comboBox_TestResult->setItemText(1, QCoreApplication::translate("TestDetails", "Fail", nullptr));
        comboBox_TestResult->setItemText(2, QCoreApplication::translate("TestDetails", "Others", nullptr));

        (void)TestDetails;
    } // retranslateUi

};

namespace Ui {
    class TestDetails: public Ui_TestDetails {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTDETAILS_H
