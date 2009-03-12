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

void ABRepeater::slotTick(int time) {
	if (m_repeating && time > m_b) {
		m_engine->seek(m_a, false, false);
		if (m_times > 0 && m_times == ++m_nth)
			stop();
	}
}

int ABRepeater::setAToCurrentTime() {
	return (m_a = m_engine->currentTime());
}

int ABRepeater::setBToCurrentTime() {
	return (m_b = m_engine->currentTime());
}

int ABRepeater::setAToSubtitleTime() {
	const Subtitle &sub = m_engine->subtitle();
	if (sub.isEmpty())
		return m_a = -1;
	int time = m_engine->currentTime();
	Subtitle::ConstIterator it = sub.find(time);
	if (it == sub.end()) {
		it = sub.upperBound(time);
		if (it == sub.begin())
			return m_a = -1;
	}
	--it;
	return (m_a = it.key());
}

int ABRepeater::setBToSubtitleTime() {
	const Subtitle &sub = m_engine->subtitle();
	if (sub.isEmpty())
		return m_b = -1;
	int time = m_engine->currentTime();
	const Subtitle::ConstIterator it = sub.upperBound(time);
	return m_b = (it == sub.end() ? m_engine->duration() : it.key());
}

bool ABRepeater::start(int times) {
	if (m_repeating)
		stop();
	m_times = times;
	m_nth = 0;
	if ((m_repeating = (m_a >= 0 && m_b > m_a)))
		connect(m_engine, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
	return m_repeating;
}

void ABRepeater::stop() {
	disconnect(m_engine, 0, this, 0);
	m_repeating = false;
}

}

