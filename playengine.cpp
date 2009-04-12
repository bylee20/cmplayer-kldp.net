#include "playengine.h"
#include "videorendererfactory.h"
#include "nativerenderer.h"
// #include <core/glrenderer.h>
#include <QtGui/QResizeEvent>
#include <QtGui/QWidget>
#include <QtCore/QDebug>
#include <QtGui/QApplication>
// #include <core/openglvideorendereriface.h>
#include "glrenderer.h"
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <core/utility.h>
#include "events.h"
#include <core/subtitle.h>
#include "glrenderer.h"
#include "qwidgetvideosink.h"
#include "info.h"
#include <core/xvideorenderer.h>
#include "pipeline.h"
#include "busthread.h"
#include <core/baseevent.h>

namespace Gst {

class UpdateVideoInfoEvent : public Core::BaseEvent {
public:
	static const int type = Core::BaseEvent::UserType + 1;
	UpdateVideoInfoEvent(GstPad *pad): Core::BaseEvent(type), pad(pad) {}
	GstPad *pad;
};
	
struct PlayEngine::Data {
	GstVideoIface *video;
	bool sos, eos, gotInfo;
	int prevTick, startTime;
	QTimer ticker;
	Info info;
	PlayBin *play;
};

PlayEngine::PlayEngine(QObject *parent)
: Core::PlayEngine(parent), d(new Data) {
	static bool init = false; if (!init) {gst_init(0, 0); init = true;}
	d->play = new PlayBin();
	d->video = 0;
	d->eos = d->sos = d->gotInfo = false;
	d->prevTick = 0;
	d->startTime = -1;

	d->ticker.setInterval(80);
	connect(&d->ticker, SIGNAL(timeout()), this, SLOT(emitTick()));

	connect(d->play, SIGNAL(durationChanged(int)), this, SLOT(slotDurationChanged(int)));
	connect(d->play, SIGNAL(stateChanged(GstState, GstState))
		, this, SLOT(slotGstStateChanged(GstState, GstState)));
	connect(d->play, SIGNAL(ended()), this, SLOT(slotFinished()));
	
	setVideoRenderer(d->info.defaultVideoRenderer());
	setAudioRenderer(d->info.defaultAudioRenderer());
}

PlayEngine::~PlayEngine() {
	stop();
//	delete d->bus;
// 	d->busReader.stop();
// 	gst_object_unref(GST_OBJECT(d->bus));
// 	d->bus = 0;
	delete d->play;
//	setGstState(GST_STATE_NULL);
//	gst_object_unref(GST_OBJECT(d->play));
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
		if (old == GST_STATE_READY)
			updateVideoInfo();
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
	} case UpdateVideoInfoEvent::type:
		updateVideoInfo(static_cast<UpdateVideoInfoEvent*>(be)->pad);
		break;
	default:
		break;
	}
}

bool PlayEngine::updateDuration() {
	const int duration = d->play->duration();
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
	if (gst_element_query(d->play->bin, query)) {
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
	const int time = d->play->clock();
	if (time != d->prevTick)
		emit tick(d->prevTick = time);
}

bool PlayEngine::setGstState(GstState gstate, Core::State state) {
	if (!d->play->setState(gstate))
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
		return d->play->clock();
	} else
		return 0;
}

void PlayEngine::play() {
	if (currentSource().isValid() && !isPlaying()) {
		d->play->updateOverlay();
		setGstState(GST_STATE_PLAYING, Core::Playing);
	}
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
	gst_element_seek(d->play->bin, speed(), GST_FORMAT_TIME, flags
			, GST_SEEK_TYPE_SET, time * GST_MSECOND, GST_SEEK_TYPE_NONE
			, GST_CLOCK_TIME_NONE);
}

void PlayEngine::updateCurrentSource(const Core::MediaSource &source) {
	d->gotInfo = false;
	if (source.isValid()) {
		setGstState(GST_STATE_NULL, Core::Stopped);
		d->play->setSource(source);
		d->sos = true;
		d->startTime = -1;
	}
}

void PlayEngine::updateVolume() {
	g_object_set(G_OBJECT(d->play->volume), "volume", realVolume(), NULL);
	d->play->setVolumeNormalized(isVolumeNormalized());
}

void PlayEngine::updateSpeed(double speed) {
	gst_element_seek(d->play->bin, speed, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH
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
	GstVideoIface *video = VideoRendererFactory::create(renderer, this);
	if (!video)
		return false;
	if (d->video) {
		setVideoRenderer(0);
		setSubtitleOsd(0);
		setMessageOsd(0);
		setTimeLineOsd(0);
		d->play->setVideo(0);
		delete d->video;
	}
	d->video = video;
	setVideoRenderer(d->video->renderer());
	d->play->setVideo(d->video);
	if (d->video->renderer()->type() == Core::OpenGL) {
		setSubtitleOsd(d->video->renderer()->createOsd());
		setMessageOsd(d->video->renderer()->createOsd());
		setTimeLineOsd(d->video->renderer()->createOsd());
	} else {
		setSubtitleOsd(d->play->subtitleOsd());
		setMessageOsd(d->play->messageOsd());
	}
	updateColorProperty();
	return true;
}

bool PlayEngine::updateAudioRenderer(const QString &renderer) {
	return d->info.audioRenderer().contains(renderer);
}

#define CONV(v) (qBound(-1000, qRound((v)*1000.0), 1000))
void PlayEngine::updateColorProperty(Core::ColorProperty::Value prop, double value) {
	if (!d->video)
		return;
	if (d->video->renderer()->type() == Core::OpenGL) {
		Core::OpenGLVideoRendererIface *renderer
				= static_cast<Core::OpenGLVideoRendererIface*>(d->video->renderer());
		renderer->setColorProperty(colorProperty());
	} else {
		NativeRenderer *r = static_cast<NativeRenderer*>(d->video);
		switch (prop) {
		case Core::ColorProperty::Brightness:
			if (useSoftwareEqualizer())
				g_object_set(d->play->videoBalance, "brightness", value, NULL);
			else if (r->xType() == NativeRenderer::Xv)
				g_object_set(r->sink(), "brightness", CONV(value), NULL);
			break;
		case Core::ColorProperty::Saturation:
			if (useSoftwareEqualizer())
				g_object_set(d->play->videoBalance, "saturation", value+1.0, NULL);
			else if (r->xType() == NativeRenderer::Xv)
				g_object_set(r->sink(), "saturation", CONV(value), NULL);
			break;
		case Core::ColorProperty::Contrast:
			if (useSoftwareEqualizer())
				g_object_set(d->play->videoBalance, "contrast", value+1.0, NULL);
			else if (r->xType() == NativeRenderer::Xv)
				g_object_set(r->sink(), "contrast", CONV(value), NULL);
			break;
		case Core::ColorProperty::Hue:
			if (useSoftwareEqualizer())
				g_object_set(d->play->videoBalance, "hue", value, NULL);
			else if (r->xType() == NativeRenderer::Xv)
				g_object_set(r->sink(), "hue", CONV(value), NULL);
			break;
		default:
			break;
		}
	}
}

void PlayEngine::updateColorProperty() {
	if (!d->video)
		return;
	const Core::ColorProperty prop = colorProperty();
	if (d->video->renderer()->type() == Core::OpenGL) {
		Core::OpenGLVideoRendererIface *renderer
				= static_cast<Core::OpenGLVideoRendererIface*>(d->video->renderer());
		renderer->setColorProperty(prop);
	} else {
		NativeRenderer *renderer = static_cast<NativeRenderer*>(d->video);
		if (useSoftwareEqualizer()) {
			g_object_set(d->play->videoBalance, "brightness", prop.brightness(), NULL);
			g_object_set(d->play->videoBalance, "saturation", prop.saturation()+1.0, NULL);
			g_object_set(d->play->videoBalance, "contrast", prop.contrast()+1.0, NULL);
			g_object_set(d->play->videoBalance, "hue", prop.hue(), NULL);
		} else if (renderer->xType() == NativeRenderer::Xv) {
			GstElement *sink = renderer->sink();
			g_object_set(sink, "brightness", CONV(prop.brightness()), NULL);
			g_object_set(sink, "saturation", CONV(prop.saturation()), NULL);
			g_object_set(sink, "contrast", CONV(prop.contrast()), NULL);
			g_object_set(sink, "hue", CONV(prop.hue()), NULL);
		}
	}
}
#undef CONV

void PlayEngine::updateVideoInfo() {
	const PtrList<GObject> videoStream = d->play->getStream("video");
	setHasVideo(!videoStream.isEmpty());
	GstPad *pad = 0;
	if (!videoStream.isEmpty()) {
		for (int i=0; !pad && i<videoStream.size(); ++i)
			g_object_get(videoStream[i], "object", &pad, NULL);
		if (pad) {
			updateVideoInfo(pad);
			g_signal_connect(pad, "notify::caps", G_CALLBACK(capsSet), this);
			gst_object_unref(pad);
		}
	}
}

void PlayEngine::capsSet(GObject *obj, GParamSpec */*pspec*/, PlayEngine *self) {
	QApplication::postEvent(self, new UpdateVideoInfoEvent(GST_PAD(obj)));
}

void PlayEngine::updateVideoInfo(GstPad *pad) {
	if (!GST_IS_PAD(pad))
		return;
	GstCaps *caps = gst_pad_get_negotiated_caps(pad);
	if (!caps)
		return;
	GstStructure *s = gst_caps_get_structure (caps, 0);
	if (s) {
		gint fpsN, fpsD, width, height;
		if (!(gst_structure_get_fraction (s, "framerate", &fpsN, &fpsD)
				&& gst_structure_get_int (s, "width", &width)
				&& gst_structure_get_int (s, "height", &height)))
			return;
		setFrameRate(double(fpsN)/double(fpsD));
		if (d->video && d->video->renderer()->type() == Core::Native) {
			NativeRenderer *r = static_cast<NativeRenderer*>(d->video->renderer());
			QSize size(width, height);
			r->setVideoSize(size);
			int height = size.height();
			if (r->videoRatio() > Core::Utility::desktopRatio())
				height = qRound(height*(r->videoRatio() / Core::Utility::desktopRatio()));
			const int margin = qRound((r->videoSize().height() - height)*0.5);
			d->play->setVerticalMargin(margin);
			height = size.height() - margin*2;
			r->setFrameSize(QSize(size.width(), height));
		}
// 		qDebug() << frameRate() << width << height;
// 		qDebug() << gst_structure_get_value (s, "pixel-aspect-ratio");
	}
	gst_caps_unref (caps);
}

}
