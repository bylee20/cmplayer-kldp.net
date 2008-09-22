/********************************************************************************
** Form generated from reading ui file 'osdwidget.ui'
**
** Created: Mon Sep 22 14:28:01 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OSDWIDGET_H
#define UI_OSDWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

namespace Pref {
namespace Ui {

class Ui_OsdWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_5;
    QLabel *font_label;
    QPushButton *font_button;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *text_color_combo;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *border_color_combo;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *scale_combo;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout;
    QDoubleSpinBox *size_spin;
    QSpacerItem *horizontalSpacer_4;
    QCheckBox *high_quality_check;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Pref__Ui__OsdWidget)
    {
    if (Pref__Ui__OsdWidget->objectName().isEmpty())
        Pref__Ui__OsdWidget->setObjectName(QString::fromUtf8("Pref__Ui__OsdWidget"));
    Pref__Ui__OsdWidget->resize(232, 205);
    gridLayout = new QGridLayout(Pref__Ui__OsdWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(Pref__Ui__OsdWidget);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    horizontalLayout_5 = new QHBoxLayout();
    horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
    font_label = new QLabel(Pref__Ui__OsdWidget);
    font_label->setObjectName(QString::fromUtf8("font_label"));
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(font_label->sizePolicy().hasHeightForWidth());
    font_label->setSizePolicy(sizePolicy);
    font_label->setMinimumSize(QSize(100, 0));
    font_label->setFrameShape(QFrame::StyledPanel);
    font_label->setFrameShadow(QFrame::Sunken);
    font_label->setAlignment(Qt::AlignCenter);

    horizontalLayout_5->addWidget(font_label);

    font_button = new QPushButton(Pref__Ui__OsdWidget);
    font_button->setObjectName(QString::fromUtf8("font_button"));

    horizontalLayout_5->addWidget(font_button);


    gridLayout->addLayout(horizontalLayout_5, 0, 1, 1, 1);

    label_3 = new QLabel(Pref__Ui__OsdWidget);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
    text_color_combo = new QComboBox(Pref__Ui__OsdWidget);
    text_color_combo->setObjectName(QString::fromUtf8("text_color_combo"));
    text_color_combo->setMinimumSize(QSize(100, 0));

    horizontalLayout_4->addWidget(text_color_combo);

    horizontalSpacer_6 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_4->addItem(horizontalSpacer_6);


    gridLayout->addLayout(horizontalLayout_4, 1, 1, 1, 1);

    label_4 = new QLabel(Pref__Ui__OsdWidget);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 2, 0, 1, 1);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    border_color_combo = new QComboBox(Pref__Ui__OsdWidget);
    border_color_combo->setObjectName(QString::fromUtf8("border_color_combo"));
    border_color_combo->setMinimumSize(QSize(100, 0));

    horizontalLayout_3->addWidget(border_color_combo);

    horizontalSpacer_7 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(horizontalSpacer_7);


    gridLayout->addLayout(horizontalLayout_3, 2, 1, 1, 1);

    label_8 = new QLabel(Pref__Ui__OsdWidget);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout->addWidget(label_8, 3, 0, 1, 1);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    scale_combo = new QComboBox(Pref__Ui__OsdWidget);
    scale_combo->setObjectName(QString::fromUtf8("scale_combo"));
    scale_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    horizontalLayout_2->addWidget(scale_combo);

    horizontalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer_3);


    gridLayout->addLayout(horizontalLayout_2, 3, 1, 1, 1);

    label_7 = new QLabel(Pref__Ui__OsdWidget);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 4, 0, 1, 1);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    size_spin = new QDoubleSpinBox(Pref__Ui__OsdWidget);
    size_spin->setObjectName(QString::fromUtf8("size_spin"));
    size_spin->setAccelerated(true);
    size_spin->setDecimals(1);
    size_spin->setMaximum(99);
    size_spin->setSingleStep(0.5);
    size_spin->setValue(10);

    horizontalLayout->addWidget(size_spin);

    horizontalSpacer_4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_4);


    gridLayout->addLayout(horizontalLayout, 4, 1, 1, 1);

    high_quality_check = new QCheckBox(Pref__Ui__OsdWidget);
    high_quality_check->setObjectName(QString::fromUtf8("high_quality_check"));

    gridLayout->addWidget(high_quality_check, 5, 0, 1, 2);

    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer, 6, 0, 1, 2);


    retranslateUi(Pref__Ui__OsdWidget);

    QMetaObject::connectSlotsByName(Pref__Ui__OsdWidget);
    } // setupUi

    void retranslateUi(QWidget *Pref__Ui__OsdWidget)
    {
    Pref__Ui__OsdWidget->setWindowTitle(QApplication::translate("Pref::Ui::OsdWidget", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Pref::Ui::OsdWidget", "\352\270\200\352\274\264", 0, QApplication::UnicodeUTF8));
    font_label->setText(QApplication::translate("Pref::Ui::OsdWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
    font_button->setText(QApplication::translate("Pref::Ui::OsdWidget", "\353\263\200\352\262\275", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Pref::Ui::OsdWidget", "\352\270\200\354\236\220\354\203\211", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("Pref::Ui::OsdWidget", "\355\205\214\353\221\220\353\246\254\354\203\211", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("Pref::Ui::OsdWidget", "\355\201\254\352\270\260\352\270\260\354\244\200", 0, QApplication::UnicodeUTF8));
    scale_combo->clear();
    scale_combo->insertItems(0, QStringList()
     << QApplication::translate("Pref::Ui::OsdWidget", "\355\231\224\353\251\264 \354\204\270\353\241\234 \352\270\270\354\235\264", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::OsdWidget", "\355\231\224\353\251\264 \352\260\200\353\241\234 \352\270\270\354\235\264", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::OsdWidget", "\355\231\224\353\251\264 \353\214\200\352\260\201\354\204\240 \352\270\270\354\235\264", 0, QApplication::UnicodeUTF8)
    );
    label_7->setText(QApplication::translate("Pref::Ui::OsdWidget", "\352\270\200\354\236\220\355\201\254\352\270\260", 0, QApplication::UnicodeUTF8));
    size_spin->setSuffix(QApplication::translate("Pref::Ui::OsdWidget", " %", 0, QApplication::UnicodeUTF8));
    high_quality_check->setText(QApplication::translate("Pref::Ui::OsdWidget", "\352\263\240\355\222\210\354\247\210 \353\252\250\353\223\234", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Pref__Ui__OsdWidget);
    } // retranslateUi

};

} // namespace Ui
} // namespace Pref

namespace Pref {
namespace Ui {
namespace Ui {
    class OsdWidget: public Ui_OsdWidget {};
} // namespace Ui
} // namespace Ui
} // namespace Pref

#endif // UI_OSDWIDGET_H
