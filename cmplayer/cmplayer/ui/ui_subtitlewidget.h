/********************************************************************************
** Form generated from reading ui file 'subtitlewidget.ui'
**
** Created: Mon Sep 22 14:28:01 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SUBTITLEWIDGET_H
#define UI_SUBTITLEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "pref/osdwidget.h"

namespace Pref {
namespace Ui {

class Ui_SubtitleWidget
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *tabs;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *auto_load_group;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QHBoxLayout *hboxLayout;
    QComboBox *auto_load_combo;
    QSpacerItem *spacerItem;
    QLabel *label_2;
    QHBoxLayout *hboxLayout1;
    QComboBox *auto_select_combo;
    QSpacerItem *spacerItem1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QComboBox *encoding_combo;
    QSpacerItem *spacerItem2;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout2;
    QLabel *label_10;
    QSpacerItem *spacerItem3;
    QSpinBox *init_pos_spin;
    QSlider *horizontalSlider;
    QHBoxLayout *hboxLayout3;
    QLabel *label_13;
    QSpacerItem *spacerItem4;
    QLabel *label_14;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    Pref::OsdWidget *osd_widget;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout2;
    QLabel *label_24;
    QLineEdit *priority_edit;
    QSpacerItem *spacerItem5;

    void setupUi(QWidget *Pref__Ui__SubtitleWidget)
    {
    if (Pref__Ui__SubtitleWidget->objectName().isEmpty())
        Pref__Ui__SubtitleWidget->setObjectName(QString::fromUtf8("Pref__Ui__SubtitleWidget"));
    Pref__Ui__SubtitleWidget->resize(387, 382);
    vboxLayout = new QVBoxLayout(Pref__Ui__SubtitleWidget);
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tabs = new QTabWidget(Pref__Ui__SubtitleWidget);
    tabs->setObjectName(QString::fromUtf8("tabs"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    verticalLayout_4 = new QVBoxLayout(tab);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    auto_load_group = new QGroupBox(tab);
    auto_load_group->setObjectName(QString::fromUtf8("auto_load_group"));
    auto_load_group->setCheckable(false);
    verticalLayout_3 = new QVBoxLayout(auto_load_group);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    label = new QLabel(auto_load_group);
    label->setObjectName(QString::fromUtf8("label"));

    verticalLayout_3->addWidget(label);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    auto_load_combo = new QComboBox(auto_load_group);
    auto_load_combo->setObjectName(QString::fromUtf8("auto_load_combo"));
    auto_load_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    hboxLayout->addWidget(auto_load_combo);

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);


    verticalLayout_3->addLayout(hboxLayout);

    label_2 = new QLabel(auto_load_group);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    verticalLayout_3->addWidget(label_2);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    auto_select_combo = new QComboBox(auto_load_group);
    auto_select_combo->setObjectName(QString::fromUtf8("auto_select_combo"));
    auto_select_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    hboxLayout1->addWidget(auto_select_combo);

    spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);


    verticalLayout_3->addLayout(hboxLayout1);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_5 = new QLabel(auto_load_group);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    horizontalLayout_2->addWidget(label_5);

    encoding_combo = new QComboBox(auto_load_group);
    encoding_combo->setObjectName(QString::fromUtf8("encoding_combo"));
    encoding_combo->setEditable(true);
    encoding_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    horizontalLayout_2->addWidget(encoding_combo);

    spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(spacerItem2);


    verticalLayout_3->addLayout(horizontalLayout_2);


    verticalLayout_4->addWidget(auto_load_group);

    groupBox = new QGroupBox(tab);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout1 = new QVBoxLayout(groupBox);
    vboxLayout1->setSpacing(4);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    label_10 = new QLabel(groupBox);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    hboxLayout2->addWidget(label_10);

    spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem3);

    init_pos_spin = new QSpinBox(groupBox);
    init_pos_spin->setObjectName(QString::fromUtf8("init_pos_spin"));
    init_pos_spin->setMaximum(100);
    init_pos_spin->setValue(100);

    hboxLayout2->addWidget(init_pos_spin);


    vboxLayout1->addLayout(hboxLayout2);

    horizontalSlider = new QSlider(groupBox);
    horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    horizontalSlider->setMaximum(100);
    horizontalSlider->setValue(100);
    horizontalSlider->setTracking(true);
    horizontalSlider->setOrientation(Qt::Horizontal);
    horizontalSlider->setTickPosition(QSlider::NoTicks);

    vboxLayout1->addWidget(horizontalSlider);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    label_13 = new QLabel(groupBox);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    hboxLayout3->addWidget(label_13);

    spacerItem4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem4);

    label_14 = new QLabel(groupBox);
    label_14->setObjectName(QString::fromUtf8("label_14"));

    hboxLayout3->addWidget(label_14);


    vboxLayout1->addLayout(hboxLayout3);


    verticalLayout_4->addWidget(groupBox);

    verticalSpacer_2 = new QSpacerItem(0, 3, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_4->addItem(verticalSpacer_2);

    tabs->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    verticalLayout_2 = new QVBoxLayout(tab_2);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    groupBox_2 = new QGroupBox(tab_2);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    verticalLayout = new QVBoxLayout(groupBox_2);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    osd_widget = new Pref::OsdWidget(groupBox_2);
    osd_widget->setObjectName(QString::fromUtf8("osd_widget"));

    verticalLayout->addWidget(osd_widget);

    label_3 = new QLabel(groupBox_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setWordWrap(true);

    verticalLayout->addWidget(label_3);


    verticalLayout_2->addWidget(groupBox_2);

    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_2->addItem(verticalSpacer);

    tabs->addTab(tab_2, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    vboxLayout2 = new QVBoxLayout(tab_3);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    label_24 = new QLabel(tab_3);
    label_24->setObjectName(QString::fromUtf8("label_24"));
    label_24->setWordWrap(true);

    vboxLayout2->addWidget(label_24);

    priority_edit = new QLineEdit(tab_3);
    priority_edit->setObjectName(QString::fromUtf8("priority_edit"));

    vboxLayout2->addWidget(priority_edit);

    spacerItem5 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout2->addItem(spacerItem5);

    tabs->addTab(tab_3, QString());

    vboxLayout->addWidget(tabs);


    retranslateUi(Pref__Ui__SubtitleWidget);
    QObject::connect(init_pos_spin, SIGNAL(valueChanged(int)), horizontalSlider, SLOT(setValue(int)));
    QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), init_pos_spin, SLOT(setValue(int)));

    tabs->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(Pref__Ui__SubtitleWidget);
    } // setupUi

    void retranslateUi(QWidget *Pref__Ui__SubtitleWidget)
    {
    Pref__Ui__SubtitleWidget->setWindowTitle(QApplication::translate("Pref::Ui::SubtitleWidget", "Form", 0, QApplication::UnicodeUTF8));
    auto_load_group->setTitle(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\220\353\247\211 \353\266\210\353\237\254\354\230\244\352\270\260", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274 \354\236\254\354\203\235\354\213\234\354\227\220 \353\213\244\354\235\214\354\227\220 \355\225\264\353\213\271\355\225\230\353\212\224 \354\236\220\353\247\211\354\235\204 \354\236\220\353\217\231\354\234\274\353\241\234 \353\266\210\353\237\254\354\230\265\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    auto_load_combo->clear();
    auto_load_combo->insertItems(0, QStringList()
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\220\353\247\211\354\235\204 \354\236\220\353\217\231\354\234\274\353\241\234 \353\266\210\353\237\254\354\230\244\354\247\200 \354\225\212\354\235\214", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274\352\263\274 \352\260\231\354\235\200 \354\235\264\353\246\204\354\235\230 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274 \354\235\264\353\246\204\354\235\204 \355\217\254\355\225\250\355\225\230\353\212\224 \353\252\250\353\223\240 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274\352\263\274 \352\260\231\354\235\200 \352\262\275\353\241\234\354\227\220 \354\236\210\353\212\224 \353\252\250\353\223\240 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\353\266\210\353\237\254\354\230\250 \354\236\220\353\247\211 \354\244\221 \353\213\244\354\235\214\354\227\220 \355\225\264\353\213\271\355\225\230\353\212\224 \354\236\220\353\247\211\354\235\204 \354\236\220\353\217\231\354\234\274\353\241\234 \354\204\240\355\203\235\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    auto_select_combo->clear();
    auto_select_combo->insertItems(0, QStringList()
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\352\260\200\354\236\245 \354\262\253\353\262\210\354\247\270 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274\352\263\274 \352\260\231\354\235\200 \354\235\264\353\246\204\354\235\230 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\353\266\210\353\237\254\354\230\250 \353\252\250\353\223\240 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\353\252\250\353\223\240 \353\213\244\353\245\270 \354\226\270\354\226\264", 0, QApplication::UnicodeUTF8)
    );
    label_5->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\220\353\247\211 \355\214\214\354\235\274 \354\235\270\354\275\224\353\224\251", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\220\353\247\211 \354\234\204\354\271\230", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\254\354\203\235 \354\213\234\354\236\221\354\213\234\354\235\230 \354\236\220\353\247\211 \355\221\234\354\213\234 \354\234\204\354\271\230", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\231\224\353\251\264 \354\234\204\354\252\275", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\231\224\353\251\264 \354\225\204\353\236\230\354\252\275", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab), QApplication::translate("Pref::Ui::SubtitleWidget", "\354\235\274\353\260\230", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\220\353\247\211 OSD \354\204\244\354\240\225", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\243\274) \354\236\254\354\203\235\354\227\224\354\247\204\354\235\230 \354\242\205\353\245\230\354\227\220 \353\224\260\353\235\274 \354\235\274\353\266\200 \354\230\265\354\205\230\354\235\200 \353\254\264\354\213\234\353\220\240 \354\210\230 \354\236\210\354\212\265\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab_2), QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\220\353\247\211 \354\266\234\353\240\245", 0, QApplication::UnicodeUTF8));
    label_24->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\206\265\355\225\251 \354\236\220\353\247\211\354\235\204 \353\213\244\354\235\214 \355\201\264\353\236\230\354\212\244 \354\210\234\354\204\234\353\214\200\353\241\234 \355\221\234\354\213\234\355\225\251\353\213\210\353\213\244. \355\201\264\353\236\230\354\212\244\353\223\244\354\235\200 ,(\354\211\274\355\221\234)\353\241\234 \352\265\254\353\266\204\355\225\251\353\213\210\353\213\244. \354\247\200\354\240\225\353\220\230\354\247\200 \354\225\212\354\235\200 \355\201\264\353\236\230\354\212\244\353\212\224 \354\225\214\355\214\214\353\262\263 \354\210\234\354\234\274\353\241\234 \355\221\234\354\213\234\355\225\251\353\213\210\353\213\244.\n"
"\354\236\205\353\240\245\354\230\210: ENCC,EGCC,KRCC,KNCC", 0, QApplication::UnicodeUTF8));
    priority_edit->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "ENCC,EGCC,KRCC", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab_3), QApplication::translate("Pref::Ui::SubtitleWidget", "\353\213\244\354\244\221/\355\206\265\355\225\251\354\236\220\353\247\211", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Pref__Ui__SubtitleWidget);
    } // retranslateUi

};

} // namespace Ui
} // namespace Pref

namespace Pref {
namespace Ui {
namespace Ui {
    class SubtitleWidget: public Ui_SubtitleWidget {};
} // namespace Ui
} // namespace Ui
} // namespace Pref

#endif // UI_SUBTITLEWIDGET_H
