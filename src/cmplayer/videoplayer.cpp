#include "videoplayer.h"
#include "recentinfo.h"
#include <core/info.h>
#include <core/subtitle.h>
#include <core/backendiface.h>
#include <core/painterosdrenderer.h>
#include <QtGui/QStackedWidget>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QLinearGradient>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QPluginLoader>

#define TO_PERCENT(rate) (qRound((rate)*100.0))
#define TO_RATE(percent) (static_cast<double>((percent))*0.01)
#define IS_DIFF_CLIP(val1, val2, min, max) ((val1) != (val2 = qBound((min), (val2), (max))))

class VideoPlayer::Widget : public QWidget {
public:
	class Osd : public Core::PainterOsdRenderer {
	public:
		Osd(QWidget *widget): widget(widget), cleared(false) {}
		void render() {cleared = false; widget->update();}
		void clear() {if (!cleared) {cleared = true; widget->update();}}
		void renderContents(QPainter *painter, const QSizeF &widget) {
			if (cleared)
				return;
			if (prev != widget.toSize()) {
				prev = widget.toSize();
				updateFontSize(widget.width(), widget.height());
			}
			if (!text().isEmpty()) {
				const QSizeF size = textSize(widget);
				if (size.height() > 0.5 && size.width() > 0.5)
					drawText(painter, QRectF(getPos(size, widget), size));
			} else if (timeLineRate() > 0.0) {
				const QSizeF size = timeLineSize(widget);
				if (size.height() > 0.5 && size.width() > 0.5)
					drawTimeLine(painter, QRectF(getPos(size, widget), size));
			}
		}
	private:
		QWidget *widget;
		QSize prev;
		bool cleared;
	};
	Widget() {
		m_logo.load(":/img/cmplayer-logo-center.png");
		m_smallLogo.load(":/img/cmplayer-logo-center-small.png");
		QLinearGradient grad(0.5, 1.0, 0.75, 0.13);
		grad.setColorAt(0.0, qRgb(51, 131, 230));
		grad.setColorAt(1.0, qRgb(110, 202, 247));
		m_bgBrush = QBrush(grad);
		
		grad = QLinearGradient(0.3, 0.1, 0.5, 0.9);
		grad.setColorAt(0.0, Qt::white);
		grad.setColorAt(1.0, Qt::transparent);
		m_lightBrush = QBrush(grad);
		
		const double oh = 0.6;
		m_lightPath.moveTo(0.0, 0.0);
		m_lightPath.lineTo(0.0, oh*0.8);
		m_lightPath.cubicTo(0.1, oh*0.9, 0.3, oh, 0.4, oh);
		m_lightPath.cubicTo(0.6, oh, 0.8, oh*0.9, 1.0, oh*0.6);
		m_lightPath.lineTo(1.0, 0.0);
		m_lightPath.closeSubpath();
		
		setMinimumSize(200, 150);
	}
	~Widget() {
		for (int i=0; i<m_osd.size(); ++i)
			delete m_osd[i];
	}
	Core::AbstractOsdRenderer* createOsd() {
		Osd *osd = new Osd(this);
		m_osd.append(osd);
		return osd;
	}
	QSize sizeHint() const {return QSize(600, 450);}
	Core::VideoType type() const {return Core::AutoVideo;}
	QImage grabCurrentFrame() const {return QImage();}
private:
	void paintEvent(QPaintEvent */*event*/) {
		QPainter painter(this);
		const double w = width();
		const double h = height();
		
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.scale(w, h);
		painter.setPen(Qt::NoPen);
		painter.fillRect(rect(), m_bgBrush);
		painter.scale(1.0/w, 1.0/h);
		
		const int len = qMin(qRound(qMin(w, h)*0.5), m_logo.width());
		QRect rect;
		rect.setX((w-len)*0.5 + 0.5);
		rect.setY((h-len)*0.5 + 0.5);
		rect.setWidth(len + 0.5);
		rect.setHeight(len + 0.5);
		if (len != m_logo.width())
			painter.drawPixmap(rect, m_logo.scaled(len, len
					, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		else
			painter.drawPixmap(rect, m_logo);
		painter.scale(w, h);
		painter.setOpacity(0.2);
		painter.setBrush(m_lightBrush);
		painter.drawPath(m_lightPath);
		painter.restore();
		
		for (int i=0; i<m_osd.size(); ++i)
			m_osd[i]->renderContents(&painter, this->size());
	}
	QList<Osd*> m_osd;
	QPixmap m_logo, m_smallLogo;
	QBrush m_bgBrush, m_lightBrush;
	QPainterPath m_lightPath;
};

struct VideoPlayer::Backend {
	~Backend() {
		for (BackendMap::iterator it = map.begin(); it != map.end(); ++it)
			delete it.value();
	}
	BackendMap map;
	QSet<QString> file;
};

struct VideoPlayer::Data {
	QWidget *main;
	QStackedWidget *stack;
	QSize minSize, maxSize;
	bool keepSize, changing;
	QMap<QString, Core::PlayEngine*> engines;
	Widget *widget;
	Core::AbstractOsdRenderer *message, *timeLine;
	Core::PlayEngine *engine;
	Core::MediaSource *next;
	
	bool muted, subVisible, volNorm, softEq;
	int volume, amp, speed, subPos, sync;
	Core::MediaSource source;
	double aspect, crop;
	QString track, spu;
	Core::Subtitle sub;
	Core::OsdStyle subStyle;
	Core::ColorProperty color;
	
	static Backend backend;
};

VideoPlayer::Backend VideoPlayer::Data::backend;

const BackendMap &VideoPlayer::load() {
	QDir dir(Core::Info::pluginPath());
	if (dir.exists()) {
		static QRegExp rxFilter("^libcmplayer_engine_(.+).so$");
		const QFileInfoList file = dir.entryInfoList(QDir::Files);
		for (int i=0; i<file.size(); ++i) {
			if (rxFilter.indexIn(file[i].fileName()) == -1)
				continue;
			QPluginLoader loader(file[i].absoluteFilePath());
			if (!loader.load()) {
				qFatal("%s", qPrintable(loader.errorString()));
				continue;
			}
			Core::BackendIface *backend = 0;
			backend = qobject_cast<Core::BackendIface*>(loader.instance());
			if (!backend)
				continue;
			const QString name = backend->info()->name();
			if (!Data::backend.map.contains(name))
				Data::backend.map[name] = backend;
		}
	}
	return Data::backend.map;
}

const BackendMap &VideoPlayer::backend() {
	return Data::backend.map;
}

VideoPlayer::VideoPlayer(QWidget *main, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->softEq = d->muted = false;
	d->subVisible = d->volNorm = true;
	d->volume = d->amp = d->speed = d->subPos = 100;
	d->sync = 0;
	d->crop = d->aspect = -1.0;
		
	d->engine = 0;
	
	d->stack = new QStackedWidget(this);
	d->stack->resize(size());
	d->stack->move(0, 0);
	d->stack->setMouseTracking(true);

	d->main = main;
	d->minSize = minimumSize();
	d->maxSize = maximumSize();
	d->changing = d->keepSize = false;
	d->next = 0;
	
	setMinimumSize(320, 240);
	setMouseTracking(true);
	setContextMenuPolicy(Qt::CustomContextMenu);

	d->widget = new Widget;
	d->widget->setMouseTracking(true);
	d->stack->addWidget(d->widget);
	d->stack->setCurrentWidget(d->widget);
	
	d->message = d->widget->createOsd();
	d->timeLine = d->widget->createOsd();
	Core::OsdStyle style;
	style.alignment = Qt::AlignLeft | Qt::AlignTop;
	style.scale = Core::OsdStyle::FitToHeight;
	d->message->setStyle(style);
	style.alignment = Qt::AlignCenter;
	style.fgColor.setAlphaF(0.8);
	style.bgColor.setAlphaF(0.8);
	d->timeLine->setStyle(style);
}

VideoPlayer::~VideoPlayer() {
	QMap<QString, Core::PlayEngine*>::iterator it = d->engines.begin();
	for (; it != d->engines.end(); ++it)
		delete it.value();
	delete d->next;
	delete d;
}

void VideoPlayer::keepSize(bool keep) {
	if ((d->keepSize = keep)) {
		setFixedSize(size());
	} else {
		setMaximumSize(d->maxSize);
		setMinimumSize(d->minSize);
		resize();
	}
}

QSize VideoPlayer::sizeHint() const {
	QSize hint = d->stack->currentWidget()->sizeHint();
	if (hint.width() < d->minSize.width())
		hint.setWidth(d->minSize.width());
	if (hint.height() < d->minSize.height())
		hint.setHeight(d->minSize.height());
	return hint;
}

void VideoPlayer::resize() {
	d->stack->resize(d->main->isFullScreen() ? d->main->size() : size());
}

void VideoPlayer::resizeEvent(QResizeEvent */*event*/) {
	if (!d->keepSize)
		resize();
}

void VideoPlayer::setBackend(const QString &name) {
	if (d->engine && d->engine->info().name() == name)
		return;
	QMap<QString, Core::PlayEngine*>::iterator it = d->engines.find(name);
	if (it == d->engines.end()) {
		Core::BackendIface *backend = Data::backend.map.value(name, 0);
		if (!backend)
			return;
		it = d->engines.insert(name, backend->createPlayEngine());
	}
	int time = -1;
	if (d->engine) {
		disconnect(d->engine, 0, this, 0);
		if (!d->engine->isStopped()) {
			time = d->engine->currentTime();
			d->engine->stop();
		}
	}
	Core::PlayEngine *engine = it.value();
	d->stack->addWidget(engine->widget());
	engine->setSpeed(TO_RATE(d->speed));
	engine->setMuted(d->muted);
	engine->setVolume(d->volume);
	engine->setAmplifyingRate(TO_RATE(d->amp));
	engine->setAspectRatio(d->aspect);
	engine->setCropRatio(d->crop);
	engine->setSubtitlePos(TO_RATE(d->subPos));
	engine->setSyncDelay(d->sync);
	engine->setSubtitle(d->sub);
	engine->setSubtitleVisible(d->subVisible);
	engine->setSubtitleStyle(d->subStyle);
	engine->setCurrentSource(d->source);
	engine->setVolumeNormalized(d->volNorm);
	engine->setUseSoftwareEqualizer(d->softEq);
	engine->setColorProperty(d->color);
	connect(engine, SIGNAL(currentSourceChanged(const Core::MediaSource&))
			, this, SIGNAL(currentSourceChanged(const Core::MediaSource&)));
	connect(engine, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged(int)));
	connect(engine, SIGNAL(mutedChanged(bool)), this, SIGNAL(mutedChanged(bool)));
	connect(engine, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(slotStateChanged(Core::State, Core::State)));
	connect(engine, SIGNAL(finished(Core::MediaSource))
			, this, SLOT(slotFinished(Core::MediaSource)));
	connect(engine, SIGNAL(stopped(Core::MediaSource, int))
			, this, SLOT(slotStopped(Core::MediaSource, int)));
	connect(engine, SIGNAL(started()), this, SIGNAL(started()));
	connect(engine, SIGNAL(durationChanged(int))
			, this, SIGNAL(durationChanged(int)));
	connect(engine, SIGNAL(seekableChanged(bool))
			, this, SIGNAL(seekableChanged(bool)));
	connect(engine, SIGNAL(tick(int)), this, SIGNAL(tick(int)));
	connect(engine, SIGNAL(tracksChanged(const QStringList&))
			, this, SIGNAL(tracksChanged(const QStringList&)));
	connect(engine, SIGNAL(currentTrackChanged(const QString&))
			, this, SIGNAL(currentTrackChanged(const QString&)));
	connect(engine, SIGNAL(spusChanged(const QStringList&))
			, this, SIGNAL(spusChanged(const QStringList&)));
	connect(engine, SIGNAL(currentSpuChanged(const QString&))
			, this, SIGNAL(currentSpuChanged(const QString&)));
	connect(engine, SIGNAL(snapshotTaken(const QImage&))
			, this, SIGNAL(snapshotTaken(const QImage&)));
	d->engine = engine;
	emit backendChanged(name);
	if (time != -1)
		play(time);
}

const VideoPlayer::Map &VideoPlayer::engines() const {
	return d->engines;
}

const Core::Info *VideoPlayer::info() const {
	return d->engine ? &d->engine->info() : 0;
}

void VideoPlayer::setNextSource(const Core::MediaSource &source) {
	if (source.isValid()) {
		if (!d->next)
			d->next = new Core::MediaSource(source);
		else if (*d->next != source)
			*d->next = source;
	} else {
		delete d->next;
		d->next = 0;
	}
}

bool VideoPlayer::hasNextSource() const {
	return d->next;
}

void VideoPlayer::showMessage(const QString &message) {
	if (d->engine && d->engine->widget() == d->stack->currentWidget())
		d->engine->showMessage(message);
	else
		d->message->renderText(message, 3000);
}

void VideoPlayer::stop() {
	if (d->engine)
		d->engine->stop();
}

void VideoPlayer::play(int time) {
	if (d->engine)
		d->engine->play(time);
}

void VideoPlayer::play() {
	if (d->engine)
		d->engine->play();
}

void VideoPlayer::playNext(int time) {
	if (d->next && d->engine) {
		d->changing = true;
		const Core::MediaSource source = *d->next;
		setCurrentSource(source);
		delete d->next;
		d->next = 0;
		play(time);
	}
}

void VideoPlayer::slotStateChanged(Core::State state, Core::State old) {
	switch (state) {
	case Core::Playing:
		d->stack->setCurrentWidget(d->engine->hasVideo()
				? d->engine->widget() : d->widget);
		break;
	case Core::Finished:
		if (hasNextSource())
			return;
	case Core::Stopped:
		if (d->changing)
			return;
		d->stack->setCurrentWidget(d->widget);
		break;
	default:
		break;
	}
	d->changing = false;
	emit stateChanged(state, old);
}

void VideoPlayer::slotFinished(Core::MediaSource source) {
	RecentInfo::get()->setFinished(source);
	emit finished(source);
	if (d->next)
		playNext(RecentInfo::get()->stoppedTime(*d->next));
}

void VideoPlayer::slotStopped(Core::MediaSource source, int time) {
	RecentInfo::get()->setStopped(source, time);
	emit stopped(source, time);
	
}

void VideoPlayer::setCurrentSource(Core::MediaSource source) {
	if (d->source != source) {
		d->source = source;
		if (d->engine)
			d->engine->setCurrentSource(d->source);
	}
}

void VideoPlayer::seek(int time, bool relative, bool show) {
	if (d->engine) {
		if (!show || d->engine->widget() == d->stack->currentWidget())
			d->engine->seek(time, relative, show);
		else {
			d->engine->seek(time, relative, false);
			if (show) {
				const double duration = d->engine->duration();
				const double time = d->engine->currentTime();
				d->timeLine->renderTimeLine(time/duration, 2000);
			}
		}
	}
}

void VideoPlayer::setColorProperty(Core::ColorProperty::Value prop, int value) {
	if (IS_DIFF_CLIP(TO_PERCENT(d->color[prop]), value, -100, 100)) {
		d->color[prop] = value;
	 	if (d->engine)
			d->engine->setColorProperty(prop, d->color[prop]);
	}
}

int VideoPlayer::colorProperty(Core::ColorProperty::Value prop) const {
	return d->color[prop];
}

QString VideoPlayer::videoRenderer() const {
	return d->engine ? d->engine->videoRenderer() : QString();
}

QString VideoPlayer::audioRenderer() const {
	return d->engine ? d->engine->audioRenderer() : QString();
}

bool VideoPlayer::setVideoRenderer(const QString &renderer) {
	d->changing = true;
	return d->engine && d->engine->setVideoRenderer(renderer);
}

bool VideoPlayer::setAudioRenderer(const QString &renderer) {
	d->changing = true;
	return d->engine && d->engine->setAudioRenderer(renderer);
}

void VideoPlayer::setSyncDelay(int delay) {
	if (d->sync != delay) {
		d->sync = delay;
		if (d->engine)
			d->engine->setSyncDelay(d->sync);
	}
}

int VideoPlayer::syncDelay() const {
	return d->sync;
}

void VideoPlayer::setAspectRatio(double ratio) {
	if (qAbs(d->aspect - ratio) > 1.0e-5) {
		d->aspect = ratio;
		if (d->engine)
			d->engine->setAspectRatio(d->aspect);
	}
}

double VideoPlayer::aspectRatio() const {
	return d->aspect;
}

void VideoPlayer::setCropRatio(double ratio) {
	if (qAbs(d->crop - ratio) > 1.0e-5) {
		d->crop = ratio;
		if (d->engine)
			d->engine->setCropRatio(d->crop);
	}
}

double VideoPlayer::cropRatio() const {
	return d->crop;
}

void VideoPlayer::setMuted(bool muted) {
	if (d->muted != muted) {
		d->muted = muted;
		if (d->engine)
			d->engine->setMuted(d->muted);
	}
}

bool VideoPlayer::isMuted() const {
	return d->muted;
}

void VideoPlayer::setVolume(int volume) {
	if (IS_DIFF_CLIP(d->volume, volume, 0, 100)) {
		d->volume = volume;
		if (d->engine)
			d->engine->setVolume(volume);
	}
}

int VideoPlayer::volume() const {
	return d->volume;
}

void VideoPlayer::setSpeed(int speed) {
	if (IS_DIFF_CLIP(d->speed, speed, 10, 1000)) {
		d->speed = speed;
		if (d->engine)
			d->engine->setSpeed(TO_RATE(d->speed));
	}
}

int VideoPlayer::speed() const {
	return d->speed;
}

void VideoPlayer::setSubtitlePos(int pos) {
	if (IS_DIFF_CLIP(d->subPos, pos, 0, 100)) {
		d->subPos = pos;
		if (d->engine)
			d->engine->setSubtitlePos(TO_RATE(d->subPos));
	}
}

int VideoPlayer::subtitlePos() const {
	return d->subPos;
}

void VideoPlayer::setAmplifyingRate(int rate) {
	if (IS_DIFF_CLIP(d->amp, rate, 10, 1000)) {
		d->amp = rate;
		if (d->engine)
			d->engine->setAmplifyingRate(TO_RATE(d->amp));
	}
}

int VideoPlayer::amplifyingRate() const {
	return d->amp;
}

Core::MediaSource VideoPlayer::currentSource() const {
	return d->source;
}

void VideoPlayer::setSubtitleStyle(const Core::OsdStyle &style) {
	d->subStyle = style;
	if (d->engine)
		d->engine->setSubtitleStyle(d->subStyle);
}

void VideoPlayer::setSubtitle(const Core::Subtitle &sub) {
	d->sub = sub;
	if (d->engine)
		d->engine->setSubtitle(sub);
}

void VideoPlayer::setCurrentTrack(const QString &track) {
	d->track = track;
	if (d->engine)
		d->engine->setCurrentTrack(d->track);
}

void VideoPlayer::setCurrentSpu(const QString &spu) {
	d->spu = spu;
	if (d->engine)
		d->engine->setCurrentSpu(d->spu);
}

void VideoPlayer::setVolumeNormalized(bool enabled) {
	d->volNorm = enabled;
	if (d->engine)
		d->engine->setVolumeNormalized(d->volNorm);
}

void VideoPlayer::setUseSoftwareEqualizer(bool enabled) {
	d->softEq = enabled;
	if (d->engine)
		d->engine->setUseSoftwareEqualizer(d->softEq);
}

void VideoPlayer::triggerSnapshot() {
	if (d->engine)
		d->engine->triggerSnapshot();
}

void VideoPlayer::pause() {
	if (d->engine)
		d->engine->pause();
}

void VideoPlayer::toggleDvdMenu() {
	if (d->engine)
		d->engine->toggleDvdMenu();
}

bool VideoPlayer::isPlaying() const {
	return d->engine && d->engine->isPlaying();
}

bool VideoPlayer::isPaused() const {
	return d->engine && d->engine->isPaused();
}

bool VideoPlayer::isStopped() const {
	return d->engine ? d->engine->isStopped() : false;
}

int VideoPlayer::currentTime() const {
	return d->engine ? d->engine->currentTime() : 0;
}

bool VideoPlayer::hasVideo() const {
	return d->engine && d->engine->hasVideo();
}

Core::State VideoPlayer::state() const {
	return d->engine ? d->engine->state() : Core::Stopped;
}

Core::ABRepeater *VideoPlayer::repeater() const {
	return d->engine ? d->engine->repeater() : 0;
}

double VideoPlayer::frameRate() const {
	return d->engine ? d->engine->frameRate() : -1.0;
}

int VideoPlayer::duration() const {
	return d->engine ? d->engine->duration() : 0;
}

bool VideoPlayer::isSeekable() const {
	return d->engine && d->engine->isSeekable();
}

void VideoPlayer::setSubtitleVisible(bool visible) {
	if (d->subVisible != visible) {
		d->subVisible = visible;
		if (d->engine)
			d->engine->setSubtitleVisible(d->subVisible);
	}
}

#undef ENGINE_SET
#undef TO_PERCENT
#undef TO_RATE
#undef IS_DIFF_CLIP
