#include "subtitlerenderer.h"
#include "subtitle.h"
#include "abstractosdrenderer.h"
#include <QtCore/QDebug>

namespace Core {

struct SubtitleRenderer::Data {
	bool available() const {return sub && renderer && !comp.isEmpty();}
	AbstractOsdRenderer *renderer;
	const Subtitle *sub;
	Subtitle::Component comp;
	int prevTime, delay;
	Subtitle::Component::const_iterator it;
	double frameRate;
};

SubtitleRenderer::SubtitleRenderer(AbstractOsdRenderer *renderer)
: d(new Data) {
	d->renderer = renderer;
	d->sub = 0;
	d->delay = 0;
	d->frameRate = -1.0;
	d->prevTime = -1;
	d->it = d->comp.end();
}

SubtitleRenderer::~SubtitleRenderer() {
	delete d;
}

void SubtitleRenderer::setRenderer(AbstractOsdRenderer *renderer) {
	d->renderer = renderer;
}

void SubtitleRenderer::show(int time) {
	if (!d->available())
		return;
	d->it = d->comp.start(time + d->delay, d->frameRate);
	if (d->it.key() != d->prevTime) {
		d->prevTime = d->it.key();
		d->renderer->renderText(d->it.value());
	}
}

AbstractOsdRenderer *SubtitleRenderer::renderer() {
	return d->renderer;
}

void SubtitleRenderer::setSubtitle(const Subtitle *sub, double frameRate) {
	d->prevTime = -1;
	d->sub = sub;
	d->comp = d->sub->component(frameRate);
	d->frameRate = frameRate;
	d->it = d->comp.end();
}

int SubtitleRenderer::delay() const {
	return d->delay;
}

void SubtitleRenderer::setDelay(int delay) {
	d->prevTime = -1;
	d->delay = delay;
}

void SubtitleRenderer::clear() {
	d->prevTime = -1;
	if (d->renderer)
		d->renderer->clear();
}

void SubtitleRenderer::setFrameRate(double frameRate) {
	if (qAbs(frameRate - d->frameRate) > 1.0e-5) {
		d->frameRate = frameRate;
		if (d->sub)
			d->comp = d->sub->component(frameRate);
	}
}

}
