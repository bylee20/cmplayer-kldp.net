/********************************************************************************
** Form generated from reading ui file 'selecttitledialog.ui'
**
** Created: Tue Jul 15 21:22:19 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SELECTTITLEDIALOG_H
#define UI_SELECTTITLEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

namespace Ui {

class Ui_SelectTitleDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTreeWidget *list;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *play_button;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *Ui__SelectTitleDialog)
    {
    if (Ui__SelectTitleDialog->objectName().isEmpty())
        Ui__SelectTitleDialog->setObjectName(QString::fromUtf8("Ui__SelectTitleDialog"));
    Ui__SelectTitleDialog->resize(298, 272);
    verticalLayout = new QVBoxLayout(Ui__SelectTitleDialog);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    label = new QLabel(Ui__SelectTitleDialog);
    label->setObjectName(QString::fromUtf8("label"));

    verticalLayout->addWidget(label);

    list = new QTreeWidget(Ui__SelectTitleDialog);
    list->setObjectName(QString::fromUtf8("list"));
    list->setAlternatingRowColors(true);
    list->setRootIsDecorated(false);
    list->setItemsExpandable(false);

    verticalLayout->addWidget(list);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    play_button = new QPushButton(Ui__SelectTitleDialog);
    play_button->setObjectName(QString::fromUtf8("play_button"));
    play_button->setEnabled(false);

    horizontalLayout->addWidget(play_button);

    pushButton_2 = new QPushButton(Ui__SelectTitleDialog);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    horizontalLayout->addWidget(pushButton_2);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_2);


    verticalLayout->addLayout(horizontalLayout);


    retranslateUi(Ui__SelectTitleDialog);
    QObject::connect(play_button, SIGNAL(clicked()), Ui__SelectTitleDialog, SLOT(accept()));
    QObject::connect(pushButton_2, SIGNAL(clicked()), Ui__SelectTitleDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Ui__SelectTitleDialog);
    } // setupUi

    void retranslateUi(QDialog *Ui__SelectTitleDialog)
    {
    Ui__SelectTitleDialog->setWindowTitle(QApplication::translate("Ui::SelectTitleDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Ui::SelectTitleDialog", "\354\236\254\354\203\235\355\225\230\353\240\244\353\212\224 DVD\354\227\220\353\212\224 \353\221\230 \354\235\264\354\203\201\354\235\230 \355\203\200\354\235\264\355\213\200\354\235\264 \354\241\264\354\236\254\355\225\251\353\213\210\353\213\244.\n"
"\354\236\254\354\203\235\355\225\240 \355\203\200\354\235\264\355\213\200\354\235\204 \354\204\240\355\203\235\355\225\264\354\243\274\354\204\270\354\232\224.", 0, QApplication::UnicodeUTF8));
    play_button->setText(QApplication::translate("Ui::SelectTitleDialog", "\354\236\254\354\203\235", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("Ui::SelectTitleDialog", "\354\267\250\354\206\214", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Ui__SelectTitleDialog);
    } // retranslateUi

};

} // namespace Ui

namespace Ui {
namespace Ui {
    class SelectTitleDialog: public Ui_SelectTitleDialog {};
} // namespace Ui
} // namespace Ui

#endif // UI_SELECTTITLEDIALOG_H
