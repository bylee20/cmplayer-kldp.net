#include "textosdrenderer.hpp"
#include "glrenderer_osd.hpp"
#include <QtOpenGL/QGLFramebufferObject>
#include <QtGui/QPainter>
#include <math.h>

struct GLRenderer::Osd::Data {
	TextOsdRenderer *text;
	QSize bg;
//	QRectF rect;
	QGLFramebufferObject *fbo;
	OsdRenderer *osd;
	GLRenderer *renderer;
};

GLRenderer::Osd::Osd(OsdRenderer *osd, GLRenderer *renderer): d(new Data) {
	d->renderer = renderer;
	d->osd = osd;
	d->fbo = 0;
	d->text = qobject_cast<TextOsdRenderer*>(d->osd);
	connect(osd, SIGNAL(needToRerender()), this, SLOT(cache()));
	connect(osd, SIGNAL(sizeChanged(QSizeF)), this, SLOT(updateSize(QSizeF)));
}

GLRenderer::Osd::~Osd() {
	if (d->fbo)
		delete d->fbo;
	delete d;
}

void GLRenderer::Osd::add(OsdRenderer *renderer) {
//	d->osds << renderer;
	connect(renderer, SIGNAL(needToRerender()), this, SLOT(cache()));
}

void GLRenderer::Osd::updateSize(const QSizeF &size) {
	const QSize newSize = cachingSize(size);
	if (d->fbo && d->fbo->size() != newSize) {
		delete d->fbo;
		d->fbo = 0;
	}
	d->renderer->makeCurrent();
	if (!d->fbo)
		d->fbo = new QGLFramebufferObject(newSize);
	cache();
}

QSize GLRenderer::Osd::cachingSize(const QSizeF &size) {
	const int w = qRound(size.width() + 0.5);
	const int h = qRound(size.height() + 0.5);
	if (!d->fbo)
		return QSize(w, h);
	return QSize(qMax(d->fbo->width(), w), qMax(d->fbo->height(), h));
//	const int w = (1 << qRound(log2(size.width()) + 0.5));
//	const int h = (1 << qRound(log2(size.height()) + 0.5));
//	return QSize(qMax(w, 256), qMax(h, 256));
}

void GLRenderer::Osd::setBackgroundSize(const QSize &bg) {
	if (d->bg == bg)
		return;
	d->bg = bg;
	d->osd->setBackgroundSize(d->bg);
	updateSize(d->osd->size());
}

void GLRenderer::Osd::cache() {
	if (!d->fbo)
		return;
	d->fbo->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	QPainter painter(d->fbo);
	render(&painter);
	d->fbo->release();
	d->renderer->update();
}

void GLRenderer::Osd::render(QPainter *painter) {
	painter->save();
//	painter->translate(-d->rect.topLeft());
	d->osd->render(painter, QPointF(0, 0));
	painter->restore();
}

void GLRenderer::Osd::render() {
	if (d->fbo) {
		QPointF pos = d->osd->posHint();
		QSizeF size = d->fbo->size();
		if (d->text)
			size *= d->text->scaler();
		d->renderer->drawTexture(QRectF(pos, size), d->fbo->texture());
	}
}

GLuint GLRenderer::Osd::texture() const {
	if (d->fbo)
		return d->fbo->texture();
	return 0;
}
