#ifndef XINEENGINE_H
#define XINEENGINE_H

#include <xine.h>
#include <QObject>
#include <sys/time.h>
#include "xinenamespace.h"

class QTimer			;					class QWidget;

namespace Xine {

class XineWidget;					class MediaSource;

class XineEngine : public QObject {
	Q_OBJECT
public:
	XineEngine(QWidget *parentWidget = 0);
	~XineEngine();
	bool isInitialized() const {return m_init;}
	bool initialize(const QString &videoDriver = QString("auto"), const QString &audioDriver = QString("auto"));
	xine_t *xine() {return m_xine;}
	QWidget *widget();
	bool play(const QString &mrl);
	State state() const {return m_state;}
	qint64 currentTime() const;
	qint32 tickInterval() const {return m_tickInterval;}
	void setTickInterval(qint32 interval);
	int volume() const {return m_volume;}
	void setVolumeAmplification(qreal amp);
	const MediaSource &currentSource() const;
	bool hasVideo() const;
	bool isSeekable() const;
	qint64 remainingTime() const;
	inline bool isPlaying() const {return state() == PlayingState;}
	inline bool isPaused() const {return state() == PausedState;}
	inline bool isStopped() const {return state() == StoppedState;}
	bool isRunning();
	double speed() const;
public slots:
	bool play();
	void pause();
	void stop();
	void seek(qint64 time);
	void setVolume(int volume);
	void setCurrentSource(const MediaSource &source);
	void setSpeed(double speed);
	void replay();
	void seekChapter(int number);
	void playTitle(int number);
signals:
	void currentSourceChanged(const Xine::MediaSource &source);
	void aboutToFinished();
	void finished();
	void started();
	void hasVideoChanged(bool hasVideo);
	void seekableChanged(bool isSeekable);
	void speedChanged(double speed);
	void stopped(qint64 time);
	void aboutToStarted();
	void tick(qint64 time);
	void totalTimeChanged(qint64 time);
	void stateChanged(State newState, State oldState);
	void volumeChanged(int volume);
private slots:
	void emitTick();
private:
	void setState(State state);
	static void eventListener(void *user_data, const xine_event_t *event);
	bool updateTime();
	xine_t *m_xine;
	xine_stream_t *m_stream;
	xine_audio_port_t *m_audioPort;
	xine_event_queue_t *m_eventQueue;
	XineWidget *m_widget;
	QWidget *m_parentWidget;
	bool m_init;
	State m_state;
	qint64 m_curTime, m_totalTime;
	qint32 m_tickInterval;
	QTimer *m_ticker;
	timeval m_lastTimeUpdated;
	int m_volume;
	qreal m_volAmp;
};

}

#endif
