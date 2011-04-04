#include "subtitlerenderer.hpp"
#include "subtitlemodel.hpp"
#include "osdstyle.hpp"
#include "subtitleview.hpp"
#include <QtCore/QDebug>

struct SubtitleRenderer::Data {
	SubtitleView *view;
	TextOsdRenderer *osd;
	Subtitle sub;
	QVector<Subtitle::Component::const_iterator> prev;
	QVector<SubtitleComponentModel*> model;
	double fps;
	int delay, ms;
	double pos;
	bool visible, empty;
};

SubtitleRenderer::SubtitleRenderer(): d(new Data) {
	d->empty = d->visible = true;
	d->osd = 0;
	d->fps = 30;
	d->ms = d->delay = 0;
	d->pos = 1.0;
	d->view = 0;
}

SubtitleRenderer::~SubtitleRenderer() {
	delete d;
}

QWidget *SubtitleRenderer::view(QWidget *parent) const {
	if (!d->view) {
		d->view = new SubtitleView(parent);
		d->view->setModel(d->model);
	}
	return d->view;
}

TextOsdRenderer *SubtitleRenderer::osd() const {
	return d->osd;
}

void SubtitleRenderer::setOsd(TextOsdRenderer *osd) {
	d->osd = osd;
}

void SubtitleRenderer::setSubtitle(const Subtitle &subtitle) {
	d->sub = subtitle;
	d->prev.resize(d->sub.size());
	qDeleteAll(d->model);
	d->model.resize(d->sub.size());
	d->empty = true;
	for (int i=0; i<d->sub.size(); ++i) {
		d->prev[i] = d->sub[i].end();
		d->model[i] = new SubtitleComponentModel(&d->sub[i], this);
		if (!d->sub[i].isEmpty())
			d->empty = false;
	}
	if (d->empty)
		clear();
	else
		render(d->ms);
	if (d->view)
		d->view->setModel(d->model);
}

void SubtitleRenderer::setFrameRate(double fps) {
	if (d->fps != fps) {
		for (int i=0; i<d->prev.size(); ++i)
			d->prev[i] = d->sub[i].end();
	}
}

void SubtitleRenderer::render(int ms) {
	d->ms = ms;
	if (!d->visible || d->empty || ms == 0 || !d->osd)
		return;
	bool changed = false;
	for (int i=0; i<d->sub.size(); ++i) {
		Subtitle::Component::const_iterator it = d->sub[i].start(ms - d->delay, d->fps);
		if (it != d->prev[i]) {
			d->prev[i] = it;
			d->model[i]->setCurrentNode(&(*it));
			changed = true;
		}
	}
	if (changed) {
		RichString text;
		for (int i=0; i<d->prev.size(); ++i) {
			if (d->prev[i] != d->sub[i].end())
				text.merge(d->prev[i]->text);
		}
		d->osd->showText(text);
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
	for (int i=0; i<d->prev.size(); ++i)
		d->prev[i] = d->sub[i].end();
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
	return !d->empty;
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
