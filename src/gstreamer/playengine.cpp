#include "playengine.h"
#include <core/glrenderer.h>
#include <QtGui/QResizeEvent>
#include <QtGui/QWidget>
#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include "events.h"
#include <core/subtitle.h>
#include "glrenderer.h"
#include "qwidgetvideosink.h"
#include "info.h"
#include <core/xvideorenderer.h>
#include <QtCore/QThread>
#include "pipeline.h"

namespace Gst {

class BusThread : public QThread {
public:
	BusThread(PlayEngine *engine) {
		this->engine = engine;
		this->bus = 0;
		this->pipeline = 0;
	}
	~BusThread() {
		stop();
		gst_object_unref(GST_OBJECT(bus));
		bus = 0;
	}
	void setPipeline(GstElement *pipeline) {
		this->pipeline = pipeline;
		bus = gst_pipeline_get_bus(GST_PIPELINE(this->pipeline));
	}
	void stop() {
		if (isRunning()) {
			GstBus *temp = bus;
			bus = 0;
			if (!wait(1000)) {
				terminate();
				wait(1000);
			}
			bus = temp;
		}
	}
private:
	void run() {
		while (bus) {
			GstMessage *msg = gst_bus_pop(bus);
			if (!msg) {
				msleep(1);
				continue;
			}
			const GstMessageType type = GST_MESSAGE_TYPE(msg);
			switch (type) {
			case GST_MESSAGE_EOS:
				qDebug("GST_MESSAGE_EOS");
				QApplication::sendEvent(engine, new Core::EndOfStreamEvent);
				break;
			case GST_MESSAGE_TAG:
				qDebug("GST_MESSAGE_TAG");
// 				GstTagList* list = 0;
// 				gst_message_parse_tag(msg, &list);
// 				if (list) {
// 					QMap<QString, QString> tags;
// 					gst_tag_list_foreach(list, foreachTag, &tags);
// 					gst_tag_list_free(list);
// 				}
				break;
			case GST_MESSAGE_STATE_CHANGED: {
				qDebug("GST_MESSAGE_STATE_CHANGED");
				if (msg->src != GST_OBJECT(pipeline))
					break;
				GstState old, state;
				gst_message_parse_state_changed(msg, &old, &state, 0);
				QApplication::sendEvent(engine, new StateChangeEvent(state, old));
				break;
			} case GST_MESSAGE_ERROR: {
				gchar *buffer = 0;
				GError *error = 0;
				gst_message_parse_error(msg, &error, &buffer);
				QString debug = QString::fromUtf8(buffer);
				g_free(buffer);
				buffer = gst_error_get_message(error->domain, error->code);
				QString errorMsg = QString::fromUtf8(buffer);
				g_free(buffer);
				qDebug("Error: %s Message:%s (%s) Code:%d", qPrintable(debug)
						, error->message, qPrintable(errorMsg), error->code);
				g_error_free(error);
				break;
			} case GST_MESSAGE_WARNING: {
				gchar *debug;
				GError *err;
				gst_message_parse_warning(msg, &err, &debug);
				qDebug("Warning: %s\nMessage:%s", debug, err->message);
				g_free(debug);
				g_error_free (err);
				break;
			} case GST_MESSAGE_DURATION: {
				qDebug("GST_MESSAGE_DURATION");
				GstFormat format = GST_FORMAT_TIME;
				gint64 duration = 0;
				gst_message_parse_duration(msg, &format, &duration);
				QApplication::sendEvent(engine, new Core::DurationEvent(duration));
				break;
			}
			case GST_MESSAGE_BUFFERING:
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			case GST_MESSAGE_ELEMENT:
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			case GST_MESSAGE_INFO:
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			case GST_MESSAGE_STREAM_STATUS:
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			case GST_MESSAGE_CLOCK_PROVIDE:
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			case GST_MESSAGE_NEW_CLOCK:
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			case GST_MESSAGE_STEP_DONE:
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			case GST_MESSAGE_LATENCY: //only from 0.10.12
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			case GST_MESSAGE_ASYNC_DONE: //only from 0.10.13
				qDebug("GST_MESSAGE_BUFFERING");
				break;
			default:
				qDebug("%d: Invalid message type!", type);
				break;	
			}
			msleep(1);
		}
	}
	PlayEngine *engine;
	GstElement *pipeline;
	GstBus *bus;
};

struct PlayEngine::Data {
	GLRenderer *video;
	bool sos, eos, gotInfo;
	int prevTick, startTime;
	QTimer ticker;
	Info info;
	Pipeline *pipeline;
};

PlayEngine::PlayEngine(QObject *parent)
: Core::PlayEngine(parent), d(new Data) {
	static bool init = false; if (!init) {gst_init(0, 0); init = true;}
	
	d->pipeline = new Pipeline;
	d->eos = d->sos = d->gotInfo = false;
	d->prevTick = 0;
	d->startTime = -1;
	
	d->video = new GLRenderer(this);
	setVideoRenderer(d->video);
	setSubtitleOsd(d->video->createOsd());
	setMessageOsd(d->video->createOsd());
	setTimeLineOsd(d->video->createOsd());
	
	reinterpret_cast<QWidgetVideoSink*>(d->pipeline->videoSink())->sink = d->video;
	d->video->setVideoBin(d->pipeline->videoBin());

	d->ticker.setInterval(80);
	connect(&d->ticker, SIGNAL(timeout()), this, SLOT(emitTick()));

	connect(d->pipeline, SIGNAL(durationChanged(int)), this, SLOT(slotDurationChanged(int)));
	connect(d->pipeline, SIGNAL(stateChanged(GstState, GstState))
		, this, SLOT(slotGstStateChanged(GstState, GstState)));
	connect(d->pipeline, SIGNAL(ended()), this, SLOT(slotFinished()));
}

PlayEngine::~PlayEngine() {
	stop();
//	delete d->bus;
// 	d->busReader.stop();
// 	gst_object_unref(GST_OBJECT(d->bus));
// 	d->bus = 0;
	delete d->pipeline;
//	setGstState(GST_STATE_NULL);
//	gst_object_unref(GST_OBJECT(d->pipeline));
	delete d;
}

void PlayEngine::slotFinished() {
	d->ticker.stop();
	if (!d->eos) {
		d->eos = true;
		emit finished(currentSource());
		setState(Core::Stopped);
	}
}

void PlayEngine::slotGstStateChanged(GstState state, GstState old) {
	Q_UNUSED(old);
	d->eos = false;
	switch (state) {
	case GST_STATE_PLAYING: {
		d->sos = false;
		d->ticker.start();
		setState(Core::Playing);
		if (d->startTime != -1) {
			seek(d->startTime);
			d->startTime = -1;
		}
		break;
	} case GST_STATE_NULL:
		setState(Core::Stopped);
		d->ticker.stop();
		break;
	case GST_STATE_PAUSED:
		d->ticker.start();
		if (!d->gotInfo)
			d->gotInfo = updateStreamInfo();
		break;
	case GST_STATE_READY:
		setState(Core::Stopped);
		d->ticker.stop();
		break;
	case GST_STATE_VOID_PENDING:
		d->ticker.stop();
		break;
	default:
		break;
	}
}

void PlayEngine::slotDurationChanged(int duration) {
	if (duration > 0)
		setDuration(duration);
	else
		updateDuration();
}

void PlayEngine::cbStreamInfoNotify(GObject */*obj*/, GParamSpec */*pspec*/, gpointer data) {
	PlayEngine *self = reinterpret_cast<PlayEngine*>(data);
	QApplication::postEvent(self, new NotifyStreamInfoEvent());
}

void PlayEngine::foreachTag(const GstTagList *list, const gchar *tag, gpointer /*userData*/) {
// 	QMap<QString, QString> *tags = static_cast<QMap<QString, QString>*>(userData);
	QString value;
	GType type = gst_tag_get_type(tag);
	switch (type) {
	case G_TYPE_STRING: {
		gchar *str = 0;
		gst_tag_list_get_string(list, tag, &str);
		value = QString::fromUtf8(str);
		g_free(str);
		break;
	} case G_TYPE_BOOLEAN: {
		int bval;
		gst_tag_list_get_boolean(list, tag, &bval);
		value = QString::number(bval);
		break;
	} case G_TYPE_INT: {
		int ival;
		gst_tag_list_get_int(list, tag, &ival);
		value = QString::number(ival);
		break;
	} case G_TYPE_UINT: {
		unsigned int uival;
		gst_tag_list_get_uint(list, tag, &uival);
		value = QString::number(uival);
		break;
	} case G_TYPE_FLOAT: {
		float fval;
		gst_tag_list_get_float(list, tag, &fval);
		value = QString::number(fval);
		break;
	} case G_TYPE_DOUBLE: {
		double dval;
		gst_tag_list_get_double(list, tag, &dval);
		value = QString::number(dval);
		break;
	} default:
		qDebug() << "Unsupported tag type:" << g_type_name(type);
		break;
	}
	qDebug() << tag << value;
// 	tags[QString(tag).toUpper()] = value;
}

void PlayEngine::customEvent(QEvent *event) {
	Core::BaseEvent *be = static_cast<Core::BaseEvent*>(event);
	switch (be->type()) {
	case NotifyStreamInfo: {
		if (!currentSource().isValid())
			break;
		break;
	} default:
		break;
	}
}

bool PlayEngine::updateDuration() {
	const int duration = d->pipeline->duration();
	if (duration < 0)
		return false;
	setDuration(duration);
	return true;
}

bool PlayEngine::updateStreamInfo() {
// 	qDebug() << "update!";
	if (!currentSource().isValid())
		return false;
	GstQuery *query = gst_query_new_seeking(GST_FORMAT_TIME);
	if (gst_element_query(d->pipeline->element(), query)) {
		gint64 start, stop;
		gboolean seekable;
		GstFormat format;
		gst_query_parse_seeking(query, &format, &seekable, &start, &stop);
		setSeekable(seekable);
	} else
		qDebug() << "Seekable Query Failed.";
	gst_query_unref(query);
	updateDuration();
	return true;
}

void PlayEngine::emitTick() {
	const int time = d->pipeline->clock();
	if (time != d->prevTick)
		emit tick(d->prevTick = time);
}

bool PlayEngine::setGstState(GstState gstate, Core::State state) {
	if (!d->pipeline->setState(gstate))
		return false;
	setState(state);
	return true;
}

int PlayEngine::currentTime() const {
	if (isPlaying() || isPaused()) {
		if (d->eos)
			return duration();
		if (d->sos)
			return 0;
		return d->pipeline->clock();
	} else
		return 0;
}

void PlayEngine::play() {
	if (currentSource().isValid() && !isPlaying())
		setGstState(GST_STATE_PLAYING, Core::Playing);
}

void PlayEngine::play(int time) {
	d->startTime = time > 1000 ? time : -1;
	play();
}

void PlayEngine::stop() {
	if (!isStopped()) {
		const int time = currentTime();
		setGstState(GST_STATE_READY, Core::Stopped);
		emit stopped(currentSource(), time);
	}
}

void PlayEngine::pause() {
	if (isPlaying())
		setGstState(GST_STATE_PAUSED, Core::Paused);
}

void PlayEngine::seek(int time) {
	if (/*!isSeekable() || */(!isPlaying() && !isPaused()))
		return;
	d->sos = (time <= 0);
	// Go to buffering state, we resume paused state when ready
	GstSeekFlags flags = static_cast<GstSeekFlags>(GST_SEEK_FLAG_KEY_UNIT | GST_SEEK_FLAG_SEGMENT | GST_SEEK_FLAG_FLUSH);
	gst_element_seek(d->pipeline->element(), speed(), GST_FORMAT_TIME, flags
			, GST_SEEK_TYPE_SET, time * GST_MSECOND, GST_SEEK_TYPE_NONE
			, GST_CLOCK_TIME_NONE);
}

void PlayEngine::updateCurrentSource(const Core::MediaSource &source) {
	d->gotInfo = false;
	if (source.isValid()) {
		setGstState(GST_STATE_NULL, Core::Stopped);
		d->pipeline->setCurrentSource(source);
		d->sos = true;
		d->startTime = -1;
	}
}

void PlayEngine::updateVolume() {
	double val = 0.0;
	if (!isMuted())
		val = qBound(0.0, double(volume())/100.0*amplifyingRate(), 10.0);
	g_object_set(G_OBJECT(d->pipeline->volume()), "volume", val, NULL);
}

void PlayEngine::updateSpeed(double speed) {
	gst_element_seek(d->pipeline->element(), speed, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH
			, GST_SEEK_TYPE_NONE, 0, GST_SEEK_TYPE_NONE, 0);
}

const Core::Info &PlayEngine::info() const {
	return d->info;
}

// void PlayEngine::updateVideoProperty(Core::VideoProperty prop, double value) {
// 	switch (prop) {
// 	case Core::Brightness:
// 		g_object_set(G_OBJECT(d->videoBalance), "brightness", value, NULL);
// 		break;
// 	case Core::Hue:
// 		g_object_set(G_OBJECT(d->videoBalance), "hue", value, NULL);
// 		break;
// 	case Core::Saturation:
// 		g_object_set(G_OBJECT(d->videoBalance), "saturation", value + 1.0, NULL);
// 		break;
// 	case Core::Contrast:
// 		g_object_set(G_OBJECT(d->videoBalance), "contrast", value + 1.0, NULL);
// 		break;
// 	default:
// 		break;
// 	}
// }

bool PlayEngine::updateVideoRenderer(const QString &renderer) {
	return d->info.defaultVideoRenderer() == renderer;
}

bool PlayEngine::updateAudioRenderer(const QString &renderer) {
	return d->info.defaultAudioRenderer() == renderer;
}

GstElement *PlayEngine::videoSink() {
	return d->pipeline->videoSink();
}

}
