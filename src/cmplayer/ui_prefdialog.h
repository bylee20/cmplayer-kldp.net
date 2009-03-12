/********************************************************************************
** Form generated from reading ui file 'prefdialog.ui'
**
** Created: Thu Mar 12 09:58:05 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PREFDIALOG_H
#define UI_PREFDIALOG_H

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

QT_BEGIN_NAMESPACE

class Ui_PrefDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *list;
    QStackedWidget *stack;
    QWidget *page;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QPushButton *apply;
    QPushButton *pushButton_2;

    void setupUi(QDialog *PrefDialog)
    {
    if (PrefDialog->objectName().isEmpty())
        PrefDialog->setObjectName(QString::fromUtf8("PrefDialog"));
    PrefDialog->resize(285, 452);
    verticalLayout = new QVBoxLayout(PrefDialog);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    list = new QListWidget(PrefDialog);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/img/system-run.png")), QIcon::Normal, QIcon::Off);
    QListWidgetItem *__listItem = new QListWidgetItem(list);
    __listItem->setIcon(icon);
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8(":/img/subtitle.png")), QIcon::Normal, QIcon::Off);
    QListWidgetItem *__listItem1 = new QListWidgetItem(list);
    __listItem1->setIcon(icon1);
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8(":/img/preferences-desktop-keyboard.png")), QIcon::Normal, QIcon::Off);
    QListWidgetItem *__listItem2 = new QListWidgetItem(list);
    __listItem2->setIcon(icon2);
    QIcon icon3;
    icon3.addPixmap(QPixmap(QString::fromUtf8(":/img/applications-multimedia.png")), QIcon::Normal, QIcon::Off);
    QListWidgetItem *__listItem3 = new QListWidgetItem(list);
    __listItem3->setIcon(icon3);
    list->setObjectName(QString::fromUtf8("list"));
    list->setMinimumSize(QSize(98, 410));
    list->setMaximumSize(QSize(98, 16777215));
    list->setDragDropMode(QAbstractItemView::NoDragDrop);
    list->setIconSize(QSize(64, 64));
    list->setMovement(QListView::Static);
    list->setFlow(QListView::TopToBottom);
    list->setLayoutMode(QListView::SinglePass);
    list->setSpacing(10);
    list->setViewMode(QListView::IconMode);

    horizontalLayout_2->addWidget(list);

    stack = new QStackedWidget(PrefDialog);
    stack->setObjectName(QString::fromUtf8("stack"));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    stack->addWidget(page);

    horizontalLayout_2->addWidget(stack);


    verticalLayout->addLayout(horizontalLayout_2);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    pushButton_3 = new QPushButton(PrefDialog);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

    horizontalLayout->addWidget(pushButton_3);

    apply = new QPushButton(PrefDialog);
    apply->setObjectName(QString::fromUtf8("apply"));

    horizontalLayout->addWidget(apply);

    pushButton_2 = new QPushButton(PrefDialog);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    horizontalLayout->addWidget(pushButton_2);


    verticalLayout->addLayout(horizontalLayout);


    retranslateUi(PrefDialog);
    QObject::connect(list, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));
    QObject::connect(pushButton_3, SIGNAL(clicked()), PrefDialog, SLOT(accept()));
    QObject::connect(pushButton_2, SIGNAL(clicked()), PrefDialog, SLOT(reject()));

    stack->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(PrefDialog);
    } // setupUi

    void retranslateUi(QDialog *PrefDialog)
    {
    PrefDialog->setWindowTitle(QApplication::translate("PrefDialog", "Preferences", 0, QApplication::UnicodeUTF8));

    const bool __sortingEnabled = list->isSortingEnabled();
    list->setSortingEnabled(false);
    list->item(0)->setText(QApplication::translate("PrefDialog", "General", 0, QApplication::UnicodeUTF8));
    list->item(1)->setText(QApplication::translate("PrefDialog", "Subtitle", 0, QApplication::UnicodeUTF8));
    list->item(2)->setText(QApplication::translate("PrefDialog", "Interface", 0, QApplication::UnicodeUTF8));
    list->item(3)->setText(QApplication::translate("PrefDialog", "Play Engine", 0, QApplication::UnicodeUTF8));

    list->setSortingEnabled(__sortingEnabled);
    pushButton_3->setText(QApplication::translate("PrefDialog", "&Ok", 0, QApplication::UnicodeUTF8));
    apply->setText(QApplication::translate("PrefDialog", "&Apply", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("PrefDialog", "&Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(PrefDialog);
    } // retranslateUi

};

namespace Ui {
    class PrefDialog: public Ui_PrefDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFDIALOG_H
