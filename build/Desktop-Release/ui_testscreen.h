/********************************************************************************
** Form generated from reading UI file 'testscreen.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTSCREEN_H
#define UI_TESTSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_TestScreen
{
public:
    QWidget *centralwidget;
    QLabel *label_BP;
    QLineEdit *lineEdit_SD1;
    QLineEdit *lineEdit_calset;
    QCustomPlot *Plot;
    QPushButton *pushButton_SaveButton;
    QProgressBar *progressBar;
    QLabel *label_PH;
    QLineEdit *lineEdit_TH2;
    QLineEdit *lineEdit_TH1;
    QLineEdit *lineEdit_Range;
    QLineEdit *lineEdit_PH1;
    QLineEdit *lineEdit_SD2;
    QLabel *label_calset_2;
    QLineEdit *lineEdit_BP1;
    QLineEdit *lineEdit_Machine;
    QLabel *label_Reject;
    QLabel *label_G2ST;
    QLabel *label_G2ED;
    QLineEdit *lineEdit_Velocity;
    QLabel *label_G1ST;
    QLabel *label_Range;
    QLabel *label_meter;
    QLabel *label_D;
    QLabel *label_TH2;
    QLineEdit *lineEdit_G2ED;
    QLineEdit *lineEdit_P2P;
    QLineEdit *lineEdit_Mode;
    QLabel *label_TH1;
    QLineEdit *lineEdit_G1ST;
    QLineEdit *lineEdit_Angle;
    QLabel *label_P2P;
    QLineEdit *lineEdit_BP2;
    QLabel *label_SD;
    QLabel *label_Angle;
    QLineEdit *lineEdit_Gain;
    QLabel *label_G1ED;
    QLabel *label_Gain;
    QLineEdit *lineEdit_Reject;
    QLineEdit *lineEdit_G2ST;
    QLineEdit *lineEdit_D1;
    QLabel *label_calset;
    QLineEdit *lineEdit_D2;
    QLabel *label_Velocity;
    QLineEdit *lineEdit_PH2;
    QLabel *label_Mode;
    QLabel *label_Delay;
    QLabel *label_Machine;
    QLineEdit *lineEdit_G1ED;
    QLineEdit *lineEdit_Delay;
    QLineEdit *lineEdit_ch;

    void setupUi(QMainWindow *TestScreen)
    {
        if (TestScreen->objectName().isEmpty())
            TestScreen->setObjectName("TestScreen");
        TestScreen->resize(640, 480);
        centralwidget = new QWidget(TestScreen);
        centralwidget->setObjectName("centralwidget");
        label_BP = new QLabel(centralwidget);
        label_BP->setObjectName("label_BP");
        label_BP->setGeometry(QRect(120, 440, 21, 25));
        lineEdit_SD1 = new QLineEdit(centralwidget);
        lineEdit_SD1->setObjectName("lineEdit_SD1");
        lineEdit_SD1->setGeometry(QRect(360, 440, 40, 25));
        lineEdit_SD1->setReadOnly(true);
        lineEdit_calset = new QLineEdit(centralwidget);
        lineEdit_calset->setObjectName("lineEdit_calset");
        lineEdit_calset->setGeometry(QRect(200, 0, 40, 20));
        lineEdit_calset->setReadOnly(false);
        Plot = new QCustomPlot(centralwidget);
        Plot->setObjectName("Plot");
        Plot->setGeometry(QRect(0, 30, 500, 400));
        pushButton_SaveButton = new QPushButton(centralwidget);
        pushButton_SaveButton->setObjectName("pushButton_SaveButton");
        pushButton_SaveButton->setGeometry(QRect(580, 450, 51, 21));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(520, 0, 71, 23));
        progressBar->setValue(80);
        label_PH = new QLabel(centralwidget);
        label_PH->setObjectName("label_PH");
        label_PH->setGeometry(QRect(10, 440, 21, 25));
        lineEdit_TH2 = new QLineEdit(centralwidget);
        lineEdit_TH2->setObjectName("lineEdit_TH2");
        lineEdit_TH2->setGeometry(QRect(580, 360, 40, 20));
        lineEdit_TH1 = new QLineEdit(centralwidget);
        lineEdit_TH1->setObjectName("lineEdit_TH1");
        lineEdit_TH1->setGeometry(QRect(580, 270, 40, 20));
        lineEdit_Range = new QLineEdit(centralwidget);
        lineEdit_Range->setObjectName("lineEdit_Range");
        lineEdit_Range->setGeometry(QRect(580, 90, 40, 20));
        lineEdit_PH1 = new QLineEdit(centralwidget);
        lineEdit_PH1->setObjectName("lineEdit_PH1");
        lineEdit_PH1->setGeometry(QRect(30, 440, 40, 25));
        lineEdit_PH1->setReadOnly(true);
        lineEdit_SD2 = new QLineEdit(centralwidget);
        lineEdit_SD2->setObjectName("lineEdit_SD2");
        lineEdit_SD2->setGeometry(QRect(400, 440, 40, 25));
        lineEdit_SD2->setReadOnly(true);
        label_calset_2 = new QLabel(centralwidget);
        label_calset_2->setObjectName("label_calset_2");
        label_calset_2->setGeometry(QRect(530, 30, 50, 20));
        lineEdit_BP1 = new QLineEdit(centralwidget);
        lineEdit_BP1->setObjectName("lineEdit_BP1");
        lineEdit_BP1->setGeometry(QRect(140, 440, 40, 25));
        lineEdit_BP1->setReadOnly(true);
        lineEdit_Machine = new QLineEdit(centralwidget);
        lineEdit_Machine->setObjectName("lineEdit_Machine");
        lineEdit_Machine->setGeometry(QRect(70, 0, 60, 20));
        lineEdit_Machine->setReadOnly(true);
        label_Reject = new QLabel(centralwidget);
        label_Reject->setObjectName("label_Reject");
        label_Reject->setGeometry(QRect(530, 150, 51, 20));
        label_G2ST = new QLabel(centralwidget);
        label_G2ST->setObjectName("label_G2ST");
        label_G2ST->setGeometry(QRect(530, 300, 41, 20));
        label_G2ED = new QLabel(centralwidget);
        label_G2ED->setObjectName("label_G2ED");
        label_G2ED->setGeometry(QRect(530, 330, 41, 20));
        lineEdit_Velocity = new QLineEdit(centralwidget);
        lineEdit_Velocity->setObjectName("lineEdit_Velocity");
        lineEdit_Velocity->setGeometry(QRect(320, 0, 40, 20));
        lineEdit_Velocity->setReadOnly(true);
        label_G1ST = new QLabel(centralwidget);
        label_G1ST->setObjectName("label_G1ST");
        label_G1ST->setGeometry(QRect(530, 210, 41, 20));
        label_Range = new QLabel(centralwidget);
        label_Range->setObjectName("label_Range");
        label_Range->setGeometry(QRect(530, 90, 51, 20));
        label_meter = new QLabel(centralwidget);
        label_meter->setObjectName("label_meter");
        label_meter->setGeometry(QRect(360, 0, 31, 20));
        label_meter->setTextFormat(Qt::TextFormat::MarkdownText);
        label_D = new QLabel(centralwidget);
        label_D->setObjectName("label_D");
        label_D->setGeometry(QRect(230, 440, 21, 25));
        label_TH2 = new QLabel(centralwidget);
        label_TH2->setObjectName("label_TH2");
        label_TH2->setGeometry(QRect(530, 360, 41, 20));
        lineEdit_G2ED = new QLineEdit(centralwidget);
        lineEdit_G2ED->setObjectName("lineEdit_G2ED");
        lineEdit_G2ED->setGeometry(QRect(580, 330, 40, 20));
        lineEdit_P2P = new QLineEdit(centralwidget);
        lineEdit_P2P->setObjectName("lineEdit_P2P");
        lineEdit_P2P->setGeometry(QRect(480, 440, 40, 25));
        lineEdit_Mode = new QLineEdit(centralwidget);
        lineEdit_Mode->setObjectName("lineEdit_Mode");
        lineEdit_Mode->setGeometry(QRect(435, 0, 40, 20));
        lineEdit_Mode->setReadOnly(true);
        label_TH1 = new QLabel(centralwidget);
        label_TH1->setObjectName("label_TH1");
        label_TH1->setGeometry(QRect(530, 270, 41, 20));
        lineEdit_G1ST = new QLineEdit(centralwidget);
        lineEdit_G1ST->setObjectName("lineEdit_G1ST");
        lineEdit_G1ST->setGeometry(QRect(580, 210, 40, 20));
        lineEdit_Angle = new QLineEdit(centralwidget);
        lineEdit_Angle->setObjectName("lineEdit_Angle");
        lineEdit_Angle->setGeometry(QRect(580, 180, 40, 20));
        label_P2P = new QLabel(centralwidget);
        label_P2P->setObjectName("label_P2P");
        label_P2P->setGeometry(QRect(450, 440, 26, 25));
        lineEdit_BP2 = new QLineEdit(centralwidget);
        lineEdit_BP2->setObjectName("lineEdit_BP2");
        lineEdit_BP2->setGeometry(QRect(180, 440, 40, 25));
        lineEdit_BP2->setReadOnly(true);
        label_SD = new QLabel(centralwidget);
        label_SD->setObjectName("label_SD");
        label_SD->setGeometry(QRect(340, 440, 21, 25));
        label_Angle = new QLabel(centralwidget);
        label_Angle->setObjectName("label_Angle");
        label_Angle->setGeometry(QRect(530, 180, 51, 20));
        lineEdit_Gain = new QLineEdit(centralwidget);
        lineEdit_Gain->setObjectName("lineEdit_Gain");
        lineEdit_Gain->setGeometry(QRect(580, 60, 40, 20));
        label_G1ED = new QLabel(centralwidget);
        label_G1ED->setObjectName("label_G1ED");
        label_G1ED->setGeometry(QRect(530, 240, 41, 20));
        label_Gain = new QLabel(centralwidget);
        label_Gain->setObjectName("label_Gain");
        label_Gain->setGeometry(QRect(530, 60, 51, 20));
        lineEdit_Reject = new QLineEdit(centralwidget);
        lineEdit_Reject->setObjectName("lineEdit_Reject");
        lineEdit_Reject->setGeometry(QRect(580, 150, 40, 20));
        lineEdit_G2ST = new QLineEdit(centralwidget);
        lineEdit_G2ST->setObjectName("lineEdit_G2ST");
        lineEdit_G2ST->setGeometry(QRect(580, 300, 40, 20));
        lineEdit_D1 = new QLineEdit(centralwidget);
        lineEdit_D1->setObjectName("lineEdit_D1");
        lineEdit_D1->setGeometry(QRect(250, 440, 40, 25));
        lineEdit_D1->setReadOnly(true);
        label_calset = new QLabel(centralwidget);
        label_calset->setObjectName("label_calset");
        label_calset->setGeometry(QRect(150, 0, 41, 20));
        lineEdit_D2 = new QLineEdit(centralwidget);
        lineEdit_D2->setObjectName("lineEdit_D2");
        lineEdit_D2->setGeometry(QRect(290, 440, 40, 25));
        lineEdit_D2->setReadOnly(true);
        label_Velocity = new QLabel(centralwidget);
        label_Velocity->setObjectName("label_Velocity");
        label_Velocity->setGeometry(QRect(260, 0, 61, 20));
        lineEdit_PH2 = new QLineEdit(centralwidget);
        lineEdit_PH2->setObjectName("lineEdit_PH2");
        lineEdit_PH2->setGeometry(QRect(70, 440, 40, 25));
        lineEdit_PH2->setReadOnly(true);
        label_Mode = new QLabel(centralwidget);
        label_Mode->setObjectName("label_Mode");
        label_Mode->setGeometry(QRect(400, 0, 31, 20));
        label_Delay = new QLabel(centralwidget);
        label_Delay->setObjectName("label_Delay");
        label_Delay->setGeometry(QRect(530, 120, 51, 20));
        label_Machine = new QLabel(centralwidget);
        label_Machine->setObjectName("label_Machine");
        label_Machine->setGeometry(QRect(20, 0, 51, 20));
        lineEdit_G1ED = new QLineEdit(centralwidget);
        lineEdit_G1ED->setObjectName("lineEdit_G1ED");
        lineEdit_G1ED->setGeometry(QRect(580, 240, 40, 20));
        lineEdit_Delay = new QLineEdit(centralwidget);
        lineEdit_Delay->setObjectName("lineEdit_Delay");
        lineEdit_Delay->setGeometry(QRect(580, 120, 40, 20));
        lineEdit_ch = new QLineEdit(centralwidget);
        lineEdit_ch->setObjectName("lineEdit_ch");
        lineEdit_ch->setGeometry(QRect(580, 30, 40, 20));
        TestScreen->setCentralWidget(centralwidget);

        retranslateUi(TestScreen);

        QMetaObject::connectSlotsByName(TestScreen);
    } // setupUi

    void retranslateUi(QMainWindow *TestScreen)
    {
        TestScreen->setWindowTitle(QCoreApplication::translate("TestScreen", "TestScreen", nullptr));
        label_BP->setText(QCoreApplication::translate("TestScreen", "BP", nullptr));
        lineEdit_SD1->setText(QString());
        lineEdit_calset->setText(QString());
        pushButton_SaveButton->setText(QCoreApplication::translate("TestScreen", "Save", nullptr));
        label_PH->setText(QCoreApplication::translate("TestScreen", "PH", nullptr));
        lineEdit_TH2->setText(QString());
        lineEdit_TH1->setText(QString());
        lineEdit_Range->setText(QString());
        lineEdit_SD2->setText(QString());
        label_calset_2->setText(QCoreApplication::translate("TestScreen", "CH No.", nullptr));
        lineEdit_Machine->setText(QCoreApplication::translate("TestScreen", "AT-211D", nullptr));
        label_Reject->setText(QCoreApplication::translate("TestScreen", "Reject", nullptr));
        label_G2ST->setText(QCoreApplication::translate("TestScreen", "G2ST", nullptr));
        label_G2ED->setText(QCoreApplication::translate("TestScreen", "G2ED", nullptr));
        lineEdit_Velocity->setText(QString());
        label_G1ST->setText(QCoreApplication::translate("TestScreen", "G1ST", nullptr));
        label_Range->setText(QCoreApplication::translate("TestScreen", "Range", nullptr));
        label_meter->setText(QCoreApplication::translate("TestScreen", "m/s", nullptr));
        label_D->setText(QCoreApplication::translate("TestScreen", "D", nullptr));
        label_TH2->setText(QCoreApplication::translate("TestScreen", "TH2", nullptr));
        lineEdit_G2ED->setText(QString());
        lineEdit_Mode->setText(QString());
        label_TH1->setText(QCoreApplication::translate("TestScreen", "TH1", nullptr));
        lineEdit_G1ST->setText(QString());
        lineEdit_Angle->setText(QString());
        label_P2P->setText(QCoreApplication::translate("TestScreen", "P2P", nullptr));
        label_SD->setText(QCoreApplication::translate("TestScreen", "SD", nullptr));
        label_Angle->setText(QCoreApplication::translate("TestScreen", "Angle", nullptr));
        lineEdit_Gain->setText(QString());
        label_G1ED->setText(QCoreApplication::translate("TestScreen", "G1ED", nullptr));
        label_Gain->setText(QCoreApplication::translate("TestScreen", "Gain", nullptr));
        lineEdit_Reject->setText(QString());
        lineEdit_G2ST->setText(QString());
        lineEdit_D1->setText(QString());
        label_calset->setText(QCoreApplication::translate("TestScreen", "CalSet", nullptr));
        lineEdit_D2->setText(QString());
        label_Velocity->setText(QCoreApplication::translate("TestScreen", "Velocity", nullptr));
        label_Mode->setText(QCoreApplication::translate("TestScreen", "Mode", nullptr));
        label_Delay->setText(QCoreApplication::translate("TestScreen", "Delay", nullptr));
        label_Machine->setText(QCoreApplication::translate("TestScreen", "MC. No", nullptr));
        lineEdit_G1ED->setText(QString());
        lineEdit_Delay->setText(QString());
        lineEdit_ch->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TestScreen: public Ui_TestScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTSCREEN_H
