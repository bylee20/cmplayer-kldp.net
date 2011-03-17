#include "framebufferobjectoverlay.hpp"
#include <QtCore/QDebug>
#include <QtOpenGL/QGLFramebufferObject>

struct FramebufferObjectOverlay::Data {
	QRect area;
	QList<OsdRenderer*> osds;
	QGLFramebufferObject *fbo;
	bool pending;
};

FramebufferObjectOverlay::FramebufferObjectOverlay(QGLWidget *video)
: Overlay(video), d(new Data) {
	d->pending = false;
	d->fbo = 0;
	setArea(video->rect());
}

FramebufferObjectOverlay::~FramebufferObjectOverlay() {
	qDeleteAll(d->osds);
	delete d->fbo;
	delete d;
}


qint64 FramebufferObjectOverlay::addOsd(OsdRenderer *osd) {
	if (!osd)
		return -1;
	d->osds << osd;
	connect(osd, SIGNAL(needToRerender()), this, SLOT(cache()));
	connect(osd, SIGNAL(sizeChanged(QSizeF)), this, SLOT(cache()));
	return d->osds.size() - 1;
}

void FramebufferObjectOverlay::setArea(const QRect &area) {
	if (area == d->area)
		return;
	d->pending = true;
	d->area = area;
	const QSize newSize = OsdRenderer::cachedSize(d->area.size());
	if (!d->fbo || d->fbo->size() != newSize) {
		delete d->fbo;
		video()->makeCurrent();
		d->fbo = new QGLFramebufferObject(newSize);
	}
	double w, h;
	w = h = 0.0;
	for (int i=0; i<d->osds.size(); ++i) {
		d->osds[i]->setBackgroundSize(d->area.size());
		w = qMax(w, d->osds[i]->size().width());
		h = qMax(h, d->osds[i]->size().height());
	}

	d->pending = false;
	cache();
}

void FramebufferObjectOverlay::cache() {
	if (d->pending)
		return;
	video()->makeCurrent();

	d->fbo->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	d->fbo->release();

	QPainter painter(d->fbo);
	for (int i=0; i<d->osds.size(); ++i)
		d->osds[i]->render(&painter, d->osds[i]->posHint());
	painter.end();
	video()->update();
}

void FramebufferObjectOverlay::render(QPainter */*painter*/) {
	video()->drawTexture(QRectF(d->area.topLeft(), d->fbo->size()), d->fbo->texture());
}


