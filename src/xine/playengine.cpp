#include "playengine.h"
#include "events.h"
#include "xinestream.h"
#include "xineengine.h"
#include "nativerenderer.h"
#include "xcbnativerenderer.h"
#include "x11nativerenderer.h"
#include "xinepost.h"
#include "glrenderer.h"
#include "info.h"
#include <core/newframeevent.h>
#include <core/utility.h>
#include <core/subtitle.h>
#include <core/openglfactory.h>
#include <xine/xineutils.h>
#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

namespace Xine {

struct PlayEngine::Data {
	QMap<QString, int> tracks, spus;
	bool sos, eos, gotInfo, expanded, volnorm, eq2;
	int prevTick, prevSeekTime, seekCount;
	QTimer ticker;
	XineStream stream;
	XineVideoIface *video;
	Core::VideoRendererIface *renderer;
	Info info;
};

PlayEngine::PlayEngine(QObject *parent)
: Core::PlayEngine(parent), d(new Data) {
	d->eq2 = d->volnorm = d->expanded = d->sos = d->eos = false;
	d->prevTick = d->prevSeekTime = -1;
	d->seekCount = 0;
	d->video = 0;
	d->renderer = 0;
	d->ticker.setInterval(40);
	connect(&d->ticker, SIGNAL(timeout()), this, SLOT(emitTick()));
	connect(this, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(slotStateChanged(Core::State, Core::State)));
	d->ticker.start();
	updateVideoRenderer(d->info.defaultVideoRenderer());
}

PlayEngine::~PlayEngine() {
	stop();
	d->ticker.stop();
	d->stream.close();
	if (d->video)
		delete d->video;
	setVideoRenderer(0);
	delete d;
}

void PlayEngine::slotStateChanged(Core::State state, Core::State old) {
	Q_UNUSED(old);
	switch (state) {
	case Core::Playing:
		d->sos = false;
		if (d->video)
			d->video->activateMouseTracking(currentSource().isDisc());
		break;
	case Core::Stopped:
	case Core::Finished:
		emit tick(d->prevTick = 0);
		break;
	case Core::Paused:
		break;
	default:
		break;
	}
}

void PlayEngine::eventListener(void *userData, const xine_event_t *event) {
	PlayEngine *self = static_cast<PlayEngine*>(userData);
	switch(event->type) {
	case XINE_EVENT_UI_PLAYBACK_FINISHED: {
		qDebug() << "XINE_EVENT_UI_PLAYBACK_FINISHED";
		QApplication::postEvent(self
				, new Core::EndOfStreamEvent(self->currentSource()));
		break;
	} case XINE_EVENT_FRAME_FORMAT_CHANGE: {
		qDebug() << "XINE_EVENT_FRAME_FORMAT_CHANGE";
		xine_format_change_data_t *data = static_cast<xine_format_change_data_t*>(event->data);
		Core::VideoFrame frame;
		Core::VideoFrame::Info info;
		info.size = QSize(data->width, data->height);
		frame.setInfo(info);
		QApplication::postEvent(self, new Core::NewFrameEvent(frame));
		break;
// 	} case XINE_EVENT_MRL_REFERENCE_EXT: {
// 		xine_mrl_reference_data_ext_t *ref = static_cast<xine_mrl_reference_data_ext_t *>(event->data);
// 		qDebug() << "XINE_EVENT_MRL_REFERENCE_EXT: " << ref->alternative
// 		<< ", " << ref->start_time
// 		<< ", " << ref->duration
// 		<< ", " << ref->mrl
// 		<< ", " << (ref->mrl + strlen(ref->mrl) + 1);
// 		break;
	} case XINE_EVENT_UI_SET_TITLE:
	case XINE_EVENT_UI_CHANNELS_CHANGED: {
		qDebug() << "XINE_EVENT_UI_CHANNELS_CHANGED || XINE_EVENT_UI_SET_TITLE";
		QApplication::postEvent(self, new Core::UpdateInfoEvent);
		break;
	} case XINE_EVENT_SPU_BUTTON: {
		xine_spu_button_t* button = static_cast<xine_spu_button_t*>(event->data);
		Qt::CursorShape shape = Qt::ArrowCursor;
		if (button->direction == 1)
			shape = Qt::PointingHandCursor;
		QApplication::postEvent(self, new SetCursorEvent(shape));
		break;
	} default:
		break;
	}
}

void PlayEngine::customEvent(QEvent *event) {
	Core::BaseEvent *be = static_cast<Core::BaseEvent*>(event);
	if (be->type() == Core::BaseEvent::EndOfStream) {
		setState(Core::Finished);
		emit finished(static_cast<Core::EndOfStreamEvent*>(be)->source());
	} else if (be->type() == Core::BaseEvent::UpdateInfo) {
		updateStreamInfo();
	} else if (be->type() == Core::BaseEvent::NewFrame) {
		updateVideoSize(static_cast<Core::NewFrameEvent*>(be)->frame().info().size);
	} else if (be->type() == SetCursor) {
		if (!d->renderer)
			return;
		const Qt::CursorShape shape = static_cast<SetCursorEvent*>(be)->shape();
		if (shape == Qt::PointingHandCursor)
			d->renderer->target()->setCursor(shape);
		else
			d->renderer->target()->unsetCursor();
	}
}

void PlayEngine::toggleDvdMenu() {
	if (d->stream.stream && currentSource().isDisc() && !isStopped()) {
		xine_event_t event;
		event.type = XINE_EVENT_INPUT_MENU1;
		event.data = 0;
		event.data_length = 0;
		xine_event_send(d->stream.stream, &event);
	}
}

int PlayEngine::getStreamTime() const {
	int time = -1;
	for (int i=0; i<5 && d->stream.stream; ++i) {
		if (xine_get_pos_length(d->stream.stream, 0, &time, 0))
			break;
		xine_usec_sleep(10000);
	}
	return time;
}

bool PlayEngine::updateDuration() {
	int duration = -1;
	for (int i=0; i<5 && d->stream.stream; ++i) {
		if (xine_get_pos_length(d->stream.stream, 0, 0, &duration))
			break;
		xine_usec_sleep(100000);
	}
	if (duration >= 0) {
		setDuration(duration);
		return true;
	} else
		return false;
}

bool PlayEngine::updateStreamInfo() {
	if (!d->stream.stream || !currentSource().isValid())
		return false;
	const int len = xine_get_stream_info(d->stream.stream, XINE_STREAM_INFO_FRAME_DURATION);
	setFrameRate(len > 0 ? (90000.0 / len) : -1.0);
	setHasVideo(xine_get_stream_info(d->stream.stream, XINE_STREAM_INFO_HAS_VIDEO));
	setSeekable(xine_get_stream_info(d->stream.stream, XINE_STREAM_INFO_SEEKABLE));
	updateVideoSize();
	updateTracks();
	updateSpus();
	return updateDuration();
}

void PlayEngine::updateSpus() {
	if (!d->stream.stream || !currentSource().isValid())
		return;
	static const int MaxCountInfo = XINE_STREAM_INFO_MAX_SPU_CHANNEL;
	const int count = xine_get_stream_info(d->stream.stream, MaxCountInfo);
	d->spus.clear();
	char buffer[256] = {'\0'};
	for(int i=0; i<count; ++i) {
		QString spu = "Subtitle " + QString::number(i+1);
		if (xine_get_spu_lang(d->stream.stream, i, buffer)) {
			const QString lang = QString::fromLocal8Bit(buffer);
			if (!lang.isEmpty())
				spu += " : " + lang;
		}
		d->spus[spu] = i;
	}
	const int idx = xine_get_param(d->stream.stream, XINE_PARAM_SPU_CHANNEL);
	const QStringList spus = d->spus.keys();
	setSpus(spus, idx == -1 ? QString() : spus[idx]);
}

bool PlayEngine::updateCurrentSpu(const QString &spu) {
	if (!d->stream.stream)
		return false;
	const int idx = spu.isEmpty() ? -1 : d->spus.value(spu, -2);
	if (idx < -1)
		return false;
	xine_set_param(d->stream.stream, XINE_PARAM_SPU_CHANNEL, idx);
	return true;
}

void PlayEngine::updateTracks() {
	if (!d->stream.stream || !currentSource().isValid())
		return;
	static const int MaxCountInfo = XINE_STREAM_INFO_MAX_AUDIO_CHANNEL;
	const int count = xine_get_stream_info(d->stream.stream, MaxCountInfo);
	d->tracks.clear();
	d->tracks["Auto Track"] = -1;
	char buffer[256] = {'\0'};
	for(int i=0; i<count; ++i) {
		QString track = "Track " + QString::number(i+1);
		if (xine_get_audio_lang(d->stream.stream, i, buffer)) {
			const QString lang = QString::fromLocal8Bit(buffer);
			if (!lang.isEmpty())
				track += " : " + lang;
		}
		d->tracks[track] = i;
	}
	int idx = xine_get_param(d->stream.stream, XINE_PARAM_AUDIO_CHANNEL_LOGICAL) + 1;
	const QStringList tracks = d->tracks.keys();
	if (idx < 0 || idx >= tracks.size())
		idx = 0;
	setTracks(tracks, tracks[0]);
}

bool PlayEngine::updateCurrentTrack(const QString &track) {
	if (!d->stream.stream || !d->tracks.contains(track))
		return false;
	xine_set_param(d->stream.stream, XINE_PARAM_AUDIO_CHANNEL_LOGICAL, d->tracks[track]);
	return true;
}

void PlayEngine::emitTick() {
	switch(state()) {
	case Core::Stopped:
	case Core::Finished:
		return;
	default: {
		const int time = getStreamTime();
		if (time == d->prevTick)
			return;
		d->prevTick = time;
		emit tick(time);
	}}
}

int PlayEngine::currentTime() const {
	if (isPlaying() || isPaused()) {
		if (d->eos)
			return duration();
		if (d->sos)
			return 0;
		return d->prevTick;
	} else
		return 0;
}

void PlayEngine::play(int time) {
	stop();
	if (!d->stream.stream)
		return;
	const Core::MediaSource src = currentSource();
	const QString mrl = src.isDisc() ? "dvd:/" : src.url().toString();
	if (xine_open(d->stream.stream, mrl.toLocal8Bit())) {
		updateStreamInfo();
		xine_play(d->stream.stream, 0, time);
		updateSpeed(speed());
		updateVolume();
		setState(Core::Playing);
	}
}

void PlayEngine::play() {
	if (isPlaying() || !d->stream.stream)
		return;
	if (isPaused()) {
		xine_set_param(d->stream.stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
		updateSpeed(speed());
		updateVolume();
		setState(Core::Playing);
	} else
		play(0);
}

void PlayEngine::stop() {
	if (isStopped())
		return;
	if (d->stream.stream) {
		const int time = currentTime();
		xine_stop(d->stream.stream);
		setState(Core::Stopped);
		emit stopped(currentSource(), time);
	} else
		setState(Core::Stopped);
}

void PlayEngine::pause() {
	if (isPlaying() && d->stream.stream) {
		xine_set_param(d->stream.stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
		setState(Core::Paused);
	}
}

void PlayEngine::seek(int time) {
	if (!d->stream.stream || !isSeekable() || (!isPlaying() && !isPaused()))
		return;
	d->sos = (time <= 0);
	if (time > d->prevTick && time == d->prevSeekTime) {
		time += 5000*(++d->seekCount);
	} else {
		d->prevSeekTime = time;
		d->seekCount = 0;
	}
	xine_play(d->stream.stream, 0, time);
	updateSpeed(speed());
}

void PlayEngine::updateVolume() {
	xine_stream_t *const stream = d->stream.stream;
	if (stream) {
		if (d->volnorm != isVolumeNormalized()) {
			d->volnorm = isVolumeNormalized();
			d->stream.setPost(XineStream::AudioPost, "volnorm", d->volnorm);
		}
		xine_set_param(stream, XINE_PARAM_AUDIO_AMP_LEVEL, qRound(realVolume()*100.0));
		xine_set_param(stream, XINE_PARAM_AUDIO_AMP_MUTE, isMuted() ? 1 : 0);
	}
}

void PlayEngine::updateVideoSize(const QSize &size) {
	if (!d->renderer)
		return;
	if (d->renderer->type() == Core::OpenGL) {
		d->renderer->rerender();
	} else if (d->renderer->type() == Core::Native && d->renderer->videoSize() != size) {
		NativeRenderer *renderer = static_cast<NativeRenderer*>(d->renderer);
		renderer->setVideoSize(size);
		bool needToExpand = !currentSource().isDisc();
		if (needToExpand)
			needToExpand = renderer->videoRatio() > Core::Utility::desktopRatio();
		expand(needToExpand);
	}
}

void PlayEngine::updateVideoSize() {
	if (!d->stream.stream || !d->video || !d->renderer)
		return;
	QSize size;
	size.setWidth(xine_get_stream_info(d->stream.stream, XINE_STREAM_INFO_VIDEO_WIDTH));
	size.setHeight(xine_get_stream_info(d->stream.stream, XINE_STREAM_INFO_VIDEO_HEIGHT));
	if (!size.isEmpty())
		updateVideoSize(size);
}

void PlayEngine::updateSpeed(double speed) {
	const int value = (qAbs(speed-1.0) < 0.01) ? XINE_FINE_SPEED_NORMAL
		: double(XINE_FINE_SPEED_NORMAL)*speed;
	xine_set_param(d->stream.stream, XINE_PARAM_FINE_SPEED, value);
}

const Core::Info &PlayEngine::info() const {
	return d->info;
}

void PlayEngine::updateColorProperty(Core::ColorProperty::Value prop, double value) {
	if (!d->renderer)
		return;
	if (d->renderer->type() == Core::Native) {
		int param = -1;
		switch (prop) {
			case Core::ColorProperty::Brightness:
			param = XINE_PARAM_VO_BRIGHTNESS;
			break;
		case Core::ColorProperty::Saturation:
			param = XINE_PARAM_VO_SATURATION;
			break;
		case Core::ColorProperty::Hue:
			param = XINE_PARAM_VO_HUE;
			break;
		case Core::ColorProperty::Contrast:
			param = XINE_PARAM_VO_CONTRAST;
			break;
		default:
			return;
		}
		if (d->eq2 != useSoftwareEqualizer()) {
			d->eq2 = useSoftwareEqualizer();
			d->stream.setPost(XineStream::VideoPost, "eq2", d->eq2);
		}
		xine_set_param(d->stream.stream, param, convertColorProperty(value));
	}/* else if (d->renderer->type() == Core::OpenGL)
		Core::PlayEngine::updateVideoProperty(prop, value);*/
}

void PlayEngine::updateColorProperty() {
	if (!d->renderer)
		return;
	if (d->renderer->type() == Core::Native && d->stream.stream) {
		if (d->eq2 != useSoftwareEqualizer()) {
			d->eq2 = useSoftwareEqualizer();
			d->stream.setPost(XineStream::VideoPost, "eq2", d->eq2);
		}
		const Core::ColorProperty &prop = colorProperty();
		xine_set_param(d->stream.stream, XINE_PARAM_VO_BRIGHTNESS
				, convertColorProperty(prop.brightness()));
		xine_set_param(d->stream.stream, XINE_PARAM_VO_SATURATION
				, convertColorProperty(prop.saturation()));
		xine_set_param(d->stream.stream, XINE_PARAM_VO_HUE
				, convertColorProperty(prop.hue()));
		xine_set_param(d->stream.stream, XINE_PARAM_VO_CONTRAST
				, convertColorProperty(prop.contrast()));
	}
}

void PlayEngine::expand(bool activated) {
	if (d->expanded == activated)
		return;
	if (activated) {
		XinePost *post = d->stream.addPost(XineStream::VideoPost, "expand");
		XinePost::DoubleParam *param = 0;
		param = (XinePost::DoubleParam*)(post->parameters()->value("aspect"));
		param->setValue(Core::Utility::desktopRatio());
	} else
		d->stream.removePost(XineStream::VideoPost, "expand");
	d->expanded = activated;
}

bool PlayEngine::updateVideoRenderer(const QString &name) {
	const int idx = d->info.videoRenderer().indexOf(name);
	if (idx == -1)
		return false;
	stop();
	setVideoRenderer(0);
	setSubtitleOsd(0);
	setMessageOsd(0);
	setTimeLineOsd(0);
	d->stream.close();
	delete d->renderer;
#if HAS_RAW	
	if (name == "raw" && Core::OpenGLFactory::isAvailable()) {
		GLRenderer *renderer = new GLRenderer(&d->stream);
		d->renderer = renderer->renderer();
		d->video = renderer;
	} else {
#endif
		NativeRenderer *renderer = new XcbNativeRenderer(this, &d->stream);
		d->renderer = renderer;
		d->video = renderer;
#if HAS_RAW
	}
#endif
	d->stream.videoDriver = name;
	if (!d->stream.open(d->video->xineType(), d->video->visual(), eventListener, this))
		return false;
	setSubtitleOsd(d->renderer->createOsd());
	setMessageOsd(d->renderer->createOsd());
	setTimeLineOsd(d->renderer->createOsd());
	setVideoRenderer(d->renderer);
	updateVideoSize();
	return true;
}

bool PlayEngine::updateAudioRenderer(const QString &name) {
	stop();
	d->stream.close();
	const int idx = d->info.audioRenderer().indexOf(name);
	if (idx == -1)
		return false;
	d->stream.audioDriver = idx == 0? "auto" : name;
	return d->stream.open(d->video->xineType(), d->video->visual(), eventListener, this);
}

}
