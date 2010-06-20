#include "history.hpp"
#include "playengine.hpp"
#include <QtCore/QList>
#include <QtCore/QDateTime>

struct History::Data {
	QMap<Mrl, int> stopped;
	QList<Mrl> mrl;
	QList<QDateTime> date;
	int maxCount;
};

History::History(const PlayEngine *engine, QObject *parent)
: QObject(parent), d(new Data) {
	d->maxCount = DefaultMaximumCount;
//	connect(engine, SIGNAL(finished()), this, SLOT()));
}

History::~History() {
	delete d;
}
//
//History &History::get() {
//	static History self;
//	return sefl;
//}
