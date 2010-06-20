#include "historyview.hpp"
#include "playengine.hpp"
#include "recentinfo.hpp"

struct HistoryView::Item : public QTreeWidgetItem {
	enum Column {Name, Latest, Location, ColumnCount};
	Item() {}
	Item(const Mrl &mrl, const QDateTime &date = QDateTime::currentDateTime());
	~Item() {}
	int stoppedTime() const {return m_stopped;}
	void setStoppedTime(int time) {m_stopped = time;}
	bool isStopped() const {return time >= 0;}
	const Mrl &mrl() const {return m_mrl;}
	void update(const QDateTime &date = QDateTime::currentDateTime()) {
		m_date = date;
		setText(Latest, date.toString(Qt::ISODate));
	}
	void setMrl(const Mrl &mrl) {
		m_mrl = mrl;
		setText(Name, mrl.fileName());
		setText(Location, mrl.toString());
	}
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
};

HistoryView::HistoryView(PlayEngine *engine, QWidget *parent)
: QTreeWidget(parent), d(new Data) {
	setRootIsDecorated(false);
	setHeaderLabels(QStringList() << tr("Name") << tr("Latest Play") << tr("Location"));
	connect(engine,	SIGNAL(stopped(Mrl,int)), this, SLOT(handleStopped(Mrl,int)));
	connect(engine, SIGNAL(finished(Mrl)), this, SLOT(handleFinished(Mrl)));
	connect(engine, SIGNAL(stateChanged(MediaState,MediaState))
		, this, SLOT(handleStateChanged(MediaState,MediaState)));
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int))
		, this, SLOT(play(QTreeWidgetItem*)));
	d->engine = engine;
}

HistoryView::~HistoryView() {
	delete d;
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

//HistoryView::Item *HistoryView::getItem(const Mrl &mrl) const {
//	for (int i=0; i<topLevelItemCount(); ++i) {
//		Item *item = static_cast<Item*>(topLevelItem(i));
//		if (item->mrl() == mrl)
//			return item;
//	}
//	return 0;
//}

void HistoryView::handleFinished(Mrl mrl) {
	Item *item = findItem(mrl);
	if (item) {
		item->setStoppedTime(-1);
		RecentInfo::get().setFinished(mrl);
	}
}

void HistoryView::handleStopped(Mrl mrl, int time) {
	Item *item = findItem(mrl);
	if (item) {
		item->update();
		item->setStoppedTime(time);
		RecentInfo::get().setStopped(mrl, time);
	}
}

void HistoryView::handleStateChanged(MediaState state, MediaState old) {
	if ((old == StoppedState || old == FinishedState)
			&& (state == PausedState || state == PlayingState)) {
		const Mrl mrl = d->engine->mrl();
		if (mrl.url().isEmpty())
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
			Item *item = new Item(mrl);
			insertTopLevelItem(0, item);
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
