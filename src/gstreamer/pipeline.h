#ifndef GST_PIPELINE_H
#define GST_PIPELINE_H

#include <QtCore/QThread>
#include <QtCore/QSize>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <gst/gst.h>

class QWidget;

namespace Core {
class MediaSource;			class VideoRendererIface;
class AbstractOsdRenderer;
}

namespace Gst {

template<typename T>
class PtrList {
public:
	PtrList(): list() {}
	~PtrList() {
		for (int i=0; i<list.size(); ++i)
			unref(list[i]);
	}
	T *operator[] (int i) const {return list[i];}
	void append(T *ptr) {ref(ptr); list.append(ptr);}
	int size() const {return list.size();}
	bool isEmpty() const {return list.isEmpty();}
private:
	void ref(void *ptr) {
		if (GST_IS_OBJECT(ptr))
			gst_object_ref(ptr);
		else if (G_IS_OBJECT(ptr))
			g_object_ref(ptr);
	}
	void unref(void *ptr) {
		if (GST_IS_OBJECT(ptr))
			gst_object_unref(ptr);
		else if (G_IS_OBJECT(ptr))
			g_object_unref(ptr);
	}
	QList<T*> list;
};

class NativeRenderer;			class GstVideoIface;
class TextOverlay;

class PlayBin : public QThread {
	Q_OBJECT
public:
	GstElement *bin, *audioBin, *videoBin;
	GstElement *volume, *videoBox, *videoBalance;
	PlayBin(QObject *parent = 0);
	~PlayBin();
	void setSource(const Core::MediaSource &source);
	int clock() const;
	int duration() const;
	bool setState(GstState state);
	void stop();
	void setVideo(GstVideoIface *video);
	Core::AbstractOsdRenderer *subtitleOsd();
	Core::AbstractOsdRenderer *messageOsd();
	PtrList<GObject> getStream(const QString &name);
	void setVerticalMargin(int margin);
	void setVolumeNormalized(bool on);
	void updateOverlay();
signals:
	void ended();
	void videoSizeChanged(const QSize &size);
	void stateChanged(GstState state, GstState old);
	void durationChanged(int duration);
private:
	void customEvent(QEvent *event);
	void startBus();
	void stopBus();
	void run();
	void linkVideo(GstPad* pad);
	void linkAudio(GstPad *pad);
	void emitBusSignal(GstMessage *message);
	struct Data {
		GstVideoIface *video;
		GstElement *videoLast, *volnorm;
		GstBus *bus;
		gulong capsHandler;
		bool quit;
		QMutex mutex;
		QWaitCondition cond;
		TextOverlay *subOverlay, *msgOverlay;
	};
	Data *d;
};

}

#endif // PIPELINE_H
