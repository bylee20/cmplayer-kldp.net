#include "historyview.hpp"
#include <QtCore/QDebug>
#include <QtGui/QMenu>
#include "playengine.hpp"
#include "recentinfo.hpp"
#include "record.hpp"

struct HistoryView::Item : public QTreeWidgetItem {
	enum Column {Name = 0, Latest = 1, Location = 2, ColumnCount = 3};
	Item() {}
	Item(const Mrl &mrl, const QDateTime &date);
	~Item() {}
	int stoppedTime() const {return m_stopped;}
	void setStoppedTime(int time) {m_stopped = time;}
	bool isStopped() const {return time >= 0;}
	const Mrl &mrl() const {return m_mrl;}
	void update(const QDateTime &date) {
		m_date = date;
		setText(Latest, m_date.toString(Qt::ISODate));
	}
	void update() {update(QDateTime::currentDateTime());}
	void setMrl(const Mrl &mrl) {
		m_mrl = mrl;
		setText(Name, mrl.displayName());
		setText(Location, mrl.toString());
	}
	const QDateTime &date() const {return m_date;}
private:
	Mrl m_mrl;
	QDateTime m_date;
	int m_stopped;
};

HistoryView::Item::Item(const Mrl &mrl, const QDateTime &date)
: m_mrl(mrl), m_date(m_date) {
	setMrl(mrl);
	update(date);
	m_stopped = -1;
}

struct HistoryView::Data {
	PlayEngine *engine;
	QMenu *context;
};

HistoryView::HistoryView(PlayEngine *engine, QWidget *parent)
: QTreeWidget(parent), d(new Data) {
	setSelectionMode(ExtendedSelection);
	setRootIsDecorated(false);
	setHeaderLabels(QStringList() << tr("Name") << tr("Latest Play") << tr("Location"));
	connect(engine,	SIGNAL(stopped(Mrl,int,int)), this, SLOT(handleStopped(Mrl,int,int)));
	connect(engine, SIGNAL(finished(Mrl)), this, SLOT(handleFinished(Mrl)));
	connect(engine, SIGNAL(stateChanged(MediaState,MediaState))
		, this, SLOT(handleStateChanged(MediaState,MediaState)));
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int))
		, this, SLOT(play(QTreeWidgetItem*)));
	d->engine = engine;
	load();

	d->context = new QMenu(this);
	connect(d->context->addAction(tr("Erase")), SIGNAL(triggered()), this, SLOT(erase()));
	connect(d->context->addAction(tr("Clear")), SIGNAL(triggered()), this, SLOT(clearAll()));
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu()));

}

HistoryView::~HistoryView() {
	save();
	delete d;
}

void HistoryView::showContextMenu() {
	d->context->exec(QCursor::pos());
}

void HistoryView::erase() {
	QList<QTreeWidgetItem*> items = selectedItems();
	for (int i=0; i<items.size(); ++i)
		delete items[i];
}

int HistoryView::findIndex(const Mrl &mrl) const {
	for (int i=0; i<topLevelItemCount(); ++i) {
		Item *item = static_cast<Item*>(topLevelItem(i));
		if (item->mrl() == mrl)
			return i;
	}
	return -1;
}

HistoryView::Item *HistoryView::item(int index) const {
	return static_cast<Item*>(topLevelItem(index));
}

void HistoryView::handleFinished(Mrl mrl) {
	Item *item = findItem(mrl);
	if (item) {
		item->setStoppedTime(-1);
		RecentInfo::get().setFinished(mrl);
	}
}

void HistoryView::handleStopped(Mrl mrl, int time, int duration) {
	if (mrl.isDVD() || duration < 500 || duration - time < 500)
		return;
	Item *item = findItem(mrl);
	if (item) {
		item->update();
		item->setStoppedTime(time);
		RecentInfo::get().setStopped(mrl, time, item->date());
	}
}

void HistoryView::handleStateChanged(MediaState state, MediaState old) {
	if ((old == StoppedState || old == FinishedState)
			&& (state == PausedState || state == PlayingState)) {
		const Mrl mrl = d->engine->mrl();
		if (mrl.isEmpty())
			return;
		int idx = findIndex(mrl);
		if (idx >= 0) {
			Item *item = this->item(idx);
			if (idx > 0) {
				takeTopLevelItem(idx);
				insertTopLevelItem(0, item);
				emit historyChanged();
			}
			item->update();
			item->setStoppedTime(-1);
		} else {
			const QDateTime date = QDateTime::currentDateTime();
			Item *item = new Item();
			item->setMrl(mrl);
			item->update(date);
			insertTopLevelItem(0, item);
			while (topLevelItemCount() > 999)
				delete takeTopLevelItem(topLevelItemCount()-1);
			emit historyChanged();
		}
	}
}

QList<Mrl> HistoryView::top(int count) const {
	QList<Mrl> list;
	for (int i=0; i<count && i<topLevelItemCount(); ++i)
		list << item(i)->mrl();
	return list;
}

int HistoryView::stoppedTime(const Mrl &mrl) const {
	const Item *item = findItem(mrl);
	return item ? item->stoppedTime() : -1;
}

void HistoryView::play(QTreeWidgetItem *treeItem) {
	const Item *item = static_cast<Item*>(treeItem);
	if (!item)
		return;
	if (d->engine->mrl() != item->mrl()) {
		d->engine->stop();
		d->engine->setMrl(item->mrl());
	}
	d->engine->play();
}

void HistoryView::clearAll() {
	clear();
}

void HistoryView::save() const {
	Record r;
	r.beginGroup("history");
	const int size = topLevelItemCount();
	r.beginWriteArray("list", size);
	for (int i=0; i<size; ++i) {
		const Item *item = this->item(i);
		r.setArrayIndex(i);
		r.setValue("mrl", item->mrl().toString());
		r.setValue("date", item->date());
		r.setValue("stopped-position", item->stoppedTime());
	}
	r.endArray();
	r.endGroup();
}

void HistoryView::load() {
	Record r;
	r.beginGroup("history");
	const int size = r.beginReadArray("list");
	for (int i=0; i<size; ++i) {
		r.setArrayIndex(i);
		const Mrl mrl = r.value("mrl", QString()).toString();
		if (mrl.isEmpty())
			continue;
		const QDateTime date = r.value("date", QDateTime()).toDateTime();
		int stopped = r.value("stopped-position", -1).toInt();
		Item *item = new Item;
		item->setMrl(mrl);
		item->update(date);
		item->setStoppedTime(stopped);
		addTopLevelItem(item);
		if (stopped > 0)
			RecentInfo::get().setStopped(mrl, stopped, date);
	}
}
