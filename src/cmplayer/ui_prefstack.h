/********************************************************************************
** Form generated from reading ui file 'prefstack.ui'
**
** Created: Thu Mar 12 09:58:05 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PREFSTACK_H
#define UI_PREFSTACK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "encodingcombobox.h"
#include "prefosdwidget.h"
#include "simplelistwidget.h"

QT_BEGIN_NAMESPACE

class Ui_PrefStack
{
public:
    QWidget *general;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout;
    QLabel *label_6;
    QComboBox *autoAdd;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *pauseMinimized;
    QHBoxLayout *_2;
    QSpacerItem *spacer;
    QCheckBox *playRestored;
    QFrame *line;
    QCheckBox *startStopped;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_5;
    QLabel *label_22;
    QComboBox *lang;
    QSpacerItem *spacer_8;
    QWidget *subtitle;
    QVBoxLayout *verticalLayout_12;
    QTabWidget *tabs;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_2;
    QComboBox *autoLoad;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_3;
    QComboBox *autoSelect;
    QGroupBox *auto_load_group;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_5;
    EncodingComboBox *encoding;
    QSpacerItem *spacer_9;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_10;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_11;
    PrefOsdWidget *subOsd;
    QSpacerItem *verticalSpacer_4;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_7;
    SimpleListWidget *priority;
    QSpacerItem *spacer_2;
    QWidget *iface;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *shortcutTree;
    QHBoxLayout *_3;
    QLabel *label;
    QPushButton *shortcut1;
    QPushButton *shortcut2;
    QPushButton *shortcut3;
    QPushButton *shortcut4;
    QSpacerItem *spacer_10;
    QWidget *widget_6;
    QVBoxLayout *mouseLayout;
    QSpacerItem *verticalSpacer;
    QWidget *widget_7;
    QVBoxLayout *verticalLayout_21;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_15;
    QSpinBox *seek1;
    QLabel *label_16;
    QSpinBox *seek2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_17;
    QSpinBox *seek3;
    QLabel *label_9;
    QSpinBox *speed;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_2;
    QLabel *label_18;
    QSpinBox *volume;
    QLabel *label_19;
    QSpinBox *amp;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_13;
    QGridLayout *gridLayout_4;
    QLabel *label_12;
    QSpinBox *spinBox;
    QLabel *label_14;
    QSpinBox *spinBox_3;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_13;
    QSpinBox *spinBox_2;
    QLabel *label_20;
    QSpinBox *spinBox_4;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_3;
    QLabel *label_10;
    QSpinBox *subPos;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_3;
    QDoubleSpinBox *sync;
    QSpacerItem *spacer_7;
    QWidget *widget;
    QVBoxLayout *verticalLayout_16;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_4;
    QHBoxLayout *_11;
    QLineEdit *pathEdit;
    QPushButton *pathButton;
    QLabel *label_21;
    QGroupBox *groupBox_8;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_8;
    QTreeWidget *media;

    void setupUi(QStackedWidget *PrefStack)
    {
    if (PrefStack->objectName().isEmpty())
        PrefStack->setObjectName(QString::fromUtf8("PrefStack"));
    PrefStack->resize(491, 401);
    general = new QWidget();
    general->setObjectName(QString::fromUtf8("general"));
    verticalLayout_5 = new QVBoxLayout(general);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    groupBox_4 = new QGroupBox(general);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    verticalLayout = new QVBoxLayout(groupBox_4);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    label_6 = new QLabel(groupBox_4);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    verticalLayout->addWidget(label_6);

    autoAdd = new QComboBox(groupBox_4);
    autoAdd->setObjectName(QString::fromUtf8("autoAdd"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(autoAdd->sizePolicy().hasHeightForWidth());
    autoAdd->setSizePolicy(sizePolicy);

    verticalLayout->addWidget(autoAdd);


    verticalLayout_5->addWidget(groupBox_4);

    groupBox_5 = new QGroupBox(general);
    groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
    verticalLayout_4 = new QVBoxLayout(groupBox_5);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    pauseMinimized = new QCheckBox(groupBox_5);
    pauseMinimized->setObjectName(QString::fromUtf8("pauseMinimized"));

    verticalLayout_4->addWidget(pauseMinimized);

    _2 = new QHBoxLayout();
    _2->setObjectName(QString::fromUtf8("_2"));
    spacer = new QSpacerItem(15, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

    _2->addItem(spacer);

    playRestored = new QCheckBox(groupBox_5);
    playRestored->setObjectName(QString::fromUtf8("playRestored"));
    playRestored->setEnabled(false);
    playRestored->setChecked(true);

    _2->addWidget(playRestored);


    verticalLayout_4->addLayout(_2);

    line = new QFrame(groupBox_5);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    verticalLayout_4->addWidget(line);

    startStopped = new QCheckBox(groupBox_5);
    startStopped->setObjectName(QString::fromUtf8("startStopped"));

    verticalLayout_4->addWidget(startStopped);


    verticalLayout_5->addWidget(groupBox_5);

    groupBox_9 = new QGroupBox(general);
    groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
    gridLayout_5 = new QGridLayout(groupBox_9);
    gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
    label_22 = new QLabel(groupBox_9);
    label_22->setObjectName(QString::fromUtf8("label_22"));

    gridLayout_5->addWidget(label_22, 0, 0, 1, 1);

    lang = new QComboBox(groupBox_9);
    lang->setObjectName(QString::fromUtf8("lang"));
    sizePolicy.setHeightForWidth(lang->sizePolicy().hasHeightForWidth());
    lang->setSizePolicy(sizePolicy);

    gridLayout_5->addWidget(lang, 0, 1, 1, 1);


    verticalLayout_5->addWidget(groupBox_9);

    spacer_8 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_5->addItem(spacer_8);

    PrefStack->addWidget(general);
    subtitle = new QWidget();
    subtitle->setObjectName(QString::fromUtf8("subtitle"));
    verticalLayout_12 = new QVBoxLayout(subtitle);
    verticalLayout_12->setSpacing(0);
    verticalLayout_12->setMargin(0);
    verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
    tabs = new QTabWidget(subtitle);
    tabs->setObjectName(QString::fromUtf8("tabs"));
    widget_3 = new QWidget();
    widget_3->setObjectName(QString::fromUtf8("widget_3"));
    verticalLayout_6 = new QVBoxLayout(widget_3);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    groupBox_3 = new QGroupBox(widget_3);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    verticalLayout_7 = new QVBoxLayout(groupBox_3);
    verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
    label_2 = new QLabel(groupBox_3);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    verticalLayout_7->addWidget(label_2);

    autoLoad = new QComboBox(groupBox_3);
    autoLoad->setObjectName(QString::fromUtf8("autoLoad"));
    autoLoad->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    verticalLayout_7->addWidget(autoLoad);


    verticalLayout_6->addWidget(groupBox_3);

    groupBox_6 = new QGroupBox(widget_3);
    groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
    verticalLayout_8 = new QVBoxLayout(groupBox_6);
    verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
    label_3 = new QLabel(groupBox_6);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    verticalLayout_8->addWidget(label_3);

    autoSelect = new QComboBox(groupBox_6);
    autoSelect->setObjectName(QString::fromUtf8("autoSelect"));
    autoSelect->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    verticalLayout_8->addWidget(autoSelect);


    verticalLayout_6->addWidget(groupBox_6);

    auto_load_group = new QGroupBox(widget_3);
    auto_load_group->setObjectName(QString::fromUtf8("auto_load_group"));
    auto_load_group->setCheckable(false);
    verticalLayout_9 = new QVBoxLayout(auto_load_group);
    verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
    label_5 = new QLabel(auto_load_group);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    verticalLayout_9->addWidget(label_5);

    encoding = new EncodingComboBox(auto_load_group);
    encoding->setObjectName(QString::fromUtf8("encoding"));
    encoding->setEditable(true);
    encoding->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

    verticalLayout_9->addWidget(encoding);


    verticalLayout_6->addWidget(auto_load_group);

    spacer_9 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_6->addItem(spacer_9);

    tabs->addTab(widget_3, QString());
    widget_2 = new QWidget();
    widget_2->setObjectName(QString::fromUtf8("widget_2"));
    verticalLayout_10 = new QVBoxLayout(widget_2);
    verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
    groupBox_7 = new QGroupBox(widget_2);
    groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
    verticalLayout_11 = new QVBoxLayout(groupBox_7);
    verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
    subOsd = new PrefOsdWidget(groupBox_7);
    subOsd->setObjectName(QString::fromUtf8("subOsd"));

    verticalLayout_11->addWidget(subOsd);


    verticalLayout_10->addWidget(groupBox_7);

    verticalSpacer_4 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_10->addItem(verticalSpacer_4);

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

    verticalLayout_12->addWidget(tabs);

    PrefStack->addWidget(subtitle);
    iface = new QWidget();
    iface->setObjectName(QString::fromUtf8("iface"));
    verticalLayout_2 = new QVBoxLayout(iface);
    verticalLayout_2->setSpacing(0);
    verticalLayout_2->setMargin(0);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    tabWidget = new QTabWidget(iface);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
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

    _3->addWidget(label);

    shortcut1 = new QPushButton(widget_5);
    shortcut1->setObjectName(QString::fromUtf8("shortcut1"));

    _3->addWidget(shortcut1);

    shortcut2 = new QPushButton(widget_5);
    shortcut2->setObjectName(QString::fromUtf8("shortcut2"));

    _3->addWidget(shortcut2);

    shortcut3 = new QPushButton(widget_5);
    shortcut3->setObjectName(QString::fromUtf8("shortcut3"));

    _3->addWidget(shortcut3);

    shortcut4 = new QPushButton(widget_5);
    shortcut4->setObjectName(QString::fromUtf8("shortcut4"));

    _3->addWidget(shortcut4);

    spacer_10 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _3->addItem(spacer_10);


    verticalLayout_3->addLayout(_3);

    tabWidget->addTab(widget_5, QString());
    widget_6 = new QWidget();
    widget_6->setObjectName(QString::fromUtf8("widget_6"));
    mouseLayout = new QVBoxLayout(widget_6);
    mouseLayout->setObjectName(QString::fromUtf8("mouseLayout"));
    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    mouseLayout->addItem(verticalSpacer);

    tabWidget->addTab(widget_6, QString());
    widget_7 = new QWidget();
    widget_7->setObjectName(QString::fromUtf8("widget_7"));
    verticalLayout_21 = new QVBoxLayout(widget_7);
    verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
    groupBox_2 = new QGroupBox(widget_7);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    gridLayout = new QGridLayout(groupBox_2);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_15 = new QLabel(groupBox_2);
    label_15->setObjectName(QString::fromUtf8("label_15"));
    label_15->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    gridLayout->addWidget(label_15, 0, 0, 1, 1);

    seek1 = new QSpinBox(groupBox_2);
    seek1->setObjectName(QString::fromUtf8("seek1"));
    seek1->setMinimum(1);
    seek1->setMaximum(300);
    seek1->setValue(5);

    gridLayout->addWidget(seek1, 0, 1, 1, 1);

    label_16 = new QLabel(groupBox_2);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    gridLayout->addWidget(label_16, 0, 2, 1, 1);

    seek2 = new QSpinBox(groupBox_2);
    seek2->setObjectName(QString::fromUtf8("seek2"));
    seek2->setMinimum(1);
    seek2->setMaximum(600);
    seek2->setValue(30);

    gridLayout->addWidget(seek2, 0, 3, 1, 1);

    horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);

    label_17 = new QLabel(groupBox_2);
    label_17->setObjectName(QString::fromUtf8("label_17"));

    gridLayout->addWidget(label_17, 1, 0, 1, 1);

    seek3 = new QSpinBox(groupBox_2);
    seek3->setObjectName(QString::fromUtf8("seek3"));
    seek3->setMinimum(1);
    seek3->setMaximum(600);
    seek3->setValue(60);

    gridLayout->addWidget(seek3, 1, 1, 1, 1);

    label_9 = new QLabel(groupBox_2);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout->addWidget(label_9, 1, 2, 1, 1);

    speed = new QSpinBox(groupBox_2);
    speed->setObjectName(QString::fromUtf8("speed"));
    speed->setValue(10);

    gridLayout->addWidget(speed, 1, 3, 1, 1);

    horizontalSpacer_4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer_4, 1, 4, 1, 1);


    verticalLayout_21->addWidget(groupBox_2);

    groupBox_11 = new QGroupBox(widget_7);
    groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
    gridLayout_2 = new QGridLayout(groupBox_11);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    label_18 = new QLabel(groupBox_11);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    gridLayout_2->addWidget(label_18, 0, 0, 1, 1);

    volume = new QSpinBox(groupBox_11);
    volume->setObjectName(QString::fromUtf8("volume"));
    volume->setMinimum(1);
    volume->setMaximum(50);
    volume->setValue(2);

    gridLayout_2->addWidget(volume, 0, 1, 1, 1);

    label_19 = new QLabel(groupBox_11);
    label_19->setObjectName(QString::fromUtf8("label_19"));
    label_19->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    gridLayout_2->addWidget(label_19, 0, 2, 1, 1);

    amp = new QSpinBox(groupBox_11);
    amp->setObjectName(QString::fromUtf8("amp"));
    amp->setMinimum(1);
    amp->setMaximum(50);
    amp->setValue(10);

    gridLayout_2->addWidget(amp, 0, 3, 1, 1);

    horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_2, 0, 4, 1, 1);


    verticalLayout_21->addWidget(groupBox_11);

    groupBox_13 = new QGroupBox(widget_7);
    groupBox_13->setObjectName(QString::fromUtf8("groupBox_13"));
    gridLayout_4 = new QGridLayout(groupBox_13);
    gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
    label_12 = new QLabel(groupBox_13);
    label_12->setObjectName(QString::fromUtf8("label_12"));

    gridLayout_4->addWidget(label_12, 0, 0, 1, 1);

    spinBox = new QSpinBox(groupBox_13);
    spinBox->setObjectName(QString::fromUtf8("spinBox"));
    spinBox->setValue(1);

    gridLayout_4->addWidget(spinBox, 0, 1, 1, 1);

    label_14 = new QLabel(groupBox_13);
    label_14->setObjectName(QString::fromUtf8("label_14"));

    gridLayout_4->addWidget(label_14, 0, 2, 1, 1);

    spinBox_3 = new QSpinBox(groupBox_13);
    spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
    spinBox_3->setValue(1);

    gridLayout_4->addWidget(spinBox_3, 0, 3, 1, 1);

    horizontalSpacer_5 = new QSpacerItem(106, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_4->addItem(horizontalSpacer_5, 0, 4, 1, 1);

    label_13 = new QLabel(groupBox_13);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    gridLayout_4->addWidget(label_13, 1, 0, 1, 1);

    spinBox_2 = new QSpinBox(groupBox_13);
    spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
    spinBox_2->setValue(1);

    gridLayout_4->addWidget(spinBox_2, 1, 1, 1, 1);

    label_20 = new QLabel(groupBox_13);
    label_20->setObjectName(QString::fromUtf8("label_20"));

    gridLayout_4->addWidget(label_20, 1, 2, 1, 1);

    spinBox_4 = new QSpinBox(groupBox_13);
    spinBox_4->setObjectName(QString::fromUtf8("spinBox_4"));
    spinBox_4->setValue(1);

    gridLayout_4->addWidget(spinBox_4, 1, 3, 1, 1);

    horizontalSpacer_6 = new QSpacerItem(106, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_4->addItem(horizontalSpacer_6, 1, 4, 1, 1);


    verticalLayout_21->addWidget(groupBox_13);

    groupBox_12 = new QGroupBox(widget_7);
    groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
    gridLayout_3 = new QGridLayout(groupBox_12);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    label_10 = new QLabel(groupBox_12);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    gridLayout_3->addWidget(label_10, 0, 0, 1, 1);

    subPos = new QSpinBox(groupBox_12);
    subPos->setObjectName(QString::fromUtf8("subPos"));
    subPos->setValue(1);

    gridLayout_3->addWidget(subPos, 0, 1, 1, 1);

    label_11 = new QLabel(groupBox_12);
    label_11->setObjectName(QString::fromUtf8("label_11"));

    gridLayout_3->addWidget(label_11, 0, 2, 1, 1);

    horizontalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_3->addItem(horizontalSpacer_3, 0, 4, 1, 1);

    sync = new QDoubleSpinBox(groupBox_12);
    sync->setObjectName(QString::fromUtf8("sync"));
    sync->setSingleStep(0.1);
    sync->setValue(0.5);

    gridLayout_3->addWidget(sync, 0, 3, 1, 1);


    verticalLayout_21->addWidget(groupBox_12);

    spacer_7 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_21->addItem(spacer_7);

    tabWidget->addTab(widget_7, QString());

    verticalLayout_2->addWidget(tabWidget);

    PrefStack->addWidget(iface);
    widget = new QWidget();
    widget->setObjectName(QString::fromUtf8("widget"));
    verticalLayout_16 = new QVBoxLayout(widget);
    verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
    groupBox = new QGroupBox(widget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    verticalLayout_14 = new QVBoxLayout(groupBox);
    verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    verticalLayout_14->addWidget(label_4);

    _11 = new QHBoxLayout();
    _11->setObjectName(QString::fromUtf8("_11"));
    pathEdit = new QLineEdit(groupBox);
    pathEdit->setObjectName(QString::fromUtf8("pathEdit"));
    pathEdit->setReadOnly(true);

    _11->addWidget(pathEdit);

    pathButton = new QPushButton(groupBox);
    pathButton->setObjectName(QString::fromUtf8("pathButton"));

    _11->addWidget(pathButton);


    verticalLayout_14->addLayout(_11);

    label_21 = new QLabel(groupBox);
    label_21->setObjectName(QString::fromUtf8("label_21"));

    verticalLayout_14->addWidget(label_21);


    verticalLayout_16->addWidget(groupBox);

    groupBox_8 = new QGroupBox(widget);
    groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
    verticalLayout_15 = new QVBoxLayout(groupBox_8);
    verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
    label_8 = new QLabel(groupBox_8);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
    label_8->setSizePolicy(sizePolicy1);

    verticalLayout_15->addWidget(label_8);

    media = new QTreeWidget(groupBox_8);
    media->setObjectName(QString::fromUtf8("media"));
    media->setRootIsDecorated(false);

    verticalLayout_15->addWidget(media);


    verticalLayout_16->addWidget(groupBox_8);

    PrefStack->addWidget(widget);
    groupBox_8->raise();
    groupBox->raise();

    retranslateUi(PrefStack);
    QObject::connect(pauseMinimized, SIGNAL(toggled(bool)), playRestored, SLOT(setEnabled(bool)));

    PrefStack->setCurrentIndex(0);
    tabs->setCurrentIndex(0);
    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(PrefStack);
    } // setupUi

    void retranslateUi(QStackedWidget *PrefStack)
    {
    groupBox_4->setTitle(QApplication::translate("PrefStack", "Generate Play List", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("PrefStack", "Add next files to play list when a file is open", 0, QApplication::UnicodeUTF8));
    groupBox_5->setTitle(QApplication::translate("PrefStack", "Play", 0, QApplication::UnicodeUTF8));
    pauseMinimized->setText(QApplication::translate("PrefStack", "Pause when minimized", 0, QApplication::UnicodeUTF8));
    playRestored->setText(QApplication::translate("PrefStack", "Start to play when restored", 0, QApplication::UnicodeUTF8));
    startStopped->setText(QApplication::translate("PrefStack", "When a file is stopped, play it from there next time", 0, QApplication::UnicodeUTF8));
    groupBox_9->setTitle(QApplication::translate("PrefStack", "Language", 0, QApplication::UnicodeUTF8));
    label_22->setText(QApplication::translate("PrefStack", "Language", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("PrefStack", "Automatic Load", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("PrefStack", "Load next subtitles when a file to play is open", 0, QApplication::UnicodeUTF8));
    groupBox_6->setTitle(QApplication::translate("PrefStack", "Auto selection", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("PrefStack", "Select next subtitles in loading ones", 0, QApplication::UnicodeUTF8));
    auto_load_group->setTitle(QApplication::translate("PrefStack", "Default Encoding", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("PrefStack", "Open subtitle file with next encoding", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(widget_3), QApplication::translate("PrefStack", "Load Subtitle", 0, QApplication::UnicodeUTF8));
    groupBox_7->setTitle(QApplication::translate("PrefStack", "Subtitle OSD", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(widget_2), QApplication::translate("PrefStack", "Subtitle Display", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("PrefStack", "You can specify the order of unified(mulit) subtitles with the 'Name' property in subtitle file.\n"
"If not specified, They will be displayed in selected order.\n"
"e.g.: English, Korean", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(widget_4), QApplication::translate("PrefStack", "Unified/Multi-Subtitle", 0, QApplication::UnicodeUTF8));
    shortcutTree->headerItem()->setText(0, QApplication::translate("PrefStack", "Menu", 0, QApplication::UnicodeUTF8));
    shortcutTree->headerItem()->setText(1, QApplication::translate("PrefStack", "Shortcut1", 0, QApplication::UnicodeUTF8));
    shortcutTree->headerItem()->setText(2, QApplication::translate("PrefStack", "Shortcut2", 0, QApplication::UnicodeUTF8));
    shortcutTree->headerItem()->setText(3, QApplication::translate("PrefStack", "Shortcut3", 0, QApplication::UnicodeUTF8));
    shortcutTree->headerItem()->setText(4, QApplication::translate("PrefStack", "Shortcut4", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("PrefStack", "Change:", 0, QApplication::UnicodeUTF8));
    shortcut1->setText(QApplication::translate("PrefStack", "Shortcut1", 0, QApplication::UnicodeUTF8));
    shortcut2->setText(QApplication::translate("PrefStack", "Shortcut2", 0, QApplication::UnicodeUTF8));
    shortcut3->setText(QApplication::translate("PrefStack", "Shortcut3", 0, QApplication::UnicodeUTF8));
    shortcut4->setText(QApplication::translate("PrefStack", "Shortcut4", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(widget_5), QApplication::translate("PrefStack", "Shortcut", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(widget_6), QApplication::translate("PrefStack", "Mouse", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("PrefStack", "Play", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("PrefStack", "Seek Step 1", 0, QApplication::UnicodeUTF8));
    seek1->setSuffix(QApplication::translate("PrefStack", "sec.", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("PrefStack", "Seek Step 2", 0, QApplication::UnicodeUTF8));
    seek2->setSuffix(QApplication::translate("PrefStack", "sec.", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("PrefStack", "Seek Step 3", 0, QApplication::UnicodeUTF8));
    seek3->setSuffix(QApplication::translate("PrefStack", "sec.", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("PrefStack", "Speed Step", 0, QApplication::UnicodeUTF8));
    speed->setSuffix(QApplication::translate("PrefStack", "%", 0, QApplication::UnicodeUTF8));
    groupBox_11->setTitle(QApplication::translate("PrefStack", "Audio", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("PrefStack", "Volum Step", 0, QApplication::UnicodeUTF8));
    volume->setSuffix(QApplication::translate("PrefStack", "%", 0, QApplication::UnicodeUTF8));
    label_19->setText(QApplication::translate("PrefStack", "Amp. Step", 0, QApplication::UnicodeUTF8));
    amp->setSuffix(QApplication::translate("PrefStack", "%", 0, QApplication::UnicodeUTF8));
    groupBox_13->setTitle(QApplication::translate("PrefStack", "Video", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("PrefStack", "Brightness Step", 0, QApplication::UnicodeUTF8));
    spinBox->setSuffix(QApplication::translate("PrefStack", "%", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("PrefStack", "Contrast Step", 0, QApplication::UnicodeUTF8));
    spinBox_3->setSuffix(QApplication::translate("PrefStack", "%", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("PrefStack", "Saturation Step", 0, QApplication::UnicodeUTF8));
    spinBox_2->setSuffix(QApplication::translate("PrefStack", "%", 0, QApplication::UnicodeUTF8));
    label_20->setText(QApplication::translate("PrefStack", "Hue Step", 0, QApplication::UnicodeUTF8));
    spinBox_4->setSuffix(QApplication::translate("PrefStack", "%", 0, QApplication::UnicodeUTF8));
    groupBox_12->setTitle(QApplication::translate("PrefStack", "Subtitle", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("PrefStack", "Position Step", 0, QApplication::UnicodeUTF8));
    subPos->setSuffix(QApplication::translate("PrefStack", "%", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("PrefStack", "Sync Delay Step", 0, QApplication::UnicodeUTF8));
    sync->setSuffix(QApplication::translate("PrefStack", "sec.", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(widget_7), QApplication::translate("PrefStack", "Control Step", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("PrefStack", "Location", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("PrefStack", "Path where play engine plugins are located", 0, QApplication::UnicodeUTF8));
    pathButton->setText(QApplication::translate("PrefStack", "Choose", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("PrefStack", "Warning: You must restart this application if the path changed.", 0, QApplication::UnicodeUTF8));
    groupBox_8->setTitle(QApplication::translate("PrefStack", "Priority", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("PrefStack", "Set priority of play engine for each media type", 0, QApplication::UnicodeUTF8));
    media->headerItem()->setText(0, QApplication::translate("PrefStack", "Media Type", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(PrefStack);
    } // retranslateUi

};

namespace Ui {
    class PrefStack: public Ui_PrefStack {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFSTACK_H
