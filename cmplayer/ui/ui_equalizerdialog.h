/********************************************************************************
** Form generated from reading ui file 'equalizerdialog.ui'
**
** Created: Thu Jul 17 00:18:21 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EQUALIZERDIALOG_H
#define UI_EQUALIZERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>

namespace Ui {

class Ui_EqualizerDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QSlider *horizontalSlider_5;
    QSpinBox *brightness_spin;
    QLabel *label_2;
    QSlider *horizontalSlider_6;
    QSpinBox *contrast_spin;
    QLabel *label_5;
    QSlider *horizontalSlider_7;
    QSpinBox *gamma_spin;
    QLabel *label_3;
    QSlider *horizontalSlider_8;
    QSpinBox *hue_spin;
    QLabel *label_4;
    QSlider *horizontalSlider_9;
    QSpinBox *saturation_spin;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout;
    QPushButton *default_button;
    QPushButton *reset_button;
    QSpacerItem *spacerItem1;
    QPushButton *close_button;

    void setupUi(QDialog *Ui__EqualizerDialog)
    {
    if (Ui__EqualizerDialog->objectName().isEmpty())
        Ui__EqualizerDialog->setObjectName(QString::fromUtf8("Ui__EqualizerDialog"));
    Ui__EqualizerDialog->resize(275, 212);
    gridLayout = new QGridLayout(Ui__EqualizerDialog);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(Ui__EqualizerDialog);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    horizontalSlider_5 = new QSlider(Ui__EqualizerDialog);
    horizontalSlider_5->setObjectName(QString::fromUtf8("horizontalSlider_5"));
    horizontalSlider_5->setMinimum(-100);
    horizontalSlider_5->setMaximum(100);
    horizontalSlider_5->setOrientation(Qt::Horizontal);
    horizontalSlider_5->setTickPosition(QSlider::TicksBelow);

    gridLayout->addWidget(horizontalSlider_5, 0, 1, 1, 1);

    brightness_spin = new QSpinBox(Ui__EqualizerDialog);
    brightness_spin->setObjectName(QString::fromUtf8("brightness_spin"));
    brightness_spin->setAccelerated(true);
    brightness_spin->setMinimum(-100);
    brightness_spin->setMaximum(100);
    brightness_spin->setValue(0);

    gridLayout->addWidget(brightness_spin, 0, 2, 1, 1);

    label_2 = new QLabel(Ui__EqualizerDialog);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    horizontalSlider_6 = new QSlider(Ui__EqualizerDialog);
    horizontalSlider_6->setObjectName(QString::fromUtf8("horizontalSlider_6"));
    horizontalSlider_6->setMinimum(-100);
    horizontalSlider_6->setMaximum(100);
    horizontalSlider_6->setOrientation(Qt::Horizontal);
    horizontalSlider_6->setTickPosition(QSlider::TicksBelow);

    gridLayout->addWidget(horizontalSlider_6, 1, 1, 1, 1);

    contrast_spin = new QSpinBox(Ui__EqualizerDialog);
    contrast_spin->setObjectName(QString::fromUtf8("contrast_spin"));
    contrast_spin->setAccelerated(true);
    contrast_spin->setMinimum(-100);
    contrast_spin->setMaximum(100);
    contrast_spin->setValue(0);

    gridLayout->addWidget(contrast_spin, 1, 2, 1, 1);

    label_5 = new QLabel(Ui__EqualizerDialog);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout->addWidget(label_5, 2, 0, 1, 1);

    horizontalSlider_7 = new QSlider(Ui__EqualizerDialog);
    horizontalSlider_7->setObjectName(QString::fromUtf8("horizontalSlider_7"));
    horizontalSlider_7->setMinimum(-100);
    horizontalSlider_7->setMaximum(100);
    horizontalSlider_7->setOrientation(Qt::Horizontal);
    horizontalSlider_7->setTickPosition(QSlider::TicksBelow);

    gridLayout->addWidget(horizontalSlider_7, 2, 1, 1, 1);

    gamma_spin = new QSpinBox(Ui__EqualizerDialog);
    gamma_spin->setObjectName(QString::fromUtf8("gamma_spin"));
    gamma_spin->setAccelerated(true);
    gamma_spin->setMinimum(-100);
    gamma_spin->setMaximum(100);
    gamma_spin->setValue(0);

    gridLayout->addWidget(gamma_spin, 2, 2, 1, 1);

    label_3 = new QLabel(Ui__EqualizerDialog);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 3, 0, 1, 1);

    horizontalSlider_8 = new QSlider(Ui__EqualizerDialog);
    horizontalSlider_8->setObjectName(QString::fromUtf8("horizontalSlider_8"));
    horizontalSlider_8->setMinimum(-100);
    horizontalSlider_8->setMaximum(100);
    horizontalSlider_8->setOrientation(Qt::Horizontal);
    horizontalSlider_8->setTickPosition(QSlider::TicksBelow);

    gridLayout->addWidget(horizontalSlider_8, 3, 1, 1, 1);

    hue_spin = new QSpinBox(Ui__EqualizerDialog);
    hue_spin->setObjectName(QString::fromUtf8("hue_spin"));
    hue_spin->setAccelerated(true);
    hue_spin->setMinimum(-100);
    hue_spin->setMaximum(100);
    hue_spin->setValue(0);

    gridLayout->addWidget(hue_spin, 3, 2, 1, 1);

    label_4 = new QLabel(Ui__EqualizerDialog);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 4, 0, 1, 1);

    horizontalSlider_9 = new QSlider(Ui__EqualizerDialog);
    horizontalSlider_9->setObjectName(QString::fromUtf8("horizontalSlider_9"));
    horizontalSlider_9->setMinimum(-100);
    horizontalSlider_9->setMaximum(100);
    horizontalSlider_9->setOrientation(Qt::Horizontal);
    horizontalSlider_9->setTickPosition(QSlider::TicksBelow);

    gridLayout->addWidget(horizontalSlider_9, 4, 1, 1, 1);

    saturation_spin = new QSpinBox(Ui__EqualizerDialog);
    saturation_spin->setObjectName(QString::fromUtf8("saturation_spin"));
    saturation_spin->setAccelerated(true);
    saturation_spin->setMinimum(-100);
    saturation_spin->setMaximum(100);
    saturation_spin->setValue(0);

    gridLayout->addWidget(saturation_spin, 4, 2, 1, 1);

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(spacerItem, 5, 0, 1, 3);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    default_button = new QPushButton(Ui__EqualizerDialog);
    default_button->setObjectName(QString::fromUtf8("default_button"));

    hboxLayout->addWidget(default_button);

    reset_button = new QPushButton(Ui__EqualizerDialog);
    reset_button->setObjectName(QString::fromUtf8("reset_button"));

    hboxLayout->addWidget(reset_button);

    spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);

    close_button = new QPushButton(Ui__EqualizerDialog);
    close_button->setObjectName(QString::fromUtf8("close_button"));
    close_button->setDefault(true);

    hboxLayout->addWidget(close_button);


    gridLayout->addLayout(hboxLayout, 6, 0, 1, 3);


    retranslateUi(Ui__EqualizerDialog);
    QObject::connect(close_button, SIGNAL(clicked()), Ui__EqualizerDialog, SLOT(accept()));
    QObject::connect(horizontalSlider_5, SIGNAL(valueChanged(int)), brightness_spin, SLOT(setValue(int)));
    QObject::connect(horizontalSlider_6, SIGNAL(valueChanged(int)), contrast_spin, SLOT(setValue(int)));
    QObject::connect(horizontalSlider_7, SIGNAL(valueChanged(int)), gamma_spin, SLOT(setValue(int)));
    QObject::connect(horizontalSlider_8, SIGNAL(valueChanged(int)), hue_spin, SLOT(setValue(int)));
    QObject::connect(horizontalSlider_9, SIGNAL(valueChanged(int)), saturation_spin, SLOT(setValue(int)));
    QObject::connect(saturation_spin, SIGNAL(valueChanged(int)), horizontalSlider_9, SLOT(setValue(int)));
    QObject::connect(hue_spin, SIGNAL(valueChanged(int)), horizontalSlider_8, SLOT(setValue(int)));
    QObject::connect(gamma_spin, SIGNAL(valueChanged(int)), horizontalSlider_7, SLOT(setValue(int)));
    QObject::connect(contrast_spin, SIGNAL(valueChanged(int)), horizontalSlider_6, SLOT(setValue(int)));
    QObject::connect(brightness_spin, SIGNAL(valueChanged(int)), horizontalSlider_5, SLOT(setValue(int)));

    QMetaObject::connectSlotsByName(Ui__EqualizerDialog);
    } // setupUi

    void retranslateUi(QDialog *Ui__EqualizerDialog)
    {
    Ui__EqualizerDialog->setWindowTitle(QApplication::translate("Ui::EqualizerDialog", "\355\231\224\353\251\264 \354\204\244\354\240\225\355\225\230\352\270\260", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Ui::EqualizerDialog", "\353\260\235\352\270\260", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Ui::EqualizerDialog", "\353\214\200\353\271\204", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("Ui::EqualizerDialog", "\353\252\205\354\225\224", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Ui::EqualizerDialog", "\354\203\211\354\241\260", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("Ui::EqualizerDialog", "\354\261\204\353\217\204", 0, QApplication::UnicodeUTF8));
    default_button->setText(QApplication::translate("Ui::EqualizerDialog", "\352\270\260\353\263\270\352\260\222", 0, QApplication::UnicodeUTF8));
    reset_button->setText(QApplication::translate("Ui::EqualizerDialog", "\354\264\210\352\270\260\355\231\224", 0, QApplication::UnicodeUTF8));
    close_button->setText(QApplication::translate("Ui::EqualizerDialog", "\353\213\253\352\270\260", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Ui__EqualizerDialog);
    } // retranslateUi

};

} // namespace Ui

namespace Ui {
namespace Ui {
    class EqualizerDialog: public Ui_EqualizerDialog {};
} // namespace Ui
} // namespace Ui

#endif // UI_EQUALIZERDIALOG_H
