#ifndef BACKEND_PLAYENGINE_H
#define BACKEND_PLAYENGINE_H

#include <QtCore/QObject>
#include "mediasource.h"
#include "backend.h"

namespace Backend {

class VideoOutput;				class AudioOutput;
class SubtitleOutput;			class ABRepeater;
class FactoryIface;

class PlayEngine : public QObject {
	Q_OBJECT
public:
	PlayEngine(const FactoryIface *factory, QObject *parent);
	virtual ~PlayEngine();
	inline AudioOutput *audio() {return m_audio;}
	inline VideoOutput *video() {return m_video;}
	inline SubtitleOutput *subtitle() {return m_subtitle;}
	inline const AudioOutput *audio() const {return m_audio;}
	inline const VideoOutput *video() const {return m_video;}
	inline const SubtitleOutput *subtitle() const {return m_subtitle;}
	inline int currentTime() const {return m_curTime;}
	inline int currentBytes() const {return m_curBytes;}
	void setCurrentSource(const MediaSource &source);
	inline const MediaSource &currentSource() const {return m_source;}
	inline State state() const {return m_state;}
	inline int totalTime() const {return m_totalTime;}
	inline bool isPlaying() const {return state() == PlayingState;}
	inline bool isPaused() const {return state() == PausedState;}
	inline bool isStopped() const {return state() == StoppedState;}
	inline qreal speed() const {return m_speed;}
	inline bool isSeekable() const {return m_seekable;}
	inline bool hasVideo() const {return m_hasVideo;}
	inline ABRepeater *repeater() {return m_repeater;}
	virtual int maximumBytes() const {return 0;}
	const FactoryIface *factory() const {return m_factory;}
public slots:
	void replay();
	virtual void toggleDvdMenu() {}
	virtual void play(int start) = 0;
	virtual void play() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	virtual void seek(int time, bool relative, bool showTime) = 0;
	virtual void seekByBytes(int) {}
	void setSpeed(double speed);
signals:
	void tickBytes(int pos);
	void tick(int time);
	void totalTimeChanged(int time);
	void stateChanged(Backend::State newState, Backend::State oldState);
	void hasVideoChanged(bool hasVideo);
	void seekableChanged(bool seekable);
	void currentSourceChanged(const Backend::MediaSource &source);
	void openStateChanged(bool open);
	void started();
	void stopped(Backend::MediaSource source, int time);
	void finished(Backend::MediaSource source);
	void speedChanged(double speed);
protected:
	virtual void updateSpeed(double speed) = 0;
	virtual void updateCurrentSource(const MediaSource &source) = 0;
	inline void setState(State state) {
		if (m_state != state) {
			State old = m_state; emit stateChanged(m_state = state, old);}}
	inline void setCurrentTime(int time) {
		if (m_curTime != time) emit tick(m_curTime = time);}
	inline void setCurrentBytes(int bytes) {
		if (m_curBytes != bytes) emit tickBytes(m_curBytes = bytes);}
	inline void setAudio(AudioOutput *audio) {m_audio = audio;}
	inline void setVideo(VideoOutput *video) {m_video = video;}
	inline void setSubtitle(SubtitleOutput *subtitle) {m_subtitle = subtitle;}
	inline void setHasVideo(bool has) {
		if (m_hasVideo != has) emit hasVideoChanged(m_hasVideo = has);}
	inline void setSeekable(bool seekable) {
		if (m_seekable != seekable) emit seekableChanged(m_seekable = seekable);}
	inline void setTotalTime(int time) {
		if (m_totalTime != time) emit totalTimeChanged(m_totalTime = time);}
private:
	bool m_hasVideo, m_seekable;
	VideoOutput *m_video;
	AudioOutput *m_audio;
	SubtitleOutput *m_subtitle;
	ABRepeater *m_repeater;
	MediaSource m_source;
	State m_state;
	double m_speed;
	int m_totalTime, m_curTime, m_curBytes;
	const FactoryIface *m_factory;
};

}

#endif
