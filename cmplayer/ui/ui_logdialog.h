/********************************************************************************
** Form generated from reading ui file 'logdialog.ui'
**
** Created: Sun Jun 1 03:58:53 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOGDIALOG_H
#define UI_LOGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

namespace Ui {

class Ui_LogDialog
{
public:
    QVBoxLayout *vboxLayout;
    QTextBrowser *log;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QToolButton *save_button;
    QPushButton *close_button;

    void setupUi(QDialog *Ui__LogDialog)
    {
    if (Ui__LogDialog->objectName().isEmpty())
        Ui__LogDialog->setObjectName(QString::fromUtf8("Ui__LogDialog"));
    Ui__LogDialog->resize(400, 300);
    vboxLayout = new QVBoxLayout(Ui__LogDialog);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    log = new QTextBrowser(Ui__LogDialog);
    log->setObjectName(QString::fromUtf8("log"));

    vboxLayout->addWidget(log);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    save_button = new QToolButton(Ui__LogDialog);
    save_button->setObjectName(QString::fromUtf8("save_button"));
    const QIcon icon = QIcon(QString::fromUtf8(":/img/document-save.png"));
    save_button->setIcon(icon);

    hboxLayout->addWidget(save_button);

    close_button = new QPushButton(Ui__LogDialog);
    close_button->setObjectName(QString::fromUtf8("close_button"));

    hboxLayout->addWidget(close_button);


    vboxLayout->addLayout(hboxLayout);


    retranslateUi(Ui__LogDialog);
    QObject::connect(close_button, SIGNAL(clicked()), Ui__LogDialog, SLOT(close()));

    QMetaObject::connectSlotsByName(Ui__LogDialog);
    } // setupUi

    void retranslateUi(QDialog *Ui__LogDialog)
    {
    Ui__LogDialog->setWindowTitle(QApplication::translate("Ui::LogDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    save_button->setText(QApplication::translate("Ui::LogDialog", "...", 0, QApplication::UnicodeUTF8));
    close_button->setText(QApplication::translate("Ui::LogDialog", "\353\213\253\352\270\260(&C)", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Ui__LogDialog);
    } // retranslateUi

};

} // namespace Ui

namespace Ui {
namespace Ui {
    class LogDialog: public Ui_LogDialog {};
} // namespace Ui
} // namespace Ui

#endif // UI_LOGDIALOG_H
