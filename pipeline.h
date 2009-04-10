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
	
class NativeRenderer;			class GstVideoIface;
class TextOverlay;

class Pipeline : public QThread {
	Q_OBJECT
public:
	enum Type {Invalid, DecodeBin, PlayBin};
	Pipeline(QObject *parent = 0);
	~Pipeline();
	void setCurrentSource(const Core::MediaSource &source);
	int clock() const;
	int duration() const;
	bool setState(GstState state) {
		return gst_element_set_state(d->pipeline, state)
				!= GST_STATE_CHANGE_FAILURE;}
	void stop();
	GstElement *element() {return d->pipeline;}
	GstElement *videoBin();
	GstElement *videoSink();
	GstElement *volume() {return d->volume;}
	GstElement *decodeBin() {return d->decodeBin;}
	void setVideo(GstVideoIface *video);
	Core::AbstractOsdRenderer *subtitleOsd();
	Core::AbstractOsdRenderer *messageOsd();
signals:
	void ended();
	void videoSizeChanged(const QSize &size);
	void stateChanged(GstState state, GstState old);
	void durationChanged(int duration);
private:
	void customEvent(QEvent *event);
	void setType(Type type);
	void startBus();
	void stopBus();
	void run();
	static void cbNewPad(GstElement *decodeBin, GstPad *pad, gboolean last, gpointer data);
	static void cbNotifyVideoCaps(GObject *obj, GParamSpec *pspec, gpointer data);
	void linkVideo(GstPad* pad);
	void linkAudio(GstPad *pad);
	void removeSource();
	void emitBusSignal(GstMessage *message);
	struct Data {
		GstVideoIface *video;
		GstElement *pipeline, *playBin, *decodeBin, *source;
		GstElement *audioBin, *videoBin, *volume, *videoBox, *videoLast;
		GstBus *bus;
		gulong capsHandler;
		bool quit;
		QMutex mutex;
		QWaitCondition cond;
		Type type;
		TextOverlay *subOverlay, *msgOverlay;
	};
	Data *d;
};

}

#endif // PIPELINE_H
