#include <QDebug>
#include "videooutput.h"
#include "playengine.h"
#include "videowidget.h"
#include "subtitleoutput.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QPainter>

namespace MPlayer {

struct VideoOutput::Data {
	static bool isSame(qreal n1, qreal n2, qreal prec = 0.001) {return qAbs(n1 - n2) < prec;}
	static void drawBG(QWidget *widget, const QColor &color = Qt::black) {
		QPainter painter(widget);
		painter.fillRect(widget->rect(), color);
	}
	static qreal ratio(qreal width, qreal height) {return width/height;}
	static qreal ratio(const QSizeF &size) {return size.width()/size.height();}
	static QPoint toPoint(const QSizeF &size) {return QPoint(size.width(), size.height());}
	VideoOutput *p;
	VideoWidget *widget;
	MPlayer::PlayEngine *engine;
	QWidget *visual, *video;
	qreal cropRatio, aspectRatio;
	int brightness, contrast, gamma, hue, saturation;
	QSize videoSize;
	QString driver;
	bool enabledSoftEq, expanded;
	qreal monitorRatio;
};

VideoOutput::VideoOutput(QWidget *parent)
 : Controller(parent), d(new Data) {
	d->p = this;
	d->widget = new VideoWidget(this, parent);
	d->engine = 0;
	d->visual = new QWidget(d->widget);
	d->video = new QWidget(d->visual);
	d->cropRatio = CropOff;
	d->aspectRatio = AspectRatioAuto;
	d->brightness = d->contrast = d->gamma = d->hue = d->saturation = 0;
	d->enabledSoftEq = d->expanded = false;
	d->monitorRatio = Data::ratio(QApplication::desktop()->size());
	d->widget->setStyleSheet("background-color: black;");
	d->widget->setMouseTracking(true);
	d->widget->installEventFilter(this);

	d->video->setStyleSheet("background-color: black;");
	d->visual->setMouseTracking(true);
	d->visual->move(0, 0);
	d->visual->resize(40, 30);

	d->video->setMouseTracking(true);
	d->video->setAutoFillBackground(true);
	d->video->setAttribute(Qt::WA_NoSystemBackground);
	d->video->setAttribute(Qt::WA_StaticContents);
	d->video->setAttribute(Qt::WA_PaintOnScreen);
	d->video->setAttribute(Qt::WA_PaintUnclipped);
	d->videoSize.scale(4, 3, Qt::IgnoreAspectRatio);
	d->video->resize(40, 30);
	d->video->installEventFilter(this);
}

VideoOutput::~VideoOutput() {
	delete d;
}

qreal VideoOutput::monitorRatio() const {
	return d->monitorRatio;
}

PlayEngine *VideoOutput::playEngine() const {
	return d->engine;
}

const QSize &VideoOutput::videoSize() const {
	return d->videoSize;
}

qreal VideoOutput::aspectRatio() const {
	return d->aspectRatio;
}

qreal VideoOutput::cropRatio() const {
	return d->cropRatio;
}

bool VideoOutput::cropOn() const {
	return !Data::isSame(CropOff, d->cropRatio);
}

int VideoOutput::videoWID() const {
	return int(d->video->winId());
}

void VideoOutput::crop(qreal ratio) {
	d->cropRatio = ratio;
	updateVisual();
}

void VideoOutput::setAspectRatio(qreal ratio) {
	d->aspectRatio = ratio;
	updateVisual();
}

bool VideoOutput::eventFilter(QObject *obj, QEvent *event) {
	if (obj == d->video && event->type() == QEvent::Paint && (!d->engine || d->engine->isStopped())) {
		Data::drawBG(d->video);
		return true;
	} else if (obj == d->widget) {
		if (event->type() == QEvent::Resize) {
			updateVisual();
			emit widgetResized(d->widget->size());
			return true;
		} else if (event->type() == QEvent::Paint) {
			Data::drawBG(d->widget);
			return true;
		}
	}
	return Controller::eventFilter(obj, event);
}

void VideoOutput::setVideoSize(const QSize &size) {
	if (size != d->videoSize) {
		emit widgetSizeHintChanged(d->videoSize = size);
		updateVisual();
	}
}

void VideoOutput::updateVisual() {
	SubtitleOutput *subout = d->engine->subtitleOutput();
	const bool showMargin = subout && subout->isDisplayOnMarginEnabled();
	const QSizeF widget = d->widget->size();
	const qreal widgetRatio = widget.width() / widget.height();
	const qreal originalRatio = Data::ratio(d->videoSize);
	qreal aspect;
	if (Data::isSame(d->aspectRatio, AspectRatioAuto))
		aspect = originalRatio;
	else if (Data::isSame(d->aspectRatio, AspectRatioWidget))
		aspect = widgetRatio;
	else
		aspect = d->aspectRatio;
	QSizeF video(1.0, 1.0), visual(1.0, 1.0);
	if (d->expanded) {
		video.setWidth(d->monitorRatio);
		visual.setWidth(originalRatio);
		video.rwidth() *= (aspect/originalRatio);
		visual.rwidth() *= (aspect/originalRatio);
		if (showMargin && widgetRatio < Data::ratio(visual)) {
			if (widgetRatio < d->monitorRatio) {
				visual.setWidth(d->monitorRatio * visual.height());
				visual.scale(widget, Qt::KeepAspectRatio);
			} else
				visual = widget;
		} else
			visual.scale(widget, Qt::KeepAspectRatio);
		if (Data::ratio(visual) > Data::ratio(video))
			video.scale(visual, Qt::KeepAspectRatioByExpanding);
		else
			video.scale(visual, Qt::KeepAspectRatio);
		if (cropOn()) {
			visual.setWidth(visual.height()*d->cropRatio);
			visual.scale(widget, Qt::KeepAspectRatio);
			video.scale(visual, Qt::KeepAspectRatioByExpanding);
			if (d->cropRatio < originalRatio)
				video *= (originalRatio / d->cropRatio);
		}
	} else {
		video.setWidth(aspect);
		if (cropOn()) {
			visual.setWidth(d->cropRatio);
			visual.scale(widget, Qt::KeepAspectRatio);
			video.scale(visual, Qt::KeepAspectRatioByExpanding);
		} else {
			visual = video;
			visual.scale(widget, Qt::KeepAspectRatio);
			video = visual;
		}
	}

	d->visual->resize(visual.toSize());
	d->video->resize(video.toSize());
	d->visual->move(Data::toPoint((widget-visual)/2));
	d->video->move(Data::toPoint((visual-video)/2));

	if (subout)
		subout->setPosScale((visual.height() + video.height())/(2*video.height()));
}

QSize VideoOutput::widgetSizeHint() const {
	if (cropOn()) {
		QSize hint = d->visual->size();
		hint.scale(d->videoSize, Qt::KeepAspectRatio);
		return hint;
	} else
		return d->videoSize;
}

void VideoOutput::link(Controller *controller) {
	PlayEngine *engine = qobject_cast<PlayEngine*>(controller);
	if (engine) {
		if (d->engine)
			unlink(d->engine);
		d->engine = engine;
		connect(engine, SIGNAL(started()), this, SLOT(update()));
		return;
	}
}

QWidget *VideoOutput::widget() const {
	return d->widget;
}

void VideoOutput::unlink(Controller *controller) {
	if (d->engine && d->engine == controller) {
		connect(d->engine, 0, this, 0);
		d->engine = 0;
	}
}

#define DecalreEqProp(Prop, prop) \
void VideoOutput::set##Prop(int value) {\
	int temp = qBound(-100, value, 100);\
	if (d->prop != temp) {\
		d->prop = temp;\
		if (d->engine) d->engine->tellmp(#prop" " + QString::number(d->prop) + " 1");}}\
int VideoOutput::prop() const{return d->prop;}
DecalreEqProp(Brightness, brightness)
DecalreEqProp(Contrast, contrast)
DecalreEqProp(Gamma, gamma)
DecalreEqProp(Hue, hue)
DecalreEqProp(Saturation, saturation)
#undef setValue

const QString &VideoOutput::driver() const {
	return d->driver;
}

void VideoOutput::setDriver(const QString &driver) {
	d->driver = driver;
}

bool VideoOutput::isSoftwareEqualizerEnabled() const {
	return d->enabledSoftEq;
}

void VideoOutput::setSoftwareEqualizerEnabled(bool enabled) {
	d->enabledSoftEq = enabled;
}

void VideoOutput::setExpanded(bool expanded) {
	d->expanded = expanded;
}

void VideoOutput::update() {
	d->engine->tellmp("brightness " + QString::number(d->brightness) + " 1");
	d->engine->tellmp("contrast " + QString::number(d->contrast) + " 1");
	d->engine->tellmp("gamma " + QString::number(d->gamma) + " 1");
	d->engine->tellmp("hue " + QString::number(d->hue) + " 1");
	d->engine->tellmp("saturation " + QString::number(d->saturation) + " 1");
	updateVisual();
}

}

