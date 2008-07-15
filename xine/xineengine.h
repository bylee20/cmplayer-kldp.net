#ifndef XINEENGINE_H
#define XINEENGINE_H

#include <xine.h>
#include <QObject>
#include <sys/time.h>
#include "xinenamespace.h"

class QTimer			;					class QWidget;

namespace Xine {

class XineWidget;					class MediaSource;
class XineStream;					class XineAudio;
class XineVideo;

class XineEngine : public QObject {
	Q_OBJECT
public:
	XineEngine(QWidget *parentWidget = 0);
	~XineEngine();
	bool isInitialized() const {return m_init;}
	bool initialize(const QString &videoDriver = QString("auto"), const QString &audioDriver = QString("auto"));
	xine_t *xine() {return m_xine;}
	XineAudio *audio() {return m_audio;}
	XineVideo *video() {return m_video;}
	XineStream *stream() {return m_stream;}
	QWidget *widget();
	const MediaSource &currentSource() const;
	bool hasVideo() const;
	bool isSeekable() const;
	qint64 remainingTime() const;
	bool isRunning();
	double speed() const;
public slots:
	//void setCurrentSource(const MediaSource &source);
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
private:
	xine_t *m_xine;
	XineStream *m_stream;
	XineAudio *m_audio;
	XineVideo *m_video;
	//xine_audio_port_t *m_audioPort;
	QWidget *m_parentWidget;
	bool m_init;
};

}

#endif
