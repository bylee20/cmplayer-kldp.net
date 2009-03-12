/********************************************************************************
** Form generated from reading ui file 'prefosdwidget.ui'
**
** Created: Thu Mar 12 09:58:05 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PREFOSDWIDGET_H
#define UI_PREFOSDWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrefOsdWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *fontLabel;
    QPushButton *fontButton;
    QLabel *label_3;
    QLabel *fgColorLabel;
    QPushButton *fgColorButton;
    QLabel *label_4;
    QLabel *bgColorLabel;
    QPushButton *bgColorButton;
    QLabel *label_8;
    QComboBox *scale;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout;
    QDoubleSpinBox *size;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_5;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *PrefOsdWidget)
    {
    if (PrefOsdWidget->objectName().isEmpty())
        PrefOsdWidget->setObjectName(QString::fromUtf8("PrefOsdWidget"));
    PrefOsdWidget->resize(301, 198);
    gridLayout = new QGridLayout(PrefOsdWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(PrefOsdWidget);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    fontLabel = new QLabel(PrefOsdWidget);
    fontLabel->setObjectName(QString::fromUtf8("fontLabel"));
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(fontLabel->sizePolicy().hasHeightForWidth());
    fontLabel->setSizePolicy(sizePolicy);
    fontLabel->setMinimumSize(QSize(100, 0));
    fontLabel->setFrameShape(QFrame::StyledPanel);
    fontLabel->setFrameShadow(QFrame::Sunken);
    fontLabel->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(fontLabel, 0, 1, 1, 1);

    fontButton = new QPushButton(PrefOsdWidget);
    fontButton->setObjectName(QString::fromUtf8("fontButton"));

    gridLayout->addWidget(fontButton, 0, 2, 1, 1);

    label_3 = new QLabel(PrefOsdWidget);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    fgColorLabel = new QLabel(PrefOsdWidget);
    fgColorLabel->setObjectName(QString::fromUtf8("fgColorLabel"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(fgColorLabel->sizePolicy().hasHeightForWidth());
    fgColorLabel->setSizePolicy(sizePolicy1);
    fgColorLabel->setFrameShape(QFrame::StyledPanel);
    fgColorLabel->setFrameShadow(QFrame::Sunken);
    fgColorLabel->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(fgColorLabel, 1, 1, 1, 1);

    fgColorButton = new QPushButton(PrefOsdWidget);
    fgColorButton->setObjectName(QString::fromUtf8("fgColorButton"));

    gridLayout->addWidget(fgColorButton, 1, 2, 1, 1);

    label_4 = new QLabel(PrefOsdWidget);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 2, 0, 1, 1);

    bgColorLabel = new QLabel(PrefOsdWidget);
    bgColorLabel->setObjectName(QString::fromUtf8("bgColorLabel"));
    sizePolicy1.setHeightForWidth(bgColorLabel->sizePolicy().hasHeightForWidth());
    bgColorLabel->setSizePolicy(sizePolicy1);
    bgColorLabel->setFrameShape(QFrame::StyledPanel);
    bgColorLabel->setFrameShadow(QFrame::Sunken);
    bgColorLabel->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(bgColorLabel, 2, 1, 1, 1);

    bgColorButton = new QPushButton(PrefOsdWidget);
    bgColorButton->setObjectName(QString::fromUtf8("bgColorButton"));

    gridLayout->addWidget(bgColorButton, 2, 2, 1, 1);

    label_8 = new QLabel(PrefOsdWidget);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout->addWidget(label_8, 3, 0, 1, 1);

    scale = new QComboBox(PrefOsdWidget);
    scale->setObjectName(QString::fromUtf8("scale"));
    scale->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    gridLayout->addWidget(scale, 3, 1, 1, 2);

    label_7 = new QLabel(PrefOsdWidget);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 4, 0, 1, 1);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    size = new QDoubleSpinBox(PrefOsdWidget);
    size->setObjectName(QString::fromUtf8("size"));
    size->setAccelerated(true);
    size->setDecimals(1);
    size->setMaximum(99);
    size->setSingleStep(0.5);
    size->setValue(10);

    horizontalLayout->addWidget(size);

    horizontalSpacer_4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_4);


    gridLayout->addLayout(horizontalLayout, 4, 1, 1, 2);

    label_5 = new QLabel(PrefOsdWidget);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setWordWrap(true);

    gridLayout->addWidget(label_5, 5, 0, 1, 3);

    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer, 6, 0, 1, 3);


    retranslateUi(PrefOsdWidget);

    QMetaObject::connectSlotsByName(PrefOsdWidget);
    } // setupUi

    void retranslateUi(QWidget *PrefOsdWidget)
    {
    label->setText(QApplication::translate("PrefOsdWidget", "Font", 0, QApplication::UnicodeUTF8));
    fontLabel->setText(QString());
    fontButton->setText(QApplication::translate("PrefOsdWidget", "Change", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("PrefOsdWidget", "Foreground Color", 0, QApplication::UnicodeUTF8));
    fgColorLabel->setText(QString());
    fgColorButton->setText(QApplication::translate("PrefOsdWidget", "Change", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("PrefOsdWidget", "Background Color", 0, QApplication::UnicodeUTF8));
    bgColorLabel->setText(QString());
    bgColorButton->setText(QApplication::translate("PrefOsdWidget", "Change", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("PrefOsdWidget", "Auto Size", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("PrefOsdWidget", "Size Scale", 0, QApplication::UnicodeUTF8));
    size->setSuffix(QApplication::translate("PrefOsdWidget", " %", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("PrefOsdWidget", "Some options might be ignored in case that the play engine doesn't support them.", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(PrefOsdWidget);
    } // retranslateUi

};

namespace Ui {
    class PrefOsdWidget: public Ui_PrefOsdWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFOSDWIDGET_H
