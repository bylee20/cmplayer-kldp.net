#ifndef BACKEND_XINE_PLAYENGINE_H
#define BACKEND_XINE_PLAYENGINE_H

#include <backend/playengine.h>
#include <backend/mediasource.h>
#include <xine.h>

namespace Backend {

namespace Xine {

class AudioOutput;						class VideoOutput;
class XinePost;							class SubtitleOutput;
class XineStream;

class PlayEngine : public Backend::PlayEngine {
	Q_OBJECT
public:
	PlayEngine(const Backend::BackendIface *factory, QObject *parent);
	~PlayEngine();
	virtual bool supportsSeekingByBytes() const {return true;}
	virtual int maximumBytes() const {return 65535;}
	virtual bool supportsDvdMenu() const {return true;}
	virtual bool supportsSeekingByDrag() const {return true;}
	XineStream *stream();
//slots	
	virtual void toggleDvdMenu();
	virtual void play(int start);
	virtual void play();
	virtual void pause();
	virtual void stop();
	virtual void seek(int time, bool relative, bool showTime);
	virtual void seekByBytes(int);
	bool isSeeking() const;
protected:
	void updateSpeed(double speed);
private slots:
	void slotStateChanged(Backend::State state, Backend::State old);
private:
	void updateCurrentSource(const MediaSource &source);
	void stopThreads();
	void updateState(State state);
	void updateInfo();
	void updateMediaInfo();
	friend class SeekThread;
	friend class TickThread;
	friend class XineStream;
	friend class Data;
	class Data;
	Data *d;
};

}

}

#endif
