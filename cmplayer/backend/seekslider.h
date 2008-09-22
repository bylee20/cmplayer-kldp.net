#ifndef BACKEND_SEEKSLIDER_H
#define BACKEND_SEEKSLIDER_H

#include "private/jumpslider.h"

namespace Backend {

class PlayEngine;

class SeekSlider : public JumpSlider {
	Q_OBJECT
public:
	SeekSlider(PlayEngine *engine, QWidget *parent = 0);
	void setEngine(PlayEngine *engine);
private slots:
	void setTotalTime(int time) {m_tick = true; setRange(0, time); m_tick = false;}
	void slotTick(int value) {
		m_tick = true;if (!m_dragKeep || !m_drag) setValue(value);m_tick = false;}
	void seek(int value);
	void setDraggingStarted() {m_drag = true;}
	void setDraggingEnded();
private:
	PlayEngine *m_engine;
	bool m_tick, m_drag, m_dragKeep, m_byteSeek;
	int m_dragPos;
};

}

#endif
