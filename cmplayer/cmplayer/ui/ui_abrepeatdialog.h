/********************************************************************************
** Form generated from reading ui file 'abrepeatdialog.ui'
**
** Created: Mon Sep 22 14:28:01 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ABREPEATDIALOG_H
#define UI_ABREPEATDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTimeEdit>
#include <QtGui/QVBoxLayout>

namespace Ui {

class Ui_ABRepeatDialog
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QTimeEdit *a_time_edit;
    QLabel *label_2;
    QHBoxLayout *hboxLayout1;
    QTimeEdit *b_time_edit;
    QSpacerItem *spacerItem;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout2;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QPushButton *a_time_button;
    QPushButton *a_subtitle_button;
    QLabel *label_6;
    QPushButton *b_time_button;
    QPushButton *b_subtitle_button;
    QSpacerItem *spacerItem1;
    QHBoxLayout *hboxLayout3;
    QLabel *label_3;
    QHBoxLayout *hboxLayout4;
    QSpinBox *times_spin;
    QSpacerItem *spacerItem2;
    QLabel *label_4;
    QDoubleSpinBox *add_time_spin;
    QLabel *label_7;
    QHBoxLayout *hboxLayout5;
    QSpacerItem *spacerItem3;
    QPushButton *start_button;
    QPushButton *quit_button;
    QPushButton *close_button;

    void setupUi(QDialog *Ui__ABRepeatDialog)
    {
    if (Ui__ABRepeatDialog->objectName().isEmpty())
        Ui__ABRepeatDialog->setObjectName(QString::fromUtf8("Ui__ABRepeatDialog"));
    Ui__ABRepeatDialog->resize(299, 266);
    vboxLayout = new QVBoxLayout(Ui__ABRepeatDialog);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    groupBox_2 = new QGroupBox(Ui__ABRepeatDialog);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    hboxLayout = new QHBoxLayout(groupBox_2);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    label = new QLabel(groupBox_2);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout->addWidget(label);

    a_time_edit = new QTimeEdit(groupBox_2);
    a_time_edit->setObjectName(QString::fromUtf8("a_time_edit"));

    hboxLayout->addWidget(a_time_edit);

    label_2 = new QLabel(groupBox_2);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    hboxLayout->addWidget(label_2);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    b_time_edit = new QTimeEdit(groupBox_2);
    b_time_edit->setObjectName(QString::fromUtf8("b_time_edit"));

    hboxLayout1->addWidget(b_time_edit);

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);


    hboxLayout->addLayout(hboxLayout1);


    vboxLayout->addWidget(groupBox_2);

    groupBox = new QGroupBox(Ui__ABRepeatDialog);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    hboxLayout2 = new QHBoxLayout(groupBox);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout->addWidget(label_5, 0, 0, 1, 1);

    a_time_button = new QPushButton(groupBox);
    a_time_button->setObjectName(QString::fromUtf8("a_time_button"));

    gridLayout->addWidget(a_time_button, 0, 1, 1, 1);

    a_subtitle_button = new QPushButton(groupBox);
    a_subtitle_button->setObjectName(QString::fromUtf8("a_subtitle_button"));

    gridLayout->addWidget(a_subtitle_button, 0, 2, 1, 1);

    label_6 = new QLabel(groupBox);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    gridLayout->addWidget(label_6, 1, 0, 1, 1);

    b_time_button = new QPushButton(groupBox);
    b_time_button->setObjectName(QString::fromUtf8("b_time_button"));

    gridLayout->addWidget(b_time_button, 1, 1, 1, 1);

    b_subtitle_button = new QPushButton(groupBox);
    b_subtitle_button->setObjectName(QString::fromUtf8("b_subtitle_button"));

    gridLayout->addWidget(b_subtitle_button, 1, 2, 1, 1);


    hboxLayout2->addLayout(gridLayout);

    spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem1);


    vboxLayout->addWidget(groupBox);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    label_3 = new QLabel(Ui__ABRepeatDialog);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    hboxLayout3->addWidget(label_3);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setSpacing(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    times_spin = new QSpinBox(Ui__ABRepeatDialog);
    times_spin->setObjectName(QString::fromUtf8("times_spin"));
    times_spin->setAccelerated(true);
    times_spin->setMaximum(999);

    hboxLayout4->addWidget(times_spin);

    spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem2);


    hboxLayout3->addLayout(hboxLayout4);

    label_4 = new QLabel(Ui__ABRepeatDialog);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    hboxLayout3->addWidget(label_4);

    add_time_spin = new QDoubleSpinBox(Ui__ABRepeatDialog);
    add_time_spin->setObjectName(QString::fromUtf8("add_time_spin"));
    add_time_spin->setAccelerated(true);
    add_time_spin->setDecimals(1);
    add_time_spin->setMaximum(9);
    add_time_spin->setSingleStep(0.1);
    add_time_spin->setValue(0);

    hboxLayout3->addWidget(add_time_spin);

    label_7 = new QLabel(Ui__ABRepeatDialog);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    hboxLayout3->addWidget(label_7);


    vboxLayout->addLayout(hboxLayout3);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem3);

    start_button = new QPushButton(Ui__ABRepeatDialog);
    start_button->setObjectName(QString::fromUtf8("start_button"));

    hboxLayout5->addWidget(start_button);

    quit_button = new QPushButton(Ui__ABRepeatDialog);
    quit_button->setObjectName(QString::fromUtf8("quit_button"));

    hboxLayout5->addWidget(quit_button);

    close_button = new QPushButton(Ui__ABRepeatDialog);
    close_button->setObjectName(QString::fromUtf8("close_button"));
    close_button->setDefault(true);

    hboxLayout5->addWidget(close_button);


    vboxLayout->addLayout(hboxLayout5);


    retranslateUi(Ui__ABRepeatDialog);
    QObject::connect(close_button, SIGNAL(clicked()), Ui__ABRepeatDialog, SLOT(hide()));

    QMetaObject::connectSlotsByName(Ui__ABRepeatDialog);
    } // setupUi

    void retranslateUi(QDialog *Ui__ABRepeatDialog)
    {
    Ui__ABRepeatDialog->setWindowTitle(QApplication::translate("Ui::ABRepeatDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("Ui::ABRepeatDialog", "\353\260\230\353\263\265 \352\265\254\352\260\204", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Ui::ABRepeatDialog", "A:", 0, QApplication::UnicodeUTF8));
    a_time_edit->setDisplayFormat(QApplication::translate("Ui::ABRepeatDialog", "h:mm:ss.zzz", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Ui::ABRepeatDialog", "B:", 0, QApplication::UnicodeUTF8));
    b_time_edit->setDisplayFormat(QApplication::translate("Ui::ABRepeatDialog", "h:mm:ss.zzz", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("Ui::ABRepeatDialog", "\354\236\220\353\217\231\354\236\205\353\240\245", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("Ui::ABRepeatDialog", "A \354\236\205\353\240\245", 0, QApplication::UnicodeUTF8));
    a_time_button->setText(QApplication::translate("Ui::ABRepeatDialog", "\355\230\204\354\236\254 \354\213\234\352\260\204(&A)", 0, QApplication::UnicodeUTF8));
    a_subtitle_button->setText(QApplication::translate("Ui::ABRepeatDialog", "\355\230\204\354\236\254 \354\236\220\353\247\211 \354\213\234\354\236\221(&S)", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("Ui::ABRepeatDialog", "B \354\236\205\353\240\245", 0, QApplication::UnicodeUTF8));
    b_time_button->setText(QApplication::translate("Ui::ABRepeatDialog", "\355\230\204\354\236\254 \354\213\234\352\260\204(&B)", 0, QApplication::UnicodeUTF8));
    b_subtitle_button->setText(QApplication::translate("Ui::ABRepeatDialog", "\355\230\204\354\236\254 \354\236\220\353\247\211 \354\242\205\353\243\214(&E)", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Ui::ABRepeatDialog", "\353\260\230\353\263\265\355\232\237\354\210\230: ", 0, QApplication::UnicodeUTF8));
    times_spin->setSpecialValueText(QApplication::translate("Ui::ABRepeatDialog", "\353\254\264\355\225\234\353\260\230\353\263\265", 0, QApplication::UnicodeUTF8));
    times_spin->setSuffix(QApplication::translate("Ui::ABRepeatDialog", " \353\262\210", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("Ui::ABRepeatDialog", "\354\225\236\353\222\244\353\241\234", 0, QApplication::UnicodeUTF8));
    add_time_spin->setSuffix(QApplication::translate("Ui::ABRepeatDialog", " \354\264\210", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("Ui::ABRepeatDialog", "\354\266\224\352\260\200", 0, QApplication::UnicodeUTF8));
    start_button->setText(QApplication::translate("Ui::ABRepeatDialog", "\353\260\230\353\263\265\354\213\234\354\236\221", 0, QApplication::UnicodeUTF8));
    quit_button->setText(QApplication::translate("Ui::ABRepeatDialog", "\354\242\205\353\243\214", 0, QApplication::UnicodeUTF8));
    close_button->setText(QApplication::translate("Ui::ABRepeatDialog", "\353\213\253\352\270\260", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Ui__ABRepeatDialog);
    } // retranslateUi

};

} // namespace Ui

namespace Ui {
namespace Ui {
    class ABRepeatDialog: public Ui_ABRepeatDialog {};
} // namespace Ui
} // namespace Ui

#endif // UI_ABREPEATDIALOG_H
