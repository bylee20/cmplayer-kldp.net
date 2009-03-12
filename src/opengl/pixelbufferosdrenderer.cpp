#include "pixelbufferosdrenderer.h"
#include <QtOpenGL/QGLPixelBuffer>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

namespace OpenGL {

 struct PixelBufferOsdRenderer::Data {
	QSizeF visual, widget;
	bool cleared, rendered;
	QGLPixelBuffer *pbuffer;
	static int hasPbuffer;
};

int PixelBufferOsdRenderer::Data::hasPbuffer = -1;

PixelBufferOsdRenderer::PixelBufferOsdRenderer()
: d(new Data) {
	if (d->hasPbuffer < 0)
		d->hasPbuffer = QGLPixelBuffer::hasOpenGLPbuffers();
	d->rendered = d->cleared = false;
	d->widget = QSizeF(10, 10);
	d->pbuffer = 0;
	if (d->hasPbuffer)
		alloc();
}

PixelBufferOsdRenderer::~PixelBufferOsdRenderer() {
	free();
// 	if (d->context)
// 		delete d->context;
	delete d;
}

void PixelBufferOsdRenderer::alloc() {
	if (d->hasPbuffer) {
		free();
		d->pbuffer = new QGLPixelBuffer(d->widget.toSize());
	}
}

void PixelBufferOsdRenderer::free() {
	if (d->pbuffer) {
		delete d->pbuffer;
		d->pbuffer = 0;
	}
}

void PixelBufferOsdRenderer::render() {
	d->cleared = false;
	if ((d->rendered = (d->pbuffer && (!text().isEmpty() || timeLineRate() >= 0.0)))) {
// 		d->pbuffer->bind();
		d->pbuffer->makeCurrent();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		QPainter painter(d->pbuffer);
		drawText(&painter, d->widget, d->widget);
		drawTimeLine(&painter, d->visual, d->widget);
// 		d->pbuffer->release();
	}
}

void PixelBufferOsdRenderer::clear() {
	if (!d->cleared) {
		d->cleared = true;
// 		if (d->renderer)
// 			d->renderer->update();
	}
}

void PixelBufferOsdRenderer::renderContents(QPainter *painter) {
	if (d->cleared)
		return;
// 	if (d->hasPbuffer) {
// 		if (d->pbuffer && d->rendered && d->renderer)
// 			d->renderer->drawTexture(d->renderer->rect(), d->fbo->texture());
// 	} else {
		drawText(painter, d->widget, d->widget);
		drawTimeLine(painter, d->visual, d->widget);
// 	}
}

void PixelBufferOsdRenderer::setRect(const QRect &rect) {
	const QSizeF widget = rect.size();
	const QSizeF visual = rect.size();
	setSize(visual, widget);
}

void PixelBufferOsdRenderer::setSize(const QSizeF &visual, const QSizeF &widget) {
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

bool PixelBufferOsdRenderer::grab(QImage &image) const {
	if (d->hasPbuffer && d->pbuffer) {
		image = d->pbuffer->toImage();
		return true;
	} else
		return false;
}

}
