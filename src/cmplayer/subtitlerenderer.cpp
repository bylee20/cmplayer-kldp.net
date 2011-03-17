#include "subtitlerenderer.hpp"
#include "textosdrenderer.hpp"
#include <QtCore/QDebug>
#include "osdstyle.hpp"

struct SubtitleRenderer::Data {
	TextOsdRenderer *osd;
	Subtitle sub;
	double fps;
	Subtitle::Component comp;
	Subtitle::Component::const_iterator prev;
	int delay, ms;
	double pos;
	bool visible;
};

SubtitleRenderer::SubtitleRenderer(): d(new Data) {
	d->visible = true;
	d->osd = 0;
//	d->osd = new TextOsdRenderer(Qt::AlignBottom | Qt::AlignHCenter);
	d->fps = 30;
	d->delay = 0;
	d->prev = d->comp.end();
	d->ms = 0;
	d->pos = 1.0;
}

SubtitleRenderer::~SubtitleRenderer() {
	delete d;
}

TextOsdRenderer *SubtitleRenderer::osd() const {
	return d->osd;
}

void SubtitleRenderer::setOsd(TextOsdRenderer *osd) {
	d->osd = osd;
}

void SubtitleRenderer::setSubtitle(const Subtitle &subtitle) {
	d->sub = subtitle;
	d->comp = d->sub.component(d->fps);
	d->prev = d->comp.end();
	if (d->comp.isEmpty())
		clear();
	else
		render(d->ms);
}

void SubtitleRenderer::setFrameRate(double fps) {
	if (d->fps != fps) {
		d->comp = d->sub.component(d->fps = fps);
		d->prev = d->comp.end();
	}
}

void SubtitleRenderer::render(int ms) {
	d->ms = ms;
	if (!d->visible || d->comp.isEmpty() || ms == 0 || !d->osd)
		return;
	Subtitle::Component::const_iterator it = d->comp.start(ms - d->delay, d->fps);
	if (it != d->prev) {
		d->prev = it;
		if (it != d->comp.end())
			d->osd->showText(it.value());
	}
}

void SubtitleRenderer::setVisible(bool visible) {
	if (visible == d->visible)
		return;
	d->visible = visible;
	if (d->visible)
		render(d->ms);
	else
		clear();
}

void SubtitleRenderer::clear() {
	d->prev = d->comp.end();
	if (d->osd)
		d->osd->clear();
}

const Subtitle &SubtitleRenderer::subtitle() const {
	return d->sub;
}

double SubtitleRenderer::frameRate() const {
	return d->fps;
}

int SubtitleRenderer::delay() const {
	return d->delay;
}

int SubtitleRenderer::start(int pos) const {
	return d->sub.start(pos - d->delay, d->fps);
}

int SubtitleRenderer::end(int pos) const {
	return d->sub.end(pos - d->delay, d->fps);
}

void SubtitleRenderer::setDelay(int delay) {
	if (d->delay != delay) {
		d->delay = delay;
		render(d->ms);
	}
}

bool SubtitleRenderer::hasSubtitle() const {
	return !d->comp.isEmpty();
}

void SubtitleRenderer::setPos(double pos) {
	if (!qFuzzyCompare(pos, d->pos)) {
		d->pos = qBound(0.0, pos, 1.0);
		d->osd->setMargin(0, 1.0 - d->pos, 0, 0);
	}
}

double SubtitleRenderer::pos() const {
	return d->pos;
}
