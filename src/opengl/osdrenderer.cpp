#include "osdrenderer.h"
#include "videorenderer.h"
#include <core/utility.h>
#include <QtOpenGL/QGLFramebufferObject>
#include <QtOpenGL/QGLPixelBuffer>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

namespace OpenGL {

struct OsdRenderer::Data {
	VideoRenderer *renderer;
	QSizeF visual, widget;
	bool cleared, rendered;
	QGLFramebufferObject *fbo;
	static int hasFbuffer;
};

int OsdRenderer::Data::hasFbuffer = -1;

OsdRenderer::OsdRenderer(VideoRenderer *renderer)
: d(new Data) {
	if (d->hasFbuffer < 0)
		d->hasFbuffer = QGLFramebufferObject::hasOpenGLFramebufferObjects();
	d->renderer = renderer;
	if (!d->renderer)
		d->renderer = new VideoRenderer;
	d->rendered = d->cleared = false;
	d->widget = QSizeF(10, 10);
	d->fbo = 0;
	if (d->hasFbuffer)
		alloc();
}

OsdRenderer::~OsdRenderer() {
	free();
	delete d;
}

void OsdRenderer::alloc() {
	if (d->hasFbuffer) {
		bool expand = false;
		if (d->fbo) {
			expand = d->fbo->height() < d->widget.height() || d->fbo->width() < d->widget.width();
			if (expand)
				free();
		}
		if (!d->fbo && d->renderer) {
			d->renderer->makeCurrent();
			if (expand)
				d->fbo = new QGLFramebufferObject(d->widget.toSize());
			else
				d->fbo = new QGLFramebufferObject(Core::Utility::desktopSize());
		}
	}
}

void OsdRenderer::free() {
	if (d->fbo) {
		delete d->fbo;
		d->fbo = 0;
	}
}

void OsdRenderer::render() {
	d->cleared = false;
	if ((d->rendered = d->fbo && (!text().isEmpty() || timeLineRate() >= 0.0))) {
		d->fbo->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		QPainter painter(d->fbo);
		drawText(&painter, d->widget, d->widget);
		drawTimeLine(&painter, d->visual, d->widget);
		d->fbo->release();
	}
	if (d->renderer)
		d->renderer->update();
}

void OsdRenderer::clear() {
	if (!d->cleared) {
		d->cleared = true;
		if (d->renderer)
			d->renderer->update();
	}
}

void OsdRenderer::renderContents(QPainter *painter) {
	if (d->cleared)
		return;
	if (d->hasFbuffer) {
		if (d->fbo && d->rendered && d->renderer)
			d->renderer->drawTexture(QPointF(0, 0), d->fbo->texture());
	} else {
		drawText(painter, d->widget, d->widget);
		drawTimeLine(painter, d->visual, d->widget);
	}
}

void OsdRenderer::setRect(const QRect &rect) {
	const QSizeF widget = rect.size();
	const QSizeF visual = rect.size();
	setSize(visual, widget);
}

void OsdRenderer::setSize(const QSizeF &visual, const QSizeF &widget) {
	bool rerender = false;
	if (d->visual.toSize() != visual.toSize()) {
		d->visual = visual;
		updateFontSize(visual.width(), visual.height());
		rerender = true;
	}
	if (d->widget.toSize() != widget.toSize()) {
		d->widget = widget;
		alloc();
		rerender = true;
	}
	if (rerender && !d->cleared)
		render();
}

bool OsdRenderer::grab(QImage &image) const {
	if (d->hasFbuffer && d->fbo) {
		image = d->fbo->toImage();
		return true;
	} else
		return false;
}

}

