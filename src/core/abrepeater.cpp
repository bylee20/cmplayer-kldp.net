#include "subtitle.h"
#include "playengine.h"
#include "abrepeater.h"

namespace Core {

ABRepeater::ABRepeater(PlayEngine *engine)
: QObject(engine) {
	m_engine = engine;
	m_a = m_b = -1;
	m_repeating = false;
	m_times = m_nth = 0;
}

ABRepeater::~ABRepeater() {
	stop();
}

void ABRepeater::slotTick(int time) {
	if (m_repeating && time > m_b) {
		m_engine->seek(m_a, false, false);
		if (m_times < 0)
			emit repeated(-1);
		else {
			const int rest = m_times - (++m_nth);
			emit repeated(rest);
			if (rest <= 0) {
				stop();
				emit stopped();
			}
		}
	}
}

int ABRepeater::setAToCurrentTime() {
	return (m_a = m_engine->currentTime());
}

int ABRepeater::setBToCurrentTime() {
	return (m_b = m_engine->currentTime());
}

#define SUB_TIME (m_engine->currentTime() - m_engine->syncDelay())

int ABRepeater::setAToSubtitleTime() {
	const Subtitle &sub = m_engine->subtitle();
	if (sub.isEmpty())
		return m_a = -1;
	m_a = sub.start(SUB_TIME, m_engine->frameRate());
	return m_a;
}

int ABRepeater::setBToSubtitleTime() {
	const Subtitle &sub = m_engine->subtitle();
	if (sub.isEmpty())
		return m_b = -1;
	m_b = sub.end(SUB_TIME, m_engine->frameRate());
	return m_b;
}

#undef SUB_TIME

bool ABRepeater::start(int times) {
	if (m_repeating)
		stop();
	m_times = times;
	m_nth = 0;
	if ((m_repeating = (m_a >= 0 && m_b > m_a))) {
		connect(m_engine, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
		emit started();
	}
	return m_repeating;
}

void ABRepeater::stop() {
	disconnect(m_engine, 0, this, 0);
	m_repeating = false;
	emit stopped();
}

}

