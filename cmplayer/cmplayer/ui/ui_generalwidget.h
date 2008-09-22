/********************************************************************************
** Form generated from reading ui file 'generalwidget.ui'
**
** Created: Mon Sep 22 14:28:01 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GENERALWIDGET_H
#define UI_GENERALWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Pref {
namespace Ui {

class Ui_GeneralWidget
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *tabs;
    QWidget *tab;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_6;
    QHBoxLayout *hboxLayout;
    QComboBox *auto_add_combo;
    QSpacerItem *spacerItem;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *pause_check;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem1;
    QCheckBox *play_restored_check;
    QFrame *line_2;
    QCheckBox *restart_stopped_files_check;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *reset_volume_check;
    QSpacerItem *spacer;
    QLabel *label_9;
    QSlider *volume_slider;
    QSpacerItem *spacerItem2;

    void setupUi(QWidget *Pref__Ui__GeneralWidget)
    {
    if (Pref__Ui__GeneralWidget->objectName().isEmpty())
        Pref__Ui__GeneralWidget->setObjectName(QString::fromUtf8("Pref__Ui__GeneralWidget"));
    Pref__Ui__GeneralWidget->resize(397, 322);
    vboxLayout = new QVBoxLayout(Pref__Ui__GeneralWidget);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tabs = new QTabWidget(Pref__Ui__GeneralWidget);
    tabs->setObjectName(QString::fromUtf8("tabs"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    verticalLayout_6 = new QVBoxLayout(tab);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    groupBox_4 = new QGroupBox(tab);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    verticalLayout_5 = new QVBoxLayout(groupBox_4);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    label_6 = new QLabel(groupBox_4);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    verticalLayout_5->addWidget(label_6);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    auto_add_combo = new QComboBox(groupBox_4);
    auto_add_combo->setObjectName(QString::fromUtf8("auto_add_combo"));
    auto_add_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    hboxLayout->addWidget(auto_add_combo);

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);


    verticalLayout_5->addLayout(hboxLayout);


    verticalLayout_6->addWidget(groupBox_4);

    groupBox_5 = new QGroupBox(tab);
    groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
    verticalLayout_4 = new QVBoxLayout(groupBox_5);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    pause_check = new QCheckBox(groupBox_5);
    pause_check->setObjectName(QString::fromUtf8("pause_check"));

    verticalLayout_4->addWidget(pause_check);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem1 = new QSpacerItem(15, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);

    play_restored_check = new QCheckBox(groupBox_5);
    play_restored_check->setObjectName(QString::fromUtf8("play_restored_check"));
    play_restored_check->setEnabled(false);
    play_restored_check->setChecked(true);

    hboxLayout1->addWidget(play_restored_check);


    verticalLayout_4->addLayout(hboxLayout1);

    line_2 = new QFrame(groupBox_5);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    verticalLayout_4->addWidget(line_2);

    restart_stopped_files_check = new QCheckBox(groupBox_5);
    restart_stopped_files_check->setObjectName(QString::fromUtf8("restart_stopped_files_check"));

    verticalLayout_4->addWidget(restart_stopped_files_check);

    line = new QFrame(groupBox_5);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    verticalLayout_4->addWidget(line);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    reset_volume_check = new QCheckBox(groupBox_5);
    reset_volume_check->setObjectName(QString::fromUtf8("reset_volume_check"));
    reset_volume_check->setChecked(true);

    horizontalLayout_2->addWidget(reset_volume_check);

    spacer = new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(spacer);

    label_9 = new QLabel(groupBox_5);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    horizontalLayout_2->addWidget(label_9);

    volume_slider = new QSlider(groupBox_5);
    volume_slider->setObjectName(QString::fromUtf8("volume_slider"));
    volume_slider->setMaximum(100);
    volume_slider->setOrientation(Qt::Horizontal);

    horizontalLayout_2->addWidget(volume_slider);


    verticalLayout_4->addLayout(horizontalLayout_2);


    verticalLayout_6->addWidget(groupBox_5);

    spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_6->addItem(spacerItem2);

    tabs->addTab(tab, QString());

    vboxLayout->addWidget(tabs);


    retranslateUi(Pref__Ui__GeneralWidget);
    QObject::connect(pause_check, SIGNAL(toggled(bool)), play_restored_check, SLOT(setEnabled(bool)));

    tabs->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(Pref__Ui__GeneralWidget);
    } // setupUi

    void retranslateUi(QWidget *Pref__Ui__GeneralWidget)
    {
    Pref__Ui__GeneralWidget->setWindowTitle(QApplication::translate("Pref::Ui::GeneralWidget", "Form", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235\353\252\251\353\241\235", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\355\214\214\354\235\274\354\235\204 \354\227\264 \353\225\214 \354\236\220\353\217\231\354\234\274\353\241\234 \353\213\244\354\235\214 \355\214\214\354\235\274\353\223\244\354\235\204 \354\236\254\354\203\235\353\252\251\353\241\235\354\227\220 \354\266\224\352\260\200\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    auto_add_combo->clear();
    auto_add_combo->insertItems(0, QStringList()
     << QApplication::translate("Pref::Ui::GeneralWidget", "\353\271\204\354\212\267\355\225\234 \354\235\264\353\246\204\354\235\230 \355\214\214\354\235\274", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::GeneralWidget", "\352\260\231\354\235\200 \352\262\275\353\241\234\354\235\230 \353\252\250\353\223\240 \355\214\214\354\235\274", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::GeneralWidget", "\353\213\244\353\245\270 \355\214\214\354\235\274\354\235\204 \354\266\224\352\260\200\355\225\230\354\247\200 \354\225\212\354\235\214", 0, QApplication::UnicodeUTF8)
    );
    groupBox_5->setTitle(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235", 0, QApplication::UnicodeUTF8));
    pause_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\265\234\354\206\214\355\231\224\354\213\234\354\227\220 \354\235\274\354\213\234 \354\240\225\354\247\200\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    play_restored_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\353\263\265\352\267\200\354\213\234\354\227\220 \354\236\220\353\217\231\354\234\274\353\241\234 \354\236\254\354\203\235 \354\213\234\354\236\221\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    restart_stopped_files_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235\354\244\221\354\227\220 \354\244\221\354\247\200\353\220\234 \355\214\214\354\235\274\354\235\204 \353\213\244\354\213\234 \354\227\264\353\225\214 \354\244\221\354\247\200\353\220\234 \352\263\263\353\266\200\355\204\260 \354\236\254\354\203\235\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    reset_volume_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235 \354\213\234\354\236\221\355\225\240 \353\225\214 \354\206\214\353\246\254\353\245\274 \353\213\244\354\213\234 \354\204\244\354\240\225\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("Pref::Ui::GeneralWidget", "0", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab), QApplication::translate("Pref::Ui::GeneralWidget", "\354\235\274\353\260\230", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Pref__Ui__GeneralWidget);
    } // retranslateUi

};

} // namespace Ui
} // namespace Pref

namespace Pref {
namespace Ui {
namespace Ui {
    class GeneralWidget: public Ui_GeneralWidget {};
} // namespace Ui
} // namespace Ui
} // namespace Pref

#endif // UI_GENERALWIDGET_H
