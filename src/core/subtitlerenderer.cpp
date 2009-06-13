#include "subtitlerenderer.h"
#include "subtitle.h"
#include "abstractosdrenderer.h"
#include <QtCore/QDebug>

namespace Core {
#define POS_TO_MARGIN(pos) (qBound(0.0, 1.0 - (pos), 1.0))
struct SubtitleRenderer::Data {
	bool available() const {return sub && renderer && !comp.isEmpty();}
	AbstractOsdRenderer *renderer;
	const Subtitle *sub;
	Subtitle::Component comp;
	int delay;
	Subtitle::Component::const_iterator prev;
	double frameRate, margin;
	OsdStyle style;
};

SubtitleRenderer::SubtitleRenderer(AbstractOsdRenderer *renderer)
: d(new Data) {
	d->renderer = renderer;
	d->sub = 0;
	d->delay = 0;
	d->frameRate = -1.0;
	d->prev = d->comp.end();
	d->margin = POS_TO_MARGIN(1.0);
}

SubtitleRenderer::~SubtitleRenderer() {
	delete d;
}

void SubtitleRenderer::setRenderer(AbstractOsdRenderer *renderer) {
	if ((d->renderer = renderer)) {
		d->renderer->setBottomMargin(d->margin);
		d->renderer->setStyle(d->style);
	}
}

void SubtitleRenderer::show(int time) {
	if (!d->available())
		return;
	Subtitle::Component::const_iterator it = d->comp.start(time - d->delay, d->frameRate);
	if (it != d->prev) {
		d->prev = it;
		if (it != d->comp.end())
			d->renderer->renderText(it.value());
	}
}

void SubtitleRenderer::setSubtitle(const Subtitle *sub, double frameRate) {
	d->sub = sub;
	d->comp = d->sub->component(frameRate);
	d->frameRate = frameRate;
	d->prev = d->comp.end();
}

int SubtitleRenderer::delay() const {
	return d->delay;
}

void SubtitleRenderer::setDelay(int delay) {
	d->prev = d->comp.end();
	d->delay = delay;
}

void SubtitleRenderer::clear() {
	d->prev = d->comp.end();
	if (d->renderer)
		d->renderer->clear();
}

void SubtitleRenderer::setFrameRate(double frameRate) {
	if (qAbs(frameRate - d->frameRate) > 1.0e-5) {
		d->frameRate = frameRate;
		if (d->sub) {
			d->comp = d->sub->component(frameRate);
			d->prev = d->comp.end();
		}
	}
}

void SubtitleRenderer::setPos(double pos) {
	d->margin = POS_TO_MARGIN(pos);
	if (d->renderer)
		d->renderer->setBottomMargin(d->margin);
}

void SubtitleRenderer::setStyle(const OsdStyle &style) {
	d->style = style;
	if (d->renderer)
		d->renderer->setStyle(style);
}

}
