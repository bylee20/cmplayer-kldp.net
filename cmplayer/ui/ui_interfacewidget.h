/********************************************************************************
** Form generated from reading ui file 'interfacewidget.ui'
**
** Created: Thu Jul 17 00:18:21 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_INTERFACEWIDGET_H
#define UI_INTERFACEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Pref {
namespace Ui {

class Ui_InterfaceWidget
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout1;
    QLabel *label_5;
    QTreeWidget *shortcut_tree;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout2;
    QLabel *label_6;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QHBoxLayout *hboxLayout;
    QComboBox *double_action_combo;
    QSpacerItem *spacerItem;
    QLabel *label_11;
    QHBoxLayout *hboxLayout1;
    QComboBox *middle_action_combo;
    QSpacerItem *spacerItem1;
    QLabel *label_10;
    QHBoxLayout *hboxLayout2;
    QComboBox *scroll_action_combo;
    QSpacerItem *spacerItem2;
    QSpacerItem *verticalSpacer;
    QWidget *tab;
    QGridLayout *gridLayout1;
    QLabel *label_4;
    QLabel *label;
    QHBoxLayout *hboxLayout3;
    QSpinBox *seek_spin;
    QSpacerItem *spacerItem3;
    QLabel *label_2;
    QHBoxLayout *hboxLayout4;
    QSpinBox *seek_more_spin;
    QSpacerItem *spacerItem4;
    QLabel *label_9;
    QHBoxLayout *hboxLayout5;
    QSpinBox *seek_much_more_spin;
    QSpacerItem *spacerItem5;
    QLabel *label_3;
    QHBoxLayout *hboxLayout6;
    QSpinBox *volume_spin;
    QSpacerItem *spacerItem6;
    QSpacerItem *spacerItem7;

    void setupUi(QWidget *Pref__Ui__InterfaceWidget)
    {
    if (Pref__Ui__InterfaceWidget->objectName().isEmpty())
        Pref__Ui__InterfaceWidget->setObjectName(QString::fromUtf8("Pref__Ui__InterfaceWidget"));
    Pref__Ui__InterfaceWidget->resize(268, 259);
    vboxLayout = new QVBoxLayout(Pref__Ui__InterfaceWidget);
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tabWidget = new QTabWidget(Pref__Ui__InterfaceWidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    tab_2->setGeometry(QRect(0, 0, 256, 221));
    vboxLayout1 = new QVBoxLayout(tab_2);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    label_5 = new QLabel(tab_2);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    vboxLayout1->addWidget(label_5);

    shortcut_tree = new QTreeWidget(tab_2);
    shortcut_tree->setObjectName(QString::fromUtf8("shortcut_tree"));
    shortcut_tree->setAlternatingRowColors(true);

    vboxLayout1->addWidget(shortcut_tree);

    tabWidget->addTab(tab_2, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    tab_3->setGeometry(QRect(0, 0, 256, 221));
    vboxLayout2 = new QVBoxLayout(tab_3);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    label_6 = new QLabel(tab_3);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    vboxLayout2->addWidget(label_6);

    groupBox = new QGroupBox(tab_3);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    gridLayout = new QGridLayout(groupBox);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_7 = new QLabel(groupBox);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 0, 0, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    double_action_combo = new QComboBox(groupBox);
    double_action_combo->setObjectName(QString::fromUtf8("double_action_combo"));

    hboxLayout->addWidget(double_action_combo);

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);


    gridLayout->addLayout(hboxLayout, 0, 1, 1, 1);

    label_11 = new QLabel(groupBox);
    label_11->setObjectName(QString::fromUtf8("label_11"));

    gridLayout->addWidget(label_11, 1, 0, 1, 1);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    middle_action_combo = new QComboBox(groupBox);
    middle_action_combo->setObjectName(QString::fromUtf8("middle_action_combo"));

    hboxLayout1->addWidget(middle_action_combo);

    spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);


    gridLayout->addLayout(hboxLayout1, 1, 1, 1, 1);

    label_10 = new QLabel(groupBox);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    gridLayout->addWidget(label_10, 2, 0, 1, 1);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    scroll_action_combo = new QComboBox(groupBox);
    scroll_action_combo->setObjectName(QString::fromUtf8("scroll_action_combo"));

    hboxLayout2->addWidget(scroll_action_combo);

    spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem2);


    gridLayout->addLayout(hboxLayout2, 2, 1, 1, 1);


    vboxLayout2->addWidget(groupBox);

    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout2->addItem(verticalSpacer);

    tabWidget->addTab(tab_3, QString());
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    tab->setGeometry(QRect(0, 0, 256, 221));
    gridLayout1 = new QGridLayout(tab);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    label_4 = new QLabel(tab);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout1->addWidget(label_4, 0, 0, 1, 2);

    label = new QLabel(tab);
    label->setObjectName(QString::fromUtf8("label"));
    label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    gridLayout1->addWidget(label, 1, 0, 1, 1);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setSpacing(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    seek_spin = new QSpinBox(tab);
    seek_spin->setObjectName(QString::fromUtf8("seek_spin"));
    seek_spin->setMinimum(1);
    seek_spin->setMaximum(300);
    seek_spin->setValue(5);

    hboxLayout3->addWidget(seek_spin);

    spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout3->addItem(spacerItem3);


    gridLayout1->addLayout(hboxLayout3, 1, 1, 1, 1);

    label_2 = new QLabel(tab);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    gridLayout1->addWidget(label_2, 2, 0, 1, 1);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setSpacing(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    seek_more_spin = new QSpinBox(tab);
    seek_more_spin->setObjectName(QString::fromUtf8("seek_more_spin"));
    seek_more_spin->setMinimum(1);
    seek_more_spin->setMaximum(600);
    seek_more_spin->setValue(30);

    hboxLayout4->addWidget(seek_more_spin);

    spacerItem4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout4->addItem(spacerItem4);


    gridLayout1->addLayout(hboxLayout4, 2, 1, 1, 1);

    label_9 = new QLabel(tab);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout1->addWidget(label_9, 3, 0, 1, 1);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setSpacing(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    seek_much_more_spin = new QSpinBox(tab);
    seek_much_more_spin->setObjectName(QString::fromUtf8("seek_much_more_spin"));
    seek_much_more_spin->setMinimum(1);
    seek_much_more_spin->setMaximum(600);
    seek_much_more_spin->setValue(60);

    hboxLayout5->addWidget(seek_much_more_spin);

    spacerItem5 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem5);


    gridLayout1->addLayout(hboxLayout5, 3, 1, 1, 1);

    label_3 = new QLabel(tab);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    gridLayout1->addWidget(label_3, 4, 0, 1, 1);

    hboxLayout6 = new QHBoxLayout();
    hboxLayout6->setSpacing(0);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    volume_spin = new QSpinBox(tab);
    volume_spin->setObjectName(QString::fromUtf8("volume_spin"));
    volume_spin->setMinimum(1);

    hboxLayout6->addWidget(volume_spin);

    spacerItem6 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout6->addItem(spacerItem6);


    gridLayout1->addLayout(hboxLayout6, 4, 1, 1, 1);

    spacerItem7 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem7, 5, 0, 1, 2);

    tabWidget->addTab(tab, QString());

    vboxLayout->addWidget(tabWidget);


    retranslateUi(Pref__Ui__InterfaceWidget);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(Pref__Ui__InterfaceWidget);
    } // setupUi

    void retranslateUi(QWidget *Pref__Ui__InterfaceWidget)
    {
    Pref__Ui__InterfaceWidget->setWindowTitle(QApplication::translate("Pref::Ui::InterfaceWidget", "Form", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\355\202\244\353\263\264\353\223\234\353\245\274 \354\235\264\354\232\251\355\225\234 \353\213\250\354\266\225\355\202\244\353\245\274 \354\204\244\354\240\225\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    shortcut_tree->headerItem()->setText(0, QApplication::translate("Pref::Ui::InterfaceWidget", "\354\204\244\353\252\205", 0, QApplication::UnicodeUTF8));
    shortcut_tree->headerItem()->setText(1, QApplication::translate("Pref::Ui::InterfaceWidget", "\353\213\250\354\266\225\355\202\244", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Pref::Ui::InterfaceWidget", "\355\202\244\353\263\264\353\223\234", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\353\247\210\354\232\260\354\212\244\353\245\274 \354\235\264\354\232\251\355\225\234 \354\241\260\354\236\221\354\235\204 \354\204\244\354\240\225\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("Pref::Ui::InterfaceWidget", "\355\226\211\353\217\231 \354\204\244\354\240\225", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\354\231\274\354\252\275 \353\262\204\355\212\274 \353\215\224\353\270\224 \355\201\264\353\246\255\354\213\234", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\352\260\200\354\232\264\353\215\260 \353\262\204\355\212\274 \355\201\264\353\246\255\354\213\234", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\355\234\240 \354\212\244\355\201\254\353\241\244\354\213\234", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Pref::Ui::InterfaceWidget", "\353\247\210\354\232\260\354\212\244", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\353\213\250\352\263\204\353\263\204\353\241\234 \354\241\260\354\236\221\355\225\230\353\212\224 \352\260\222\354\235\204 \354\204\244\354\240\225\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\354\225\236\354\234\274\353\241\234/\353\222\244\353\241\234", 0, QApplication::UnicodeUTF8));
    seek_spin->setSuffix(QApplication::translate("Pref::Ui::InterfaceWidget", " \354\264\210", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\353\247\216\354\235\264 \354\225\236\354\234\274\353\241\234/\353\222\244\353\241\234", 0, QApplication::UnicodeUTF8));
    seek_more_spin->setSuffix(QApplication::translate("Pref::Ui::InterfaceWidget", " \354\264\210", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\353\215\224 \353\247\216\354\235\264 \354\225\236\354\234\274\353\241\234/\353\222\244\353\241\234", 0, QApplication::UnicodeUTF8));
    seek_much_more_spin->setSuffix(QApplication::translate("Pref::Ui::InterfaceWidget", " \354\264\210", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Pref::Ui::InterfaceWidget", "\354\206\214\353\246\254 \355\201\254\352\262\214/\354\236\221\352\262\214", 0, QApplication::UnicodeUTF8));
    volume_spin->setSuffix(QApplication::translate("Pref::Ui::InterfaceWidget", " %", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Pref::Ui::InterfaceWidget", "\354\241\260\354\236\221 \353\213\250\352\263\204", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Pref__Ui__InterfaceWidget);
    } // retranslateUi

};

} // namespace Ui
} // namespace Pref

namespace Pref {
namespace Ui {
namespace Ui {
    class InterfaceWidget: public Ui_InterfaceWidget {};
} // namespace Ui
} // namespace Ui
} // namespace Pref

#endif // UI_INTERFACEWIDGET_H
