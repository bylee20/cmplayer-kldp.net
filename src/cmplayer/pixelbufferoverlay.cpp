#include "pixelbufferoverlay.hpp"
#include <QtOpenGL/QGLPixelBuffer>
#include <QtCore/QDebug>

struct PixelBufferOverlay::Data {
	QRect area;
	GLuint texture;
	QList<OsdRenderer*> osds;
	QGLPixelBuffer *pbuffer;
	bool pending;
};


PixelBufferOverlay::PixelBufferOverlay(QGLWidget *video)
: Overlay(video), d(new Data) {
	d->texture = 0;
	d->pbuffer = 0;
	d->pending = false;
	setArea(video->rect());
}

PixelBufferOverlay::~PixelBufferOverlay() {
	qDeleteAll(d->osds);
	delete d->pbuffer;
	delete d;
}

qint64 PixelBufferOverlay::addOsd(OsdRenderer *osd) {
	if (!osd)
		return -1;
	d->osds << osd;
	connect(osd, SIGNAL(needToRerender()), this, SLOT(cache()));
	connect(osd, SIGNAL(sizeChanged(QSizeF)), this, SLOT(cache()));
	return d->osds.size() - 1;
}

void PixelBufferOverlay::setArea(const QRect &area) {
	if (area == d->area)
		return;
	d->pending = true;
	d->area = area;
	const QSize newSize = cachedSize(d->area.size());
	if (!d->pbuffer || d->pbuffer->size() != newSize) {
		if (d->pbuffer)
			d->pbuffer->deleteTexture(d->texture);
		delete d->pbuffer;
		d->pbuffer = new QGLPixelBuffer(newSize, video()->format(), video());
		d->texture = d->pbuffer->generateDynamicTexture();
	}
	for (int i=0; i<d->osds.size(); ++i)
		d->osds[i]->setBackgroundSize(d->area.size());
	d->pending = false;
	cache();
}

void PixelBufferOverlay::cache() {
	if (d->pending)
		return;

	d->pbuffer->makeCurrent();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QPainter painter(d->pbuffer);
	for (int i=0; i<d->osds.size(); ++i) {
		QPointF pos = d->osds[i]->posHint();
		if (pos.y() < d->area.top())
			pos.setY(d->area.top());
		d->osds[i]->render(&painter, pos);
	}
	painter.end();
	d->pbuffer->updateDynamicTexture(d->texture);
	video()->update();
}

void PixelBufferOverlay::render(QPainter */*painter*/) {
	if (d->texture != 0)
		video()->drawTexture(QRectF(d->area.topLeft(), d->pbuffer->size()), d->texture);
}
