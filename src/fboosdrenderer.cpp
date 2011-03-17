#include "fboosdrenderer.hpp"

struct FboOsdRenderer::Data {
	QGLFramebufferObject *fbo, *temp;
	OsdRenderer *osd;
	QGLWidget *renderer;
};

FboOsdRenderer::FboOsdRenderer(OsdRenderer *osd, QGLWidget *renderer): d(new Data) {
	d->renderer = renderer;
	d->osd = osd;
	d->renderer->makeCurrent();;
	d->fbo = d->temp = 0;
	realloc();
	connect(d->osd, SIGNAL(needToRerender()), this, SLOT(slotNeedToRerender()));
	connect(d->osd, SIGNAL(sizeChanged(QSizeF)), this, SLOT(slotSizeChanged(QSizeF)));
	connect(d->osd, SIGNAL(styleChanged(OsdStyle)), this, SIGNAL(styleChanged(OsdStyle)));
}

FboOsdRenderer::~FboOsdRenderer() {
	delete d->fbo;
	delete d->temp;
	delete d;
}

void FboOsdRenderer::realloc() {
	const QSize newSize = roundUp(d->osd->size());
	if (!d->fbo || d->fbo->size() != newSize) {
		delete d->fbo;
		delete d->temp;
		d->renderer->makeCurrent();
		d->fbo = new QGLFramebufferObject(newSize);
		d->temp = new QGLFramebufferObject(newSize);
	}
}

void FboOsdRenderer::setStyle(const OsdStyle &style) {
	d->osd->setStyle(style);
}

const OsdStyle &FboOsdRenderer::style() const {
	return d->osd->style();
}

QGLFramebufferObject *FboOsdRenderer::fbo() const {
	return d->fbo;
}

GLuint FboOsdRenderer::texture() const {
	return d->fbo->texture();
}

void FboOsdRenderer::slotNeedToRerender() {
	cache();
	emit needToRerender();
}

void FboOsdRenderer::render(QPainter *painter, const QPointF &pos) {
	d->osd->render(painter, pos);
}

void FboOsdRenderer::cache() {
	d->renderer->makeCurrent();
	d->temp->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	QPainter painter(d->temp);
	d->osd->render(&painter, QPointF(0, 0));
	d->temp->release();
}

void FboOsdRenderer::slotSizeChanged(const QSizeF &size) {
	realloc();
	cache();
	emit sizeChanged(d->osd->size());
}

QPointF FboOsdRenderer::posHint() const {
	return d->osd->posHint();
}

QSizeF FboOsdRenderer::size() const {
	return d->osd->size();
}

void FboOsdRenderer::setBackgroundSize(const QSize &size) {
	d->osd->setBackgroundSize(size);
}

QRect FboOsdRenderer::rect() const {

}

void FboOsdRenderer::blit(QGLFramebufferObject *target, const QRect &dest, const QRect &source) {

}
