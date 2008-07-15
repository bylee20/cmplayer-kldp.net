#include "seekslider.h"
#include "xinestream.h"

namespace Xine {

SeekSlider::SeekSlider(XineStream *stream, QWidget *parent)
: CSlider(parent), m_stream(stream), m_tick(false) {
	setRange(0, m_stream->totalTime());
	setValue(m_stream->currentTime());
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(seek(int)));
	connect(m_stream, SIGNAL(tick(qint64)), this, SLOT(slotTick(qint64)));
	connect(m_stream, SIGNAL(totalTimeChanged(qint64)), this, SLOT(setTotalTime(qint64)));
	connect(m_stream, SIGNAL(seekableChanged(bool)), this, SLOT(setEnabled(bool)));
	setEnabled(m_stream->isSeekable());
	if (stream->tickInterval() <= 0)
		stream->setTickInterval(100);
}

void SeekSlider::setTotalTime(qint64 ms) {
	m_tick = true;
	setRange(0, ms);
	m_tick = false;
}

void SeekSlider::seek(int ms) {
	if (!m_tick) {
		qDebug("slider seek to %d\n", time);
		m_stream->seek(ms);
	}
}

void SeekSlider::slotTick(qint64 ms) {
	m_tick = true;
	setValue(ms);
	m_tick = false;
}

}
