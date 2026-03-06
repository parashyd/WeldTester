/********************************************************************************
** Form generated from reading UI file 'Openlog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENLOG_H
#define UI_OPENLOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Openlog
{
public:
    QHBoxLayout *horizontalLayout_2;
    QWidget *Left;
    QLabel *label_Folder;
    QListWidget *listWidget_Folder;
    QWidget *Right;
    QLabel *label_Images;
    QListWidget *listWidget_Images;

    void setupUi(QWidget *Openlog)
    {
        if (Openlog->objectName().isEmpty())
            Openlog->setObjectName(QString::fromUtf8("Openlog"));
        Openlog->setWindowModality(Qt::WindowModality::ApplicationModal);
        Openlog->resize(640, 480);
        horizontalLayout_2 = new QHBoxLayout(Openlog);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        Left = new QWidget(Openlog);
        Left->setObjectName(QString::fromUtf8("Left"));
        label_Folder = new QLabel(Left);
        label_Folder->setObjectName(QString::fromUtf8("label_Folder"));
        label_Folder->setGeometry(QRect(10, 10, 291, 20));
        QFont font;
        font.setBold(true);
        label_Folder->setFont(font);
        label_Folder->setAlignment(Qt::AlignmentFlag::AlignCenter);
        listWidget_Folder = new QListWidget(Left);
        listWidget_Folder->setObjectName(QString::fromUtf8("listWidget_Folder"));
        listWidget_Folder->setGeometry(QRect(10, 40, 291, 411));

        horizontalLayout_2->addWidget(Left);

        Right = new QWidget(Openlog);
        Right->setObjectName(QString::fromUtf8("Right"));
        label_Images = new QLabel(Right);
        label_Images->setObjectName(QString::fromUtf8("label_Images"));
        label_Images->setGeometry(QRect(10, 10, 291, 20));
        label_Images->setFont(font);
        label_Images->setAlignment(Qt::AlignmentFlag::AlignCenter);
        listWidget_Images = new QListWidget(Right);
        listWidget_Images->setObjectName(QString::fromUtf8("listWidget_Images"));
        listWidget_Images->setGeometry(QRect(0, 40, 291, 411));

        horizontalLayout_2->addWidget(Right);


        retranslateUi(Openlog);

        QMetaObject::connectSlotsByName(Openlog);
    } // setupUi

    void retranslateUi(QWidget *Openlog)
    {
        Openlog->setWindowTitle(QCoreApplication::translate("Openlog", "Form", nullptr));
        label_Folder->setText(QCoreApplication::translate("Openlog", "Folders", nullptr));
        label_Images->setText(QCoreApplication::translate("Openlog", "Images", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Openlog: public Ui_Openlog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENLOG_H
