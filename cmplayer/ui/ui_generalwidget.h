/********************************************************************************
** Form generated from reading ui file 'generalwidget.ui'
**
** Created: Tue Jul 15 21:22:19 2008
**      by: Qt User Interface Compiler version 4.4.0
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
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
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
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QHBoxLayout *hboxLayout;
    QLineEdit *mplayer_path_edit;
    QPushButton *find_mplayer_button;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_6;
    QHBoxLayout *hboxLayout1;
    QComboBox *auto_add_combo;
    QSpacerItem *spacerItem;
    QFrame *line;
    QCheckBox *add_all_titles_check;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *pause_check;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem1;
    QCheckBox *play_restored_check;
    QFrame *line_2;
    QCheckBox *restart_stopped_files_check;
    QFrame *line_3;
    QLabel *label_4;
    QLineEdit *additional_options_edit;
    QSpacerItem *spacerItem2;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *_2;
    QLabel *label_3;
    QComboBox *ao_combo;
    QSpacerItem *spacerItem3;
    QHBoxLayout *_3;
    QCheckBox *software_volume_check;
    QLabel *label_8;
    QDoubleSpinBox *amplification_spin;
    QHBoxLayout *_4;
    QCheckBox *reset_volume_check;
    QSpacerItem *spacerItem4;
    QHBoxLayout *_5;
    QLabel *label_7;
    QSlider *volume_slider;
    QCheckBox *auto_pitch_check;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hboxLayout3;
    QLabel *label_2;
    QComboBox *vo_combo;
    QSpacerItem *spacerItem5;
    QCheckBox *software_equalizer_check;
    QSpacerItem *spacerItem6;

    void setupUi(QWidget *Pref__Ui__GeneralWidget)
    {
    if (Pref__Ui__GeneralWidget->objectName().isEmpty())
        Pref__Ui__GeneralWidget->setObjectName(QString::fromUtf8("Pref__Ui__GeneralWidget"));
    Pref__Ui__GeneralWidget->resize(373, 473);
    vboxLayout = new QVBoxLayout(Pref__Ui__GeneralWidget);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tabs = new QTabWidget(Pref__Ui__GeneralWidget);
    tabs->setObjectName(QString::fromUtf8("tabs"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    tab->setGeometry(QRect(0, 0, 361, 435));
    verticalLayout_7 = new QVBoxLayout(tab);
    verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
    groupBox_3 = new QGroupBox(tab);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    verticalLayout_4 = new QVBoxLayout(groupBox_3);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    label = new QLabel(groupBox_3);
    label->setObjectName(QString::fromUtf8("label"));

    verticalLayout_4->addWidget(label);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    mplayer_path_edit = new QLineEdit(groupBox_3);
    mplayer_path_edit->setObjectName(QString::fromUtf8("mplayer_path_edit"));

    hboxLayout->addWidget(mplayer_path_edit);

    find_mplayer_button = new QPushButton(groupBox_3);
    find_mplayer_button->setObjectName(QString::fromUtf8("find_mplayer_button"));
    find_mplayer_button->setMaximumSize(QSize(40, 16777215));

    hboxLayout->addWidget(find_mplayer_button);


    verticalLayout_4->addLayout(hboxLayout);


    verticalLayout_7->addWidget(groupBox_3);

    groupBox_4 = new QGroupBox(tab);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    verticalLayout_5 = new QVBoxLayout(groupBox_4);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    label_6 = new QLabel(groupBox_4);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    verticalLayout_5->addWidget(label_6);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    auto_add_combo = new QComboBox(groupBox_4);
    auto_add_combo->setObjectName(QString::fromUtf8("auto_add_combo"));
    auto_add_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    hboxLayout1->addWidget(auto_add_combo);

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);


    verticalLayout_5->addLayout(hboxLayout1);

    line = new QFrame(groupBox_4);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    verticalLayout_5->addWidget(line);

    add_all_titles_check = new QCheckBox(groupBox_4);
    add_all_titles_check->setObjectName(QString::fromUtf8("add_all_titles_check"));

    verticalLayout_5->addWidget(add_all_titles_check);


    verticalLayout_7->addWidget(groupBox_4);

    groupBox_5 = new QGroupBox(tab);
    groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
    verticalLayout_6 = new QVBoxLayout(groupBox_5);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    pause_check = new QCheckBox(groupBox_5);
    pause_check->setObjectName(QString::fromUtf8("pause_check"));

    verticalLayout_6->addWidget(pause_check);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    spacerItem1 = new QSpacerItem(15, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem1);

    play_restored_check = new QCheckBox(groupBox_5);
    play_restored_check->setObjectName(QString::fromUtf8("play_restored_check"));
    play_restored_check->setEnabled(false);
    play_restored_check->setChecked(true);

    hboxLayout2->addWidget(play_restored_check);


    verticalLayout_6->addLayout(hboxLayout2);

    line_2 = new QFrame(groupBox_5);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    verticalLayout_6->addWidget(line_2);

    restart_stopped_files_check = new QCheckBox(groupBox_5);
    restart_stopped_files_check->setObjectName(QString::fromUtf8("restart_stopped_files_check"));

    verticalLayout_6->addWidget(restart_stopped_files_check);

    line_3 = new QFrame(groupBox_5);
    line_3->setObjectName(QString::fromUtf8("line_3"));
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);

    verticalLayout_6->addWidget(line_3);

    label_4 = new QLabel(groupBox_5);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    verticalLayout_6->addWidget(label_4);

    additional_options_edit = new QLineEdit(groupBox_5);
    additional_options_edit->setObjectName(QString::fromUtf8("additional_options_edit"));

    verticalLayout_6->addWidget(additional_options_edit);


    verticalLayout_7->addWidget(groupBox_5);

    spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_7->addItem(spacerItem2);

    tabs->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    tab_2->setGeometry(QRect(0, 0, 361, 435));
    verticalLayout_3 = new QVBoxLayout(tab_2);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    groupBox = new QGroupBox(tab_2);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    verticalLayout_2 = new QVBoxLayout(groupBox);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    _2 = new QHBoxLayout();
    _2->setObjectName(QString::fromUtf8("_2"));
    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    _2->addWidget(label_3);

    ao_combo = new QComboBox(groupBox);
    ao_combo->setObjectName(QString::fromUtf8("ao_combo"));
    ao_combo->setEditable(true);

    _2->addWidget(ao_combo);

    spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _2->addItem(spacerItem3);


    verticalLayout_2->addLayout(_2);

    _3 = new QHBoxLayout();
    _3->setObjectName(QString::fromUtf8("_3"));
    software_volume_check = new QCheckBox(groupBox);
    software_volume_check->setObjectName(QString::fromUtf8("software_volume_check"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(software_volume_check->sizePolicy().hasHeightForWidth());
    software_volume_check->setSizePolicy(sizePolicy);
    software_volume_check->setChecked(true);

    _3->addWidget(software_volume_check);

    label_8 = new QLabel(groupBox);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setEnabled(true);
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
    label_8->setSizePolicy(sizePolicy1);

    _3->addWidget(label_8);

    amplification_spin = new QDoubleSpinBox(groupBox);
    amplification_spin->setObjectName(QString::fromUtf8("amplification_spin"));
    amplification_spin->setAccelerated(true);
    amplification_spin->setDecimals(1);
    amplification_spin->setMinimum(10);
    amplification_spin->setMaximum(10000);
    amplification_spin->setValue(110);

    _3->addWidget(amplification_spin);


    verticalLayout_2->addLayout(_3);

    _4 = new QHBoxLayout();
    _4->setObjectName(QString::fromUtf8("_4"));
    reset_volume_check = new QCheckBox(groupBox);
    reset_volume_check->setObjectName(QString::fromUtf8("reset_volume_check"));
    reset_volume_check->setChecked(true);

    _4->addWidget(reset_volume_check);

    spacerItem4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _4->addItem(spacerItem4);

    _5 = new QHBoxLayout();
    _5->setObjectName(QString::fromUtf8("_5"));
    label_7 = new QLabel(groupBox);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    _5->addWidget(label_7);

    volume_slider = new QSlider(groupBox);
    volume_slider->setObjectName(QString::fromUtf8("volume_slider"));
    volume_slider->setMaximum(100);
    volume_slider->setOrientation(Qt::Horizontal);

    _5->addWidget(volume_slider);


    _4->addLayout(_5);


    verticalLayout_2->addLayout(_4);

    auto_pitch_check = new QCheckBox(groupBox);
    auto_pitch_check->setObjectName(QString::fromUtf8("auto_pitch_check"));

    verticalLayout_2->addWidget(auto_pitch_check);


    verticalLayout_3->addWidget(groupBox);

    groupBox_2 = new QGroupBox(tab_2);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    verticalLayout = new QVBoxLayout(groupBox_2);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    label_2 = new QLabel(groupBox_2);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    hboxLayout3->addWidget(label_2);

    vo_combo = new QComboBox(groupBox_2);
    vo_combo->setObjectName(QString::fromUtf8("vo_combo"));
    vo_combo->setEditable(true);

    hboxLayout3->addWidget(vo_combo);

    spacerItem5 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem5);


    verticalLayout->addLayout(hboxLayout3);

    software_equalizer_check = new QCheckBox(groupBox_2);
    software_equalizer_check->setObjectName(QString::fromUtf8("software_equalizer_check"));

    verticalLayout->addWidget(software_equalizer_check);

    label_2->raise();
    software_equalizer_check->raise();

    verticalLayout_3->addWidget(groupBox_2);

    spacerItem6 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_3->addItem(spacerItem6);

    tabs->addTab(tab_2, QString());
    groupBox->raise();
    groupBox_2->raise();

    vboxLayout->addWidget(tabs);


    retranslateUi(Pref__Ui__GeneralWidget);
    QObject::connect(pause_check, SIGNAL(toggled(bool)), play_restored_check, SLOT(setEnabled(bool)));

    tabs->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(Pref__Ui__GeneralWidget);
    } // setupUi

    void retranslateUi(QWidget *Pref__Ui__GeneralWidget)
    {
    Pref__Ui__GeneralWidget->setWindowTitle(QApplication::translate("Pref::Ui::GeneralWidget", "Form", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("Pref::Ui::GeneralWidget", "MPlayer \352\262\275\353\241\234", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\353\260\261\354\227\224\353\223\234\354\235\270 MPlayer\354\235\230 \352\262\275\353\241\234\353\245\274 \354\247\200\354\240\225\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    mplayer_path_edit->setText(QApplication::translate("Pref::Ui::GeneralWidget", "mplayer", 0, QApplication::UnicodeUTF8));
    find_mplayer_button->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\260\276\352\270\260", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235\353\252\251\353\241\235", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\355\214\214\354\235\274\354\235\204 \354\227\264 \353\225\214 \354\236\220\353\217\231\354\234\274\353\241\234 \353\213\244\354\235\214 \355\214\214\354\235\274\353\223\244\354\235\204 \354\236\254\354\203\235\353\252\251\353\241\235\354\227\220 \354\266\224\352\260\200\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    auto_add_combo->insertItems(0, QStringList()
     << QApplication::translate("Pref::Ui::GeneralWidget", "\353\271\204\354\212\267\355\225\234 \354\235\264\353\246\204\354\235\230 \355\214\214\354\235\274", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::GeneralWidget", "\352\260\231\354\235\200 \352\262\275\353\241\234\354\235\230 \353\252\250\353\223\240 \355\214\214\354\235\274", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Pref::Ui::GeneralWidget", "\353\213\244\353\245\270 \355\214\214\354\235\274\354\235\204 \354\266\224\352\260\200\355\225\230\354\247\200 \354\225\212\354\235\214", 0, QApplication::UnicodeUTF8)
    );
    add_all_titles_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "DVD\353\245\274 \354\227\264\353\225\214 \354\236\220\353\217\231\354\234\274\353\241\234 \353\252\250\353\223\240 \355\203\200\354\235\264\355\213\200\354\235\204 \354\236\254\354\203\235\353\252\251\353\241\235\354\227\220 \354\266\224\352\260\200\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    groupBox_5->setTitle(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235", 0, QApplication::UnicodeUTF8));
    pause_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\265\234\354\206\214\355\231\224\354\213\234\354\227\220 \354\235\274\354\213\234 \354\240\225\354\247\200\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    play_restored_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\353\263\265\352\267\200\354\213\234\354\227\220 \354\236\220\353\217\231\354\234\274\353\241\234 \354\236\254\354\203\235 \354\213\234\354\236\221\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    restart_stopped_files_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235\354\244\221\354\227\220 \354\244\221\354\247\200\353\220\234 \355\214\214\354\235\274\354\235\204 \353\213\244\354\213\234 \354\227\264\353\225\214 \354\244\221\354\247\200\353\220\234 \352\263\263\353\266\200\355\204\260 \354\236\254\354\203\235\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235\354\213\234 \355\225\255\354\203\201 \353\213\244\354\235\214\354\235\230 MPlayer \354\230\265\354\205\230\354\235\204 \354\266\224\352\260\200\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab), QApplication::translate("Pref::Ui::GeneralWidget", "\354\235\274\353\260\230", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("Pref::Ui::GeneralWidget", "\354\230\244\353\224\224\354\230\244", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\230\244\353\224\224\354\230\244 \354\266\234\353\240\245 \353\223\234\353\235\274\354\235\264\353\262\204", 0, QApplication::UnicodeUTF8));
    software_volume_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\206\214\355\224\204\355\212\270\354\233\250\354\226\264 \353\263\274\353\245\250 \354\202\254\354\232\251", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\353\263\274\353\245\250 \354\246\235\355\217\255\353\245\240", 0, QApplication::UnicodeUTF8));
    amplification_spin->setSuffix(QApplication::translate("Pref::Ui::GeneralWidget", " %", 0, QApplication::UnicodeUTF8));
    reset_volume_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235 \354\213\234\354\236\221\355\225\240 \353\225\214 \354\206\214\353\246\254\353\245\274 \353\213\244\354\213\234 \354\204\244\354\240\225\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("Pref::Ui::GeneralWidget", "0", 0, QApplication::UnicodeUTF8));
    auto_pitch_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\236\254\354\203\235 \354\206\215\353\217\204 \353\263\200\352\262\275\354\213\234 \354\236\220\353\217\231\354\234\274\353\241\234 \354\235\214\354\241\260 \354\241\260\354\240\210\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("Pref::Ui::GeneralWidget", "\353\271\204\353\224\224\354\230\244", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\353\271\204\353\224\224\354\230\244 \354\266\234\353\240\245 \353\223\234\353\235\274\354\235\264\353\262\204", 0, QApplication::UnicodeUTF8));
    software_equalizer_check->setText(QApplication::translate("Pref::Ui::GeneralWidget", "\354\206\214\355\224\204\355\212\270\354\233\250\354\226\264 \354\235\264\355\200\204\353\235\274\354\235\264\354\240\200 \354\202\254\354\232\251\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab_2), QApplication::translate("Pref::Ui::GeneralWidget", "AV", 0, QApplication::UnicodeUTF8));
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
