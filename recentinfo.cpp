#include "recentinfo.hpp"
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QSettings>

struct RecentInfo::Data {
	QList<Mrl> open;
	QMap<Mrl, int> stopped;
};

RecentInfo::RecentInfo()
: d(new Data) {
}

RecentInfo::~RecentInfo() {
	delete d;
}

RecentInfo &RecentInfo::get() {
	static RecentInfo self;
	return self;
}

QList<Mrl> RecentInfo::open() const {
	return d->open;
}

void RecentInfo::setStopped(const Mrl &mrl, int time) {
	if (time == -1)
		setFinished(mrl);
	else
		d->stopped[mrl] = time;
}

void RecentInfo::setFinished(const Mrl &mrl) {
	d->stopped.remove(mrl);
}

int RecentInfo::stoppedTime(const Mrl &mrl) {
	return d->stopped.value(mrl, 0);
}

void RecentInfo::save() const {
	QSettings set()
}

void RecentInfo::load() {

}
