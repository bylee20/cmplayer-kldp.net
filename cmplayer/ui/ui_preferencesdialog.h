/********************************************************************************
** Form generated from reading ui file 'preferencesdialog.ui'
**
** Created: Sat Jul 12 15:17:22 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace Pref {
namespace Ui {

class Ui_PreferencesDialog
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QListWidget *list;
    QStackedWidget *stack;
    QWidget *page;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *ok_button;
    QPushButton *cancel_button;

    void setupUi(QDialog *Pref__Ui__PreferencesDialog)
    {
    if (Pref__Ui__PreferencesDialog->objectName().isEmpty())
        Pref__Ui__PreferencesDialog->setObjectName(QString::fromUtf8("Pref__Ui__PreferencesDialog"));
    Pref__Ui__PreferencesDialog->resize(426, 290);
    vboxLayout = new QVBoxLayout(Pref__Ui__PreferencesDialog);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    list = new QListWidget(Pref__Ui__PreferencesDialog);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/img/games-config-background.png")), QIcon::Normal, QIcon::Off);
    QListWidgetItem *__listItem = new QListWidgetItem(list);
    __listItem->setIcon(icon);
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8(":/img/character-set.png")), QIcon::Normal, QIcon::Off);
    QListWidgetItem *__listItem1 = new QListWidgetItem(list);
    __listItem1->setIcon(icon1);
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8(":/img/preferences-desktop-keyboard.png")), QIcon::Normal, QIcon::Off);
    QListWidgetItem *__listItem2 = new QListWidgetItem(list);
    __listItem2->setIcon(icon2);
    list->setObjectName(QString::fromUtf8("list"));
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(list->sizePolicy().hasHeightForWidth());
    list->setSizePolicy(sizePolicy);
    list->setMaximumSize(QSize(100, 16777215));
    list->setDragDropMode(QAbstractItemView::NoDragDrop);
    list->setIconSize(QSize(48, 48));
    list->setMovement(QListView::Static);
    list->setSpacing(0);
    list->setGridSize(QSize(94, 70));
    list->setViewMode(QListView::IconMode);
    list->setWordWrap(false);

    hboxLayout->addWidget(list);

    stack = new QStackedWidget(Pref__Ui__PreferencesDialog);
    stack->setObjectName(QString::fromUtf8("stack"));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    page->setGeometry(QRect(0, 0, 312, 246));
    stack->addWidget(page);

    hboxLayout->addWidget(stack);


    vboxLayout->addLayout(hboxLayout);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    ok_button = new QPushButton(Pref__Ui__PreferencesDialog);
    ok_button->setObjectName(QString::fromUtf8("ok_button"));
    ok_button->setDefault(true);

    hboxLayout1->addWidget(ok_button);

    cancel_button = new QPushButton(Pref__Ui__PreferencesDialog);
    cancel_button->setObjectName(QString::fromUtf8("cancel_button"));

    hboxLayout1->addWidget(cancel_button);


    vboxLayout->addLayout(hboxLayout1);


    retranslateUi(Pref__Ui__PreferencesDialog);
    QObject::connect(cancel_button, SIGNAL(clicked()), Pref__Ui__PreferencesDialog, SLOT(reject()));
    QObject::connect(list, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));

    stack->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(Pref__Ui__PreferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *Pref__Ui__PreferencesDialog)
    {
    Pref__Ui__PreferencesDialog->setWindowTitle(QApplication::translate("Pref::Ui::PreferencesDialog", "Dialog", 0, QApplication::UnicodeUTF8));

    const bool __sortingEnabled = list->isSortingEnabled();
    list->setSortingEnabled(false);
    list->item(0)->setText(QApplication::translate("Pref::Ui::PreferencesDialog", "\354\235\274\353\260\230", 0, QApplication::UnicodeUTF8));
    list->item(1)->setText(QApplication::translate("Pref::Ui::PreferencesDialog", "\354\236\220\353\247\211", 0, QApplication::UnicodeUTF8));
    list->item(2)->setText(QApplication::translate("Pref::Ui::PreferencesDialog", "\354\235\270\355\204\260\355\216\230\354\235\264\354\212\244", 0, QApplication::UnicodeUTF8));

    list->setSortingEnabled(__sortingEnabled);
    ok_button->setText(QApplication::translate("Pref::Ui::PreferencesDialog", "\355\231\225\354\235\270(&O)", 0, QApplication::UnicodeUTF8));
    cancel_button->setText(QApplication::translate("Pref::Ui::PreferencesDialog", "\354\267\250\354\206\214(&C)", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Pref__Ui__PreferencesDialog);
    } // retranslateUi

};

} // namespace Ui
} // namespace Pref

namespace Pref {
namespace Ui {
namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui
} // namespace Ui
} // namespace Pref

#endif // UI_PREFERENCESDIALOG_H
