/********************************************************************************
** Form generated from reading ui file 'subtitlewidget.ui'
**
** Created: Thu Jul 17 00:18:20 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SUBTITLEWIDGET_H
#define UI_SUBTITLEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QGridLayout>
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

namespace Pref {
namespace Ui {

class Ui_SubtitleWidget
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *tabs;
    QWidget *tab;
    QVBoxLayout *vboxLayout1;
    QGroupBox *auto_load_group;
    QVBoxLayout *vboxLayout2;
    QLabel *label;
    QHBoxLayout *hboxLayout;
    QComboBox *auto_load_combo;
    QSpacerItem *spacerItem;
    QLabel *label_2;
    QHBoxLayout *hboxLayout1;
    QComboBox *auto_select_combo;
    QSpacerItem *spacerItem1;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout3;
    QHBoxLayout *hboxLayout2;
    QLabel *label_10;
    QSpacerItem *spacerItem2;
    QSpinBox *init_pos_spin;
    QSlider *horizontalSlider;
    QHBoxLayout *hboxLayout3;
    QLabel *label_13;
    QSpacerItem *spacerItem3;
    QLabel *label_14;
    QCheckBox *display_on_margin_check;
    QVBoxLayout *vboxLayout4;
    QHBoxLayout *hboxLayout4;
    QLabel *label_5;
    QHBoxLayout *hboxLayout5;
    QComboBox *encoding_combo;
    QSpacerItem *spacerItem4;
    QSpacerItem *spacerItem5;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout5;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QHBoxLayout *hboxLayout6;
    QFontComboBox *font_family_combo;
    QSpacerItem *spacerItem6;
    QLabel *label_9;
    QHBoxLayout *hboxLayout7;
    QDoubleSpinBox *font_size_spin;
    QSpacerItem *spacerItem7;
    QLabel *label_7;
    QHBoxLayout *hboxLayout8;
    QComboBox *auto_scale_combo;
    QSpacerItem *spacerItem8;
    QSpacerItem *spacerItem9;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout6;
    QLabel *label_24;
    QLineEdit *priority_edit;
    QSpacerItem *spacerItem10;

    void setupUi(QWidget *Pref__Ui__SubtitleWidget)
    {
    if (Pref__Ui__SubtitleWidget->objectName().isEmpty())
        Pref__Ui__SubtitleWidget->setObjectName(QString::fromUtf8("Pref__Ui__SubtitleWidget"));
    Pref__Ui__SubtitleWidget->resize(362, 391);
    vboxLayout = new QVBoxLayout(Pref__Ui__SubtitleWidget);
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tabs = new QTabWidget(Pref__Ui__SubtitleWidget);
    tabs->setObjectName(QString::fromUtf8("tabs"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    tab->setGeometry(QRect(0, 0, 350, 353));
    vboxLayout1 = new QVBoxLayout(tab);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    auto_load_group = new QGroupBox(tab);
    auto_load_group->setObjectName(QString::fromUtf8("auto_load_group"));
    auto_load_group->setCheckable(false);
    vboxLayout2 = new QVBoxLayout(auto_load_group);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    label = new QLabel(auto_load_group);
    label->setObjectName(QString::fromUtf8("label"));

    vboxLayout2->addWidget(label);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    auto_load_combo = new QComboBox(auto_load_group);
    auto_load_combo->setObjectName(QString::fromUtf8("auto_load_combo"));
    auto_load_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    hboxLayout->addWidget(auto_load_combo);

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);


    vboxLayout2->addLayout(hboxLayout);

    label_2 = new QLabel(auto_load_group);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    vboxLayout2->addWidget(label_2);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    auto_select_combo = new QComboBox(auto_load_group);
    auto_select_combo->setObjectName(QString::fromUtf8("auto_select_combo"));
    auto_select_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    hboxLayout1->addWidget(auto_select_combo);

    spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);


    vboxLayout2->addLayout(hboxLayout1);


    vboxLayout1->addWidget(auto_load_group);

    groupBox = new QGroupBox(tab);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout3 = new QVBoxLayout(groupBox);
    vboxLayout3->setSpacing(4);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    label_10 = new QLabel(groupBox);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    hboxLayout2->addWidget(label_10);

    spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem2);

    init_pos_spin = new QSpinBox(groupBox);
    init_pos_spin->setObjectName(QString::fromUtf8("init_pos_spin"));
    init_pos_spin->setMaximum(100);
    init_pos_spin->setValue(100);

    hboxLayout2->addWidget(init_pos_spin);


    vboxLayout3->addLayout(hboxLayout2);

    horizontalSlider = new QSlider(groupBox);
    horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    horizontalSlider->setMaximum(100);
    horizontalSlider->setValue(100);
    horizontalSlider->setTracking(true);
    horizontalSlider->setOrientation(Qt::Horizontal);
    horizontalSlider->setTickPosition(QSlider::NoTicks);

    vboxLayout3->addWidget(horizontalSlider);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    label_13 = new QLabel(groupBox);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    hboxLayout3->addWidget(label_13);

    spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem3);

    label_14 = new QLabel(groupBox);
    label_14->setObjectName(QString::fromUtf8("label_14"));

    hboxLayout3->addWidget(label_14);


    vboxLayout3->addLayout(hboxLayout3);


    vboxLayout1->addWidget(groupBox);

    display_on_margin_check = new QCheckBox(tab);
    display_on_margin_check->setObjectName(QString::fromUtf8("display_on_margin_check"));

    vboxLayout1->addWidget(display_on_margin_check);

    vboxLayout4 = new QVBoxLayout();
    vboxLayout4->setSpacing(0);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    label_5 = new QLabel(tab);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    hboxLayout4->addWidget(label_5);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setSpacing(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    encoding_combo = new QComboBox(tab);
    encoding_combo->setObjectName(QString::fromUtf8("encoding_combo"));
    encoding_combo->setEditable(true);
    encoding_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    hboxLayout5->addWidget(encoding_combo);

    spacerItem4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem4);


    hboxLayout4->addLayout(hboxLayout5);


    vboxLayout4->addLayout(hboxLayout4);

    spacerItem5 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout4->addItem(spacerItem5);


    vboxLayout1->addLayout(vboxLayout4);

    tabs->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    tab_2->setGeometry(QRect(0, 0, 350, 353));
    vboxLayout5 = new QVBoxLayout(tab_2);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_4 = new QLabel(tab_2);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 0, 0, 1, 1);

    hboxLayout6 = new QHBoxLayout();
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    font_family_combo = new QFontComboBox(tab_2);
    font_family_combo->setObjectName(QString::fromUtf8("font_family_combo"));

    hboxLayout6->addWidget(font_family_combo);

    spacerItem6 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout6->addItem(spacerItem6);


    gridLayout->addLayout(hboxLayout6, 0, 1, 1, 1);

    label_9 = new QLabel(tab_2);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout->addWidget(label_9, 1, 0, 1, 1);

    hboxLayout7 = new QHBoxLayout();
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    font_size_spin = new QDoubleSpinBox(tab_2);
    font_size_spin->setObjectName(QString::fromUtf8("font_size_spin"));
    font_size_spin->setAccelerated(true);
    font_size_spin->setDecimals(1);
    font_size_spin->setValue(5);

    hboxLayout7->addWidget(font_size_spin);

    spacerItem7 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout7->addItem(spacerItem7);


    gridLayout->addLayout(hboxLayout7, 1, 1, 1, 1);

    label_7 = new QLabel(tab_2);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 2, 0, 1, 1);

    hboxLayout8 = new QHBoxLayout();
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    auto_scale_combo = new QComboBox(tab_2);
    auto_scale_combo->setObjectName(QString::fromUtf8("auto_scale_combo"));

    hboxLayout8->addWidget(auto_scale_combo);

    spacerItem8 = new QSpacerItem(55, 16, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout8->addItem(spacerItem8);


    gridLayout->addLayout(hboxLayout8, 2, 1, 1, 1);


    vboxLayout5->addLayout(gridLayout);

    spacerItem9 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout5->addItem(spacerItem9);

    tabs->addTab(tab_2, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    tab_3->setGeometry(QRect(0, 0, 350, 353));
    vboxLayout6 = new QVBoxLayout(tab_3);
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    label_24 = new QLabel(tab_3);
    label_24->setObjectName(QString::fromUtf8("label_24"));
    label_24->setWordWrap(true);

    vboxLayout6->addWidget(label_24);

    priority_edit = new QLineEdit(tab_3);
    priority_edit->setObjectName(QString::fromUtf8("priority_edit"));

    vboxLayout6->addWidget(priority_edit);

    spacerItem10 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout6->addItem(spacerItem10);

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
    auto_load_group->setTitle(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\220\353\217\231\354\234\274\353\241\234 \353\266\210\353\237\254\354\230\244\352\270\260", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274 \354\236\254\354\203\235\354\213\234\354\227\220 \353\213\244\354\235\214\354\227\220 \355\225\264\353\213\271\355\225\230\353\212\224 \354\236\220\353\247\211\354\235\204 \354\236\220\353\217\231\354\234\274\353\241\234 \353\266\210\353\237\254\354\230\265\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    auto_load_combo->insertItems(0, QStringList()
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\220\353\247\211\354\235\204 \354\236\220\353\217\231\354\234\274\353\241\234 \353\266\210\353\237\254\354\230\244\354\247\200 \354\225\212\354\235\214", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274\352\263\274 \352\260\231\354\235\200 \354\235\264\353\246\204\354\235\230 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274 \354\235\264\353\246\204\354\235\204 \355\217\254\355\225\250\355\225\230\353\212\224 \353\252\250\353\223\240 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274\352\263\274 \352\260\231\354\235\200 \352\262\275\353\241\234\354\227\220 \354\236\210\353\212\224 \353\252\250\353\223\240 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\353\266\210\353\237\254\354\230\250 \354\236\220\353\247\211 \354\244\221 \353\213\244\354\235\214\354\227\220 \355\225\264\353\213\271\355\225\230\353\212\224 \354\236\220\353\247\211\354\235\204 \354\236\220\353\217\231\354\234\274\353\241\234 \354\204\240\355\203\235\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    auto_select_combo->insertItems(0, QStringList()
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\352\260\200\354\236\245 \354\262\253\353\262\210\354\247\270 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\214\214\354\235\274\352\263\274 \352\260\231\354\235\200 \354\235\264\353\246\204\354\235\230 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\353\266\210\353\237\254\354\230\250 \353\252\250\353\223\240 \354\236\220\353\247\211", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\353\252\250\353\223\240 \353\213\244\353\245\270 \354\226\270\354\226\264", 0, QApplication::UnicodeUTF8)
    );
    groupBox->setTitle(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\221\234\354\213\234 \354\234\204\354\271\230", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\236\254\354\203\235 \354\213\234\354\236\221\354\213\234\354\235\230 \354\236\220\353\247\211 \355\221\234\354\213\234 \354\234\204\354\271\230", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\231\224\353\251\264 \354\234\204\354\252\275", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\231\224\353\251\264 \354\225\204\353\236\230\354\252\275", 0, QApplication::UnicodeUTF8));
    display_on_margin_check->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\240\204\354\262\264\355\231\224\353\251\264\354\213\234 \354\203\201\355\225\230\353\213\250 \354\227\254\353\260\261\354\227\220\353\217\204 \354\236\220\353\247\211\354\235\204 \355\221\234\354\213\234\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\354\235\270\354\275\224\353\224\251: ", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab), QApplication::translate("Pref::Ui::SubtitleWidget", "\354\235\274\353\260\230", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\352\270\200\352\274\264 \354\204\240\355\203\235", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\352\270\260\353\263\270 \355\201\254\352\270\260", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("Pref::Ui::SubtitleWidget", "\355\201\254\352\270\260 \353\247\236\354\266\244", 0, QApplication::UnicodeUTF8));
    auto_scale_combo->insertItems(0, QStringList()
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\353\201\204\352\270\260", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\231\224\353\251\264 \354\204\270\353\241\234 \352\270\270\354\235\264\354\227\220 \353\247\236\354\266\224\352\270\260", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\231\224\353\251\264 \352\260\200\353\241\234 \352\270\270\354\235\264\354\227\220 \353\247\236\354\266\224\352\270\260", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::SubtitleWidget", "\355\231\224\353\251\264 \353\214\200\352\260\201\354\204\240 \352\270\270\354\235\264\354\227\220 \353\247\236\354\266\224\352\270\260", 0, QApplication::UnicodeUTF8)
    );
    tabs->setTabText(tabs->indexOf(tab_2), QApplication::translate("Pref::Ui::SubtitleWidget", "\352\270\200\352\274\264", 0, QApplication::UnicodeUTF8));
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
