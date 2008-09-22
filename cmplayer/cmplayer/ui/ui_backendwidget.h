/********************************************************************************
** Form generated from reading ui file 'backendwidget.ui'
**
** Created: Mon Sep 22 14:28:01 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BACKENDWIDGET_H
#define UI_BACKENDWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Pref {
namespace Ui {

class Ui_BackendWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabs;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QComboBox *combo;
    QSpacerItem *horizontalSpacer;
    QTreeWidget *list;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Pref__Ui__BackendWidget)
    {
    if (Pref__Ui__BackendWidget->objectName().isEmpty())
        Pref__Ui__BackendWidget->setObjectName(QString::fromUtf8("Pref__Ui__BackendWidget"));
    Pref__Ui__BackendWidget->resize(289, 319);
    verticalLayout = new QVBoxLayout(Pref__Ui__BackendWidget);
    verticalLayout->setMargin(0);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    tabs = new QTabWidget(Pref__Ui__BackendWidget);
    tabs->setObjectName(QString::fromUtf8("tabs"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    verticalLayout_2 = new QVBoxLayout(tab);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    label_3 = new QLabel(tab);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setWordWrap(true);

    verticalLayout_2->addWidget(label_3);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label_2 = new QLabel(tab);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout->addWidget(label_2);

    combo = new QComboBox(tab);
    combo->setObjectName(QString::fromUtf8("combo"));
    combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    horizontalLayout->addWidget(combo);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);


    verticalLayout_2->addLayout(horizontalLayout);

    list = new QTreeWidget(tab);
    list->setObjectName(QString::fromUtf8("list"));

    verticalLayout_2->addWidget(list);

    verticalSpacer = new QSpacerItem(0, 3, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_2->addItem(verticalSpacer);

    tabs->addTab(tab, QString());

    verticalLayout->addWidget(tabs);


    retranslateUi(Pref__Ui__BackendWidget);

    tabs->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(Pref__Ui__BackendWidget);
    } // setupUi

    void retranslateUi(QWidget *Pref__Ui__BackendWidget)
    {
    Pref__Ui__BackendWidget->setWindowTitle(QApplication::translate("Pref::Ui::BackendWidget", "Form", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Pref::Ui::BackendWidget", "\353\217\231\354\230\201\354\203\201 \353\223\261\354\235\230 \354\236\254\354\203\235\354\235\204 \354\234\204\355\225\264 \354\235\264\354\232\251\355\225\240 \354\227\224\354\247\204(\353\260\261\354\227\224\353\223\234)\353\245\274 \354\204\240\355\203\235\355\225\264\354\243\274\354\204\270\354\232\224.", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Pref::Ui::BackendWidget", "\354\236\254\354\203\235 \354\227\224\354\247\204 \354\204\240\355\203\235", 0, QApplication::UnicodeUTF8));
    list->headerItem()->setText(0, QApplication::translate("Pref::Ui::BackendWidget", "\354\235\264\354\232\251\352\260\200\353\212\245\355\225\234 \354\236\254\354\203\235 \354\227\224\354\247\204 \353\252\251\353\241\235", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tab), QApplication::translate("Pref::Ui::BackendWidget", "\354\227\224\354\247\204 \354\204\240\355\203\235", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Pref__Ui__BackendWidget);
    } // retranslateUi

};

} // namespace Ui
} // namespace Pref

namespace Pref {
namespace Ui {
namespace Ui {
    class BackendWidget: public Ui_BackendWidget {};
} // namespace Ui
} // namespace Ui
} // namespace Pref

#endif // UI_BACKENDWIDGET_H
