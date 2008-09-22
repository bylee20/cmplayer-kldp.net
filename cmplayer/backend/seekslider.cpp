#include "seekslider.h"
#include "playengine.h"
#include "info.h"
#include <backend/factoryiface.h>

namespace Backend {

SeekSlider::SeekSlider(PlayEngine *engine, QWidget *parent)
: JumpSlider(parent), m_engine(0) {
	setEngine(engine);
}

void SeekSlider::setEngine(PlayEngine *engine) {
	if (m_engine == engine)
		return;
	if (m_engine) {
		m_engine->disconnect(this);
		this->disconnect(m_engine);
		this->disconnect(this);
	}
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
	m_engine = engine;
}

void SeekSlider::setDraggingEnded() {
	m_drag = false;
	if (m_dragPos != -1) {
		if (m_byteSeek)
			m_engine->seekByBytes(m_dragPos);
		else
			m_engine->seek(m_dragPos, false, false);
		m_dragPos = -1;
	}
// 		m_engine->seek(m_dragPos, false, false);
}

void SeekSlider::seek(int value) {
	if (m_tick)
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

}
