#include "recentinfo.h"
#include "helper.h"
#include "pref/preferences.h"
#include "pref/general.h"
#include <backend/mediasource.h>
#include <backend/playlist.h>
#include <QtCore/QUrl>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

RecentStack::RecentStack(int size) {setSize(size);}

struct RecentInfo::Data {
	typedef QMap<Backend::MediaSource, qint64> StoppedMap;
	Data(): stack(DefaultRememberCount) {}
	RecentStack stack;
	StoppedMap stopped;
	Backend::PlayList playList;
	Backend::MediaSource source;
};

RecentInfo::RecentInfo(): d(new Data) {
	load();
}

RecentInfo::~RecentInfo() {
	delete d;
}

void RecentInfo::setLastPlayList(const Backend::PlayList &list) {
	d->playList = list;
}

const Backend::PlayList &RecentInfo::lastPlayList() const {
	return d->playList;
}

void RecentInfo::setLastSource(const Backend::MediaSource &source) {
	d->source = source;
}

const Backend::MediaSource &RecentInfo::lastSource() const {
	return d->source;
}

const RecentStack &RecentInfo::sources() const {
	return d->stack;
}

Backend::MediaSource RecentInfo::source(int index) const {
	return d->stack.value(index);
}

void RecentInfo::clearStack() {
	d->stack.clear();
	emit sourcesChanged(d->stack);
}

void RecentInfo::setRememberCount(int count) {
		if (count != d->stack.size()) {d->stack.setSize(count);emit rememberCountChanged(count);}}

void RecentInfo::setStopped(const Backend::MediaSource &source, qint64 time) {
	if (Pref::get()->general().playFromStoppedPoint())
		d->stopped[source] = time;
}

void RecentInfo::setFinished(const Backend::MediaSource &source) {
	d->stopped.remove(source);
}

qint64 RecentInfo::stoppedTime(const Backend::MediaSource &source) const {
	if (Pref::get()->general().playFromStoppedPoint())
		return d->stopped.value(source, 0);
	else
		return 0;
}

void RecentInfo::stackSource(const Backend::MediaSource &source) {
	d->stack.stack(source);
	emit sourcesChanged(d->stack);
}

void RecentInfo::load() {
	QSettings set(Helper::recentFile(), QSettings::IniFormat);
	set.beginGroup("RecentInfo");
	d->source = Backend::MediaSource(set.value("LastSource", QUrl()).toUrl());
	qDebug() << "load" << d->source.toMrl();
	int size = set.beginReadArray("Recents");
	QList<Backend::MediaSource> recents;
	for (int i=0; i<size; ++i) {
		set.setArrayIndex(i);
		recents.append(Backend::MediaSource(set.value("Source", QUrl()).toUrl()));
	}
	set.endArray();
	emit sourcesChanged(d->stack = recents);
	size = set.beginReadArray("LastPlayList");
	for (int i=0; i<size; ++i) {
		set.setArrayIndex(i);
		const Backend::MediaSource source(set.value("Source", QUrl()).toUrl());
		if (source.isValid())
			d->playList.append(source);
	}
	set.endArray();
	if (Pref::get()->general().playFromStoppedPoint()) {
		const int size = set.beginReadArray("StoppedList");
		for (int i=0; i<size; ++i) {
			set.setArrayIndex(i);
			const Backend::MediaSource source(set.value("Source", QUrl()).toUrl());
			if (source.isValid())
				d->stopped[source] = set.value("Time", -1).toLongLong();
		}
		set.endArray();
	}
	set.endGroup();
}

void RecentInfo::save() const {
	QSettings set(Helper::recentFile(), QSettings::IniFormat);
	set.beginGroup("RecentInfo");
	set.setValue("LastSource", d->source.url());
	int size = d->stack.size();
	set.beginWriteArray("Recents", size);
	for (int i=0; i<size; ++i) {
		set.setArrayIndex(i);
		set.setValue("Source", d->stack[i].url());
	}
	set.endArray();
	size = d->playList.size();
	set.beginWriteArray("LastPlayList", size);
	for (int i=0; i<size; ++i) {
		set.setArrayIndex(i);
		set.setValue("Source", d->playList[i].url());
	}
	set.endArray();
	if (Pref::get()->general().playFromStoppedPoint()) {
		size = d->stopped.size();
		set.beginWriteArray("StoppedList", size);
		Data::StoppedMap::const_iterator it = d->stopped.begin();
		for (int i=0; it != d->stopped.end(); ++it, ++i) {
			set.setArrayIndex(i);
			set.setValue("Source", it.key().url());
			set.setValue("Time", it.value());
		}
		set.endArray();
	}
	set.endGroup();
}

