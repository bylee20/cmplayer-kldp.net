#include "pipeline.h"
#include <core/utility.h>
#include "qwidgetvideosink.h"
#include "nativerenderer.h"
#include "textoverlay.h"
#include <core/newframeevent.h>
#include <core/mediasource.h>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>

namespace Gst {

Pipeline::Pipeline(QObject *parent)
: QThread(parent), d(new Data) {
	d->type = Invalid;
	d->pipeline = d->source = 0;
	d->capsHandler = 0;
	d->bus = 0;
	d->video = 0;
	d->subOverlay = new TextOverlay;
	d->msgOverlay = new TextOverlay;
	
	qRegisterMetaType<GstState>("GstState");
	d->quit = false;

	d->pipeline = gst_pipeline_new(0);
	gst_object_ref(GST_OBJECT(d->pipeline));
	gst_object_sink(GST_OBJECT(d->pipeline));

	d->audioBin = gst_bin_new("audioBin");
	gst_object_ref(GST_OBJECT(d->audioBin));
	gst_object_sink(GST_OBJECT(d->audioBin));
	GstElement *aqueue = gst_element_factory_make("queue", 0);
	GstElement *aconv = gst_element_factory_make("audioconvert", 0);
	GstElement *aresample = gst_element_factory_make("audioresample", 0);
	d->volume = gst_element_factory_make("volume", 0);
	GstElement *asink = gst_element_factory_make("alsasink", 0);
	gst_bin_add_many(GST_BIN(d->audioBin), aqueue, aconv, aresample, d->volume, asink, NULL);
	gst_element_link_many(aqueue, aconv, aresample, d->volume, asink, NULL);
	GstPad *apad = gst_element_get_pad(aqueue, "sink");
	gst_element_add_pad(d->audioBin, gst_ghost_pad_new("sink", apad));
	gst_object_unref(apad);
	g_object_set(G_OBJECT(asink), "sync", 1, NULL);

	d->videoBin = gst_bin_new("videoBin");
	gst_object_ref(GST_OBJECT(d->videoBin));
	gst_object_sink(GST_OBJECT(d->videoBin));
	GstElement *vqueue = gst_element_factory_make("queue", 0);
	GstElement *vconv = gst_element_factory_make("ffmpegcolorspace", 0);
	GstElement *vrate = gst_element_factory_make("videorate", 0);
	d->videoBox = gst_element_factory_make("videobox", 0);
//	d->infoOverlay = new TextOverlay;
//	d->subOverlay = new TextOverlay;
// 	d->videoSink = GST_ELEMENT(g_object_new(QWidgetVideoSink::gtype(), 0));
	d->videoLast = d->msgOverlay->element();
	gst_bin_add_many(GST_BIN(d->videoBin), vqueue, vconv, vrate, d->videoBox
			, d->subOverlay->element(), d->msgOverlay->element(), NULL);
//			, d->videoBox, d->infoOverlay->element(), d->subOverlay->element()
// 			, d->videoSink, NULL);
	gst_element_link_many(vqueue, vconv, vrate, d->videoBox
			, d->subOverlay->element(), d->msgOverlay->element(), NULL);
	GstPad *vpad = gst_element_get_pad(vqueue, "sink");
	gst_element_add_pad(d->videoBin, gst_ghost_pad_new("sink", vpad));
	gst_object_unref(vpad);
// 	g_object_set(G_OBJECT(d->videoSink), "sync", 1, NULL);

	d->decodeBin = gst_element_factory_make("decodebin", 0);
	gst_object_ref(GST_OBJECT(d->decodeBin));
	gst_object_sink(GST_OBJECT(d->decodeBin));

	g_signal_connect(d->decodeBin, "new-decoded-pad", G_CALLBACK(cbNewPad), this);
// 	g_signal_connect(d->decodeBin, "unknown-type", G_CALLBACK(cbUnknownType), this);
// 	g_signal_connect(d->decodeBin, "no-more-pads", G_CALLBACK(cbNoMorePads), this);

	d->playBin = gst_element_factory_make("playbin", 0);
	gst_object_ref(GST_OBJECT(d->playBin));
	gst_object_sink(GST_OBJECT(d->playBin));
}

Pipeline::~Pipeline() {
	stop();
	stopBus();
	removeSource();
	gst_object_unref(G_OBJECT(d->pipeline));
	gst_object_unref(G_OBJECT(d->decodeBin));
	gst_object_unref(G_OBJECT(d->playBin));
	gst_object_unref(G_OBJECT(d->audioBin));
	gst_object_unref(G_OBJECT(d->videoBin));
	delete d->subOverlay;
	delete d->msgOverlay;
	delete d;
	g_source_remove_by_user_data(this);
}

void Pipeline::cbNewPad(GstElement *decodeBin, GstPad *pad, gboolean last, gpointer data) {
	Q_UNUSED(decodeBin);
	Q_UNUSED(last);
	Pipeline *self = reinterpret_cast<Pipeline*>(data);
	GstCaps *caps = gst_pad_get_caps(pad);
	GstStructure *str = gst_caps_get_structure(caps, 0);
	if (g_strrstr(gst_structure_get_name(str), "audio"))
		self->linkAudio(pad);
	else if (g_strrstr(gst_structure_get_name(str), "video"))
		self->linkVideo(pad);
	gst_caps_unref(caps);
}

void Pipeline::linkVideo(GstPad* pad) {
	GstPad *sink = gst_element_get_pad(d->videoBin, "sink");
	if (pad && !GST_PAD_IS_LINKED(sink)) {
		gst_pad_link(pad, sink);
		d->capsHandler = g_signal_connect(pad, "notify::caps", G_CALLBACK(cbNotifyVideoCaps), this);
	}
}

void Pipeline::linkAudio(GstPad *pad) {
	GstPad *sink = gst_element_get_pad(d->audioBin, "sink");
	if (pad && !GST_PAD_IS_LINKED(sink)) {
		gst_pad_link(pad, sink);
	}
}

void Pipeline::cbNotifyVideoCaps(GObject *obj, GParamSpec *pspec, gpointer data) {
	Q_UNUSED(pspec);
	Pipeline *self = reinterpret_cast<Pipeline*>(data);
	GstPad *pad = GST_PAD(obj);
	GstCaps *caps = gst_pad_get_caps(pad);

	g_signal_handler_disconnect(pad, self->d->capsHandler);

	GstStructure *str = 0;
	gint width, height;
	if ((str = gst_caps_get_structure(caps, 0))) {
		if (gst_structure_get_int(str, "width", &width)
				&& gst_structure_get_int (str, "height", &height)) {
			Core::VideoFrame frame;
			Core::VideoFrame::Info info;
			info.size = QSize(width, height);
			frame.setInfo(info);
			QCoreApplication::postEvent(self, new Core::NewFrameEvent(frame));
		}
	}
	gst_caps_unref(caps);
}

void Pipeline::setType(Type type) {
	if (d->type == type)
		return;
	stopBus();

	if (d->type == DecodeBin) {
		removeSource();
		gst_bin_remove(GST_BIN(d->pipeline), d->decodeBin);
		gst_bin_remove(GST_BIN(d->pipeline), d->audioBin);
		gst_bin_remove(GST_BIN(d->pipeline), d->videoBin);
	} else if (d->type == PlayBin) {
		gst_bin_remove(GST_BIN(d->pipeline), d->playBin);
		g_object_set(G_OBJECT(d->pipeline), "video-sink", NULL, NULL);
		g_object_set(G_OBJECT(d->pipeline), "audio-sink", NULL, NULL);
	}

	if (type == DecodeBin) {
		gst_bin_add(GST_BIN(d->pipeline), d->decodeBin);
		gst_bin_add(GST_BIN(d->pipeline), d->audioBin);
		gst_bin_add(GST_BIN(d->pipeline), d->videoBin);
	} else if (type == PlayBin) {
		gst_bin_add(GST_BIN(d->pipeline), d->playBin);
		g_object_set(G_OBJECT(d->pipeline), "video-sink", d->videoBin, NULL);
		g_object_set(G_OBJECT(d->pipeline), "audio-sink", d->audioBin, NULL);
	}

	if ((d->type = type) != Invalid)
		startBus();
// 	d->r->setOverlay();
}

void Pipeline::setCurrentSource(const Core::MediaSource &source) {
	setType(source.isLocalFile() ? DecodeBin : PlayBin);
	if (d->type == DecodeBin) {
		removeSource();
		const QString uri = source.url().toString();
		d->source = gst_element_make_from_uri(GST_URI_SRC, qPrintable(uri), NULL);
		gst_bin_add(GST_BIN(d->pipeline), d->source);
		if (!gst_element_link(d->source, d->decodeBin))
			removeSource();
	} else if (d->type == PlayBin) {
		const QString uri = source.isDisc() ? "dvd://" : source.url().toString();
		g_object_set(G_OBJECT(d->playBin), "uri", qPrintable(uri), NULL);
	}
}

void Pipeline::removeSource() {
	stop();
	if (d->source && d->pipeline) {
		gst_bin_remove(GST_BIN(d->pipeline), d->source);
		d->source = 0;
	}
}

void Pipeline::stop() {
	if (d->pipeline) {
		GstState state;
		setState(GST_STATE_NULL);
		gst_element_get_state(d->pipeline, &state, 0, 2000000);
	}
}

int Pipeline::clock() const {
	gint64 pos = 0;
	GstFormat format = GST_FORMAT_TIME;
	gst_element_query_position(GST_ELEMENT(d->pipeline), &format, &pos);
	return (pos / GST_MSECOND);
}

int Pipeline::duration() const {
	GstFormat format = GST_FORMAT_TIME;
	gint64 duration = 0;
	if (gst_element_query_duration(GST_ELEMENT(d->pipeline), &format, &duration))
		return duration / GST_MSECOND;
	return -1;
}

void Pipeline::startBus() {
	d->mutex.lock();
	d->bus = gst_pipeline_get_bus(GST_PIPELINE(d->pipeline));
	d->mutex.unlock();
	start();
}

void Pipeline::stopBus() {
	if (!d->bus)
		return;
	if (isRunning()) {
		d->quit = true;
		GstBus *temp = d->bus;
		d->mutex.lock();
		d->bus = 0;
		d->cond.wakeAll();
		d->mutex.unlock();
		if (!wait(1000)) {
			terminate();
			wait(1000);
		}
		d->mutex.lock();
		d->bus = temp;
		d->mutex.unlock();
	}
	gst_object_unref(GST_OBJECT(d->bus));
	d->bus = 0;
}

void Pipeline::emitBusSignal(GstMessage *message) {
	const GstMessageType type = GST_MESSAGE_TYPE(message);
	switch (type) {
	case GST_MESSAGE_EOS:
		qDebug("GST_MESSAGE_EOS");
		emit ended();
		break;
	case GST_MESSAGE_TAG:
		qDebug("GST_MESSAGE_TAG");
// 		GstTagList* list = 0;
// 		gst_message_parse_tag(msg, &list);
// 		if (list) {
// 			QMap<QString, QString> tags;
// 			gst_tag_list_foreach(list, foreachTag, &tags);
// 			gst_tag_list_free(list);
// 		}
		break;
	case GST_MESSAGE_STATE_CHANGED: {
		if (message->src != GST_OBJECT(d->pipeline))
			break;
		GstState old, state;
		gst_message_parse_state_changed(message, &old, &state, 0);
		qDebug("GST_MESSAGE_STATE_CHANGED: %d to %d", old, state);
		if (old != state) {
			emit stateChanged(state, old);
		}
		break;
	} case GST_MESSAGE_ERROR: {
		gchar *buffer = 0;
		GError *error = 0;
		gst_message_parse_error(message, &error, &buffer);
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
		gst_message_parse_warning(message, &err, &debug);
		qDebug("Warning: %s\nMessage:%s", debug, err->message);
		g_free(debug);
		g_error_free (err);
		break;
	} case GST_MESSAGE_DURATION: {
		qDebug("GST_MESSAGE_DURATION");
		GstFormat format = GST_FORMAT_TIME;
		gint64 duration = 0;
		gst_message_parse_duration(message, &format, &duration);
		emit durationChanged(duration);
		break;
	} case GST_MESSAGE_BUFFERING:
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
}

void Pipeline::run() {
	while (!d->quit) {
		while (!d->quit && !d->bus)
			d->cond.wait(&d->mutex, 1000);
		if (d->quit)
			break;
		if (d->bus) {
			d->mutex.lock();
			GstMessage *message = gst_bus_pop(d->bus);
			d->mutex.unlock();
			if (message)
				emitBusSignal(message);
		}
		msleep(1);
	}
}

GstElement *Pipeline::videoBin() {
	return d->videoBin;
}

GstElement *Pipeline::videoSink() {
	return d->video ? d->video->sink() : 0;
}

void Pipeline::setVideo(GstVideoIface *video) {
	if (d->video)
		gst_bin_remove(GST_BIN(d->videoBin), d->video->sink());
	if ((d->video = video)) {
		gst_bin_add(GST_BIN(d->videoBin), d->video->sink());
		gst_element_link(d->videoLast, d->video->sink());
		if (video->renderer()->type() == Core::Native) {
			NativeRenderer *r = static_cast<NativeRenderer*>(video->renderer());
			r->setOverlay();
			connect(r, SIGNAL(osdRectChanged(const QRect&))
					, d->subOverlay, SLOT(setRect(const QRect&)));
			connect(r, SIGNAL(osdRectChanged(const QRect&))
					, d->msgOverlay, SLOT(setRect(const QRect&)));
		}
	}
	
}

void Pipeline::customEvent(QEvent *event) {
	if (static_cast<Core::BaseEvent*>(event)->type() == Core::BaseEvent::NewFrame) {
		if (!d->video || d->video->renderer()->type() != Core::Native)
			return;
		Core::NativeRenderer *r = static_cast<Core::NativeRenderer*>(d->video->renderer());
		QSize size = static_cast<Core::NewFrameEvent*>(event)->frame().info().size;
		r->setVideoSize(size);
// 		renderer->setVideoSize(size);
// 		bool needToExpand = !currentSource().isDisc();
// 		if (needToExpand)
// 			needToExpand = renderer->videoRatio() > Core::Utility::desktopRatio();
		if (r->videoRatio() > Core::Utility::desktopRatio())
			size.rheight()*= r->videoRatio() / Core::Utility::desktopRatio();
		const int margin = (r->videoSize().height() - size.height())*0.5 + 0.5;
		g_object_set(G_OBJECT(d->videoBox), "top", margin, NULL);
		g_object_set(G_OBJECT(d->videoBox), "bottom", margin, NULL);
// 		expand(needToExpand);
		r->setFrameSize(size);
	}
}

Core::AbstractOsdRenderer *Pipeline::subtitleOsd() {
	return d->subOverlay;
}

Core::AbstractOsdRenderer *Pipeline::messageOsd() {
	return d->msgOverlay;
}

}
