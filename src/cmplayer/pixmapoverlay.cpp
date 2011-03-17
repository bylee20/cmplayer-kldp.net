#include "pixmapoverlay.hpp"
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include "osdrenderer.hpp"
#include <QtOpenGL/QGLWidget>
#include <QtGui/QPixmap>
#include <QtCore/QMap>

struct PixmapOverlay::Data {
	QRect area;
	QMap<OsdRenderer*, QPixmap> caches;
	bool pending;
};


PixmapOverlay::PixmapOverlay(QGLWidget *video)
: Overlay(video), d(new Data) {
	setArea(video->rect());
}

PixmapOverlay::~PixmapOverlay() {
	QMap<OsdRenderer*, QPixmap>::iterator it = d->caches.begin();
	for (; it != d->caches.end(); ++it)
		delete it.key();
	delete d;
}

qint64 PixmapOverlay::addOsd(OsdRenderer *osd) {
	if (!osd)
		return -1;
	d->caches[osd] = QPixmap();
	connect(osd, SIGNAL(needToRerender()), this, SLOT(cache()));
	connect(osd, SIGNAL(sizeChanged(QSizeF)), this, SLOT(cache()));
	return (qint64)osd;
}

void PixmapOverlay::setArea(const QRect &area) {
	if (d->area == area)
		return;
	QMap<OsdRenderer*, QPixmap>::iterator it = d->caches.begin();
	for (; it != d->caches.end(); ++it)
		it.key()->setBackgroundSize(area.size());
}

void PixmapOverlay::cache() {
	OsdRenderer *osd = qobject_cast<OsdRenderer*>(sender());
	if (!osd)
		return;
	const int w = osd->width() + 0.5;
	const int h = osd->height() + 0.5;
	QPixmap &pix = d->caches[osd];
	if (pix.width() != w || pix.height() != h)
		pix = QPixmap(w, h);
	pix.fill(Qt::transparent);
	QPainter painter(&pix);
	painter.fillRect(pix.rect(), Qt::transparent);
	osd->render(&painter, QPointF(0.0, 0.0));
	painter.end();
}

void PixmapOverlay::render(QPainter *painter) {
	QMap<OsdRenderer*, QPixmap>::const_iterator it = d->caches.begin();
	for (; it != d->caches.end(); ++it) {
		if (!it->isNull())
			painter->drawPixmap(d->area.topLeft() + it.key()->posHint(), *it);
	}
}
