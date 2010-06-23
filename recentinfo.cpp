#include "recentinfo.hpp"
#include <QtCore/QList>
#include <QtCore/QMap>
#include "record.hpp"
#include "playlist.hpp"

struct RecentInfo::Data {
	int max;
	Playlist openList, lastList;
	QMap<Mrl, int> stopped;
	Mrl lastMrl;
};

RecentInfo::RecentInfo()
: d(new Data) {
	d->max = 10;
	load();
}

RecentInfo::~RecentInfo() {
	delete d;
}

RecentInfo &RecentInfo::get() {
	static RecentInfo self;
	return self;
}

QList<Mrl> RecentInfo::openList() const {
	return d->openList;
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

void RecentInfo::stack(const Mrl &mrl) {
	if (mrl.url().isEmpty())
		return;
	d->openList.removeAll(mrl);
	d->openList.prepend(mrl);
	while (d->openList.size() > d->max)
		d->openList.pop_back();
	emit openListChanged(d->openList);
}

void RecentInfo::clear() {
	d->openList.clear();
	emit openListChanged(d->openList);
}

void RecentInfo::save() const {
	Record r;
	r.beginGroup("recent-info");
	d->openList.save("recent-open-list", &r);
	d->lastList.save("last-playlist", &r);
	r.setValue("last-mrl", d->lastMrl.url());
	r.endGroup();
}

void RecentInfo::load() {
	Record r;
	r.beginGroup("recent-info");
	d->openList.load("recent-open-list", &r);
	d->lastList.load("last-playlist", &r);
	d->lastMrl = r.value("last-mrl", QUrl()).toUrl();
	r.endGroup();
}

void RecentInfo::setLastPlaylist(const Playlist &list) {
	d->lastList = list;
}

Playlist RecentInfo::lastPlaylist() const {
	return d->lastList;
}

void RecentInfo::setLastMrl(const Mrl &mrl) {
	d->lastMrl = mrl;
}

Mrl RecentInfo::lastMrl() const {
	return d->lastMrl;
}
