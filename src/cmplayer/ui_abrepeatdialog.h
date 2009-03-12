/********************************************************************************
** Form generated from reading ui file 'abrepeatdialog.ui'
**
** Created: Thu Mar 12 09:58:05 2009
**      by: Qt User Interface Compiler version 4.4.3
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

QT_BEGIN_NAMESPACE

class Ui_ABRepeatDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QTimeEdit *a;
    QSpacerItem *spacerItem;
    QLabel *label_2;
    QTimeEdit *b;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QPushButton *aTime;
    QPushButton *aSub;
    QLabel *label_6;
    QPushButton *bTime;
    QPushButton *bSub;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSpinBox *times;
    QSpacerItem *spacerItem1;
    QLabel *label_4;
    QDoubleSpinBox *add;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *spacerItem2;
    QPushButton *start;
    QPushButton *quit;
    QPushButton *close;

    void setupUi(QDialog *ABRepeatDialog)
    {
    if (ABRepeatDialog->objectName().isEmpty())
        ABRepeatDialog->setObjectName(QString::fromUtf8("ABRepeatDialog"));
    ABRepeatDialog->resize(328, 245);
    verticalLayout = new QVBoxLayout(ABRepeatDialog);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    groupBox_2 = new QGroupBox(ABRepeatDialog);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    horizontalLayout_3 = new QHBoxLayout(groupBox_2);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    label = new QLabel(groupBox_2);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout_3->addWidget(label);

    a = new QTimeEdit(groupBox_2);
    a->setObjectName(QString::fromUtf8("a"));

    horizontalLayout_3->addWidget(a);

    spacerItem = new QSpacerItem(15, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(spacerItem);

    label_2 = new QLabel(groupBox_2);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout_3->addWidget(label_2);

    b = new QTimeEdit(groupBox_2);
    b->setObjectName(QString::fromUtf8("b"));

    horizontalLayout_3->addWidget(b);


    verticalLayout->addWidget(groupBox_2);

    groupBox = new QGroupBox(ABRepeatDialog);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    gridLayout = new QGridLayout(groupBox);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
    label_5->setSizePolicy(sizePolicy);

    gridLayout->addWidget(label_5, 0, 0, 1, 1);

    aTime = new QPushButton(groupBox);
    aTime->setObjectName(QString::fromUtf8("aTime"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(aTime->sizePolicy().hasHeightForWidth());
    aTime->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(aTime, 0, 1, 1, 1);

    aSub = new QPushButton(groupBox);
    aSub->setObjectName(QString::fromUtf8("aSub"));
    sizePolicy1.setHeightForWidth(aSub->sizePolicy().hasHeightForWidth());
    aSub->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(aSub, 0, 2, 1, 1);

    label_6 = new QLabel(groupBox);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
    label_6->setSizePolicy(sizePolicy);

    gridLayout->addWidget(label_6, 1, 0, 1, 1);

    bTime = new QPushButton(groupBox);
    bTime->setObjectName(QString::fromUtf8("bTime"));
    sizePolicy1.setHeightForWidth(bTime->sizePolicy().hasHeightForWidth());
    bTime->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(bTime, 1, 1, 1, 1);

    bSub = new QPushButton(groupBox);
    bSub->setObjectName(QString::fromUtf8("bSub"));
    sizePolicy1.setHeightForWidth(bSub->sizePolicy().hasHeightForWidth());
    bSub->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(bSub, 1, 2, 1, 1);


    verticalLayout->addWidget(groupBox);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_3 = new QLabel(ABRepeatDialog);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    horizontalLayout_2->addWidget(label_3);

    times = new QSpinBox(ABRepeatDialog);
    times->setObjectName(QString::fromUtf8("times"));
    times->setAccelerated(true);
    times->setMaximum(999);

    horizontalLayout_2->addWidget(times);

    spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(spacerItem1);

    label_4 = new QLabel(ABRepeatDialog);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    horizontalLayout_2->addWidget(label_4);

    add = new QDoubleSpinBox(ABRepeatDialog);
    add->setObjectName(QString::fromUtf8("add"));
    add->setAccelerated(true);
    add->setDecimals(1);
    add->setMaximum(9);
    add->setSingleStep(0.1);
    add->setValue(0);

    horizontalLayout_2->addWidget(add);


    verticalLayout->addLayout(horizontalLayout_2);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(spacerItem2);

    start = new QPushButton(ABRepeatDialog);
    start->setObjectName(QString::fromUtf8("start"));

    horizontalLayout->addWidget(start);

    quit = new QPushButton(ABRepeatDialog);
    quit->setObjectName(QString::fromUtf8("quit"));

    horizontalLayout->addWidget(quit);

    close = new QPushButton(ABRepeatDialog);
    close->setObjectName(QString::fromUtf8("close"));
    close->setDefault(true);

    horizontalLayout->addWidget(close);


    verticalLayout->addLayout(horizontalLayout);


    retranslateUi(ABRepeatDialog);
    QObject::connect(close, SIGNAL(clicked()), ABRepeatDialog, SLOT(hide()));

    QMetaObject::connectSlotsByName(ABRepeatDialog);
    } // setupUi

    void retranslateUi(QDialog *ABRepeatDialog)
    {
    ABRepeatDialog->setWindowTitle(QApplication::translate("ABRepeatDialog", "A-B Repeat", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("ABRepeatDialog", "Repeat Range", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("ABRepeatDialog", "A:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("ABRepeatDialog", "B:", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("ABRepeatDialog", "Automatic Selection", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("ABRepeatDialog", "A: ", 0, QApplication::UnicodeUTF8));
    aTime->setText(QApplication::translate("ABRepeatDialog", "Current Time", 0, QApplication::UnicodeUTF8));
    aSub->setText(QApplication::translate("ABRepeatDialog", "Current Subtitle Start", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("ABRepeatDialog", "B: ", 0, QApplication::UnicodeUTF8));
    bTime->setText(QApplication::translate("ABRepeatDialog", "Current TIme", 0, QApplication::UnicodeUTF8));
    bSub->setText(QApplication::translate("ABRepeatDialog", "Current Subtitle End", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("ABRepeatDialog", "Repetition:", 0, QApplication::UnicodeUTF8));
    times->setSpecialValueText(QApplication::translate("ABRepeatDialog", "Infinity", 0, QApplication::UnicodeUTF8));
    times->setSuffix(QApplication::translate("ABRepeatDialog", "times", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("ABRepeatDialog", "Add time:", 0, QApplication::UnicodeUTF8));
    add->setSuffix(QApplication::translate("ABRepeatDialog", "sec.", 0, QApplication::UnicodeUTF8));
    start->setText(QApplication::translate("ABRepeatDialog", "Start", 0, QApplication::UnicodeUTF8));
    quit->setText(QApplication::translate("ABRepeatDialog", "Quit", 0, QApplication::UnicodeUTF8));
    close->setText(QApplication::translate("ABRepeatDialog", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ABRepeatDialog);
    } // retranslateUi

};

namespace Ui {
    class ABRepeatDialog: public Ui_ABRepeatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABREPEATDIALOG_H
