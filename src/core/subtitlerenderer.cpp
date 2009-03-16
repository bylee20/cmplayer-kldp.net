#include "subtitlerenderer.h"
#include "subtitle.h"
#include "abstractosdrenderer.h"

namespace Core {

struct SubtitleRenderer::Data {
	bool available() const {return sub && renderer && !sub->isEmpty();}
	AbstractOsdRenderer *renderer;
	const Subtitle *sub;
	int prevTime, delay;
	Core::Subtitle::ConstIterator subIt;
};

SubtitleRenderer::SubtitleRenderer(AbstractOsdRenderer *renderer)
: d(new Data) {
	d->renderer = renderer;
	d->sub = 0;
	d->delay = 0;
	d->prevTime = -1;
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
	time += d->delay;
	d->subIt = d->sub->find(time);
	if (d->subIt == d->sub->end()) {
		d->subIt = d->sub->upperBound(time);
		if (d->subIt == d->sub->begin())
			return;
		--d->subIt;
	}
	if (d->subIt.key() != d->prevTime) {
		d->prevTime = d->subIt.key();
		d->renderer->renderText(d->subIt.value());
	}
}

AbstractOsdRenderer *SubtitleRenderer::renderer() {
	return d->renderer;
}

void SubtitleRenderer::setSubtitle(const Subtitle *subtitle) {
	d->prevTime = -1;
	d->sub = subtitle;
	if (d->sub)
		d->subIt = d->sub->begin();
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

}
