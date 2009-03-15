#include "recentinfo.h"
#include "helper.h"
#include "pref.h"
#include <core/mediasource.h>
#include <core/playlist.h>
#include <QtCore/QUrl>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

RecentStack::RecentStack(int size) {setSize(size);}

struct RecentInfo::Data {
	typedef QMap<Core::MediaSource, int> StoppedMap;
	Data(): stack(DefaultRememberCount) {}
	RecentStack stack;
	StoppedMap stopped;
	Core::Playlist playlist;
	Core::MediaSource source;
};

RecentInfo *RecentInfo::get() {
	static RecentInfo obj;
	return &obj;
}

RecentInfo::RecentInfo(): d(new Data) {load();}

RecentInfo::~RecentInfo() {
	delete d;
}

void RecentInfo::setLastPlaylist(const Core::Playlist &list) {
	d->playlist = list;
}

const Core::Playlist &RecentInfo::lastPlaylist() const {
	return d->playlist;
}

void RecentInfo::setLastSource(const Core::MediaSource &source) {
	d->source = source;
}

const Core::MediaSource &RecentInfo::lastSource() const {
	return d->source;
}

const RecentStack &RecentInfo::sources() const {
	return d->stack;
}

Core::MediaSource RecentInfo::source(int index) const {
	return d->stack.value(index);
}

void RecentInfo::clearStack() {
	d->stack.clear();
	emit sourcesChanged(d->stack);
}

void RecentInfo::setRememberCount(int count) {
	if (count != d->stack.size()) {
		d->stack.setSize(count);
		emit rememberCountChanged(count);
	}
}

void RecentInfo::setStopped(const Core::MediaSource &source, int time) {
	if (Pref::get()->rememberStopped())
		d->stopped[source] = time;
}

void RecentInfo::setFinished(const Core::MediaSource &source) {
	d->stopped.remove(source);
}

int RecentInfo::stoppedTime(const Core::MediaSource &source) const {
	if (Pref::get()->rememberStopped())
		return d->stopped.value(source, 0);
	else
		return 0;
}

void RecentInfo::stackSource(const Core::MediaSource &source) {
	d->stack.stack(source);
	emit sourcesChanged(d->stack);
}

void RecentInfo::load() {
	QSettings set(Helper::recentFile(), QSettings::IniFormat);
	set.beginGroup("RecentInfo");
	d->source = Core::MediaSource(set.value("LastSource", QUrl()).toUrl());
	int size = set.beginReadArray("Recents");
	QList<Core::MediaSource> recents;
	for (int i=0; i<size; ++i) {
		set.setArrayIndex(i);
		recents.append(Core::MediaSource(set.value("Source", QUrl()).toUrl()));
	}
	set.endArray();
	emit sourcesChanged(d->stack = recents);
	size = set.beginReadArray("LastPlaylist");
	for (int i=0; i<size; ++i) {
		set.setArrayIndex(i);
		const Core::MediaSource source(set.value("Source", QUrl()).toUrl());
		if (source.isValid())
			d->playlist.append(source);
	}
	set.endArray();
	if (Pref::get()->rememberStopped()) {
		const int size = set.beginReadArray("StoppedList");
		for (int i=0; i<size; ++i) {
			set.setArrayIndex(i);
			const Core::MediaSource source(set.value("Source", QUrl()).toUrl());
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
	size = d->playlist.size();
	set.beginWriteArray("LastPlaylist", size);
	for (int i=0; i<size; ++i) {
		set.setArrayIndex(i);
		set.setValue("Source", d->playlist[i].url());
	}
	set.endArray();
	if (Pref::get()->rememberStopped()) {
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

