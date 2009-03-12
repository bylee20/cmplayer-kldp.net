#include "abstractosdrenderer.h"
#include <QtCore/QDebug>
#include <QtCore/QTimer>

namespace Core {

AbstractOsdRenderer::AbstractOsdRenderer()
: d(new Data) {
	d->timeRate = -1.0;
	d->top = d->bottom = d->left = d->right = 0.0;
	d->hider = new QTimer;
	d->hider->setSingleShot(true);
	connect(d->hider, SIGNAL(timeout()), this, SLOT(clear()));
}

AbstractOsdRenderer::~AbstractOsdRenderer() {
	delete d->hider;
	delete d;
}

void AbstractOsdRenderer::renderText(const QString &text, int duration) {
	setText(text);
	render(duration);
}

void AbstractOsdRenderer::renderTimeLine(double percent, int duration) {
	d->timeRate = qBound(0.0, percent, 1.0);
	render(duration);
}

void AbstractOsdRenderer::renderText(const QString &text) {
	setText(text);
	render();
}

void AbstractOsdRenderer::renderTimeLine(double percent) {
	d->timeRate = qBound(0.0, percent, 1.0);
	render();
}

void AbstractOsdRenderer::render(int duration) {
	if (duration > 0) {
		render();
		d->hider->start(duration);
	}
}

void AbstractOsdRenderer::setStyle(const Core::OsdStyle &style) {
	d->style = style;
	updateStyle(style);
	render();
}

void AbstractOsdRenderer::setMargin(double &margin, double value) {
	value = value < 1.0e-5 ? 0.0 : qBound(0.0, value, 1.0);
	if (qAbs(value - margin) > 1.0e-5) {
		margin = value;
		render();
	}
}

}

