/********************************************************************************
** Form generated from reading ui file 'playmenubar.ui'
**
** Created: Tue Sep 23 10:11:31 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PLAYMENUBAR_H
#define UI_PLAYMENUBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "clabel.h"

namespace Ui {

class Ui_PlayMenuBar
{
public:
    QVBoxLayout *vboxLayout;
    QWidget *tools_widget;
    QWidget *widget;
    QHBoxLayout *hboxLayout;
    CLabel *play_info_label;
    QHBoxLayout *hboxLayout1;
    QLabel *current_time_label;
    QLabel *label_2;
    QLabel *total_time_label;

    void setupUi(QWidget *Ui__PlayMenuBar)
    {
    if (Ui__PlayMenuBar->objectName().isEmpty())
        Ui__PlayMenuBar->setObjectName(QString::fromUtf8("Ui__PlayMenuBar"));
    Ui__PlayMenuBar->resize(224, 32);
    vboxLayout = new QVBoxLayout(Ui__PlayMenuBar);
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tools_widget = new QWidget(Ui__PlayMenuBar);
    tools_widget->setObjectName(QString::fromUtf8("tools_widget"));

    vboxLayout->addWidget(tools_widget);

    widget = new QWidget(Ui__PlayMenuBar);
    widget->setObjectName(QString::fromUtf8("widget"));
    QPalette palette;
    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    QBrush brush1(QColor(0, 0, 0, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush1);
    palette.setBrush(QPalette::Active, QPalette::Text, brush);
    palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
    palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
    QBrush brush2(QColor(146, 145, 144, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
    QBrush brush3(QColor(165, 164, 164, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush3);
    QBrush brush4(QColor(151, 150, 149, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
    widget->setPalette(palette);
    widget->setAutoFillBackground(true);
    hboxLayout = new QHBoxLayout(widget);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(2, 0, 2, 0);
    play_info_label = new CLabel(widget);
    play_info_label->setObjectName(QString::fromUtf8("play_info_label"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(play_info_label->sizePolicy().hasHeightForWidth());
    play_info_label->setSizePolicy(sizePolicy);
    play_info_label->setStyleSheet(QString::fromUtf8(""));

    hboxLayout->addWidget(play_info_label);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    current_time_label = new QLabel(widget);
    current_time_label->setObjectName(QString::fromUtf8("current_time_label"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(current_time_label->sizePolicy().hasHeightForWidth());
    current_time_label->setSizePolicy(sizePolicy1);

    hboxLayout1->addWidget(current_time_label);

    label_2 = new QLabel(widget);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy1);

    hboxLayout1->addWidget(label_2);

    total_time_label = new QLabel(widget);
    total_time_label->setObjectName(QString::fromUtf8("total_time_label"));
    sizePolicy1.setHeightForWidth(total_time_label->sizePolicy().hasHeightForWidth());
    total_time_label->setSizePolicy(sizePolicy1);
    total_time_label->setStyleSheet(QString::fromUtf8(""));
    total_time_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout1->addWidget(total_time_label);


    hboxLayout->addLayout(hboxLayout1);


    vboxLayout->addWidget(widget);


    retranslateUi(Ui__PlayMenuBar);

    QMetaObject::connectSlotsByName(Ui__PlayMenuBar);
    } // setupUi

    void retranslateUi(QWidget *Ui__PlayMenuBar)
    {
    Ui__PlayMenuBar->setWindowTitle(QApplication::translate("Ui::PlayMenuBar", "Form", 0, QApplication::UnicodeUTF8));
    play_info_label->setText(QApplication::translate("Ui::PlayMenuBar", "Ready For Play", 0, QApplication::UnicodeUTF8));
    current_time_label->setText(QApplication::translate("Ui::PlayMenuBar", "00:00:00", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Ui::PlayMenuBar", "/", 0, QApplication::UnicodeUTF8));
    total_time_label->setText(QApplication::translate("Ui::PlayMenuBar", "00:00:00", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Ui__PlayMenuBar);
    } // retranslateUi

};

} // namespace Ui

namespace Ui {
namespace Ui {
    class PlayMenuBar: public Ui_PlayMenuBar {};
} // namespace Ui
} // namespace Ui

#endif // UI_PLAYMENUBAR_H
