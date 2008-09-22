#include "playengine.h"
#include "abrepeater.h"

namespace Backend {

PlayEngine::PlayEngine(const FactoryIface *factory, QObject *parent)
: QObject(parent) {
	m_factory = factory;
	m_hasVideo = m_seekable = false;
	m_video = 0;
	m_audio = 0;
	m_subtitle = 0;
	m_repeater = new ABRepeater(this);
	m_state = StoppedState;
	m_speed = 1.0;
	m_totalTime = m_curTime = m_curBytes = 0;
}

PlayEngine::~PlayEngine() {
}

void PlayEngine::replay() {
	const int time = currentTime();
	stop();
	play(time);
}

void PlayEngine::setCurrentSource(const MediaSource &source) {
	if (m_source != source) {
		stop();
		updateCurrentSource(source);
		emit currentSourceChanged(m_source = source);
// 		if (!m_source.gotInfo())
// 			m_source.getInfo();
// 		m_video->updateVideoSize(m_source.videoSize());
// 		setTotalTime(m_source.duration());

	}
}

void PlayEngine::setSpeed(double speed) {
	speed = (qAbs(m_speed-1.0) < 0.0001) ? 1.0 : qBound(1.0, speed, 10.0);
	if (m_speed != speed) {
		updateSpeed(speed);
		emit speedChanged(m_speed = speed);
	}
}

}
