/********************************************************************************
** Form generated from reading ui file 'configwidget.ui'
**
** Created: Sun Sep 21 22:25:39 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONFIGWIDGET_H
#define UI_CONFIGWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *audio_group;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *audio_driver_combo;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSpinBox *volume_amp_spin;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *video_group;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *comboBox_2;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *spacer;

    void setupUi(QWidget *Form)
    {
    if (Form->objectName().isEmpty())
        Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(523, 496);
    verticalLayout_3 = new QVBoxLayout(Form);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    audio_group = new QGroupBox(Form);
    audio_group->setObjectName(QString::fromUtf8("audio_group"));
    verticalLayout = new QVBoxLayout(audio_group);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(audio_group);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout->addWidget(label);

    audio_driver_combo = new QComboBox(audio_group);
    audio_driver_combo->setObjectName(QString::fromUtf8("audio_driver_combo"));

    horizontalLayout->addWidget(audio_driver_combo);

    horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);


    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_3 = new QLabel(audio_group);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    horizontalLayout_2->addWidget(label_3);

    volume_amp_spin = new QSpinBox(audio_group);
    volume_amp_spin->setObjectName(QString::fromUtf8("volume_amp_spin"));

    horizontalLayout_2->addWidget(volume_amp_spin);

    horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer_2);


    verticalLayout->addLayout(horizontalLayout_2);


    verticalLayout_3->addWidget(audio_group);

    video_group = new QGroupBox(Form);
    video_group->setObjectName(QString::fromUtf8("video_group"));
    verticalLayout_2 = new QVBoxLayout(video_group);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    label_2 = new QLabel(video_group);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout_3->addWidget(label_2);

    comboBox_2 = new QComboBox(video_group);
    comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

    horizontalLayout_3->addWidget(comboBox_2);

    horizontalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(horizontalSpacer_3);


    verticalLayout_2->addLayout(horizontalLayout_3);


    verticalLayout_3->addWidget(video_group);

    spacer = new QSpacerItem(0, 289, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_3->addItem(spacer);


    retranslateUi(Form);

    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
    audio_group->setTitle(QApplication::translate("Form", "\354\230\244\353\224\224\354\230\244", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Form", "\354\266\234\353\240\245 \353\223\234\353\235\274\354\235\264\353\262\204", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Form", "\353\263\274\353\245\250 \354\246\235\355\217\255\353\245\240", 0, QApplication::UnicodeUTF8));
    video_group->setTitle(QApplication::translate("Form", "\353\271\204\353\224\224\354\230\244", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Form", "\354\266\234\353\240\245 \353\223\234\353\235\274\354\235\264\353\262\204", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGWIDGET_H
