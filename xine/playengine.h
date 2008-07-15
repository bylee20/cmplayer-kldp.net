//#ifndef MPLAYERPLAYENGINE_H
//#define MPLAYERPLAYENGINE_H
//
//#include "controller.h"
//#include "mplayer.h"
//
//class QStringList;
//
//namespace MPlayer {
//
//class MediaSource;				class VideoOutput;
//class SubtitleOutput;			class AudioOutput;
//class ABRepeater;
//
//class PlayEngine : public Controller {
//	Q_OBJECT
//public:
//	PlayEngine(QObject *parent);
//	~PlayEngine();
//	const MediaSource &currentSource() const;
//	qint64 currentTime() const;
//	bool hasVideo() const;
//	bool isSeekable() const;
//	qint64 remainingTime() const;
//	State state() const;
//	qint32 tickInterval() const;
//	qint64 totalTime() const;
//	inline bool isPlaying() const {return state() == PlayingState;}
//	inline bool isPaused() const {return state() == PausedState;}
//	inline bool isStopped() const {return state() == StoppedState;}
//	bool tellmp(const QString &command);
//	VideoOutput *videoOutput() const;
//	SubtitleOutput *subtitleOutput() const;
//	AudioOutput *audioOutput() const;
//	ABRepeater *repeater() const;
//	bool isRunning();
//	double speed() const;
//	void setOptions(const QStringList &options);
//	const QStringList &options() const;
//	void setOSDLevel(int level);
//	int osdLevel() const;
//public slots:
//	void setCurrentSource(const MediaSource &source);
//	void stop();
//	inline void pause() {if (isPlaying()) tellmp("pause");}
//	void play();
//	void play(qint64 time) {start(time);}
//	void seek(qint64 time, bool relative = false);
//	void setSpeed(double speed);
//	void replay();
//	void seekChapter(int number);
//	void playTitle(int number);
//signals:
//	void currentSourceChanged(const MPlayer::MediaSource &source);
//	void aboutToFinished();
//	void finished();
//	void started();
//	void hasVideoChanged(bool hasVideo);
//	void seekableChanged(bool isSeekable);
//	void stateChanged(MPlayer::State newstate, MPlayer::State oldstate);
//	void tick(qint64 time);
//	void totalTimeChanged(qint64 newTotalTime);
//	void speedChanged(double speed);
//	void stopped(qint64 time);
//	void aboutToStarted();
//protected slots:
//	void update();
//protected:
//	void link(Controller *controller);
//	void unlink(Controller *controller);
//private slots:
//	void slotStateChanged(MPlayer::State newstate, MPlayer::State oldstate);
//	void slotProcFinished();
//private:
//	void setState(State state);
//	void setCurrentTime(qint64 time);
//	void exiting();
//	bool start(qint64 time = 0);
//	class Data;
//	friend class Data;
//	friend class MPlayerProcess;
//	Data *d;
//};
//
//
//}
//
//#endif
