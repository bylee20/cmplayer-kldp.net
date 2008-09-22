#include "subtitleoutput.h"
#include "xinestream.h"
#include "videooutput.h"
#include "xineosd.h"
#include <backend/subtitleparsers.h>
#include <backend/subtitle.h>
#include <QFileInfo>
#include <QSet>
#include "playengine.h"

namespace Backend {

namespace Xine {

struct SubtitleOutput::Data {
	Subtitle *sub;
	Subtitle::ConstIterator it;
	int curTime;
	bool rendering;
};

SubtitleOutput::SubtitleOutput(PlayEngine *engine, XineStream *stream)
: Backend::SubtitleOutput(engine) {
	d = new Data;
	d->sub = const_cast<Subtitle*>(&currentSubtitle());
	m_engine = engine;
	m_stream = stream;
	m_osd = new XineOsd(stream->video());
	d->it = currentSubtitle().end();
	d->curTime = -1;
	d->rendering = false;
	connect(m_engine, SIGNAL(tick(int)), this, SLOT(showSubtitle(int)));
	connect(m_engine, SIGNAL(started()), this, SLOT(update()));
	connect(this, SIGNAL(syncDelayChanged(int)), this, SLOT(slotSyncDelayChanged(int)));
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
	if (d->sub->isEmpty() || m_engine->isSeeking())
		return;
	d->it = d->sub->find(time -= syncDelay());
	if (d->it == d->sub->end()) {
		d->it = d->sub->upperBound(time);
		if (d->it == d->sub->begin())
			return;
		--d->it;
	}
	if (d->it.key() != d->curTime) {
		d->curTime = d->it.key();
		renderSubtitle(d->it.value());
	}
}

void SubtitleOutput::update() {
	updatePos(initialPos());
}

void SubtitleOutput::updateClear() {
	m_osd->hide();
}

void SubtitleOutput::setVisible(bool visible) {
	m_osd->setVisible(visible);
}

void SubtitleOutput::slotSyncDelayChanged(int delay) {
	QString sec(delay > 0 ? "+" : "");
	sec += QString::number(double(delay)/1000.0);
	m_stream->showOsdText(trUtf8("자막 싱크: %1초").arg(sec));
}

void SubtitleOutput::updateSyncDelay(int msec) {
}

void SubtitleOutput::updatePos(double pos) {
	if (m_osd->margin(XineOsd::MBottom) != (1.0 - pos)) {
		m_osd->setMargin(XineOsd::MBottom, 1.0 - pos);
		m_stream->showOsdText(trUtf8("자막 위치: %1%").arg(qRound(pos*100)));
	}
}

void SubtitleOutput::updateChannels() {
	QStringList channels;
	const int count = xine_get_stream_info(m_stream->stream(), XINE_STREAM_INFO_MAX_SPU_CHANNEL);
	char buffer[256];
	for(int i=0; i<count; ++i) {
		QString channel = trUtf8("채널%1").arg(i);
		if (xine_get_spu_lang(m_stream->stream(), i, buffer))
			channel += ":" + QString::fromLocal8Bit(buffer);
		channels.append(channel);
	}
	setChannels(channels);
	const int cur = xine_get_param(m_stream->stream(), XINE_PARAM_SPU_CHANNEL);
	if (cur != currentChannel())
		setCurrentChannel(cur);
}

void SubtitleOutput::updateCurrentChannel(int index) {
	if (m_stream->isOpen())
		xine_set_param(m_stream->stream(), XINE_PARAM_SPU_CHANNEL, index);
}

void SubtitleOutput::updateStyle(const OsdStyle &style) {
	m_osd->setStyle(style);
}

}

}
