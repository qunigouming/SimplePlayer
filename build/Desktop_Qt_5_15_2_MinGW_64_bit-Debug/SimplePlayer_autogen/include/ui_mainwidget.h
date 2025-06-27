/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "displaywidget.h"
#include "videoslider.h"

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *openFileBtn;
    QComboBox *fileCombox;
    DisplayWidget *displayWidget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *playBtn;
    QPushButton *stopBtn;
    videoSlider *currentSlider;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *currentLab;
    QLabel *label_2;
    QLabel *durationLab;
    QPushButton *volumeBtn;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(800, 666);
        verticalLayout = new QVBoxLayout(MainWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(MainWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        openFileBtn = new QPushButton(widget);
        openFileBtn->setObjectName(QString::fromUtf8("openFileBtn"));

        horizontalLayout->addWidget(openFileBtn);

        fileCombox = new QComboBox(widget);
        fileCombox->setObjectName(QString::fromUtf8("fileCombox"));

        horizontalLayout->addWidget(fileCombox);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 9);

        verticalLayout->addWidget(widget);

        displayWidget = new DisplayWidget(MainWidget);
        displayWidget->setObjectName(QString::fromUtf8("displayWidget"));
        displayWidget->setMinimumSize(QSize(800, 600));
        displayWidget->setMaximumSize(QSize(800, 600));

        verticalLayout->addWidget(displayWidget);

        widget_2 = new QWidget(MainWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_4 = new QHBoxLayout(widget_2);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        playBtn = new QPushButton(widget_4);
        playBtn->setObjectName(QString::fromUtf8("playBtn"));
        playBtn->setEnabled(false);

        horizontalLayout_3->addWidget(playBtn);

        stopBtn = new QPushButton(widget_4);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));
        stopBtn->setEnabled(false);

        horizontalLayout_3->addWidget(stopBtn);


        horizontalLayout_4->addWidget(widget_4);

        currentSlider = new videoSlider(widget_2);
        currentSlider->setObjectName(QString::fromUtf8("currentSlider"));
        currentSlider->setEnabled(false);
        currentSlider->setStyleSheet(QString::fromUtf8("QSlider {\n"
"    /* \350\256\276\347\275\256\346\225\264\344\270\252\346\273\221\345\212\250\346\235\241\347\232\204\350\276\271\350\267\235 */\n"
"    padding: 0px 7px; /* \345\267\246\345\217\263\346\267\273\345\212\240\345\206\205\350\276\271\350\267\235 */\n"
"}\n"
"\n"
"/* \350\275\250\351\201\223\350\203\214\346\231\257\357\274\210\346\234\252\345\210\222\350\277\207\345\214\272\345\237\237\357\274\211 */\n"
"QSlider::groove:horizontal {\n"
"    background: #e0e0e0;\n"
"    height: 4px;\n"
"    border-radius: 2px;\n"
"}\n"
"\n"
"/* \345\267\262\345\210\222\350\277\207\345\214\272\345\237\237 */\n"
"QSlider::sub-page:horizontal {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #1ACFEB, stop:1 #2A7BFF);\n"
"    height: 4px;\n"
"    border-radius: 2px;\n"
"}\n"
"\n"
"/* \346\273\221\345\235\227\350\256\276\350\256\241 */\n"
"QSlider::handle:horizontal {\n"
"    width: 14px;\n"
"    height: 14px;\n"
"    margin: -5px 0; /* \344\273\205\350\260\203\346\225\264\345\236\202\347\233"
                        "\264\346\226\271\345\220\221\345\244\226\350\276\271\350\267\235 */\n"
"    background: white;\n"
"    border: 2px solid #1ACFEB;\n"
"    border-radius: 7px;\n"
"    box-shadow: 0 0 3px rgba(26, 207, 235, 0.8);\n"
"}\n"
"\n"
"/* \346\202\254\345\201\234\346\225\210\346\236\234 */\n"
"QSlider::handle:horizontal:hover {\n"
"    background: #f0f9ff;\n"
"    border: 2px solid #2A7BFF;\n"
"    box-shadow: 0 0 5px rgba(42, 123, 255, 0.9);\n"
"}\n"
"\n"
"/* \346\214\211\344\270\213\346\225\210\346\236\234 */\n"
"QSlider::handle:horizontal:pressed {\n"
"    background: #1ACFEB;\n"
"    border: 2px solid white;\n"
"}"));
        currentSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(currentSlider);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        currentLab = new QLabel(widget_3);
        currentLab->setObjectName(QString::fromUtf8("currentLab"));
        currentLab->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(currentLab);

        label_2 = new QLabel(widget_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_2);

        durationLab = new QLabel(widget_3);
        durationLab->setObjectName(QString::fromUtf8("durationLab"));
        QFont font;
        font.setPointSize(9);
        durationLab->setFont(font);
        durationLab->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(durationLab);

        horizontalLayout_2->setStretch(0, 4);
        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(2, 4);

        horizontalLayout_4->addWidget(widget_3);

        volumeBtn = new QPushButton(widget_2);
        volumeBtn->setObjectName(QString::fromUtf8("volumeBtn"));
        volumeBtn->setEnabled(false);
        volumeBtn->setMaximumSize(QSize(30, 30));
        QFont font1;
        volumeBtn->setFont(font1);
        volumeBtn->setStyleSheet(QString::fromUtf8("font-size: 20px;\n"
"background:transparent;\n"
"border: none;"));
        volumeBtn->setChecked(false);

        horizontalLayout_4->addWidget(volumeBtn);

        horizontalLayout_4->setStretch(0, 2);
        horizontalLayout_4->setStretch(1, 6);
        horizontalLayout_4->setStretch(2, 2);
        horizontalLayout_4->setStretch(3, 1);

        verticalLayout->addWidget(widget_2);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 9);
        verticalLayout->setStretch(2, 1);

        retranslateUi(MainWidget);

        fileCombox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QCoreApplication::translate("MainWidget", "MainWidget", nullptr));
        openFileBtn->setText(QCoreApplication::translate("MainWidget", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
        playBtn->setText(QCoreApplication::translate("MainWidget", "\346\222\255\346\224\276", nullptr));
        stopBtn->setText(QCoreApplication::translate("MainWidget", "\345\201\234\346\255\242", nullptr));
        currentLab->setText(QCoreApplication::translate("MainWidget", "00:00:00", nullptr));
        label_2->setText(QCoreApplication::translate("MainWidget", "/", nullptr));
        durationLab->setText(QCoreApplication::translate("MainWidget", "00:00:00", nullptr));
        volumeBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
