#include "historywidget.h"
#include <core/mrl.h>
#include <QtCore/QDebug>
#include "videoplayer.h"
#include <core/info.h>
#include <QtCore/QSettings>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtCore/QDateTime>

class HistoryWidget::Item : public QTreeWidgetItem {
public:
	Item(const Core::Mrl &mrl, const QDateTime &date = QDateTime::currentDateTime()) {
		setMrl(mrl);
		updateDateTime(date);
	}
	void updateDateTime(const QDateTime &date = QDateTime::currentDateTime()) {
		setData(1, Qt::UserRole, date);
		setText(1, date.toString());
		m_date = date;
	}
	void setMrl(const Core::Mrl &mrl) {
		setText(0, mrl.isLocalFile() ? mrl.fileName() : mrl.location());
		setText(2, mrl.location());
		m_mrl = mrl;
	}
	const Core::Mrl &mrl() const {return m_mrl;}
	const QDateTime &dateTime() const {return m_date;}
private:
	Core::Mrl m_mrl;
	QDateTime m_date;
};

struct HistoryWidget::Data {
	QTreeWidget *tree;
	VideoPlayer *player;
	QSpinBox *count;
	QString config;
};

HistoryWidget::HistoryWidget(VideoPlayer *player, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->config = Core::Info::privatePath() + "/recentplayed.ini";

	d->tree = new QTreeWidget(this);
	d->tree->setHeaderLabels(QStringList() << tr("Name") << tr("Last Played") << tr("Location"));
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

HistoryWidget::~HistoryWidget() {
	save();
	delete d;
}

void HistoryWidget::slotStarted() {
	const Core::Mrl mrl = d->player->currentSource().mrl();
	Item *item = 0;
	for (int i=0; i<d->tree->topLevelItemCount(); ++i) {
		if (static_cast<Item*>(d->tree->topLevelItem(i))->mrl() == mrl) {
			if (i == 0)
				return;
			item = static_cast<Item*>(d->tree->takeTopLevelItem(i));
			break;
		}
	}
	if (!item)
		item = new Item(mrl);
	else
		item->updateDateTime();
	d->tree->insertTopLevelItem(0, item);
	updateSize();
}

void HistoryWidget::updateSize() {
	const int size = d->count->value();
	while (d->tree->topLevelItemCount() > size)
		delete d->tree->takeTopLevelItem(d->tree->topLevelItemCount()-1);
}

void HistoryWidget::save() const {
	QSettings set(d->config, QSettings::IniFormat);
	set.setValue("MaximumCount", d->count->value());
	const int count = d->tree->topLevelItemCount();
	set.beginWriteArray("List", count);
	for (int i=0; i<count; ++i) {
		Item *item = static_cast<Item*>(d->tree->topLevelItem(i));
		set.setArrayIndex(i);
		set.setValue("Url", item->mrl());
		set.setValue("DateTime", item->dateTime());
	}
	set.endArray();
}

void HistoryWidget::load() {
	d->tree->clear();
	QSettings set(d->config, QSettings::IniFormat);
	d->count->setValue(set.value("MaximumCount", 100).toInt());
	const int count = set.beginReadArray("List");
	for (int i = 0; i<count; ++i) {
		set.setArrayIndex(i);
		const Core::Mrl mrl = set.value("Url", QUrl()).toUrl();
		if (mrl.isEmpty() || !mrl.isValid())
			continue;
		const QDateTime date = set.value("DateTime", QDateTime()).toDateTime();
		d->tree->addTopLevelItem(new Item(mrl, date));
	}
	set.endArray();
	updateSize();
}

void HistoryWidget::slotItemDoubleClicked(QTreeWidgetItem *item) {
	if (item)
		emit openRequested(static_cast<Item*>(item)->mrl());
}
