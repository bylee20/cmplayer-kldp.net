#include "subtitleoutput.h"
#include "xinestream.h"
#include "videooutput.h"
#include "xineosd.h"
#include "subtitleparsers.h"
#include <QFileInfo>
#include <QDebug>

namespace Xine {

struct SubtitleOutput::Data {
	Subtitle curSubtitle;
	Subtitle::ConstIterator it;
	int curTime;
	bool rendering;
	QList<Subtitle> subs;
	QStringList channels;
};

SubtitleOutput::SubtitleOutput(XineStream *stream) {
	d = new Data;
	m_stream = stream;
	m_osd = new XineOsd(stream->video());
	d->it = d->curSubtitle.end();
	d->curTime = -1;
	m_delay = 0;
	m_autoSelect = SameName;
	d->rendering = false;
	m_curChannel = -1;
	m_initPos = 1.0;
	connect(m_stream, SIGNAL(tick(int)), this, SLOT(showSubtitle(int)));
	connect(m_stream, SIGNAL(started()), this, SLOT(update()));
	m_osd->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
}

SubtitleOutput::~SubtitleOutput() {
	delete m_osd;
	delete d;
}

void SubtitleOutput::renderSubtitle(const QString &text) {
	if (d->rendering)
		return;
	d->rendering = true;
	m_osd->drawText(text);
	m_osd->update();
	d->rendering = false;
}

void SubtitleOutput::showSubtitle(int time) {
	if (d->curSubtitle.isEmpty() || m_stream->isSeeking())
		return;
	d->it = d->curSubtitle.find(time -= m_delay);
	if (d->it == d->curSubtitle.end()) {
		d->it = d->curSubtitle.upperBound(time);
		if (d->it == d->curSubtitle.begin())
			return;
		--d->it;
	}
	if (d->it.key() != d->curTime) {
		d->curTime = d->it.key();
		renderSubtitle(d->it.value());
	}
}

void SubtitleOutput::load(const QStringList &files) {
	clear(false);
	for (int i=0; i<files.size(); ++i) {
		QList<Subtitle> subs;
		if (SubtitleParsers::parse(files[i], &subs, m_encoding))
			d->subs += subs;
	}
	QSet<QString> langs;
	QString base = QFileInfo(m_stream->currentSource().filePath()).baseName();
	for (int i=0; i<d->subs.size(); ++i) {
		bool select = false;
		switch(m_autoSelect) {
		case SameName:
			select = QFileInfo(d->subs[i].filePath()).baseName() == base;
			break;
		case AllLoaded:
			select = true;
			break;
		case EachLanguage:
			if (select = (!langs.contains(d->subs[i].language())))
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
	if (emits) {
		emit availableSubtitlesChanged(names());
		emit selectedSubtitlesChanged(m_selected);
	}
}

QStringList SubtitleOutput::names() const {
	QStringList names;
	for (int i=0; i<d->subs.size(); ++i)
		names.append(d->subs[i].name());
	return names;
}

void SubtitleOutput::updateCurrent() {
	m_osd->hide();
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
	d->curTime = -1;
	d->it = d->curSubtitle.end();
	if (!m_selected.isEmpty())
		m_osd->show();
	d->thread->setSubtitle(d->curSubtitle);
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
	move(1.0 - m_osd->margin(XineOsd::MBottom) - 0.01);
}

void SubtitleOutput::moveDown() {
	move(1.0 - m_osd->margin(XineOsd::MBottom) + 0.01);
}

void SubtitleOutput::move(double percent) {
	double val = qBound(0.0, percent, 1.0);
	if (m_osd->margin(XineOsd::MBottom) != (1.0 - val)) {
		m_osd->setMargin(XineOsd::MBottom, 1.0 - val);
		m_stream->showOsdText(trUtf8("자막 위치: %1%").arg(int(val*100)));
	}
}

void SubtitleOutput::setInitialPos(double pos) {
	if (m_initPos != pos)
		m_initPos = qBound(0.0, pos, 1.0);
}

void SubtitleOutput::setSyncDelay(int msec) {
	if (m_delay != msec) {
		emit syncDelayChanged(m_delay = msec);
		QString sec(msec > 0 ? "+" : "");
		sec += QString::number(double(msec)/1000.0);
		m_stream->showOsdText(trUtf8("자막 싱크: %1초").arg(sec));
	}
}

void SubtitleOutput::addSyncDelay(int msec) {
	setSyncDelay(m_delay + msec);
}

void SubtitleOutput::updateChannels() {
	const int count = xine_get_stream_info(m_stream->stream(), XINE_STREAM_INFO_MAX_SPU_CHANNEL);
	d->channels.clear();
	char buffer[256];
	for(int i=0; i<count; ++i) {
		QString channel = trUtf8("채널%1").arg(i);
		if (xine_get_spu_lang(m_stream->stream(), i, buffer))
			channel += ":" + QString::fromLocal8Bit(buffer);
		d->channels.append(channel);
	}
	emit availableChannelsChanged(d->channels);
	const int cur = xine_get_param(m_stream->stream(), XINE_PARAM_SPU_CHANNEL);
	if (cur != m_curChannel)
		emit currentChannelChanged(m_curChannel = cur);
}

void SubtitleOutput::setCurrentChannel(int index) {
	if (m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_SPU_CHANNEL, index);
}

const Subtitle &SubtitleOutput::currentSubtitle() const {
	return d->curSubtitle;
}

void SubtitleOutput::update() {
	m_osd->setMargin(XineOsd::MBottom, 1.0 - m_initPos);
}

}

