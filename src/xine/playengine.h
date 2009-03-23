#ifndef XINE_PLAYENGINE_H
#define XINE_PLAYENGINE_H

#include <core/namespace.h>
#include <core/playengine.h>
#include <QtCore/QObject>
#include <xine.h>

namespace Xine {

class PlayEngine : public Core::PlayEngine {
	Q_OBJECT
public:
	PlayEngine(QObject *parent = 0);
	~PlayEngine();
	virtual int currentTime() const;
	virtual void play();
	virtual void play(int time);
	virtual void stop();
	virtual void pause();
	virtual void seek(int time);
	virtual const Core::Info &info() const;
	virtual void toggleDvdMenu();
	virtual void expand(bool activated);
protected:
	void customEvent(QEvent *event);
	virtual bool updateVideoRenderer(const QString &name);
	virtual bool updateAudioRenderer(const QString &name);
private slots:
	void emitTick();
	void slotStateChanged(Core::State state, Core::State old);
private:
	static int convertColorProperty(double value) {
		return Core::ColorProperty::isSame(value, 0.0) ? 32768
			: qBound(0, qRound((value + 1.0)/2.0*65535), 65535);
	}
	void updateTracks();
	void updateSpus();
	virtual bool updateCurrentSpu(const QString &spu);
	virtual bool updateCurrentTrack(const QString &track);
	virtual void updateSpeed(double speed);
	virtual void updateVolume();
	virtual void updateColorProperty(Core::ColorProperty::Value prop, double value);
	virtual void updateColorProperty();
	int getStreamTime() const;
	bool updateStreamInfo();
	bool updateDuration();
	void updateVideoSize();
	void updateVideoSize(const QSize &size);
	static void eventListener(void *userData, const xine_event_t *event);
	struct Data;
	Data *d;
};

}

#endif
