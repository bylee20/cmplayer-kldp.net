#ifndef XINE_XINESTREAM_H_
#define XINE_XINESTREAM_H_

#include <QObject>
#include <xine.h>
#include "mediasource.h"
#include "xinenamespace.h"

namespace Xine {

class AudioOutput;						class VideoOutput;
class XinePost;							class SubtitleOutput;
class ABRepeater;

class XineStream : public QObject {
	Q_OBJECT
public:
	XineStream(QObject *parent);
	~XineStream();
	bool isOpen() const {return m_open;}
	bool open();
	bool open(const QString &videoDriver, const QString &audioDriver);
	void close();
	AudioOutput *audio() {return m_audio;}
	VideoOutput *video() {return m_video;}
	xine_stream_t *stream() {return m_stream;}
	SubtitleOutput *subtitle() {return m_subout;}
	int currentTime() const;
	int currentPos() const;
	void setCurrentSource(const MediaSource &source);
	const MediaSource &currentSource() const {return m_source;}
	State state() const {return m_state;}
	int tickInterval() const {return 80;}
	void setTickInterval(qint32 interval);
	int totalTime() const {return m_totalTime;}
	bool isPlaying() const {return state() == PlayingState;}
	bool isPaused() const {return state() == PausedState;}
	bool isStopped() const {return state() == StoppedState;}
	qreal speed() const {return m_speed;}
	bool isSeekable() const {return m_seekable;}
	bool hasVideo() const {return m_hasVideo;}
	void unwireAudioPosts();
	void unwireVideoPosts();
	void wireAudioPosts();
	void wireVideoPosts();
	void rewireAudioPosts() {unwireAudioPosts(); wireAudioPosts();}
	void rewireVideoPosts() {unwireVideoPosts(); wireVideoPosts();}
	XinePost *addVideoPost(const QString &name);
	XinePost *addAudioPost(const QString &name);
	void removeVideoPost(const QString &name);
	void removeAudioPost(const QString &name);
	bool isSeeking() const;
	ABRepeater *repeater() {return m_repeater;}
public slots:
	void showOsdText(const QString &text, int time = 5000);
	void showTimeBar();
	void toggleDvdMenu();
	void play(int start);
	void play();
	void pause();
	void stop();
	void seek(int time, bool relative, bool showTimeBar = true);
	void seekPos(int pos);
	void setSpeed(qreal speed);
signals:
	void tickPos(int pos);
	void tick(int time);
	void totalTimeChanged(int time);
	void stateChanged(Xine::State newState, Xine::State oldState);
	void hasVideoChanged(bool hasVideo);
	void seekableChanged(bool seekable);
	void currentSourceChanged(const Xine::MediaSource &source);
	void openStateChanged(bool open);
	void started();
	void stopped(Xine::MediaSource source, int time);
	void finished(Xine::MediaSource source);
	void speedChanged(double speed);
protected:
	bool event(QEvent *event);
private:
	static void eventListener(void *user_data, const xine_event_t *event);
	XinePost *makePost(const QString &name);
	void updateInfo();
	void updateMediaInfo();
	void setState(State state);
	friend class SeekThread;
	friend class TickThread;
	friend class Data;
	class Data;
	Data *d;
	bool m_open, m_hasVideo, m_seekable;
	VideoOutput *m_video;
	AudioOutput *m_audio;
	SubtitleOutput *m_subout;
	ABRepeater *m_repeater;
	xine_stream_t *m_stream;
	MediaSource m_source;
	State m_state;
	double m_speed;
	int m_totalTime;
};

}

#endif
