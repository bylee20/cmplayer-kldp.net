#include "subtitle.h"
#include "subtitleoutput.h"
#include "xinestream.h"
#include "abrepeater.h"

namespace Xine {


ABRepeater::ABRepeater(XineStream *parent)
: QObject(parent) {
	m_stream = parent;
	m_a = m_b = -1;
	m_repeating = false;
	m_times = m_nth = 0;
}

void ABRepeater::slotTick(int time) {
	if (m_repeating && time > m_b) {
		m_stream->seek(m_a, false, false);
		if (m_times > 0 && m_times == ++m_nth)
			stop();
	}
}

int ABRepeater::setAToCurrentTime() {
	return (m_a = m_stream->currentTime());
}

int ABRepeater::setBToCurrentTime() {
	return (m_b = m_stream->currentTime());
}

int ABRepeater::setAToSubtitleTime() {
	SubtitleOutput *subout = m_stream->subtitle();
	if (!subout)
		return m_a = -1;
	int curTime = m_stream->currentTime();
	const Subtitle &sub = subout->currentSubtitle();
	Subtitle::ConstIterator it = sub.find(curTime);
	if (it == sub.end() && (it = sub.upperBound(curTime)) != sub.begin())
		--it;
	return (m_a = it.key());
}

int ABRepeater::setBToSubtitleTime() {
	SubtitleOutput *subout = m_stream->subtitle();
	if (!subout)
		return m_b = -1;
	int curTime = m_stream->currentTime();
	const Subtitle &sub = subout->currentSubtitle();
	Subtitle::ConstIterator it = sub.upperBound(curTime);
	return (m_a = it == sub.end() ? m_stream->totalTime() : it.key());
}

bool ABRepeater::start(int times) {
	if (m_repeating)
		stop();
	m_times = times;
	m_nth = 0;
	if (m_repeating = (m_a >= 0 && m_b > m_a))
		connect(m_stream, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
	return m_repeating;
}

void ABRepeater::stop() {
	disconnect(m_stream, 0, this, 0);
	m_repeating = false;
}

}

