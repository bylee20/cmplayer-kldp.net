#include "subtitleoutput.h"
#include "videooutput.h"
#include "subtitleparsers.h"
#include <QFileInfo>
#include <QSet>
#include "playengine.h"

namespace Backend {

struct SubtitleOutput::Data {
	Subtitle curSubtitle;
	Subtitle::ConstIterator it;
	QList<Subtitle> subs;
	QStringList channels;
};

SubtitleOutput::SubtitleOutput(PlayEngine *engine) {
	d = new Data;
	m_engine = engine;
	d->it = d->curSubtitle.end();
	m_delay = 0;
	m_autoSelect = SameName;
	m_curChannel = -1;
	m_pos = m_initPos = 1.0;
	connect(m_engine, SIGNAL(started()), this, SLOT(update()));
}

SubtitleOutput::~SubtitleOutput() {
	delete d;
}

void SubtitleOutput::load(const QStringList &files) {
	clear(false);
	for (int i=0; i<files.size(); ++i) {
		QList<Subtitle> subs;
		if (SubtitleParsers::parse(files[i], &subs, m_encoding))
			d->subs += subs;
	}
	QSet<QString> langs;
	QString base = QFileInfo(m_engine->currentSource().filePath()).completeBaseName();
	for (int i=0; i<d->subs.size(); ++i) {
		bool select = false;
		switch(m_autoSelect) {
		case SameName:
			select = QFileInfo(d->subs[i].filePath()).completeBaseName() == base;
			break;
		case AllLoaded:
			select = true;
			break;
		case EachLanguage:
			if ((select = (!langs.contains(d->subs[i].language()))))
				langs.insert(d->subs[i].language());
			break;
		default:
			break;
		}
		if (select)
			m_selected.append(i);
	}
	emit availableSubtitlesChanged(names());
	if (!d->subs.isEmpty()) {
		updateCurrent();
		emit selectedSubtitlesChanged(m_selected);
	}
}

void SubtitleOutput::clear(bool emits) {
	d->curSubtitle.clear();
	d->subs.clear();
	m_selected.clear();
	d->channels.clear();
	m_curChannel = -1;
	if (emits) {
		emit availableSubtitlesChanged(names());
		emit selectedSubtitlesChanged(m_selected);
		emit availableChannelsChanged(d->channels);
		emit currentChannelChanged(m_curChannel);
	}
	updateClear();
}

QStringList SubtitleOutput::names() const {
	QStringList names;
	for (int i=0; i<d->subs.size(); ++i)
		names.append(d->subs[i].name());
	return names;
}

void SubtitleOutput::updateCurrent() {
	setVisible(false);
	if (m_selected.isEmpty())
		return;
	QList<int> order;
	QList<int> indexes = m_selected;
	for (int i=0; i<m_priority.size(); ++i) {
		QString lang = m_priority[i];
		QMutableListIterator<int> it(indexes);
		while(it.hasNext()) {
			if (d->subs[it.next()].language() == lang) {
				order.append(it.value());
				it.remove();
			}
		}
	}
	order += indexes;
	d->curSubtitle = d->subs[order[0]];
	for (int i=1; i<order.size(); ++i)
		d->curSubtitle |= d->subs[order[i]];
	d->it = d->curSubtitle.end();
	if (!m_selected.isEmpty())
		setVisible(true);
}

void SubtitleOutput::appendSubtitles(const QStringList &files, bool display) {
	if (files.isEmpty())
		return;
	int index = d->subs.size();
	for (int i=0; i<files.size(); ++i) {
		QList<Subtitle> subs;
		if (!SubtitleParsers::parse(files[i], &subs, m_encoding))
			continue;
		d->subs += subs;
		if (display) {
			for (int j=0; j<subs.size(); ++j, ++index)
				m_selected.append(index);
		}
	}
	emit availableSubtitlesChanged(names());
	if (display) {
		updateCurrent();
		emit selectedSubtitlesChanged(m_selected);
	}
}

void SubtitleOutput::select(int index) {
	if (m_selected.contains(index) || index < 0 || index >= d->subs.size())
		return;
	m_selected.append(index);
	updateCurrent();
	emit selectedSubtitlesChanged(m_selected);
}

void SubtitleOutput::deselect(int index) {
	const int pos = m_selected.indexOf(index);
	if (pos != -1) {
		m_selected.removeAt(pos);
		updateCurrent();
		emit selectedSubtitlesChanged(m_selected);
	}
}

void SubtitleOutput::deselectAll() {
	m_selected.clear();
	updateCurrent();
	emit selectedSubtitlesChanged(m_selected);
}

void SubtitleOutput::moveUp() {
	move(m_pos - 0.01);
}

void SubtitleOutput::moveDown() {
	move(m_pos + 0.01);
}

void SubtitleOutput::move(double percent) {
	double val = qBound(0.0, percent, 1.0);
	if (qAbs(m_pos - val) > 0.0001) {
		 updatePos(val);
		 m_pos = val;
	}
}

void SubtitleOutput::setInitialPos(double pos) {
	if (m_initPos != pos)
		m_initPos = qBound(0.0, pos, 1.0);
}

void SubtitleOutput::setSyncDelay(int msec) {
	if (m_delay != msec) {
		updateSyncDelay(msec);
		emit syncDelayChanged(m_delay = msec);
	}
}

void SubtitleOutput::addSyncDelay(int msec) {
	setSyncDelay(m_delay + msec);
}

void SubtitleOutput::setChannels(const QStringList &channels) {
	emit availableChannelsChanged(d->channels = channels);
}

void SubtitleOutput::setCurrentChannel(int channel) {
	if (m_curChannel != channel) {
		updateCurrentChannel(channel);
		emit currentChannelChanged(m_curChannel = channel);
	}
}

const Subtitle &SubtitleOutput::currentSubtitle() const {
	return d->curSubtitle;
}

void SubtitleOutput::update() {
	updatePos(m_initPos);
}

}

