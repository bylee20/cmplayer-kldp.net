#include "seekslider.h"
#include "playengine.h"
#include "info.h"
#include "private/backendiface.h"

namespace Backend {

SeekSlider::SeekSlider(QWidget *parent)
: JumpSlider(parent), m_engine(0) {
}

void SeekSlider::setPlayEngine(PlayEngine *engine) {
	if (m_engine == engine)
		return;
	m_tick = m_drag = false;
	m_dragPos = -1;
	Backend::Info *info = engine->factory()->info();
	if ((m_byteSeek = info->supportsSeekingByBytes())) {
		setRange(0, engine->maximumBytes());
		setValue(engine->currentBytes());
		connect(engine, SIGNAL(tickBytes(int)), this, SLOT(slotTick(int)));
	} else {
		setRange(0, engine->totalTime());
		setValue(engine->currentTime());
		connect(this, SIGNAL(sliderPressed()), this, SLOT(setDraggingStarted()));
		connect(this, SIGNAL(sliderReleased()), this, SLOT(setDraggingEnded()));
		connect(engine, SIGNAL(totalTimeChanged(int))
				, this, SLOT(setTotalTime(int)));
		connect(engine, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
	}
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(seek(int)));
	m_dragKeep = !info->supportsSeekingByDrag();
	setEnabled(engine->isSeekable());
	connect(engine, SIGNAL(seekableChanged(bool)), this, SLOT(setEnabled(bool)));
	connect(engine, SIGNAL(destroyed(QObject*)), this, SLOT(removeEngine(QObject*)));
	m_engine = engine;
}

void SeekSlider::setDraggingEnded() {
	m_drag = false;
	if (m_dragPos == -1 || !m_engine)
		return;
	if (m_byteSeek)
		m_engine->seekByBytes(m_dragPos);
	else
		m_engine->seek(m_dragPos, false, false);
	m_dragPos = -1;
}

void SeekSlider::seek(int value) {
	if (m_tick || !m_engine)
		return;
	if (m_dragKeep && m_drag)
		m_dragPos = value;
	else {
		if (m_byteSeek)
			m_engine->seekByBytes(value);
		else
			m_engine->seek(value, false, false);
		m_dragPos = -1;
	}
}

void SeekSlider::removeEngine(QObject *obj) {
	if (obj == m_engine)
		m_engine = 0;
}

}
