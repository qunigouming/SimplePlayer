/********************************************************************************
** Form generated from reading UI file 'volume.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOLUME_H
#define UI_VOLUME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Volume
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QSlider *volumeSlider;
    QSpacerItem *horizontalSpacer_2;
    QLabel *volumeLab;

    void setupUi(QDialog *Volume)
    {
        if (Volume->objectName().isEmpty())
            Volume->setObjectName(QString::fromUtf8("Volume"));
        Volume->resize(30, 120);
        QFont font;
        font.setPointSize(9);
        Volume->setFont(font);
        Volume->setStyleSheet(QString::fromUtf8(".Dialog {\n"
"background-color: rgb(255, 255, 255);\n"
"border-style: none;\n"
"}"));
        verticalLayout = new QVBoxLayout(Volume);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(Volume);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(4, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        volumeSlider = new QSlider(widget);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        volumeSlider->setStyleSheet(QString::fromUtf8("QSlider {\n"
"	padding-top: 10px;\n"
"	padding-bottom: 5px;\n"
"	padding-left:7px;\n"
"	padding-right:8px;\n"
"	border-radius: 5px;\n"
"}\n"
" \n"
"QSlider::add-page:vertical {\n"
"	background-color: rgb(236, 65, 65);	\n"
"	width:3px;\n"
"	border-radius: 2px;\n"
"}\n"
" \n"
"QSlider::sub-page:vertical {\n"
"	background-color: rgb(229, 229, 229);\n"
"	width:3px;\n"
"	border-radius: 2px;\n"
"}\n"
" \n"
"QSlider::groove:vertical {\n"
"	background:transparent;\n"
"	width:4px;\n"
"}\n"
" \n"
"QSlider::handle:vertical {\n"
"	height: 9px;\n"
"	width: 4px;\n"
"	margin: 0px -3px 0px -3px;\n"
"	border-radius: 4px;\n"
"	background-color: rgb(236, 65, 65);\n"
"}\n"
""));
        volumeSlider->setMaximum(100);
        volumeSlider->setValue(100);
        volumeSlider->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(volumeSlider);

        horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget);

        volumeLab = new QLabel(Volume);
        volumeLab->setObjectName(QString::fromUtf8("volumeLab"));
        QFont font1;
        volumeLab->setFont(font1);
        volumeLab->setStyleSheet(QString::fromUtf8(".QLabel {\n"
"color: rgb(88, 88, 88);\n"
"font-size: 10px;\n"
"}"));
        volumeLab->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(volumeLab);


        retranslateUi(Volume);

        QMetaObject::connectSlotsByName(Volume);
    } // setupUi

    void retranslateUi(QDialog *Volume)
    {
        Volume->setWindowTitle(QCoreApplication::translate("Volume", "Dialog", nullptr));
        volumeLab->setText(QCoreApplication::translate("Volume", "100%", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Volume: public Ui_Volume {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUME_H
