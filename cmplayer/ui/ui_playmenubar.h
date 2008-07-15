/********************************************************************************
** Form generated from reading ui file 'playmenubar.ui'
**
** Created: Tue Jul 15 23:30:54 2008
**      by: Qt User Interface Compiler version 4.4.0
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
    Ui__PlayMenuBar->resize(188, 20);
    vboxLayout = new QVBoxLayout(Ui__PlayMenuBar);
    vboxLayout->setSpacing(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    tools_widget = new QWidget(Ui__PlayMenuBar);
    tools_widget->setObjectName(QString::fromUtf8("tools_widget"));

    vboxLayout->addWidget(tools_widget);

    widget = new QWidget(Ui__PlayMenuBar);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setAutoFillBackground(false);
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
    widget->setStyleSheet(QApplication::translate("Ui::PlayMenuBar", "background-color: black;\n"
"color: white;", 0, QApplication::UnicodeUTF8));
    play_info_label->setStyleSheet(QString());
    play_info_label->setText(QApplication::translate("Ui::PlayMenuBar", "Ready For Play", 0, QApplication::UnicodeUTF8));
    current_time_label->setText(QApplication::translate("Ui::PlayMenuBar", "00:00:00", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Ui::PlayMenuBar", "/", 0, QApplication::UnicodeUTF8));
    total_time_label->setStyleSheet(QString());
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
