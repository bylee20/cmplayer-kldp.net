#include "recentplayedwidget.h"
#include <QtCore/QDebug>
#include "videoplayer.h"
#include <QtCore/QFileInfo>
#include <core/info.h>
#include <QtCore/QSettings>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

struct RecentPlayedWidget::Data {
	QTreeWidget *tree;
	VideoPlayer *player;
	QSpinBox *count;
	QString config;
};

RecentPlayedWidget::RecentPlayedWidget(VideoPlayer *player, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->config = Core::Info::privatePath() + "/recentplayed.ini";
	
	d->tree = new QTreeWidget(this);
	d->tree->setHeaderLabels(QStringList() << tr("Name") << tr("Location"));
	d->tree->setRootIsDecorated(false);
	d->player = player;
	d->count = new QSpinBox(this);
	d->count->setAccelerated(true);
	d->count->setRange(10, 999);
	
	connect(player, SIGNAL(started()), this, SLOT(slotStarted()));
	connect(d->tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int))
			, this, SLOT(slotItemDoubleClicked(QTreeWidgetItem*)));
	connect(d->count, SIGNAL(valueChanged(int)), this, SLOT(updateSize()));
	
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel(tr("Maximum Count:"), this));
	hbox->addWidget(d->count);
	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(d->tree);
	vbox->addLayout(hbox);
	
	load();
}

RecentPlayedWidget::~RecentPlayedWidget() {
	save();
	delete d;
}

QTreeWidgetItem *RecentPlayedWidget::makeItem(const QUrl &url) {
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setData(0, Qt::UserRole, url);
	if (url.scheme() == "file") {
		const QFileInfo file(url.toLocalFile());
		item->setText(0, file.fileName());
		item->setText(1, file.absoluteFilePath());
	} else {
		const QString loc = url.toString();
		item->setText(0, loc);
		item->setText(1, loc);
	}
	return item;
}

void RecentPlayedWidget::slotStarted() {
	const QUrl url = d->player->currentSource().url();
	QTreeWidgetItem *item = 0;
	for (int i=0; i<d->tree->topLevelItemCount(); ++i) {
		if (d->tree->topLevelItem(i)->data(0, Qt::UserRole).toUrl() == url) {
			if (i == 0)
				return;
			item = d->tree->takeTopLevelItem(i);
			break;
		}
	}
	if (!item)
		item = makeItem(url);
	d->tree->insertTopLevelItem(0, item);
	updateSize();
}

void RecentPlayedWidget::updateSize() {
	const int size = d->count->value();
	while (d->tree->topLevelItemCount() > size)
		delete d->tree->takeTopLevelItem(d->tree->topLevelItemCount()-1);
}

void RecentPlayedWidget::save() const {
	QSettings set(d->config, QSettings::IniFormat);
	set.setValue("MaximumCount", d->count->value());
	const int count = d->tree->topLevelItemCount();
	set.beginWriteArray("List", count);
	for (int i=0; i<count; ++i) {
		QTreeWidgetItem *item = d->tree->topLevelItem(i);
		const QUrl url = item->data(0, Qt::UserRole).toUrl();
		set.setArrayIndex(i);
		set.setValue("Url", url);
	}
	set.endArray();
}

void RecentPlayedWidget::load() {
	d->tree->clear();
	QSettings set(d->config, QSettings::IniFormat);
	d->count->setValue(set.value("MaximumCount", 100).toInt());
	const int count = set.beginReadArray("List");
	for (int i = 0; i<count; ++i) {
		set.setArrayIndex(i);
		const QUrl url = set.value("Url", QUrl()).toUrl();
		if (url.isEmpty() || !url.isValid())
			continue;
		QTreeWidgetItem *item = makeItem(url);
		d->tree->addTopLevelItem(item);
	}
	set.endArray();
	updateSize();
}

void RecentPlayedWidget::slotItemDoubleClicked(QTreeWidgetItem *item) {
	if (item) 
		emit openRequested(item->data(0, Qt::UserRole).toUrl());
}
