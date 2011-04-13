/********************************************************************************
** Form generated from reading UI file 'pref_widget.ui'
**
** Created: Wed Apr 13 23:51:18 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREF_WIDGET_H
#define UI_PREF_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "controls.hpp"
#include "simplelistwidget.hpp"

QT_BEGIN_NAMESPACE

class Ui_Pref_Widget
{
public:
    QVBoxLayout *verticalLayout_14;
    QStackedWidget *stack;
    QWidget *general;
    QHBoxLayout *_4;
    QTabWidget *tabWidget_2;
    QWidget *play;
    QHBoxLayout *horizontalLayout_2;
    QFormLayout *formLayout_5;
    QCheckBox *pauseMinimized;
    QCheckBox *pauseVideoOnly;
    QFrame *line_3;
    QCheckBox *startStopped;
    QCheckBox *askWhenRecordFound;
    QFrame *line_2;
    QLabel *label_23;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QCheckBox *hideCursor;
    QSpinBox *hideDelay;
    QCheckBox *disableSS;
    QComboBox *autoAdd;
    QWidget *video;
    QHBoxLayout *horizontalLayout_13;
    QFormLayout *formLayout_6;
    QLabel *label_30;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_31;
    QSpinBox *blur_c;
    QLabel *label_32;
    QSpinBox *blur_n;
    QLabel *label_33;
    QSpinBox *blur_d;
    QLabel *label_34;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_35;
    QSpinBox *sharpen_c;
    QLabel *label_36;
    QSpinBox *sharpen_n;
    QLabel *label_37;
    QSpinBox *sharpen_d;
    QLabel *label_27;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_28;
    QSpinBox *min_luma;
    QLabel *label_29;
    QSpinBox *max_luma;
    QLabel *label_25;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_26;
    QDoubleSpinBox *auto_contrast_th;
    QWidget *subtitle;
    QHBoxLayout *horizontalLayout_7;
    QTabWidget *tabs;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_5;
    QFormLayout *formLayout_2;
    QLabel *aLabel_2;
    QComboBox *autoLoad;
    QFrame *line_9;
    QLabel *aLabel_3;
    QComboBox *autoSelect;
    QLabel *subExtLabel;
    QComboBox *subSelectExt;
    QFrame *line_11;
    QLabel *aLabel_4;
    EncodingComboBox *encoding;
    QHBoxLayout *horizontalLayout_8;
    QCheckBox *subEncAutoDet;
    QSpinBox *subConf;
    QSpacerItem *horizontalSpacer_11;
    QLabel *label_4;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_24;
    QFormLayout *formLayout_3;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_19;
    QLabel *sub_font_label;
    QPushButton *sub_font_button;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_20;
    QLabel *sub_color_fg_label;
    QPushButton *sub_color_fg_button;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_18;
    QLabel *sub_color_bg_label;
    QPushButton *sub_color_bg_button;
    QLabel *label_21;
    QComboBox *sub_auto_size;
    QLabel *label_22;
    QDoubleSpinBox *sub_size_scale;
    QFrame *line_10;
    QLabel *aLabel_6;
    QSpinBox *msPerChar;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_7;
    SimpleListWidget *priority;
    QSpacerItem *spacer_2;
    QWidget *ui;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_15;
    QFormLayout *formLayout;
    QCheckBox *singleApp;
    QFrame *line_14;
    QLabel *localeLabel;
    QComboBox *locale;
    QLabel *wLabel;
    QComboBox *winStyle;
    QLabel *aLabel;
    QCheckBox *tray;
    QFrame *line_15;
    QCheckBox *hideWhenClosed;
    QLabel *label_43;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *shortcutTree;
    QHBoxLayout *_3;
    QLabel *label;
    QPushButton *shortcut1;
    QPushButton *shortcut2;
    QPushButton *shortcut3;
    QPushButton *shortcut4;
    QWidget *mouseWidget;
    QHBoxLayout *horizontalLayout_9;
    QFormLayout *mouse_form;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_6;
    QFormLayout *formLayout_4;
    QLabel *aLabel_5;
    QSpinBox *seek1;
    QLabel *label_16;
    QSpinBox *seek2;
    QFrame *line_12;
    QLabel *label_12;
    QSpinBox *spinBox;
    QLabel *label_14;
    QSpinBox *spinBox_3;
    QFrame *line_13;
    QLabel *label_18;
    QSpinBox *volume;
    QLabel *label_19;
    QSpinBox *amp;
    QFormLayout *formLayout_7;
    QLabel *label_9;
    QSpinBox *speed;
    QLabel *label_17;
    QSpinBox *seek3;
    QLabel *label_13;
    QLabel *label_20;
    QSpinBox *spinBox_2;
    QSpinBox *spinBox_4;
    QLabel *label_10;
    QSpinBox *subPos;
    QLabel *label_11;
    QDoubleSpinBox *sync;
    QFrame *line_4;
    QFrame *line_5;

    void setupUi(QWidget *Pref_Widget)
    {
        if (Pref_Widget->objectName().isEmpty())
            Pref_Widget->setObjectName(QString::fromUtf8("Pref_Widget"));
        Pref_Widget->resize(567, 317);
        verticalLayout_14 = new QVBoxLayout(Pref_Widget);
        verticalLayout_14->setContentsMargins(0, 0, 0, 0);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        stack = new QStackedWidget(Pref_Widget);
        stack->setObjectName(QString::fromUtf8("stack"));
        general = new QWidget();
        general->setObjectName(QString::fromUtf8("general"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/preferences-desktop-wallpaper.png"), QSize(), QIcon::Normal, QIcon::Off);
        general->setProperty("icon", QVariant(icon));
        _4 = new QHBoxLayout(general);
        _4->setObjectName(QString::fromUtf8("_4"));
        tabWidget_2 = new QTabWidget(general);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        play = new QWidget();
        play->setObjectName(QString::fromUtf8("play"));
        horizontalLayout_2 = new QHBoxLayout(play);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        formLayout_5 = new QFormLayout();
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        pauseMinimized = new QCheckBox(play);
        pauseMinimized->setObjectName(QString::fromUtf8("pauseMinimized"));

        formLayout_5->setWidget(0, QFormLayout::FieldRole, pauseMinimized);

        pauseVideoOnly = new QCheckBox(play);
        pauseVideoOnly->setObjectName(QString::fromUtf8("pauseVideoOnly"));
        pauseVideoOnly->setEnabled(false);
        pauseVideoOnly->setChecked(true);

        formLayout_5->setWidget(1, QFormLayout::FieldRole, pauseVideoOnly);

        line_3 = new QFrame(play);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setFrameShape(QFrame::HLine);

        formLayout_5->setWidget(2, QFormLayout::SpanningRole, line_3);

        startStopped = new QCheckBox(play);
        startStopped->setObjectName(QString::fromUtf8("startStopped"));

        formLayout_5->setWidget(3, QFormLayout::FieldRole, startStopped);

        askWhenRecordFound = new QCheckBox(play);
        askWhenRecordFound->setObjectName(QString::fromUtf8("askWhenRecordFound"));
        askWhenRecordFound->setEnabled(false);

        formLayout_5->setWidget(4, QFormLayout::FieldRole, askWhenRecordFound);

        line_2 = new QFrame(play);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setFrameShape(QFrame::HLine);

        formLayout_5->setWidget(5, QFormLayout::SpanningRole, line_2);

        label_23 = new QLabel(play);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        formLayout_5->setWidget(6, QFormLayout::LabelRole, label_23);

        line = new QFrame(play);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        formLayout_5->setWidget(8, QFormLayout::SpanningRole, line);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        hideCursor = new QCheckBox(play);
        hideCursor->setObjectName(QString::fromUtf8("hideCursor"));

        horizontalLayout->addWidget(hideCursor);

        hideDelay = new QSpinBox(play);
        hideDelay->setObjectName(QString::fromUtf8("hideDelay"));
        hideDelay->setEnabled(false);
        hideDelay->setAccelerated(true);
        hideDelay->setMinimum(1);
        hideDelay->setValue(3);

        horizontalLayout->addWidget(hideDelay);


        formLayout_5->setLayout(9, QFormLayout::FieldRole, horizontalLayout);

        disableSS = new QCheckBox(play);
        disableSS->setObjectName(QString::fromUtf8("disableSS"));

        formLayout_5->setWidget(10, QFormLayout::FieldRole, disableSS);

        autoAdd = new QComboBox(play);
        autoAdd->setObjectName(QString::fromUtf8("autoAdd"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(autoAdd->sizePolicy().hasHeightForWidth());
        autoAdd->setSizePolicy(sizePolicy);
        autoAdd->setFocusPolicy(Qt::TabFocus);

        formLayout_5->setWidget(6, QFormLayout::FieldRole, autoAdd);


        horizontalLayout_2->addLayout(formLayout_5);

        tabWidget_2->addTab(play, QString());
        video = new QWidget();
        video->setObjectName(QString::fromUtf8("video"));
        horizontalLayout_13 = new QHBoxLayout(video);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        formLayout_6 = new QFormLayout();
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        formLayout_6->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        label_30 = new QLabel(video);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_30);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_31 = new QLabel(video);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        horizontalLayout_3->addWidget(label_31);

        blur_c = new QSpinBox(video);
        blur_c->setObjectName(QString::fromUtf8("blur_c"));
        blur_c->setMinimum(-99);

        horizontalLayout_3->addWidget(blur_c);

        label_32 = new QLabel(video);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        horizontalLayout_3->addWidget(label_32);

        blur_n = new QSpinBox(video);
        blur_n->setObjectName(QString::fromUtf8("blur_n"));
        blur_n->setMinimum(-99);

        horizontalLayout_3->addWidget(blur_n);

        label_33 = new QLabel(video);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        horizontalLayout_3->addWidget(label_33);

        blur_d = new QSpinBox(video);
        blur_d->setObjectName(QString::fromUtf8("blur_d"));
        blur_d->setMinimum(-99);

        horizontalLayout_3->addWidget(blur_d);


        formLayout_6->setLayout(0, QFormLayout::FieldRole, horizontalLayout_3);

        label_34 = new QLabel(video);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        formLayout_6->setWidget(1, QFormLayout::LabelRole, label_34);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_35 = new QLabel(video);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        horizontalLayout_4->addWidget(label_35);

        sharpen_c = new QSpinBox(video);
        sharpen_c->setObjectName(QString::fromUtf8("sharpen_c"));
        sharpen_c->setMinimum(-99);

        horizontalLayout_4->addWidget(sharpen_c);

        label_36 = new QLabel(video);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        horizontalLayout_4->addWidget(label_36);

        sharpen_n = new QSpinBox(video);
        sharpen_n->setObjectName(QString::fromUtf8("sharpen_n"));
        sharpen_n->setMinimum(-99);

        horizontalLayout_4->addWidget(sharpen_n);

        label_37 = new QLabel(video);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        horizontalLayout_4->addWidget(label_37);

        sharpen_d = new QSpinBox(video);
        sharpen_d->setObjectName(QString::fromUtf8("sharpen_d"));
        sharpen_d->setMinimum(-99);

        horizontalLayout_4->addWidget(sharpen_d);


        formLayout_6->setLayout(1, QFormLayout::FieldRole, horizontalLayout_4);

        label_27 = new QLabel(video);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        formLayout_6->setWidget(2, QFormLayout::LabelRole, label_27);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_28 = new QLabel(video);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        horizontalLayout_11->addWidget(label_28);

        min_luma = new QSpinBox(video);
        min_luma->setObjectName(QString::fromUtf8("min_luma"));
        min_luma->setAccelerated(true);
        min_luma->setMaximum(127);

        horizontalLayout_11->addWidget(min_luma);

        label_29 = new QLabel(video);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        horizontalLayout_11->addWidget(label_29);

        max_luma = new QSpinBox(video);
        max_luma->setObjectName(QString::fromUtf8("max_luma"));
        max_luma->setAccelerated(true);
        max_luma->setMinimum(128);
        max_luma->setMaximum(255);

        horizontalLayout_11->addWidget(max_luma);


        formLayout_6->setLayout(2, QFormLayout::FieldRole, horizontalLayout_11);

        label_25 = new QLabel(video);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        formLayout_6->setWidget(3, QFormLayout::LabelRole, label_25);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_26 = new QLabel(video);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        horizontalLayout_12->addWidget(label_26);

        auto_contrast_th = new QDoubleSpinBox(video);
        auto_contrast_th->setObjectName(QString::fromUtf8("auto_contrast_th"));
        auto_contrast_th->setAccelerated(true);
        auto_contrast_th->setSuffix(QString::fromUtf8("%"));
        auto_contrast_th->setMaximum(49.99);
        auto_contrast_th->setSingleStep(0.1);

        horizontalLayout_12->addWidget(auto_contrast_th);


        formLayout_6->setLayout(3, QFormLayout::FieldRole, horizontalLayout_12);


        horizontalLayout_13->addLayout(formLayout_6);

        tabWidget_2->addTab(video, QString());

        _4->addWidget(tabWidget_2);

        stack->addWidget(general);
        subtitle = new QWidget();
        subtitle->setObjectName(QString::fromUtf8("subtitle"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/subtitle.png"), QSize(), QIcon::Normal, QIcon::Off);
        subtitle->setProperty("icon", QVariant(icon1));
        horizontalLayout_7 = new QHBoxLayout(subtitle);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        tabs = new QTabWidget(subtitle);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        widget_3 = new QWidget();
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_5 = new QHBoxLayout(widget_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        aLabel_2 = new QLabel(widget_3);
        aLabel_2->setObjectName(QString::fromUtf8("aLabel_2"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, aLabel_2);

        autoLoad = new QComboBox(widget_3);
        autoLoad->setObjectName(QString::fromUtf8("autoLoad"));
        autoLoad->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, autoLoad);

        line_9 = new QFrame(widget_3);
        line_9->setObjectName(QString::fromUtf8("line_9"));
        line_9->setFrameShadow(QFrame::Plain);
        line_9->setLineWidth(1);
        line_9->setFrameShape(QFrame::HLine);

        formLayout_2->setWidget(1, QFormLayout::SpanningRole, line_9);

        aLabel_3 = new QLabel(widget_3);
        aLabel_3->setObjectName(QString::fromUtf8("aLabel_3"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, aLabel_3);

        autoSelect = new QComboBox(widget_3);
        autoSelect->setObjectName(QString::fromUtf8("autoSelect"));
        autoSelect->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, autoSelect);

        subExtLabel = new QLabel(widget_3);
        subExtLabel->setObjectName(QString::fromUtf8("subExtLabel"));
        subExtLabel->setEnabled(false);

        formLayout_2->setWidget(3, QFormLayout::LabelRole, subExtLabel);

        subSelectExt = new QComboBox(widget_3);
        subSelectExt->setObjectName(QString::fromUtf8("subSelectExt"));
        subSelectExt->setEnabled(false);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, subSelectExt);

        line_11 = new QFrame(widget_3);
        line_11->setObjectName(QString::fromUtf8("line_11"));
        line_11->setFrameShadow(QFrame::Plain);
        line_11->setFrameShape(QFrame::HLine);

        formLayout_2->setWidget(4, QFormLayout::SpanningRole, line_11);

        aLabel_4 = new QLabel(widget_3);
        aLabel_4->setObjectName(QString::fromUtf8("aLabel_4"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, aLabel_4);

        encoding = new EncodingComboBox(widget_3);
        encoding->setObjectName(QString::fromUtf8("encoding"));
        encoding->setEditable(true);
        encoding->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        formLayout_2->setWidget(5, QFormLayout::FieldRole, encoding);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        subEncAutoDet = new QCheckBox(widget_3);
        subEncAutoDet->setObjectName(QString::fromUtf8("subEncAutoDet"));

        horizontalLayout_8->addWidget(subEncAutoDet);

        subConf = new QSpinBox(widget_3);
        subConf->setObjectName(QString::fromUtf8("subConf"));
        subConf->setEnabled(false);
        subConf->setAccelerated(true);
        subConf->setMinimum(1);
        subConf->setMaximum(100);

        horizontalLayout_8->addWidget(subConf);

        horizontalSpacer_11 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_11);


        formLayout_2->setLayout(6, QFormLayout::FieldRole, horizontalLayout_8);

        label_4 = new QLabel(widget_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_4);


        horizontalLayout_5->addLayout(formLayout_2);

        tabs->addTab(widget_3, QString());
        widget_2 = new QWidget();
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_24 = new QHBoxLayout(widget_2);
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_2);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        horizontalLayout_19->setContentsMargins(-1, 0, -1, 0);
        sub_font_label = new QLabel(widget_2);
        sub_font_label->setObjectName(QString::fromUtf8("sub_font_label"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sub_font_label->sizePolicy().hasHeightForWidth());
        sub_font_label->setSizePolicy(sizePolicy1);
        sub_font_label->setMinimumSize(QSize(150, 0));
        sub_font_label->setFrameShape(QFrame::StyledPanel);
        sub_font_label->setFrameShadow(QFrame::Sunken);
        sub_font_label->setText(QString::fromUtf8(""));
        sub_font_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_19->addWidget(sub_font_label);

        sub_font_button = new QPushButton(widget_2);
        sub_font_button->setObjectName(QString::fromUtf8("sub_font_button"));

        horizontalLayout_19->addWidget(sub_font_button);


        formLayout_3->setLayout(0, QFormLayout::FieldRole, horizontalLayout_19);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_3);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        horizontalLayout_20->setContentsMargins(-1, 0, -1, 0);
        sub_color_fg_label = new QLabel(widget_2);
        sub_color_fg_label->setObjectName(QString::fromUtf8("sub_color_fg_label"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sub_color_fg_label->sizePolicy().hasHeightForWidth());
        sub_color_fg_label->setSizePolicy(sizePolicy2);
        sub_color_fg_label->setMinimumSize(QSize(150, 0));
        sub_color_fg_label->setAutoFillBackground(true);
        sub_color_fg_label->setFrameShape(QFrame::StyledPanel);
        sub_color_fg_label->setFrameShadow(QFrame::Sunken);
        sub_color_fg_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_20->addWidget(sub_color_fg_label);

        sub_color_fg_button = new QPushButton(widget_2);
        sub_color_fg_button->setObjectName(QString::fromUtf8("sub_color_fg_button"));

        horizontalLayout_20->addWidget(sub_color_fg_button);


        formLayout_3->setLayout(1, QFormLayout::FieldRole, horizontalLayout_20);

        label_5 = new QLabel(widget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_5);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        sub_color_bg_label = new QLabel(widget_2);
        sub_color_bg_label->setObjectName(QString::fromUtf8("sub_color_bg_label"));
        sizePolicy2.setHeightForWidth(sub_color_bg_label->sizePolicy().hasHeightForWidth());
        sub_color_bg_label->setSizePolicy(sizePolicy2);
        sub_color_bg_label->setMinimumSize(QSize(150, 0));
        sub_color_bg_label->setAutoFillBackground(true);
        sub_color_bg_label->setFrameShape(QFrame::StyledPanel);
        sub_color_bg_label->setFrameShadow(QFrame::Sunken);
        sub_color_bg_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_18->addWidget(sub_color_bg_label);

        sub_color_bg_button = new QPushButton(widget_2);
        sub_color_bg_button->setObjectName(QString::fromUtf8("sub_color_bg_button"));

        horizontalLayout_18->addWidget(sub_color_bg_button);


        formLayout_3->setLayout(2, QFormLayout::FieldRole, horizontalLayout_18);

        label_21 = new QLabel(widget_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_21);

        sub_auto_size = new QComboBox(widget_2);
        sub_auto_size->setObjectName(QString::fromUtf8("sub_auto_size"));
        sub_auto_size->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, sub_auto_size);

        label_22 = new QLabel(widget_2);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        formLayout_3->setWidget(4, QFormLayout::LabelRole, label_22);

        sub_size_scale = new QDoubleSpinBox(widget_2);
        sub_size_scale->setObjectName(QString::fromUtf8("sub_size_scale"));
        sub_size_scale->setAccelerated(true);
        sub_size_scale->setDecimals(1);
        sub_size_scale->setMaximum(99);
        sub_size_scale->setSingleStep(0.5);
        sub_size_scale->setValue(10);

        formLayout_3->setWidget(4, QFormLayout::FieldRole, sub_size_scale);

        line_10 = new QFrame(widget_2);
        line_10->setObjectName(QString::fromUtf8("line_10"));
        line_10->setFrameShadow(QFrame::Plain);
        line_10->setFrameShape(QFrame::HLine);

        formLayout_3->setWidget(5, QFormLayout::SpanningRole, line_10);

        aLabel_6 = new QLabel(widget_2);
        aLabel_6->setObjectName(QString::fromUtf8("aLabel_6"));

        formLayout_3->setWidget(6, QFormLayout::LabelRole, aLabel_6);

        msPerChar = new QSpinBox(widget_2);
        msPerChar->setObjectName(QString::fromUtf8("msPerChar"));
        msPerChar->setAccelerated(true);
        msPerChar->setMinimum(1);
        msPerChar->setMaximum(99999);
        msPerChar->setSingleStep(100);
        msPerChar->setValue(500);

        formLayout_3->setWidget(6, QFormLayout::FieldRole, msPerChar);


        horizontalLayout_24->addLayout(formLayout_3);

        tabs->addTab(widget_2, QString());
        widget_4 = new QWidget();
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        verticalLayout_13 = new QVBoxLayout(widget_4);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        label_7 = new QLabel(widget_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setWordWrap(true);

        verticalLayout_13->addWidget(label_7);

        priority = new SimpleListWidget(widget_4);
        priority->setObjectName(QString::fromUtf8("priority"));

        verticalLayout_13->addWidget(priority);

        spacer_2 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(spacer_2);

        tabs->addTab(widget_4, QString());

        horizontalLayout_7->addWidget(tabs);

        stack->addWidget(subtitle);
        ui = new QWidget();
        ui->setObjectName(QString::fromUtf8("ui"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/multimedia-volume-control.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->setProperty("icon", QVariant(icon2));
        verticalLayout_2 = new QVBoxLayout(ui);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tabWidget = new QTabWidget(ui);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_15 = new QVBoxLayout(tab_4);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        singleApp = new QCheckBox(tab_4);
        singleApp->setObjectName(QString::fromUtf8("singleApp"));

        formLayout->setWidget(1, QFormLayout::FieldRole, singleApp);

        line_14 = new QFrame(tab_4);
        line_14->setObjectName(QString::fromUtf8("line_14"));
        line_14->setFrameShadow(QFrame::Plain);
        line_14->setFrameShape(QFrame::HLine);

        formLayout->setWidget(2, QFormLayout::SpanningRole, line_14);

        localeLabel = new QLabel(tab_4);
        localeLabel->setObjectName(QString::fromUtf8("localeLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, localeLabel);

        locale = new QComboBox(tab_4);
        locale->setObjectName(QString::fromUtf8("locale"));
        sizePolicy.setHeightForWidth(locale->sizePolicy().hasHeightForWidth());
        locale->setSizePolicy(sizePolicy);

        formLayout->setWidget(3, QFormLayout::FieldRole, locale);

        wLabel = new QLabel(tab_4);
        wLabel->setObjectName(QString::fromUtf8("wLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, wLabel);

        winStyle = new QComboBox(tab_4);
        winStyle->setObjectName(QString::fromUtf8("winStyle"));

        formLayout->setWidget(4, QFormLayout::FieldRole, winStyle);

        aLabel = new QLabel(tab_4);
        aLabel->setObjectName(QString::fromUtf8("aLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, aLabel);

        tray = new QCheckBox(tab_4);
        tray->setObjectName(QString::fromUtf8("tray"));

        formLayout->setWidget(6, QFormLayout::FieldRole, tray);

        line_15 = new QFrame(tab_4);
        line_15->setObjectName(QString::fromUtf8("line_15"));
        line_15->setFrameShadow(QFrame::Plain);
        line_15->setFrameShape(QFrame::HLine);

        formLayout->setWidget(5, QFormLayout::SpanningRole, line_15);

        hideWhenClosed = new QCheckBox(tab_4);
        hideWhenClosed->setObjectName(QString::fromUtf8("hideWhenClosed"));
        hideWhenClosed->setEnabled(false);

        formLayout->setWidget(7, QFormLayout::FieldRole, hideWhenClosed);

        label_43 = new QLabel(tab_4);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_43);


        verticalLayout_15->addLayout(formLayout);

        tabWidget->addTab(tab_4, QString());
        widget_5 = new QWidget();
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        verticalLayout_3 = new QVBoxLayout(widget_5);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        shortcutTree = new QTreeWidget(widget_5);
        shortcutTree->setObjectName(QString::fromUtf8("shortcutTree"));
        shortcutTree->setAlternatingRowColors(true);

        verticalLayout_3->addWidget(shortcutTree);

        _3 = new QHBoxLayout();
        _3->setObjectName(QString::fromUtf8("_3"));
        label = new QLabel(widget_5);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy3);

        _3->addWidget(label);

        shortcut1 = new QPushButton(widget_5);
        shortcut1->setObjectName(QString::fromUtf8("shortcut1"));
        sizePolicy.setHeightForWidth(shortcut1->sizePolicy().hasHeightForWidth());
        shortcut1->setSizePolicy(sizePolicy);

        _3->addWidget(shortcut1);

        shortcut2 = new QPushButton(widget_5);
        shortcut2->setObjectName(QString::fromUtf8("shortcut2"));
        sizePolicy.setHeightForWidth(shortcut2->sizePolicy().hasHeightForWidth());
        shortcut2->setSizePolicy(sizePolicy);

        _3->addWidget(shortcut2);

        shortcut3 = new QPushButton(widget_5);
        shortcut3->setObjectName(QString::fromUtf8("shortcut3"));
        sizePolicy.setHeightForWidth(shortcut3->sizePolicy().hasHeightForWidth());
        shortcut3->setSizePolicy(sizePolicy);

        _3->addWidget(shortcut3);

        shortcut4 = new QPushButton(widget_5);
        shortcut4->setObjectName(QString::fromUtf8("shortcut4"));
        sizePolicy.setHeightForWidth(shortcut4->sizePolicy().hasHeightForWidth());
        shortcut4->setSizePolicy(sizePolicy);

        _3->addWidget(shortcut4);


        verticalLayout_3->addLayout(_3);

        tabWidget->addTab(widget_5, QString());
        mouseWidget = new QWidget();
        mouseWidget->setObjectName(QString::fromUtf8("mouseWidget"));
        horizontalLayout_9 = new QHBoxLayout(mouseWidget);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        mouse_form = new QFormLayout();
        mouse_form->setObjectName(QString::fromUtf8("mouse_form"));

        horizontalLayout_9->addLayout(mouse_form);

        tabWidget->addTab(mouseWidget, QString());
        widget_7 = new QWidget();
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        horizontalLayout_6 = new QHBoxLayout(widget_7);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setHorizontalSpacing(11);
        formLayout_4->setVerticalSpacing(11);
        aLabel_5 = new QLabel(widget_7);
        aLabel_5->setObjectName(QString::fromUtf8("aLabel_5"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, aLabel_5);

        seek1 = new QSpinBox(widget_7);
        seek1->setObjectName(QString::fromUtf8("seek1"));
        seek1->setMinimum(1);
        seek1->setMaximum(300);
        seek1->setValue(5);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, seek1);

        label_16 = new QLabel(widget_7);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_16);

        seek2 = new QSpinBox(widget_7);
        seek2->setObjectName(QString::fromUtf8("seek2"));
        seek2->setMinimum(1);
        seek2->setMaximum(600);
        seek2->setValue(30);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, seek2);

        line_12 = new QFrame(widget_7);
        line_12->setObjectName(QString::fromUtf8("line_12"));
        line_12->setFrameShadow(QFrame::Plain);
        line_12->setFrameShape(QFrame::HLine);

        formLayout_4->setWidget(2, QFormLayout::SpanningRole, line_12);

        label_12 = new QLabel(widget_7);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, label_12);

        spinBox = new QSpinBox(widget_7);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setValue(1);

        formLayout_4->setWidget(3, QFormLayout::FieldRole, spinBox);

        label_14 = new QLabel(widget_7);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, label_14);

        spinBox_3 = new QSpinBox(widget_7);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setMinimum(1);
        spinBox_3->setValue(1);

        formLayout_4->setWidget(4, QFormLayout::FieldRole, spinBox_3);

        line_13 = new QFrame(widget_7);
        line_13->setObjectName(QString::fromUtf8("line_13"));
        line_13->setFrameShadow(QFrame::Plain);
        line_13->setFrameShape(QFrame::HLine);

        formLayout_4->setWidget(5, QFormLayout::SpanningRole, line_13);

        label_18 = new QLabel(widget_7);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_4->setWidget(6, QFormLayout::LabelRole, label_18);

        volume = new QSpinBox(widget_7);
        volume->setObjectName(QString::fromUtf8("volume"));
        volume->setMinimum(1);
        volume->setMaximum(50);
        volume->setValue(2);

        formLayout_4->setWidget(6, QFormLayout::FieldRole, volume);

        label_19 = new QLabel(widget_7);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_4->setWidget(7, QFormLayout::LabelRole, label_19);

        amp = new QSpinBox(widget_7);
        amp->setObjectName(QString::fromUtf8("amp"));
        amp->setMinimum(1);
        amp->setMaximum(50);
        amp->setValue(10);

        formLayout_4->setWidget(7, QFormLayout::FieldRole, amp);


        horizontalLayout_6->addLayout(formLayout_4);

        formLayout_7 = new QFormLayout();
        formLayout_7->setObjectName(QString::fromUtf8("formLayout_7"));
        formLayout_7->setHorizontalSpacing(11);
        formLayout_7->setVerticalSpacing(11);
        label_9 = new QLabel(widget_7);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_7->setWidget(2, QFormLayout::LabelRole, label_9);

        speed = new QSpinBox(widget_7);
        speed->setObjectName(QString::fromUtf8("speed"));
        speed->setMinimum(1);
        speed->setValue(10);

        formLayout_7->setWidget(2, QFormLayout::FieldRole, speed);

        label_17 = new QLabel(widget_7);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        formLayout_7->setWidget(1, QFormLayout::LabelRole, label_17);

        seek3 = new QSpinBox(widget_7);
        seek3->setObjectName(QString::fromUtf8("seek3"));
        seek3->setMinimum(1);
        seek3->setMaximum(600);
        seek3->setValue(60);

        formLayout_7->setWidget(1, QFormLayout::FieldRole, seek3);

        label_13 = new QLabel(widget_7);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout_7->setWidget(4, QFormLayout::LabelRole, label_13);

        label_20 = new QLabel(widget_7);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        formLayout_7->setWidget(5, QFormLayout::LabelRole, label_20);

        spinBox_2 = new QSpinBox(widget_7);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMinimum(1);
        spinBox_2->setValue(1);

        formLayout_7->setWidget(4, QFormLayout::FieldRole, spinBox_2);

        spinBox_4 = new QSpinBox(widget_7);
        spinBox_4->setObjectName(QString::fromUtf8("spinBox_4"));
        spinBox_4->setMinimum(1);
        spinBox_4->setValue(1);

        formLayout_7->setWidget(5, QFormLayout::FieldRole, spinBox_4);

        label_10 = new QLabel(widget_7);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout_7->setWidget(7, QFormLayout::LabelRole, label_10);

        subPos = new QSpinBox(widget_7);
        subPos->setObjectName(QString::fromUtf8("subPos"));
        subPos->setMinimum(1);
        subPos->setValue(1);

        formLayout_7->setWidget(7, QFormLayout::FieldRole, subPos);

        label_11 = new QLabel(widget_7);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout_7->setWidget(8, QFormLayout::LabelRole, label_11);

        sync = new QDoubleSpinBox(widget_7);
        sync->setObjectName(QString::fromUtf8("sync"));
        sync->setMinimum(0.01);
        sync->setSingleStep(0.1);
        sync->setValue(0.5);

        formLayout_7->setWidget(8, QFormLayout::FieldRole, sync);

        line_4 = new QFrame(widget_7);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShadow(QFrame::Plain);
        line_4->setFrameShape(QFrame::HLine);

        formLayout_7->setWidget(3, QFormLayout::SpanningRole, line_4);

        line_5 = new QFrame(widget_7);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShadow(QFrame::Plain);
        line_5->setFrameShape(QFrame::HLine);

        formLayout_7->setWidget(6, QFormLayout::SpanningRole, line_5);


        horizontalLayout_6->addLayout(formLayout_7);

        tabWidget->addTab(widget_7, QString());

        verticalLayout_2->addWidget(tabWidget);

        stack->addWidget(ui);

        verticalLayout_14->addWidget(stack);

        QWidget::setTabOrder(pauseMinimized, pauseVideoOnly);
        QWidget::setTabOrder(pauseVideoOnly, startStopped);
        QWidget::setTabOrder(startStopped, askWhenRecordFound);
        QWidget::setTabOrder(askWhenRecordFound, hideDelay);
        QWidget::setTabOrder(hideDelay, subConf);
        QWidget::setTabOrder(subConf, shortcutTree);
        QWidget::setTabOrder(shortcutTree, shortcut1);
        QWidget::setTabOrder(shortcut1, shortcut2);
        QWidget::setTabOrder(shortcut2, shortcut3);
        QWidget::setTabOrder(shortcut3, shortcut4);

        retranslateUi(Pref_Widget);
        QObject::connect(subEncAutoDet, SIGNAL(toggled(bool)), subConf, SLOT(setEnabled(bool)));
        QObject::connect(hideCursor, SIGNAL(toggled(bool)), hideDelay, SLOT(setEnabled(bool)));
        QObject::connect(startStopped, SIGNAL(toggled(bool)), askWhenRecordFound, SLOT(setEnabled(bool)));
        QObject::connect(pauseMinimized, SIGNAL(toggled(bool)), pauseVideoOnly, SLOT(setEnabled(bool)));
        QObject::connect(tray, SIGNAL(toggled(bool)), hideWhenClosed, SLOT(setEnabled(bool)));

        stack->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        tabs->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Pref_Widget);
    } // setupUi

    void retranslateUi(QWidget *Pref_Widget)
    {
        Pref_Widget->setWindowTitle(QApplication::translate("Pref_Widget", "Preferences", 0, QApplication::UnicodeUTF8));
        general->setProperty("name", QVariant(QApplication::translate("Pref_Widget", "Play", 0, QApplication::UnicodeUTF8)));
        pauseMinimized->setText(QApplication::translate("Pref_Widget", "Pause when minimized", 0, QApplication::UnicodeUTF8));
        pauseVideoOnly->setText(QApplication::translate("Pref_Widget", "Apply on video only", 0, QApplication::UnicodeUTF8));
        startStopped->setText(QApplication::translate("Pref_Widget", "When a file is stopped, play it from there next time", 0, QApplication::UnicodeUTF8));
        askWhenRecordFound->setText(QApplication::translate("Pref_Widget", "Ask when stopped record found", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("Pref_Widget", "Generate Playlist:", 0, QApplication::UnicodeUTF8));
        hideCursor->setText(QApplication::translate("Pref_Widget", "Hide cursor when it doesn't move during ", 0, QApplication::UnicodeUTF8));
        hideDelay->setSuffix(QApplication::translate("Pref_Widget", "sec", 0, QApplication::UnicodeUTF8));
        disableSS->setText(QApplication::translate("Pref_Widget", "Disable screensaver while playing", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        autoAdd->setToolTip(QApplication::translate("Pref_Widget", "Add next files to playlist when a file is open", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        tabWidget_2->setTabText(tabWidget_2->indexOf(play), QApplication::translate("Pref_Widget", "Play", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("Pref_Widget", "Blur Kernel:", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("Pref_Widget", "Center:", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("Pref_Widget", "Neighbor:", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("Pref_Widget", "Diagonal:", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("Pref_Widget", "Sharpen Kernel:", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("Pref_Widget", "Center:", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("Pref_Widget", "Neighbor:", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("Pref_Widget", "Diagonal:", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("Pref_Widget", "Adjust Contrast:", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("Pref_Widget", "Min. Luma:", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("Pref_Widget", "Max. Luma:", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("Pref_Widget", "Auto Contrast:", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("Pref_Widget", "Threshold for auto contrast:", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(video), QApplication::translate("Pref_Widget", "Video", 0, QApplication::UnicodeUTF8));
        subtitle->setProperty("name", QVariant(QApplication::translate("Pref_Widget", "Subtitle", 0, QApplication::UnicodeUTF8)));
        aLabel_2->setText(QApplication::translate("Pref_Widget", "Auto-load:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        autoLoad->setToolTip(QApplication::translate("Pref_Widget", "Load next subtitles when a file to play is open", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        aLabel_3->setText(QApplication::translate("Pref_Widget", "Auto-select:", 0, QApplication::UnicodeUTF8));
        subExtLabel->setText(QApplication::translate("Pref_Widget", "Extension:", 0, QApplication::UnicodeUTF8));
        aLabel_4->setText(QApplication::translate("Pref_Widget", "Encoding:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        encoding->setToolTip(QApplication::translate("Pref_Widget", "Open subtitle file with next encoding", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        subEncAutoDet->setText(QApplication::translate("Pref_Widget", "Enable when the accuracy is over", 0, QApplication::UnicodeUTF8));
        subConf->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Pref_Widget", "Auto-dectection:", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(widget_3), QApplication::translate("Pref_Widget", "Load", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Pref_Widget", "Font:", 0, QApplication::UnicodeUTF8));
        sub_font_button->setText(QApplication::translate("Pref_Widget", "Change", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Pref_Widget", "Foreground Color:", 0, QApplication::UnicodeUTF8));
        sub_color_fg_label->setText(QString());
        sub_color_fg_button->setText(QApplication::translate("Pref_Widget", "Change", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Pref_Widget", "Background Color:", 0, QApplication::UnicodeUTF8));
        sub_color_bg_label->setText(QString());
        sub_color_bg_button->setText(QApplication::translate("Pref_Widget", "Change", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("Pref_Widget", "Auto Size:", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("Pref_Widget", "Size Scale:", 0, QApplication::UnicodeUTF8));
        sub_size_scale->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        aLabel_6->setText(QApplication::translate("Pref_Widget", "Display Time:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        msPerChar->setToolTip(QApplication::translate("Pref_Widget", "Next value will be used to calculate the end time of a subtitle line when it is not specified by subtitle file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        msPerChar->setSuffix(QApplication::translate("Pref_Widget", "msec/character", 0, QApplication::UnicodeUTF8));
        msPerChar->setPrefix(QString());
        tabs->setTabText(tabs->indexOf(widget_2), QApplication::translate("Pref_Widget", "Display", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Pref_Widget", "You can specify the order of unified(mulit) subtitles with the 'Name' property in subtitle file.\n"
"If not specified, They will be displayed in selected order.\n"
"e.g.: English, Korean", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(widget_4), QApplication::translate("Pref_Widget", "Unified/Multi-Subtitle", 0, QApplication::UnicodeUTF8));
        ui->setProperty("name", QVariant(QApplication::translate("Pref_Widget", "UI", 0, QApplication::UnicodeUTF8)));
        singleApp->setText(QApplication::translate("Pref_Widget", "Run only single CMPlayer application at the same time", 0, QApplication::UnicodeUTF8));
        localeLabel->setText(QApplication::translate("Pref_Widget", "Locale", 0, QApplication::UnicodeUTF8));
        wLabel->setText(QApplication::translate("Pref_Widget", "Window Style", 0, QApplication::UnicodeUTF8));
        aLabel->setText(QApplication::translate("Pref_Widget", "System Tray", 0, QApplication::UnicodeUTF8));
        tray->setText(QApplication::translate("Pref_Widget", "Enable system tray icon", 0, QApplication::UnicodeUTF8));
        hideWhenClosed->setText(QApplication::translate("Pref_Widget", "Hide window rather than quit when closed ", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("Pref_Widget", "(Linux Only)", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Pref_Widget", "Application", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = shortcutTree->headerItem();
        ___qtreewidgetitem->setText(4, QApplication::translate("Pref_Widget", "Shortcut4", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(3, QApplication::translate("Pref_Widget", "Shortcut3", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("Pref_Widget", "Shortcut2", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("Pref_Widget", "Shortcut1", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("Pref_Widget", "Menu", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Pref_Widget", "Change:", 0, QApplication::UnicodeUTF8));
        shortcut1->setText(QApplication::translate("Pref_Widget", "Shortcut1", 0, QApplication::UnicodeUTF8));
        shortcut2->setText(QApplication::translate("Pref_Widget", "Shortcut2", 0, QApplication::UnicodeUTF8));
        shortcut3->setText(QApplication::translate("Pref_Widget", "Shortcut3", 0, QApplication::UnicodeUTF8));
        shortcut4->setText(QApplication::translate("Pref_Widget", "Shortcut4", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(widget_5), QApplication::translate("Pref_Widget", "Shortcut", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(mouseWidget), QApplication::translate("Pref_Widget", "Mouse", 0, QApplication::UnicodeUTF8));
        aLabel_5->setText(QApplication::translate("Pref_Widget", "Seek Step1:", 0, QApplication::UnicodeUTF8));
        seek1->setSuffix(QApplication::translate("Pref_Widget", "sec", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("Pref_Widget", "Seek Step 2:", 0, QApplication::UnicodeUTF8));
        seek2->setSuffix(QApplication::translate("Pref_Widget", "sec", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Pref_Widget", "Brightness Step:", 0, QApplication::UnicodeUTF8));
        spinBox->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Pref_Widget", "Contrast Step:", 0, QApplication::UnicodeUTF8));
        spinBox_3->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Pref_Widget", "Volum Step:", 0, QApplication::UnicodeUTF8));
        volume->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("Pref_Widget", "Amp. Step:", 0, QApplication::UnicodeUTF8));
        amp->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Pref_Widget", "Speed Step:", 0, QApplication::UnicodeUTF8));
        speed->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Pref_Widget", "Seek Step 3:", 0, QApplication::UnicodeUTF8));
        seek3->setSuffix(QApplication::translate("Pref_Widget", "sec", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Pref_Widget", "Saturation Step:", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("Pref_Widget", "Hue Step:", 0, QApplication::UnicodeUTF8));
        spinBox_2->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        spinBox_4->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Pref_Widget", "Position Step:", 0, QApplication::UnicodeUTF8));
        subPos->setSuffix(QApplication::translate("Pref_Widget", "%", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Pref_Widget", "Sync Delay Step:", 0, QApplication::UnicodeUTF8));
        sync->setSuffix(QApplication::translate("Pref_Widget", "sec", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(widget_7), QApplication::translate("Pref_Widget", "Control Step", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Pref_Widget: public Ui_Pref_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREF_WIDGET_H
