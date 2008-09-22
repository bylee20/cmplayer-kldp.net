#ifndef BACKEND_MPLAYER_PLAYENGINE_H
#define BACKEND_MPLAYER_PLAYENGINE_H

#include <backend/playengine.h>

class QStringList;

namespace Backend {

namespace MPlayer {

class VideoOutput;
class SubtitleOutput;			class AudioOutput;

class PlayEngine : public Backend::PlayEngine {
	Q_OBJECT
public:
	PlayEngine(const Backend::FactoryIface *factory, QObject *parent);
	~PlayEngine();
	bool tellmp(const QString &command);
	VideoOutput *videoOutput() const;
	SubtitleOutput *subtitleOutput() const;
	AudioOutput *audioOutput() const;
	bool isRunning();
	void setOSDLevel(int level);
	int osdLevel() const;
public slots:
	virtual void play(int time) {start(time);}
	virtual void play();
	virtual void pause()  {if (isPlaying()) tellmp("pause");}
	virtual void stop();
	virtual void seek(int time, bool relative, bool showTime = false);
	void replay();
	void seekChapter(int number);
	void playTitle(int number);
protected slots:
	void update();
private slots:
	void slotStateChanged(Backend::State state, Backend::State old);
	void slotProcFinished();
private:
	virtual void updateSpeed(double speed);
	virtual void updateCurrentSource(const MediaSource &source);
	void exiting();
	bool start(int time = 0);
	class Data;
	friend class Data;
	friend class MPlayerProcess;
	Data *d;
};

}

}

#endif
