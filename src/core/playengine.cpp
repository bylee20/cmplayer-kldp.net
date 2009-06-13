#include "playengine.h"
#include "subtitle.h"
#include "osdstyle.h"
#include "abstractosdrenderer.h"
#include "subtitlerenderer.h"
#include "videorendereriface.h"
#include "info.h"
#include "abrepeater.h"
#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

namespace Core {

class PlayEngine::Screen : public QWidget {
public:
	Screen() {
		m_widget = 0;
		setMouseTracking(true);
		setAutoFillBackground(true);
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_OpaquePaintEvent);
	}
	~Screen() {
		if (m_widget)
			m_widget->setParent(0);
	}
	QSize sizeHint() const {
		return m_widget ? m_widget->sizeHint() : QWidget::sizeHint();
	}
	void setWidget(QWidget *widget) {
		if (m_widget != widget) {
			if ((m_widget = widget)) {
				m_widget->setParent(this);
				m_widget->move(0, 0);
				m_widget->resize(size());
				m_widget->show();
			}
		}
	}
	QWidget *widget() {return m_widget;}
private:
	void resizeEvent(QResizeEvent *event) {
		QWidget::resizeEvent(event);
		if (m_widget)
			m_widget->resize(size());
	}
	void paintEvent(QPaintEvent *event) {
		QPainter painter(this);
		painter.fillRect(event->rect(), Qt::black);
	}
	QWidget *m_widget;
};

PlayEngine::PlayEngine(QObject *parent)
: QObject(parent), d(new Data) {
	d->hasVideo = d->gotInfo = d->muted = d->seekable = false;
	d->softEq = false;
	d->volnorm = d->subVisible = true;
	d->prevTick = d->prevSubTime = -1;
	d->ampRate = d->pos = d->speed = 1.0;
	d->aspect = d->crop = -1.0;
	d->frameRate = -1.0;
	d->msgOsd = d->timeOsd = 0;
	d->renderer = 0;
	d->subRenderer = 0;
	d->duration = d->syncDelay = 0;
	d->volume = 100;
	d->state = Stopped;
	d->repeater = new ABRepeater(this);
	d->sub = new Subtitle;
	d->source = new MediaSource;
	d->msgStyle = new OsdStyle;
	d->subStyle = new OsdStyle;
	d->timeStyle = new OsdStyle;
	d->screen = new Screen;
	d->msgStyle->alignment = Qt::AlignLeft | Qt::AlignTop;
	d->msgStyle->scale = OsdStyle::FitToHeight;
	d->subStyle->alignment = Qt::AlignHCenter | Qt::AlignBottom;
	d->subStyle->font.setBold(true);
	d->timeStyle->alignment = Qt::AlignCenter;
	d->timeStyle->fgColor.setAlphaF(0.8);
	d->timeStyle->bgColor.setAlphaF(0.8);
	d->tracks.append("Auto Track");
	d->track = d->tracks[0];
}

PlayEngine::~PlayEngine() {
	if (d->subRenderer)
		delete d->subRenderer;
	delete d->sub;
	delete d->source;
	delete d->msgStyle;
	delete d->subStyle;
	delete d->timeStyle;
	delete d->screen;
	delete d->repeater;
	delete d;
}

QWidget *PlayEngine::widget() {
	return d->screen;
}

void PlayEngine::setVideoRenderer(VideoRendererIface *renderer) {
	if (d->renderer != renderer)
		d->screen->setWidget((d->renderer = renderer) ? d->renderer->target() : 0);
}

void PlayEngine::play(int time) {
	play();
	seek(time);
}

void PlayEngine::setCurrentSource(const MediaSource &source) {
	stop();
	updateCurrentSource(source);
	emit currentSourceChanged(*d->source = source);
}

void PlayEngine::seek(int time, bool relative, bool showTimeLine, int timeout) {
	if (!isSeekable() || (!isPlaying() && !isPaused()))
		return;
	if (relative)
		time += currentTime();
	time = qBound(0, time, d->duration);
	seek(time);
	if (showTimeLine)
		this->showTimeLine(time, timeout);
}

void PlayEngine::showMessage(const QString &message, int timeout) {
	if (timeout > 0 && !message.isEmpty() && d->msgOsd)
		d->msgOsd->renderText(message, timeout);
}

void PlayEngine::showTimeLine(int time, int duration, int timeout) {
	if (d->timeOsd && timeout > 0) {
		const double rate = double(time)/double(duration);
		d->timeOsd->renderTimeLine(rate, timeout);
	}
}

void PlayEngine::showTimeLine(int time, int timeout) {
	showTimeLine(time, duration(), timeout);
}

void PlayEngine::showTimeLine(int timeout) {
	showTimeLine(currentTime(), duration(), timeout);
}

double PlayEngine::amplifyingRate() const {
	return d->ampRate;
}

void PlayEngine::setAmplifyingRate(double rate) {
	const double val = qBound(0., rate, 10.);
	if (qAbs((val - d->ampRate)/val) > 1.e-5) {
		d->ampRate = val;
		updateVolume();
	}
}

void PlayEngine::setVolume(int volume) {
	const int val = qBound(0, volume, 100);
	if (val != d->volume) {
		d->volume = val;
		updateVolume();
		emit volumeChanged(d->volume);
	}
}

bool PlayEngine::isMuted() const {
	return d->muted;
}

void PlayEngine::setMuted(bool muted) {
	if (d->muted != muted) {
		d->muted = muted;
		updateVolume();
		emit mutedChanged(d->muted);
	}
}

void PlayEngine::setSubtitle(const Subtitle &subtitle) {
	*d->sub = subtitle;
	updateSubtitle(*d->sub);
}

double PlayEngine::aspectRatio() const {
	return d->aspect;
}

void PlayEngine::setAspectRatio(double ratio) {
	ratio = ratio > 0.0 ? ratio : -1.0;
	updateAspectRatio(ratio);
	d->aspect = ratio;
}

double PlayEngine::cropRatio() const {
	return d->crop;
}

void PlayEngine::setCropRatio(double ratio) {
	ratio = ratio > 0.0 ? ratio : -1.0;
	updateCropRatio(ratio);
	d->crop = ratio;
}

void PlayEngine::setSubtitleVisible(bool visible) {
	if (d->subVisible != visible) {
		updateSubtitleVisiblity(visible);
		d->subVisible = visible;
	}
}

bool PlayEngine::isSubtitleVisible() const {
	return d->subVisible;
}

void PlayEngine::setSubtitlePos(double pos) {
	pos = qBound(0.0, pos, 1.0);
	updateSubtitlePos(pos);
	d->pos = pos;
}

double PlayEngine::subtitlePos() const {
	return d->pos;
}

void PlayEngine::setSyncDelay(int delay) {
	if (d->syncDelay != delay) {
		updateSyncDelay(delay);
		emit syncDelayChanged(d->syncDelay = delay);
	}
}

void PlayEngine::setSubtitleStyle(const OsdStyle &style) {
	updateSubtitleStyle(style);
	*d->subStyle = style;
}

void PlayEngine::setMessageStyle(const OsdStyle &style) {
	updateMessageStyle(style);
	*d->msgStyle = style;
}

void PlayEngine::setTimeLineStyle(const OsdStyle &style) {
	updateTimeLineStyle(style);
	*d->timeStyle = style;
}

void PlayEngine::setSubtitleOsd(AbstractOsdRenderer *osd) {
	if (!d->subRenderer) {
		d->subRenderer = new SubtitleRenderer(osd);
		d->subRenderer->setFrameRate(d->frameRate);
	} else
		d->subRenderer->setRenderer(osd);
	if (osd) {
		osd->setStyle(*d->subStyle);
		updateSubtitlePos(d->pos);
		connect(this, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
	}
}

void PlayEngine::setTimeLineOsd(AbstractOsdRenderer *osd) {
	if ((d->timeOsd = osd))
		osd->setStyle(*d->timeStyle);
}

void PlayEngine::setMessageOsd(AbstractOsdRenderer *osd) {
	if ((d->msgOsd = osd))
		osd->setStyle(*d->msgStyle);
}

void PlayEngine::slotTick(int time) {
	if (isSubtitleVisible() && d->subRenderer)
		d->subRenderer->show(time);
}

void PlayEngine::updateSubtitle(const Subtitle &subtitle) {
	if (d->subRenderer) {
		d->subRenderer->setSubtitle(&subtitle, d->frameRate);
		if (subtitle.isEmpty() || !isSubtitleVisible())
			d->subRenderer->clear();
		else
			d->subRenderer->show(currentTime());
	}
}

void PlayEngine::updateTimeLineStyle(const OsdStyle &style) {
	if (d->timeOsd)
		d->timeOsd->setStyle(style);
}

void PlayEngine::updateMessageStyle(const OsdStyle &style) {
	if (d->msgOsd)
		d->msgOsd->setStyle(style);
}

void PlayEngine::updateSubtitleStyle(const OsdStyle &style) {
	if (d->subRenderer)
		d->subRenderer->setStyle(style);
}

void PlayEngine::updateSubtitleVisiblity(bool visible) {
	if (d->subRenderer) {
		if (visible)
			d->subRenderer->show(currentTime());
		else
			d->subRenderer->clear();
	}
}

void PlayEngine::updateSubtitlePos(double pos) {
	if (d->subRenderer)
		d->subRenderer->setPos(pos);
}

void PlayEngine::updateSyncDelay(int delay) {
	if (d->subRenderer) {
		d->subRenderer->setDelay(delay);
		if (isSubtitleVisible())
			d->subRenderer->show(currentTime());
	}
}

void PlayEngine::updateAspectRatio(double ratio) {
	if (d->renderer)
		d->renderer->setAspectRatio(ratio);
}

void PlayEngine::updateCropRatio(double ratio) {
	if (d->renderer)
		d->renderer->setCropRatio(ratio);
}

void PlayEngine::setState(State state) {
	if (d->state != state) {
		const State old = d->state;
		emit stateChanged(d->state = state, old);
		if ((old == Stopped || old == Finished) && d->state == Playing)
			emit started();
	}
}

void PlayEngine::setDuration(int duration) {
	if (d->duration != duration)
		emit durationChanged(d->duration = duration);
}

void PlayEngine::setSeekable(bool seekable) {
	if (d->seekable != seekable)
		emit seekableChanged(d->seekable = seekable);
}

void PlayEngine::setSpeed(double speed) {
	speed = qBound(0.1, speed, 10.0);
	if (isSame(speed, 1.0))
		speed = 1.0;
	if (!isSame(d->speed, speed) > 1.0e-5) {
		updateSpeed(speed);
		emit speedChanged(d->speed = speed);
	}
}

double PlayEngine::speed() const {
	return d->speed;
}

bool PlayEngine::setVideoRenderer(const QString &name) {
	const QString prev = videoRenderer();
	if (prev == name)
		return true;
	int time = -1;
	if (!isStopped()) {
		time = currentTime();
		stop();
	}
	const bool changed = updateVideoRenderer(name);
	if (changed)
		d->videoRenderer = name;
	else
		updateVideoRenderer(prev);
	updateVideo();
	if (time != -1)
		play(time);
	return changed;
}

bool PlayEngine::setAudioRenderer(const QString &name) {
	const QString prev = audioRenderer();
	if (prev == name)
		return true;
	int time = -1;
	if (!isStopped()) {
		time = currentTime();
		stop();
	}
	const bool changed = updateAudioRenderer(name);
	if (changed)
		d->audioRenderer = name;
	else
		updateAudioRenderer(prev);
	updateAudio();
	if (time != -1)
		play(time);
	return changed;
}

void PlayEngine::updateAudio() {
	updateVolume();
}

void PlayEngine::updateVideo() {
	updateAspectRatio(d->aspect);
	updateCropRatio(d->crop);
	updateColorProperty();
}

// void PlayEngine::updateColorProperty(ColorProperty::Value prop, double value) {
// 	PlayEngine::updateColorProperty();
// }
// 
// void PlayEngine::updateColorProperty() {
// }

void PlayEngine::setTracks(const QStringList &tracks, const QString &track) {
	if (d->tracks != tracks) {
		d->tracks.clear();
		d->tracks.append("Auto Track");
		d->tracks += tracks;
		emit tracksChanged(d->tracks);
	}
	d->track = (!track.isEmpty() && d->tracks.contains(track)) ? track : d->tracks[0];
	emit currentTrackChanged(d->track);
}

void PlayEngine::setCurrentTrack(const QString &track) {
	if (d->track != track) {
		int idx = d->tracks.indexOf(track);
		if (idx == 0)
			idx = (d->tracks.size() < 2) ? -1 : 1;
		if (idx > 0 && updateCurrentTrack(d->tracks[idx]))
			emit currentTrackChanged(d->track = track);
	}
}

void PlayEngine::setSpus(const QStringList &spus, const QString &spu) {
	if (d->spus != spus) {
		emit spusChanged(d->spus = spus);
		emit currentSpuChanged(d->spu = spu);
	}
}

void PlayEngine::setCurrentSpu(const QString &spu) {
	if (d->spu != spu && updateCurrentSpu(spu))
		emit currentSpuChanged(d->spu = spu);
}

double PlayEngine::realVolume() const {
	const double max = info().maximumAmplifyingRate();
	return qBound(0.0, volume()*0.01*amplifyingRate(), max);
}

void PlayEngine::setHasVideo(bool has) {
	if (d->hasVideo != has)
		emit hasVideoChanged(d->hasVideo = has);
}

void PlayEngine::setVolumeNormalized(bool enabled) {
	if (d->volnorm != enabled) {
		d->volnorm = enabled;
		updateVolume();
	}
}

const QString &PlayEngine::videoRenderer() const {
	return d->videoRenderer.isEmpty() ? info().defaultVideoRenderer() : d->videoRenderer;
}

const QString &PlayEngine::audioRenderer() const {
	return d->audioRenderer.isEmpty() ? info().defaultAudioRenderer() : d->audioRenderer;
}

void PlayEngine::setUseSoftwareEqualizer(bool use) {
	if (d->softEq != use) {
		d->softEq = use;
		updateColorProperty();
	}
}

void PlayEngine::setColorProperty(ColorProperty::Value p, double v) {
	if (!ColorProperty::isSame(v, d->colorProp[p])) {
		d->colorProp[p] = v;
		updateColorProperty(p, v);
		emit colorPropertyChanged(d->colorProp);
	}
}

void PlayEngine::setColorProperty(const ColorProperty &prop) {
	if (prop != d->colorProp) {
		d->colorProp = prop;
		updateColorProperty();
		emit colorPropertyChanged(d->colorProp);
	}
}

void PlayEngine::setFrameRate(double rate) {
	d->frameRate = rate > 0.0 ? rate : -1.0;
	if (d->subRenderer)
		d->subRenderer->setFrameRate(d->frameRate);
}

void PlayEngine::triggerSnapshot() {
	QImage image;
	if (d->renderer)
		image = d->renderer->grabCurrentFrame();
	emit snapshotTaken(image);
}

QString PlayEngine::makeTrackName(int nth, const QString &lang) {
	const QString name = "Track " + QString::number(nth);
	return lang.isEmpty() ? name : name + " (" + lang + ')';
}

}


