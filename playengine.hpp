#ifndef PLAYENGINE_HPP
#define PLAYENGINE_HPP

#include <QtCore/QObject>
#include <gst/gst.h>
#include "global.hpp"
#include "mrl.hpp"

class NativeVideoRenderer;	class AudioController;

typedef QMap<MediaMetaData, QVariant> StreamData;

class PlayEngine : public QObject {
	Q_OBJECT
public:
	PlayEngine();
	~PlayEngine();
	int position() const;
	void setMrl(const Mrl &mrl);
	MediaState state() const;
	bool isSeekable() const;
	void setSpeed(double speed);
	double speed() const;
	bool hasVideo() const;
	bool hasAudio() const;
	MediaStatus status() const;
	int duration() const;
	bool atEnd() const;
	NativeVideoRenderer *renderer() const;
	AudioController *audio() const;
	Mrl mrl() const;
	void flush();
	bool isPlaying() const {return state() == PlayingState;}
	bool isPaused() const {return state() == PausedState;}
	bool isStopped() const {return state() == StoppedState;}
	const QList<StreamData> &audioStreams() const;
	int currentAudioStream() const;
public slots:
	void setCurrentAudioStream(int idx);
	bool play();
	void stop();
	bool pause();
	bool seek(int pos);
	void navigateDVDMenu(int cmd);
signals:
	void aboutToFinished();
	void stopped(Mrl mrl, int pos);
	void finished(Mrl mrl);
	void tick(int pos);
	void mrlChanged(const Mrl &mrl);
	void stateChanged(MediaState state, MediaState old);
	void seekableChanged(bool seekable);
	void speedChanged(double speed);
	void positionChanged(int pos);
	void hasVideoChanged(bool has);
	void hasAudioChanged(bool has);
	void streamsChanged();
	void durationChanged(int duration);
	void tagsChanged();
	void statusChanged(MediaStatus status);
private slots:
	void handleGstMessage(void *ptr);
	void ticking();
private:
	void setSeekable(bool seekable);
	void setState(MediaState state);
	void setStatus(MediaStatus status);
	void eos();
	void getStreamInfo();
	void queryDuration();
private:
	static void getAboutToFinish(GstElement *object, gpointer user_data);
	void finish();
	struct Data;
	Data *d;
};

#endif // PLAYENGINE_HPP
