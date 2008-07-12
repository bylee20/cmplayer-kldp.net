#include "seekslider.h"
#include "playengine.h"

namespace MPlayer {

SeekSlider::SeekSlider(PlayEngine *engine, QWidget *parent)
: CSlider(parent), m_engine(engine), m_tick(false), m_drag(false), m_dragPos(-1) {
	setRange(0, engine->totalTime());
	setValue(engine->currentTime());
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(seek(int)));
	connect(this, SIGNAL(sliderPressed()), this, SLOT(setDraggingStarted()));
	connect(this, SIGNAL(sliderReleased()), this, SLOT(setDraggingEnded()));
	connect(m_engine, SIGNAL(tick(qint64)), this, SLOT(slotTick(qint64)));
	connect(m_engine, SIGNAL(totalTimeChanged(qint64)), this, SLOT(setTotalTime(qint64)));
	connect(m_engine, SIGNAL(seekableChanged(bool)), this, SLOT(setEnabled(bool)));
	setEnabled(m_engine->isSeekable());
}

void SeekSlider::setTotalTime(qint64 ms) {
	m_tick = true;
	setRange(0, ms);
	m_tick = false;
}

void SeekSlider::setDraggingEnded() {
	m_drag = false;
	if (m_dragPos != -1)
		m_engine->seek(m_dragPos);
}

void SeekSlider::seek(int ms) {
	if (!m_tick) {
		if (m_drag) {
			m_dragPos = value();
		} else {
			m_engine->seek(ms);
			m_dragPos = -1;
		}
	}
}


}
