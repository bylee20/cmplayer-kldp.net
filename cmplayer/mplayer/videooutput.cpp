#include <QtCore/QDebug>
#include <QtGui/QWidget>
#include "videooutput.h"
#include "playengine.h"
#include "subtitleoutput.h"

namespace Backend {

namespace MPlayer {

struct VideoOutput::Data {
	VideoOutput *p;
	MPlayer::PlayEngine *engine;
};

VideoOutput::VideoOutput(PlayEngine *engine)
: Backend::VideoOutput(engine), d(new Data) {
	d->p = this;
	d->engine = engine;
}

VideoOutput::~VideoOutput() {
	delete d;
}

int VideoOutput::internalWidgetId() const {
	return int(internalWidget()->winId());
}
/*
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
}*/

void VideoOutput::updateHue(double value) {
	const int temp = qBound(-100, qRound(value*100.0), 100);
	d->engine->tellmp("hue " + QString::number(temp) + " 1");
}

void VideoOutput::updateContrast(double value) {
	const int temp = qBound(-100, qRound(value*100.0), 100);
	d->engine->tellmp("contrast " + QString::number(temp) + " 1");
}

void VideoOutput::updateSaturation(double value) {
	const int temp = qBound(-100, qRound(value*100.0), 100);
	d->engine->tellmp("saturation " + QString::number(temp) + " 1");
}

void VideoOutput::updateBrightness(double value) {
	const int temp = qBound(-100, qRound(value*100.0), 100);
	d->engine->tellmp("brightness " + QString::number(temp) + " 1");
}

bool VideoOutput::updateExpand(bool expand) {
	return true;
}

// void VideoOutput::update() {
// 	d->engine->tellmp("brightness " + QString::number(d->brightness) + " 1");
// 	d->engine->tellmp("contrast " + QString::number(d->contrast) + " 1");
// 	d->engine->tellmp("gamma " + QString::number(d->gamma) + " 1");
// 	d->engine->tellmp("hue " + QString::number(d->hue) + " 1");
// 	d->engine->tellmp("saturation " + QString::number(d->saturation) + " 1");
// 	updateVisual();
// }

}

}
