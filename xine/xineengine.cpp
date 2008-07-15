#include "xineengine.h"
#include "xinewidget.h"
#include <QDir>
#include <QTimer>
#include <QTime>

namespace Xine {

XineEngine::XineEngine(QWidget *parentWidget) {
	m_xine = 0;
	m_stream = 0;
	m_audioPort = 0;
	m_eventQueue = 0;
	m_widget = 0;
	m_tickInterval = 0;
	m_curTime = -1;
	m_totalTime = 0;
	m_init = false;
	m_parentWidget = parentWidget;
	m_ticker = new QTimer(this);
	m_state = StoppedState;
	m_volume = 100;
	m_volAmp = 1.0;
	connect(m_ticker, SIGNAL(timeout()), this, SLOT(emitTick()));
}

XineEngine::~XineEngine() {
	if (m_state != StoppedState)
		stop();
	xine_close(m_stream);
	xine_event_dispose_queue(m_eventQueue);
	xine_dispose(m_stream);
	if (m_audioPort)
		xine_close_audio_driver(m_xine, m_audioPort);
	delete m_widget;
	xine_exit(m_xine);
}

void XineEngine::eventListener(void *user_data, const xine_event_t *event) {
	XineEngine *e = reinterpret_cast<XineEngine*>(user_data);
	switch(event->type) {
	case XINE_EVENT_UI_PLAYBACK_FINISHED:
		e->setState(StoppedState);
		break;
	}
}

bool XineEngine::initialize(const QString &videoDriver, const QString &audioDriver) {
	if (m_init)
		return true;

	m_xine = xine_new();
	const QString configFile = QDir::homePath() + "/.xine/config";
	xine_config_load(m_xine, QFile::encodeName(configFile));
	xine_init(m_xine);

	m_widget = new XineWidget(this, videoDriver, m_parentWidget);
	if (!m_widget->isValid())
		return false;
	m_audioPort = xine_open_audio_driver(m_xine , audioDriver.toLatin1(), 0);
	m_stream = xine_stream_new(m_xine, m_audioPort, m_widget->videoPort());
	m_eventQueue = xine_event_new_queue(m_stream);
	xine_event_create_listener_thread(m_eventQueue, eventListener, this);

	return m_init = true;
}

QWidget *XineEngine::widget() {
	return m_widget;
}

bool XineEngine::play(const QString &mrl) {
	stop();
	return xine_open(m_stream, mrl.toLocal8Bit()) && play();
}

bool XineEngine::play() {
	if ((m_state != PausedState && m_state != StoppedState) || !m_init)
		return false;
	if (m_state == PausedState) {
		xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
	} else {
		xine_play(m_stream, 0, 0);
	}
	setState(PlayingState);
	return true;
}

void XineEngine::pause() {
	if (!m_init || m_state != PlayingState)
		return;
	xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
	setState(PausedState);
// 	} else {
// 		xine_play(m_stream, 0, 0);
// 		xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
// 		changeState(Phonon::PausedState);
//     }
}

void XineEngine::stop() {
	if (!m_init || m_state == StoppedState)
		return;
	xine_stop(m_stream);
	setState(StoppedState);
}

qint64 XineEngine::currentTime() const {
	if (!m_init)
		return -1;
	if (m_state == PlayingState) {
		timeval now;
		gettimeofday(&now, 0);
		const int diff = (now.tv_sec - m_lastTimeUpdated.tv_sec)*1000 + (now.tv_usec - m_lastTimeUpdated.tv_usec)/1000;
		return m_curTime + diff;
	}
	return m_curTime;
}

bool XineEngine::updateTime() {
	if (!m_init)
		return false;
	int totalTime, curTime;
	if (!xine_get_pos_length(m_stream, 0, &curTime, &totalTime))
		return false;
	if (totalTime != m_totalTime)
		emit totalTimeChanged(m_totalTime = totalTime);
	if (curTime <= 0) {
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

void XineEngine::setTickInterval(qint32 interval) {
	if (interval <= 0) {
		m_tickInterval = 0;
		m_ticker->stop();
	} else {
		m_ticker->setInterval(interval);
		if (m_tickInterval == 0 && m_state == PlayingState)
			m_ticker->start();
		m_tickInterval = interval;
	}
}

void XineEngine::emitTick() {
	if (!updateTime())
		return;
	if (m_tickInterval > 0)
		emit tick(m_curTime);
}

void XineEngine::setState(State state) {
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

void XineEngine::seek(qint64 time) {
	if (m_state != PlayingState && m_state != PausedState)
		return;
	xine_play(m_stream, 0, time);
	if (m_state == PausedState)
		xine_set_param(m_stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
	updateTime();
}

void XineEngine::setVolume(int volume) {
	m_volume = qBound(0, volume, 100);
	xine_set_param(m_stream, XINE_PARAM_AUDIO_AMP_LEVEL, qBound(0, int(m_volume*m_volAmp), 200));
	emit volumeChanged(volume);
}

void XineEngine::setVolumeAmplification(qreal amp) {
	m_volAmp = qBound(0.0, amp, 2.0);
}

}
