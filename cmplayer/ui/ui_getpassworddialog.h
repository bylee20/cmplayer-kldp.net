/********************************************************************************
** Form generated from reading ui file 'getpassworddialog.ui'
**
** Created: Sun Jun 1 04:02:41 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GETPASSWORDDIALOG_H
#define UI_GETPASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

namespace Ui {

class Ui_GetPasswordDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *hboxLayout;
    QLineEdit *password_edit;
    QPushButton *ok_button;
    QPushButton *cancel_button;

    void setupUi(QDialog *Ui__GetPasswordDialog)
    {
    if (Ui__GetPasswordDialog->objectName().isEmpty())
        Ui__GetPasswordDialog->setObjectName(QString::fromUtf8("Ui__GetPasswordDialog"));
    Ui__GetPasswordDialog->resize(283, 109);
    vboxLayout = new QVBoxLayout(Ui__GetPasswordDialog);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    label = new QLabel(Ui__GetPasswordDialog);
    label->setObjectName(QString::fromUtf8("label"));
    label->setWordWrap(true);

    vboxLayout->addWidget(label);

    label_2 = new QLabel(Ui__GetPasswordDialog);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setStyleSheet(QString::fromUtf8("color: red;"));

    vboxLayout->addWidget(label_2);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    password_edit = new QLineEdit(Ui__GetPasswordDialog);
    password_edit->setObjectName(QString::fromUtf8("password_edit"));
    password_edit->setEchoMode(QLineEdit::Password);

    hboxLayout->addWidget(password_edit);

    ok_button = new QPushButton(Ui__GetPasswordDialog);
    ok_button->setObjectName(QString::fromUtf8("ok_button"));
    ok_button->setEnabled(true);
    ok_button->setDefault(true);

    hboxLayout->addWidget(ok_button);

    cancel_button = new QPushButton(Ui__GetPasswordDialog);
    cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
    cancel_button->setAutoDefault(false);

    hboxLayout->addWidget(cancel_button);


    vboxLayout->addLayout(hboxLayout);


    retranslateUi(Ui__GetPasswordDialog);
    QObject::connect(cancel_button, SIGNAL(clicked()), Ui__GetPasswordDialog, SLOT(reject()));
    QObject::connect(ok_button, SIGNAL(clicked()), Ui__GetPasswordDialog, SLOT(accept()));

    QMetaObject::connectSlotsByName(Ui__GetPasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *Ui__GetPasswordDialog)
    {
    Ui__GetPasswordDialog->setWindowTitle(QApplication::translate("Ui::GetPasswordDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Ui::GetPasswordDialog", "\354\213\234\354\212\244\355\205\234\354\235\204 \354\242\205\353\243\214\355\225\230\352\270\260 \354\234\204\355\225\264\354\204\234\353\212\224 \353\243\250\355\212\270\352\266\214\355\225\234\354\235\264 \355\225\204\354\232\224\355\225\251\353\213\210\353\213\244. \355\230\204\354\236\254 \354\202\254\354\232\251\354\236\220\354\235\230 \355\214\250\354\212\244\354\233\214\353\223\234\353\245\274 \354\236\205\353\240\245\355\225\264\354\243\274\354\204\270\354\232\224.", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Ui::GetPasswordDialog", "\354\243\274\354\235\230 - \353\243\250\355\212\270\352\266\214\355\225\234\354\234\274\353\241\234 \355\224\204\353\241\234\352\267\270\353\236\250\354\235\204 \354\236\254\354\213\234\354\236\221\355\225\251\353\213\210\353\213\244.", 0, QApplication::UnicodeUTF8));
    ok_button->setText(QApplication::translate("Ui::GetPasswordDialog", "\355\231\225\354\235\270", 0, QApplication::UnicodeUTF8));
    cancel_button->setText(QApplication::translate("Ui::GetPasswordDialog", "\354\267\250\354\206\214", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Ui__GetPasswordDialog);
    } // retranslateUi

};

} // namespace Ui

namespace Ui {
namespace Ui {
    class GetPasswordDialog: public Ui_GetPasswordDialog {};
} // namespace Ui
} // namespace Ui

#endif // UI_GETPASSWORDDIALOG_H
