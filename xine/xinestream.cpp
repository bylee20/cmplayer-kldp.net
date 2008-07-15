#include "xinestream.h"
#include "xineengine.h"
#include "xineaudio.h"
#include "xinevideo.h"
#include <QTimer>
#include <QDebug>

namespace Xine {

XineStream::XineStream(XineEngine *engine) {
	m_open = m_hasVideo = m_seekable = m_validInfo = m_seeking = false;
	m_engine = engine;
	m_stream = 0;
	m_eventQueue = 0;
	m_tickInterval = 0;
	m_curTime = -1;
	m_totalTime = 0;
	m_ticker = new QTimer(this);
	m_state = StoppedState;
	connect(m_ticker, SIGNAL(timeout()), this, SLOT(emitTick()));
}

XineStream::~XineStream() {
	close();
}

bool XineStream::open(XineAudio *audio, XineVideo *video) {
	if (isOpen())
		close();
	xine_t *xine = m_engine->xine();
	if (!xine || !audio->port() || !video->port())
		return false;
	if (!(m_stream = xine_stream_new(xine, audio->port(), video->port())))
		return false;
	m_eventQueue = xine_event_new_queue(m_stream);
	xine_event_create_listener_thread(m_eventQueue, eventListener, this);
	return m_open = true;
}

void XineStream::close() {
	if (!isOpen())
		return;
	xine_close(m_stream);
	xine_event_dispose_queue(m_eventQueue);
	xine_dispose(m_stream);
	m_stream = 0;
	m_eventQueue = 0;
	m_open = false;
}

void XineStream::eventListener(void *user_data, const xine_event_t *event) {
	XineStream *s = reinterpret_cast<XineStream*>(user_data);
	switch(event->type) {
	case XINE_EVENT_UI_PLAYBACK_FINISHED:
		s->setState(StoppedState);
		break;
	default:
		break;
	}
}

bool XineStream::play(const QString &mrl) {
	stop();
	m_mrl = mrl;
	if (!xine_open(m_stream, m_mrl.toLocal8Bit()))
		return false;
	updateInfo();
	return play();
}

bool XineStream::play() {
	if ((m_state != PausedState && m_state != StoppedState) || !isOpen())
		return false;
	if (m_state == PausedState) {
		xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
	} else {
		xine_play(m_stream, 0, 0);
	}
	setState(PlayingState);
	return true;
}

void XineStream::pause() {
	if (!isOpen() || m_state != PlayingState)
		return;
	xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
	setState(PausedState);
// 	} else {
// 		xine_play(m_stream, 0, 0);
// 		xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
// 		changeState(Phonon::PausedState);
//     }
}

void XineStream::stop() {
	if (!isOpen() || m_state == StoppedState)
		return;
	xine_stop(m_stream);
	setState(StoppedState);
}

qint64 XineStream::currentTime() const {
	if (!isOpen())
		return -1;
	if (m_state == PlayingState) {
		timeval now;
		gettimeofday(&now, 0);
		const int diff = (now.tv_sec - m_lastTimeUpdated.tv_sec)*1000 + (now.tv_usec - m_lastTimeUpdated.tv_usec)/1000;
		qDebug() << diff << m_curTime <<m_curTime + diff;
		return m_curTime + diff;
	}
	qDebug() << m_curTime;
	return m_curTime;
}

bool XineStream::updateTime() {
	if (!isOpen())
		return false;
	int totalTime, curTime;
	if (!xine_get_pos_length(m_stream, 0, &curTime, &totalTime))
		return false;
	if (totalTime != m_totalTime)
		emit totalTimeChanged(m_totalTime = totalTime);
	if (curTime < 0) {
		// are we seeking? when xine seeks xine_get_pos_length returns 0 for m_currentTime
		// m_lastTimeUpdate.tv_sec = 0;
		// XineStream::currentTime will still return the old value counting with gettimeofday
		return false;
	}
	if (curTime != m_curTime) {
		gettimeofday(&m_lastTimeUpdated, 0);
		m_curTime = curTime;
	}
	return true;
}

void XineStream::setTickInterval(qint32 interval) {
	if (interval <= 0) {
		m_tickInterval = 0;
		m_ticker->stop();
	} else {
		m_ticker->setInterval(interval);
		if (m_tickInterval != 0 && m_state == PlayingState)
			m_ticker->start();
		m_tickInterval = interval;
	}
}

void XineStream::emitTick() {
	if (!updateTime())
		return;
	if (m_tickInterval > 0)
		emit tick(m_curTime);
}

void XineStream::setState(State state) {
	if (m_state == state)
		return;
	State old = m_state;
	m_state = state;
	if (m_state == PlayingState) {
		if (m_tickInterval > 0)
			m_ticker->start();
	} else if (old == PlayingState) {
		m_ticker->stop();
	}
	if (m_state == StoppedState) {
		m_curTime = 0;
	} else
		updateTime();
	emit stateChanged(m_state, old);
}

void XineStream::seek(qint64 time) {
	if (m_state != PlayingState && m_state != PausedState)
		return;
	if (m_seeking)
		return;
	time = qBound(Q_INT64_C(0), time, m_totalTime);
	m_seeking = true;
	xine_play(m_stream, 0, time);
	if (m_state == PausedState)
		xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
	else
		gettimeofday(&m_lastTimeUpdated, 0);
	m_curTime = time;
	m_seeking = false;
}

void XineStream::updateInfo() {
	if (!isOpen() || m_mrl.isEmpty())
		return;
	const bool seekable = xine_get_stream_info(m_stream, XINE_STREAM_INFO_SEEKABLE);
	const bool hasVideo = xine_get_stream_info(m_stream, XINE_STREAM_INFO_HAS_VIDEO);
	if (m_hasVideo != hasVideo)
		emit hasVideoChanged(m_hasVideo = hasVideo);
	if (m_seekable != seekable)
		emit seekableChanged(m_seekable = seekable);
//	if (m_hasVideo) {
//		uint32_t width = xine_get_stream_info(m_stream, XINE_STREAM_INFO_VIDEO_WIDTH);
//		uint32_t height = xine_get_stream_info(m_stream, XINE_STREAM_INFO_VIDEO_HEIGHT);
//		handleDownstreamEvent(new FrameFormatChangeEvent(width, height, 0, 0));
//	}
//	}
//	m_waitingForStreamInfo.wakeAll();
}

}
