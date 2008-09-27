#ifndef BACKEND_XINE_PLAYENGINE_P_H
#define BACKEND_XINE_PLAYENGINE_P_H

#include <QtCore/QThread>

namespace Backend {

namespace Xine {

class PlayEngine;

class SeekThread : public QThread {
	Q_OBJECT
public:
	enum Mode {Invalid = 0, Bytes = 1, Time = 2};
	SeekThread(PlayEngine *engine);
	virtual ~SeekThread();
	void setTime(int time, bool paused) {m_time = time; m_paused = paused;}
	void setBytes(int pos, bool paused) {m_pos = pos; m_paused = paused;}
	void setMode(Mode mode) {m_mode = mode;}
private:
	virtual void run();
	Mode m_mode;
	int m_time, m_before;
	int m_pos, m_count;
	bool m_paused;
	PlayEngine *m_engine;
};

class TickThread : public QThread {
	Q_OBJECT
public:
	TickThread(PlayEngine *engine);
	int currentTime() const {return m_time;}
	int currentPos() const {return m_pos;}
private:
	virtual void run();
	PlayEngine *m_engine;
	mutable int m_time;
	mutable int m_pos;
};

}

}

#endif
