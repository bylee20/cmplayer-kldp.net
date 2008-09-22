/********************************************************************************
** Form generated from reading ui file 'getshortcutdialog.ui'
**
** Created: Mon Sep 22 14:28:01 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GETSHORTCUTDIALOG_H
#define UI_GETSHORTCUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

namespace Pref {
namespace Ui {

class Ui_GetShortcutDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLineEdit *key_edit;
    QHBoxLayout *hboxLayout;
    QHBoxLayout *hboxLayout1;
    QPushButton *begin_button;
    QSpacerItem *spacerItem;
    QPushButton *ok_button;
    QPushButton *cancel_button;

    void setupUi(QDialog *Pref__Ui__GetShortcutDialog)
    {
    if (Pref__Ui__GetShortcutDialog->objectName().isEmpty())
        Pref__Ui__GetShortcutDialog->setObjectName(QString::fromUtf8("Pref__Ui__GetShortcutDialog"));
    Pref__Ui__GetShortcutDialog->resize(267, 77);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(Pref__Ui__GetShortcutDialog->sizePolicy().hasHeightForWidth());
    Pref__Ui__GetShortcutDialog->setSizePolicy(sizePolicy);
    vboxLayout = new QVBoxLayout(Pref__Ui__GetShortcutDialog);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    key_edit = new QLineEdit(Pref__Ui__GetShortcutDialog);
    key_edit->setObjectName(QString::fromUtf8("key_edit"));
    key_edit->setEnabled(true);
    key_edit->setReadOnly(true);

    vboxLayout->addWidget(key_edit);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    begin_button = new QPushButton(Pref__Ui__GetShortcutDialog);
    begin_button->setObjectName(QString::fromUtf8("begin_button"));
    begin_button->setFocusPolicy(Qt::NoFocus);
    begin_button->setCheckable(true);
    begin_button->setDefault(false);

    hboxLayout1->addWidget(begin_button);

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);


    hboxLayout->addLayout(hboxLayout1);

    ok_button = new QPushButton(Pref__Ui__GetShortcutDialog);
    ok_button->setObjectName(QString::fromUtf8("ok_button"));
    ok_button->setFocusPolicy(Qt::NoFocus);

    hboxLayout->addWidget(ok_button);

    cancel_button = new QPushButton(Pref__Ui__GetShortcutDialog);
    cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
    cancel_button->setAutoDefault(false);

    hboxLayout->addWidget(cancel_button);


    vboxLayout->addLayout(hboxLayout);


    retranslateUi(Pref__Ui__GetShortcutDialog);
    QObject::connect(ok_button, SIGNAL(clicked()), Pref__Ui__GetShortcutDialog, SLOT(accept()));
    QObject::connect(cancel_button, SIGNAL(clicked()), Pref__Ui__GetShortcutDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Pref__Ui__GetShortcutDialog);
    } // setupUi

    void retranslateUi(QDialog *Pref__Ui__GetShortcutDialog)
    {
    Pref__Ui__GetShortcutDialog->setWindowTitle(QApplication::translate("Pref::Ui::GetShortcutDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    begin_button->setText(QApplication::translate("Pref::Ui::GetShortcutDialog", "\354\236\205\353\240\245 \354\213\234\354\236\221", 0, QApplication::UnicodeUTF8));
    ok_button->setText(QApplication::translate("Pref::Ui::GetShortcutDialog", "\355\231\225\354\235\270", 0, QApplication::UnicodeUTF8));
    cancel_button->setText(QApplication::translate("Pref::Ui::GetShortcutDialog", "\354\267\250\354\206\214", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Pref__Ui__GetShortcutDialog);
    } // retranslateUi

};

} // namespace Ui
} // namespace Pref

namespace Pref {
namespace Ui {
namespace Ui {
    class GetShortcutDialog: public Ui_GetShortcutDialog {};
} // namespace Ui
} // namespace Ui
} // namespace Pref

#endif // UI_GETSHORTCUTDIALOG_H
