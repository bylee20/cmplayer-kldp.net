#ifndef GST_PLAYENGINE_H
#define GST_PLAYENGINE_H

#include <QtCore/QObject>
#include <gst/gst.h>
#include <core/namespace.h>
#include <core/playengine.h>

class QSize;

namespace Gst {

class PlayEngine : public Core::PlayEngine {
	Q_OBJECT
public:
	PlayEngine(QObject *parent = 0);
	~PlayEngine();
	virtual const Core::Info &info() const;
	virtual int currentTime() const;
	virtual void play();
	virtual void play(int time);
	virtual void stop();
	virtual void pause();
	virtual void seek(int time);
	GstElement *videoSink();
protected:
	void customEvent(QEvent *event);
private slots:
	void emitTick();
	bool updateStreamInfo();
	void slotFinished();
	void slotGstStateChanged(GstState state, GstState old);
	void slotDurationChanged(int duration);
private:
	virtual void updateCurrentSource(const Core::MediaSource &source);
	virtual void updateVolume();
	virtual void updateSpeed(double speed);
	virtual bool updateVideoRenderer(const QString &renderer);
	virtual bool updateAudioRenderer(const QString &renderer);
	static void cbStreamInfoNotify(GObject *obj, GParamSpec *pspec, gpointer data);
	static void foreachTag(const GstTagList *tags, const gchar *tag, gpointer userData);
	bool setGstState(GstState gstate, Core::State state);
// 	bool updateStreamInfo();
	bool updateDuration();
	struct Data;
	Data *d;
};

}

#endif
