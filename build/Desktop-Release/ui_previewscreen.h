/********************************************************************************
** Form generated from reading UI file 'previewscreen.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEWSCREEN_H
#define UI_PREVIEWSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_PreviewScreen
{
public:
    QLabel *label_ModelMcNo;
    QLineEdit *lineEdit_ModelMcNo;
    QLabel *label_FilePath;
    QLineEdit *lineEdit_FilePath;
    QLabel *label_OperatorName;
    QLineEdit *lineEdit_OperatorName;
    QLabel *label_DateTime;
    QLineEdit *lineEdit_DateTime;
    QLabel *label_RecDtTm;
    QLineEdit *lineEdit_RecDtTm;
    QLabel *label_AxleWheelType;
    QLineEdit *lineEdit_AxleWheelType;
    QLabel *label_AxleWheelNo;
    QLineEdit *lineEdit_AxleWheelNo;
    QLabel *label_DefectDetails;
    QFrame *line;
    QLabel *label_PH;
    QLabel *label_BP;
    QLabel *label_D;
    QLabel *label_SD;
    QLineEdit *lineEdit_PHG1;
    QLineEdit *lineEdit_PHG2;
    QLineEdit *lineEdit_BPG1;
    QLineEdit *lineEdit_BPG2;
    QLineEdit *lineEdit_DG1;
    QLineEdit *lineEdit_DG2;
    QLineEdit *lineEdit_SDG1;
    QLineEdit *lineEdit_SDG2;
    QLabel *label_G1;
    QLabel *label_G2;
    QLabel *label_TestResult;
    QLineEdit *lineEdit_TestResult;
    QLabel *label_Remarks;
    QTextEdit *textEdit_Remarks;
    QLabel *label_TestingParameters;
    QLabel *label_Gain;
    QLineEdit *lineEdit_Gain;
    QLabel *label;
    QLineEdit *lineEdit_Delay;
    QLabel *label_RJ;
    QLineEdit *lineEdit_RJ;
    QLabel *label_Range;
    QLineEdit *lineEdit_Range;
    QLabel *label_Velocity;
    QLineEdit *lineEdit_Velocity;
    QLabel *label_Angle;
    QLineEdit *lineEdit_Angle;
    QLabel *label_Mode;
    QLineEdit *lineEdit_Mode;
    QLabel *label_Thickness;
    QLineEdit *lineEdit_Thickness;
    QLabel *label_FrameNo;
    QLineEdit *lineEdit_FrameNo;
    QLabel *label_Location;
    QLineEdit *lineEdit_Location;
    QPushButton *pushButton_save;
    QCustomPlot *Plot;
    QLabel *label_P2P;
    QLineEdit *lineEdit_P2P;

    void setupUi(QDialog *PreviewScreen)
    {
        if (PreviewScreen->objectName().isEmpty())
            PreviewScreen->setObjectName("PreviewScreen");
        PreviewScreen->resize(640, 480);
        PreviewScreen->setAcceptDrops(false);
        label_ModelMcNo = new QLabel(PreviewScreen);
        label_ModelMcNo->setObjectName("label_ModelMcNo");
        label_ModelMcNo->setGeometry(QRect(10, 10, 111, 20));
        lineEdit_ModelMcNo = new QLineEdit(PreviewScreen);
        lineEdit_ModelMcNo->setObjectName("lineEdit_ModelMcNo");
        lineEdit_ModelMcNo->setGeometry(QRect(130, 10, 113, 26));
        lineEdit_ModelMcNo->setReadOnly(true);
        label_FilePath = new QLabel(PreviewScreen);
        label_FilePath->setObjectName("label_FilePath");
        label_FilePath->setGeometry(QRect(250, 10, 63, 20));
        lineEdit_FilePath = new QLineEdit(PreviewScreen);
        lineEdit_FilePath->setObjectName("lineEdit_FilePath");
        lineEdit_FilePath->setGeometry(QRect(320, 10, 311, 26));
        lineEdit_FilePath->setReadOnly(true);
        label_OperatorName = new QLabel(PreviewScreen);
        label_OperatorName->setObjectName("label_OperatorName");
        label_OperatorName->setGeometry(QRect(10, 40, 111, 20));
        lineEdit_OperatorName = new QLineEdit(PreviewScreen);
        lineEdit_OperatorName->setObjectName("lineEdit_OperatorName");
        lineEdit_OperatorName->setGeometry(QRect(130, 40, 151, 26));
        lineEdit_OperatorName->setReadOnly(true);
        label_DateTime = new QLabel(PreviewScreen);
        label_DateTime->setObjectName("label_DateTime");
        label_DateTime->setGeometry(QRect(10, 70, 101, 20));
        lineEdit_DateTime = new QLineEdit(PreviewScreen);
        lineEdit_DateTime->setObjectName("lineEdit_DateTime");
        lineEdit_DateTime->setGeometry(QRect(130, 70, 151, 26));
        lineEdit_DateTime->setReadOnly(true);
        label_RecDtTm = new QLabel(PreviewScreen);
        label_RecDtTm->setObjectName("label_RecDtTm");
        label_RecDtTm->setGeometry(QRect(10, 100, 121, 20));
        lineEdit_RecDtTm = new QLineEdit(PreviewScreen);
        lineEdit_RecDtTm->setObjectName("lineEdit_RecDtTm");
        lineEdit_RecDtTm->setGeometry(QRect(132, 100, 151, 26));
        lineEdit_RecDtTm->setReadOnly(true);
        label_AxleWheelType = new QLabel(PreviewScreen);
        label_AxleWheelType->setObjectName("label_AxleWheelType");
        label_AxleWheelType->setGeometry(QRect(290, 40, 131, 20));
        lineEdit_AxleWheelType = new QLineEdit(PreviewScreen);
        lineEdit_AxleWheelType->setObjectName("lineEdit_AxleWheelType");
        lineEdit_AxleWheelType->setGeometry(QRect(430, 40, 113, 26));
        lineEdit_AxleWheelType->setReadOnly(true);
        label_AxleWheelNo = new QLabel(PreviewScreen);
        label_AxleWheelNo->setObjectName("label_AxleWheelNo");
        label_AxleWheelNo->setGeometry(QRect(290, 70, 131, 20));
        lineEdit_AxleWheelNo = new QLineEdit(PreviewScreen);
        lineEdit_AxleWheelNo->setObjectName("lineEdit_AxleWheelNo");
        lineEdit_AxleWheelNo->setGeometry(QRect(430, 70, 113, 26));
        lineEdit_AxleWheelNo->setReadOnly(true);
        label_DefectDetails = new QLabel(PreviewScreen);
        label_DefectDetails->setObjectName("label_DefectDetails");
        label_DefectDetails->setGeometry(QRect(490, 130, 111, 20));
        line = new QFrame(PreviewScreen);
        line->setObjectName("line");
        line->setGeometry(QRect(487, 140, 101, 20));
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);
        label_PH = new QLabel(PreviewScreen);
        label_PH->setObjectName("label_PH");
        label_PH->setGeometry(QRect(440, 170, 21, 20));
        label_BP = new QLabel(PreviewScreen);
        label_BP->setObjectName("label_BP");
        label_BP->setGeometry(QRect(440, 200, 21, 20));
        label_D = new QLabel(PreviewScreen);
        label_D->setObjectName("label_D");
        label_D->setGeometry(QRect(440, 230, 16, 20));
        label_SD = new QLabel(PreviewScreen);
        label_SD->setObjectName("label_SD");
        label_SD->setGeometry(QRect(440, 260, 21, 20));
        lineEdit_PHG1 = new QLineEdit(PreviewScreen);
        lineEdit_PHG1->setObjectName("lineEdit_PHG1");
        lineEdit_PHG1->setGeometry(QRect(500, 170, 41, 26));
        lineEdit_PHG1->setReadOnly(true);
        lineEdit_PHG2 = new QLineEdit(PreviewScreen);
        lineEdit_PHG2->setObjectName("lineEdit_PHG2");
        lineEdit_PHG2->setGeometry(QRect(560, 170, 41, 26));
        lineEdit_BPG1 = new QLineEdit(PreviewScreen);
        lineEdit_BPG1->setObjectName("lineEdit_BPG1");
        lineEdit_BPG1->setGeometry(QRect(500, 200, 41, 26));
        lineEdit_BPG1->setReadOnly(true);
        lineEdit_BPG2 = new QLineEdit(PreviewScreen);
        lineEdit_BPG2->setObjectName("lineEdit_BPG2");
        lineEdit_BPG2->setGeometry(QRect(560, 200, 41, 26));
        lineEdit_DG1 = new QLineEdit(PreviewScreen);
        lineEdit_DG1->setObjectName("lineEdit_DG1");
        lineEdit_DG1->setGeometry(QRect(500, 230, 41, 26));
        lineEdit_DG1->setReadOnly(true);
        lineEdit_DG2 = new QLineEdit(PreviewScreen);
        lineEdit_DG2->setObjectName("lineEdit_DG2");
        lineEdit_DG2->setGeometry(QRect(560, 230, 41, 26));
        lineEdit_SDG1 = new QLineEdit(PreviewScreen);
        lineEdit_SDG1->setObjectName("lineEdit_SDG1");
        lineEdit_SDG1->setGeometry(QRect(500, 260, 41, 26));
        lineEdit_SDG1->setReadOnly(true);
        lineEdit_SDG2 = new QLineEdit(PreviewScreen);
        lineEdit_SDG2->setObjectName("lineEdit_SDG2");
        lineEdit_SDG2->setGeometry(QRect(560, 260, 41, 26));
        label_G1 = new QLabel(PreviewScreen);
        label_G1->setObjectName("label_G1");
        label_G1->setGeometry(QRect(510, 150, 31, 20));
        label_G2 = new QLabel(PreviewScreen);
        label_G2->setObjectName("label_G2");
        label_G2->setGeometry(QRect(570, 150, 31, 20));
        label_TestResult = new QLabel(PreviewScreen);
        label_TestResult->setObjectName("label_TestResult");
        label_TestResult->setGeometry(QRect(440, 330, 81, 20));
        lineEdit_TestResult = new QLineEdit(PreviewScreen);
        lineEdit_TestResult->setObjectName("lineEdit_TestResult");
        lineEdit_TestResult->setGeometry(QRect(520, 330, 113, 26));
        lineEdit_TestResult->setReadOnly(true);
        label_Remarks = new QLabel(PreviewScreen);
        label_Remarks->setObjectName("label_Remarks");
        label_Remarks->setGeometry(QRect(440, 350, 63, 20));
        textEdit_Remarks = new QTextEdit(PreviewScreen);
        textEdit_Remarks->setObjectName("textEdit_Remarks");
        textEdit_Remarks->setGeometry(QRect(440, 370, 191, 41));
        textEdit_Remarks->setReadOnly(true);
        label_TestingParameters = new QLabel(PreviewScreen);
        label_TestingParameters->setObjectName("label_TestingParameters");
        label_TestingParameters->setGeometry(QRect(10, 410, 131, 20));
        label_Gain = new QLabel(PreviewScreen);
        label_Gain->setObjectName("label_Gain");
        label_Gain->setGeometry(QRect(10, 440, 40, 25));
        lineEdit_Gain = new QLineEdit(PreviewScreen);
        lineEdit_Gain->setObjectName("lineEdit_Gain");
        lineEdit_Gain->setGeometry(QRect(40, 440, 40, 25));
        lineEdit_Gain->setReadOnly(true);
        label = new QLabel(PreviewScreen);
        label->setObjectName("label");
        label->setGeometry(QRect(90, 440, 41, 25));
        lineEdit_Delay = new QLineEdit(PreviewScreen);
        lineEdit_Delay->setObjectName("lineEdit_Delay");
        lineEdit_Delay->setGeometry(QRect(130, 440, 40, 25));
        lineEdit_Delay->setReadOnly(true);
        label_RJ = new QLabel(PreviewScreen);
        label_RJ->setObjectName("label_RJ");
        label_RJ->setGeometry(QRect(180, 440, 21, 25));
        lineEdit_RJ = new QLineEdit(PreviewScreen);
        lineEdit_RJ->setObjectName("lineEdit_RJ");
        lineEdit_RJ->setGeometry(QRect(200, 440, 40, 25));
        lineEdit_RJ->setReadOnly(true);
        label_Range = new QLabel(PreviewScreen);
        label_Range->setObjectName("label_Range");
        label_Range->setGeometry(QRect(250, 440, 41, 25));
        lineEdit_Range = new QLineEdit(PreviewScreen);
        lineEdit_Range->setObjectName("lineEdit_Range");
        lineEdit_Range->setGeometry(QRect(290, 440, 40, 25));
        lineEdit_Range->setReadOnly(true);
        label_Velocity = new QLabel(PreviewScreen);
        label_Velocity->setObjectName("label_Velocity");
        label_Velocity->setGeometry(QRect(520, 420, 61, 20));
        lineEdit_Velocity = new QLineEdit(PreviewScreen);
        lineEdit_Velocity->setObjectName("lineEdit_Velocity");
        lineEdit_Velocity->setGeometry(QRect(580, 420, 40, 25));
        lineEdit_Velocity->setReadOnly(true);
        label_Angle = new QLabel(PreviewScreen);
        label_Angle->setObjectName("label_Angle");
        label_Angle->setGeometry(QRect(420, 420, 41, 20));
        lineEdit_Angle = new QLineEdit(PreviewScreen);
        lineEdit_Angle->setObjectName("lineEdit_Angle");
        lineEdit_Angle->setGeometry(QRect(470, 420, 40, 25));
        lineEdit_Angle->setReadOnly(true);
        label_Mode = new QLabel(PreviewScreen);
        label_Mode->setObjectName("label_Mode");
        label_Mode->setGeometry(QRect(420, 450, 41, 20));
        lineEdit_Mode = new QLineEdit(PreviewScreen);
        lineEdit_Mode->setObjectName("lineEdit_Mode");
        lineEdit_Mode->setGeometry(QRect(470, 450, 40, 25));
        lineEdit_Mode->setReadOnly(true);
        label_Thickness = new QLabel(PreviewScreen);
        label_Thickness->setObjectName("label_Thickness");
        label_Thickness->setGeometry(QRect(520, 450, 63, 20));
        lineEdit_Thickness = new QLineEdit(PreviewScreen);
        lineEdit_Thickness->setObjectName("lineEdit_Thickness");
        lineEdit_Thickness->setGeometry(QRect(580, 450, 40, 25));
        lineEdit_Thickness->setReadOnly(true);
        label_FrameNo = new QLabel(PreviewScreen);
        label_FrameNo->setObjectName("label_FrameNo");
        label_FrameNo->setGeometry(QRect(290, 100, 81, 20));
        lineEdit_FrameNo = new QLineEdit(PreviewScreen);
        lineEdit_FrameNo->setObjectName("lineEdit_FrameNo");
        lineEdit_FrameNo->setGeometry(QRect(430, 100, 113, 26));
        lineEdit_FrameNo->setReadOnly(true);
        label_Location = new QLabel(PreviewScreen);
        label_Location->setObjectName("label_Location");
        label_Location->setGeometry(QRect(440, 300, 71, 20));
        lineEdit_Location = new QLineEdit(PreviewScreen);
        lineEdit_Location->setObjectName("lineEdit_Location");
        lineEdit_Location->setGeometry(QRect(520, 300, 113, 26));
        lineEdit_Location->setReadOnly(true);
        pushButton_save = new QPushButton(PreviewScreen);
        pushButton_save->setObjectName("pushButton_save");
        pushButton_save->setGeometry(QRect(560, 100, 51, 29));
        Plot = new QCustomPlot(PreviewScreen);
        Plot->setObjectName("Plot");
        Plot->setGeometry(QRect(10, 139, 420, 270));
        label_P2P = new QLabel(PreviewScreen);
        label_P2P->setObjectName("label_P2P");
        label_P2P->setGeometry(QRect(340, 440, 21, 25));
        lineEdit_P2P = new QLineEdit(PreviewScreen);
        lineEdit_P2P->setObjectName("lineEdit_P2P");
        lineEdit_P2P->setGeometry(QRect(370, 440, 40, 25));

        retranslateUi(PreviewScreen);

        QMetaObject::connectSlotsByName(PreviewScreen);
    } // setupUi

    void retranslateUi(QDialog *PreviewScreen)
    {
        PreviewScreen->setWindowTitle(QCoreApplication::translate("PreviewScreen", "Dialog", nullptr));
        label_ModelMcNo->setText(QCoreApplication::translate("PreviewScreen", "Model / Mc No. :", nullptr));
        label_FilePath->setText(QCoreApplication::translate("PreviewScreen", "File Path :", nullptr));
        label_OperatorName->setText(QCoreApplication::translate("PreviewScreen", "Operator Name :", nullptr));
        lineEdit_OperatorName->setText(QString());
        label_DateTime->setText(QCoreApplication::translate("PreviewScreen", "Date & Time :", nullptr));
        lineEdit_DateTime->setText(QString());
        label_RecDtTm->setText(QCoreApplication::translate("PreviewScreen", "Rec Dt & Tm :", nullptr));
        lineEdit_RecDtTm->setText(QString());
        label_AxleWheelType->setText(QCoreApplication::translate("PreviewScreen", "Axle / Wheel Type :", nullptr));
        lineEdit_AxleWheelType->setText(QString());
        label_AxleWheelNo->setText(QCoreApplication::translate("PreviewScreen", "Axle / Wheel No. :", nullptr));
        label_DefectDetails->setText(QCoreApplication::translate("PreviewScreen", "Defect Details", nullptr));
        label_PH->setText(QCoreApplication::translate("PreviewScreen", "PH", nullptr));
        label_BP->setText(QCoreApplication::translate("PreviewScreen", "BP", nullptr));
        label_D->setText(QCoreApplication::translate("PreviewScreen", "D", nullptr));
        label_SD->setText(QCoreApplication::translate("PreviewScreen", "SD", nullptr));
        label_G1->setText(QCoreApplication::translate("PreviewScreen", "G1", nullptr));
        label_G2->setText(QCoreApplication::translate("PreviewScreen", "G2", nullptr));
        label_TestResult->setText(QCoreApplication::translate("PreviewScreen", "Test Result :", nullptr));
        label_Remarks->setText(QCoreApplication::translate("PreviewScreen", "Remarks :", nullptr));
        label_TestingParameters->setText(QCoreApplication::translate("PreviewScreen", "Testing Parameters", nullptr));
        label_Gain->setText(QCoreApplication::translate("PreviewScreen", "Gain", nullptr));
        lineEdit_Gain->setText(QString());
        label->setText(QCoreApplication::translate("PreviewScreen", "Delay", nullptr));
        label_RJ->setText(QCoreApplication::translate("PreviewScreen", "RJ", nullptr));
        label_Range->setText(QCoreApplication::translate("PreviewScreen", "Range", nullptr));
        lineEdit_Range->setText(QString());
        label_Velocity->setText(QCoreApplication::translate("PreviewScreen", "Velocity", nullptr));
        lineEdit_Velocity->setText(QString());
        label_Angle->setText(QCoreApplication::translate("PreviewScreen", "Angle", nullptr));
        label_Mode->setText(QCoreApplication::translate("PreviewScreen", "Mode", nullptr));
        label_Thickness->setText(QCoreApplication::translate("PreviewScreen", "Thickness", nullptr));
        label_FrameNo->setText(QCoreApplication::translate("PreviewScreen", "Frame No. :", nullptr));
        label_Location->setText(QCoreApplication::translate("PreviewScreen", "Location :", nullptr));
        pushButton_save->setText(QCoreApplication::translate("PreviewScreen", "Save", nullptr));
        label_P2P->setText(QCoreApplication::translate("PreviewScreen", "P2P", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PreviewScreen: public Ui_PreviewScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREVIEWSCREEN_H
