/********************************************************************************
** Form generated from reading ui file 'configwidget.ui'
**
** Created: Mon Sep 22 20:39:34 2008
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

namespace Backend {
namespace Ui {

class Ui_ConfigWidget
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *video_group;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *vo_combo;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *audio_group;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *ao_combo;
    QSpacerItem *horizontalSpacer;
    QWidget *volume_amp_widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSpinBox *volume_amp_spin;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *spacer;

    void setupUi(QWidget *Backend__Ui__ConfigWidget)
    {
    if (Backend__Ui__ConfigWidget->objectName().isEmpty())
        Backend__Ui__ConfigWidget->setObjectName(QString::fromUtf8("Backend__Ui__ConfigWidget"));
    Backend__Ui__ConfigWidget->resize(208, 202);
    verticalLayout_3 = new QVBoxLayout(Backend__Ui__ConfigWidget);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    video_group = new QGroupBox(Backend__Ui__ConfigWidget);
    video_group->setObjectName(QString::fromUtf8("video_group"));
    verticalLayout_2 = new QVBoxLayout(video_group);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    label_2 = new QLabel(video_group);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout_3->addWidget(label_2);

    vo_combo = new QComboBox(video_group);
    vo_combo->setObjectName(QString::fromUtf8("vo_combo"));

    horizontalLayout_3->addWidget(vo_combo);

    horizontalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(horizontalSpacer_3);


    verticalLayout_2->addLayout(horizontalLayout_3);


    verticalLayout_3->addWidget(video_group);

    audio_group = new QGroupBox(Backend__Ui__ConfigWidget);
    audio_group->setObjectName(QString::fromUtf8("audio_group"));
    verticalLayout = new QVBoxLayout(audio_group);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(audio_group);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout->addWidget(label);

    ao_combo = new QComboBox(audio_group);
    ao_combo->setObjectName(QString::fromUtf8("ao_combo"));

    horizontalLayout->addWidget(ao_combo);

    horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);


    verticalLayout->addLayout(horizontalLayout);

    volume_amp_widget = new QWidget(audio_group);
    volume_amp_widget->setObjectName(QString::fromUtf8("volume_amp_widget"));
    horizontalLayout_2 = new QHBoxLayout(volume_amp_widget);
    horizontalLayout_2->setMargin(0);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_3 = new QLabel(volume_amp_widget);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    horizontalLayout_2->addWidget(label_3);

    volume_amp_spin = new QSpinBox(volume_amp_widget);
    volume_amp_spin->setObjectName(QString::fromUtf8("volume_amp_spin"));
    volume_amp_spin->setAccelerated(true);

    horizontalLayout_2->addWidget(volume_amp_spin);

    horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer_2);


    verticalLayout->addWidget(volume_amp_widget);


    verticalLayout_3->addWidget(audio_group);

    spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_3->addItem(spacer);


    retranslateUi(Backend__Ui__ConfigWidget);

    QMetaObject::connectSlotsByName(Backend__Ui__ConfigWidget);
    } // setupUi

    void retranslateUi(QWidget *Backend__Ui__ConfigWidget)
    {
    Backend__Ui__ConfigWidget->setWindowTitle(QApplication::translate("Backend::Ui::ConfigWidget", "Form", 0, QApplication::UnicodeUTF8));
    video_group->setTitle(QApplication::translate("Backend::Ui::ConfigWidget", "\353\271\204\353\224\224\354\230\244", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Backend::Ui::ConfigWidget", "\354\266\234\353\240\245 \353\223\234\353\235\274\354\235\264\353\262\204", 0, QApplication::UnicodeUTF8));
    audio_group->setTitle(QApplication::translate("Backend::Ui::ConfigWidget", "\354\230\244\353\224\224\354\230\244", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Backend::Ui::ConfigWidget", "\354\266\234\353\240\245 \353\223\234\353\235\274\354\235\264\353\262\204", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Backend::Ui::ConfigWidget", "\353\263\274\353\245\250 \354\246\235\355\217\255\353\245\240", 0, QApplication::UnicodeUTF8));
    volume_amp_spin->setSuffix(QApplication::translate("Backend::Ui::ConfigWidget", " %", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Backend__Ui__ConfigWidget);
    } // retranslateUi

};

} // namespace Ui
} // namespace Backend

namespace Backend {
namespace Ui {
namespace Ui {
    class ConfigWidget: public Ui_ConfigWidget {};
} // namespace Ui
} // namespace Ui
} // namespace Backend

#endif // UI_CONFIGWIDGET_H
