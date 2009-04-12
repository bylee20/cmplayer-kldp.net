#include "pipeline.h"
#include <core/utility.h>
#include <QtCore/QCoreApplication>
#include "qwidgetvideosink.h"
#include "nativerenderer.h"
#include "textoverlay.h"
#include "volumenormalizer.h"
#include <core/newframeevent.h>
#include <core/mediasource.h>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>

namespace Gst {

PlayBin::PlayBin(QObject *parent)
: QThread(parent), d(new Data) {
	d->capsHandler = 0;
	d->bus = 0;
	d->video = 0;
	d->subOverlay = new TextOverlay;
	d->msgOverlay = new TextOverlay;
	
	qRegisterMetaType<GstState>("GstState");
	d->quit = false;

	audioBin = gst_bin_new("audioBin");
	d->volnorm = GST_ELEMENT(g_object_new(VolumeNormalizer::gtype(), 0));
	gst_object_ref(GST_OBJECT(audioBin));
	gst_object_sink(GST_OBJECT(audioBin));
	GstElement *aqueue = gst_element_factory_make("queue", 0);
	GstElement *aconv = gst_element_factory_make("audioconvert", 0);
	GstElement *aresample = gst_element_factory_make("audioresample", 0);
	volume = gst_element_factory_make("volume", 0);
	GstElement *asink = gst_element_factory_make("alsasink", 0);
	gst_bin_add_many(GST_BIN(audioBin), aqueue, aconv, d->volnorm, aresample, volume, asink, NULL);
	gst_element_link_many(aqueue, aconv, d->volnorm, aresample, volume, asink, NULL);
	GstPad *apad = gst_element_get_pad(aqueue, "sink");
	gst_element_add_pad(audioBin, gst_ghost_pad_new("sink", apad));
	gst_object_unref(apad);
	g_object_set(G_OBJECT(asink), "sync", 1, NULL);

	videoBin = gst_bin_new("videoBin");
	gst_object_ref(GST_OBJECT(videoBin));
	gst_object_sink(GST_OBJECT(videoBin));
	GstElement *vqueue = gst_element_factory_make("queue", 0);
	GstElement *vconv = gst_element_factory_make("ffmpegcolorspace", 0);
	GstElement *vrate = gst_element_factory_make("videorate", 0);
	videoBalance = gst_element_factory_make("videobalance", 0);
	videoBox = gst_element_factory_make("videobox", 0);
	d->videoLast = d->msgOverlay->element();
	gst_bin_add_many(GST_BIN(videoBin), vqueue, vconv, videoBalance, vrate, videoBox
			, d->subOverlay->element(), d->msgOverlay->element(), NULL);
	gst_element_link_many(vqueue, vconv, videoBalance, vrate, videoBox
			, d->subOverlay->element(), d->msgOverlay->element(), NULL);
	GstPad *vpad = gst_element_get_pad(vqueue, "sink");
	gst_element_add_pad(videoBin, gst_ghost_pad_new("sink", vpad));
	gst_object_unref(vpad);

	bin = gst_element_factory_make("playbin", 0);
	gst_object_ref(GST_OBJECT(bin));
	gst_object_sink(GST_OBJECT(bin));
	
	g_object_set(G_OBJECT(bin), "video-sink", videoBin, NULL);
	g_object_set(G_OBJECT(bin), "audio-sink", audioBin, NULL);

	startBus();
}

PlayBin::~PlayBin() {
	stop();
	stopBus();
	gst_object_unref(G_OBJECT(bin));
	gst_object_unref(G_OBJECT(audioBin));
	gst_object_unref(G_OBJECT(videoBin));
	delete d->subOverlay;
	delete d->msgOverlay;
	delete d;
	g_source_remove_by_user_data(this);
}

void PlayBin::setSource(const Core::MediaSource &source) {
	const QString uri = source.isDisc() ? "dvd://" : source.url().toString();
	g_object_set(G_OBJECT(bin), "uri", uri.toLocal8Bit().data(), NULL);
}

void PlayBin::stop() {
	if (bin) {
		GstState state;
		setState(GST_STATE_NULL);
		gst_element_get_state(bin, &state, 0, 2000000);
	}
}

int PlayBin::clock() const {
	gint64 pos = 0;
	GstFormat format = GST_FORMAT_TIME;
	gst_element_query_position(GST_ELEMENT(bin), &format, &pos);
	return (pos / GST_MSECOND);
}

int PlayBin::duration() const {
	GstFormat format = GST_FORMAT_TIME;
	gint64 duration = 0;
	if (gst_element_query_duration(GST_ELEMENT(bin), &format, &duration))
		return duration / GST_MSECOND;
	return -1;
}

void PlayBin::startBus() {
	d->mutex.lock();
	d->bus = gst_pipeline_get_bus(GST_PIPELINE(bin));
	d->mutex.unlock();
	start();
}

void PlayBin::stopBus() {
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

void PlayBin::emitBusSignal(GstMessage *message) {
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
		if (message->src != GST_OBJECT(bin))
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

void PlayBin::run() {
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

void PlayBin::setVerticalMargin(int margin) {
	g_object_set(G_OBJECT(videoBox), "top", margin, NULL);
	g_object_set(G_OBJECT(videoBox), "bottom", margin, NULL);
}

void PlayBin::setVideo(GstVideoIface *video) {
	if (d->video)
		gst_bin_remove(GST_BIN(videoBin), d->video->sink());
	setVerticalMargin(0);
	d->subOverlay->clear();
	d->msgOverlay->clear();
	if ((d->video = video)) {
		gst_bin_add(GST_BIN(videoBin), d->video->sink());
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

void PlayBin::customEvent(QEvent */*event*/) {

}

Core::AbstractOsdRenderer *PlayBin::subtitleOsd() {
	return d->subOverlay;
}

Core::AbstractOsdRenderer *PlayBin::messageOsd() {
	return d->msgOverlay;
}

PtrList<GObject> PlayBin::getStream(const QString &name) {
	PtrList<GObject> info;
	if (!bin)
		return info;
	GValueArray *arr = 0;
	g_object_get(bin, "stream-info-value-array", &arr, NULL);
	if (!arr)
		return info;
	for (uint i = 0; i < arr->n_values; ++i) {
		GObject *obj = reinterpret_cast<GObject*>(
				g_value_get_object(g_value_array_get_nth(arr, i)));
		if (!obj)
			continue;
		gint type = -1;
		g_object_get(obj, "type", &type, NULL);
		GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(obj), "type");
		GEnumValue *value = g_enum_get_value(G_PARAM_SPEC_ENUM(pspec)->enum_class, type);
		if (value && (name == value->value_nick || name == value->value_name)) {
			info.append(obj);
		}
	}
	g_value_array_free(arr);
	return info;
}

bool PlayBin::setState(GstState state) {
	return gst_element_set_state(bin, state) != GST_STATE_CHANGE_FAILURE;
}

void PlayBin::setVolumeNormalized(bool on) {
	GstBaseTransform *trans = GST_BASE_TRANSFORM(d->volnorm);
	const bool through = gst_base_transform_is_passthrough(trans);
	if (on == through)
		gst_base_transform_set_passthrough(trans, !through);
}

void PlayBin::updateOverlay() {
	if (d->video && d->video->renderer()->type() == Core::Native)
		static_cast<NativeRenderer*>(d->video->renderer())->setOverlay();
}

}
