#ifndef XINE_XINESTREAM_H_
#define XINE_XINESTREAM_H_

#include <QObject>
#include <xine.h>
#include "xinenamespace.h"

class QTimer;

namespace Xine {

class XineEngine;				class XineAudio;
class XineVideo;

class XineStream : public QObject {
	Q_OBJECT
public:
	XineStream(XineEngine *engine);
	~XineStream();
	bool isOpen() const {return m_open;}
	bool open(XineAudio *audio, XineVideo *video);
	void close();
	xine_stream_t *stream() {return m_stream;}
	qint64 currentTime() const;
	bool play(const QString &mrl);
	State state() const {return m_state;}
	qint32 tickInterval() const {return m_tickInterval;}
	void setTickInterval(qint32 interval);
	qint64 totalTime() const {return m_totalTime;}
	bool isPlaying() const {return state() == PlayingState;}
	bool isPaused() const {return state() == PausedState;}
	bool isStopped() const {return state() == StoppedState;}
	bool isSeekable() const {return m_seekable;}
	bool hasVideo() const {return m_hasVideo;}
public slots:
	bool play();
	void pause();
	void stop();
	void seek(qint64 time);
signals:
	void tick(qint64 time);
	void totalTimeChanged(qint64 time);
	void stateChanged(State newState, State oldState);
	void hasVideoChanged(bool hasVideo);
	void seekableChanged(bool seekable);
private slots:
	void emitTick();
private:
	void updateInfo();
	static void eventListener(void *user_data, const xine_event_t *event);
	void setState(State state);
	bool updateTime();
	bool m_open, m_hasVideo, m_seekable, m_validInfo, m_seeking;
	XineEngine *m_engine;
	xine_stream_t *m_stream;
	xine_event_queue_t *m_eventQueue;
	State m_state;
	qint64 m_curTime, m_totalTime;
	qint32 m_tickInterval;
	QTimer *m_ticker;
	timeval m_lastTimeUpdated;
	QString m_mrl;
};

}

#endif /* XINESTREAM_H_ */
