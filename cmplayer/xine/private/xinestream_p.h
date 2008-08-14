#ifndef SEEKTHREAD_H_
#define SEEKTHREAD_H_

#include <QThread>

namespace Xine {

class XineStream;

class SeekThread : public QThread {
	Q_OBJECT
public:
	SeekThread(XineStream *stream);
	virtual ~SeekThread();
	void setTime(int time, bool paused) {m_time = time; m_paused = paused;}
	void setPos(int pos, bool paused) {m_pos = pos; m_paused = paused;}
	void setSeekTime(bool time) {m_seekTime = time;}
private:
	virtual void run();
	int m_time, m_before;
	int m_pos, m_count;
	bool m_seekTime;
	bool m_paused;
	XineStream *m_stream;
};

class TickThread : public QThread {
	Q_OBJECT
public:
	TickThread(XineStream *stream);
	int currentTime() const {return m_time;}
	int currentPos() const {return m_pos;}
signals:
	void tick(int time);
	void tickPos(int pos);
private:
	virtual void run();
	XineStream *m_stream;
	mutable int m_time;
	mutable int m_pos;
};

}

#endif /* SEEKTHREAD_H_ */
