// #include "osdrenderer.h"
// #include "glrenderer.h"
// #include <QtGui/QTextDocument>
// #include <QtGui/QFont>
// #include <QtCore/QRegExp>
// #include <QtGui/QTextOption>
// #include <QtGui/QPainter>
// #include <cmath>
// #include <QtCore/QDebug>
// #include <QtCore/QRectF>
// #include <QtCore/QPointF>
// #include <QtCore/QSizeF>
// #include "osdstyle.h"
// 
// namespace Core {
// 
// struct OsdRenderer::Data {
// };
// 
// OsdRenderer::OsdRenderer(GLRenderer *video)
// : d(new Data){
// 	d->video = video;
// 	d->clear = false;
// }
// 
// OsdRenderer::~OsdRenderer() {
// 	delete d;
// }
// 
// void OsdRenderer::render() {
// 	d->clear = false;
// 	d->video->rerender();
// }
// 
// void OsdRenderer::drawText(QPainter *painter, const QSizeF &visual, const QSizeF &widget) {
// 	if (d->prevVisual != visual.toSize()) {
// 		d->prevVisual = visual.toSize();
// 		updateFontSize(visual.width(), visual.height());
// 	}
// 	const QSizeF size = textSize(widget);
// 	if (size.height() < 1.0 || size.width() < 1.0)
// 		return;
// 	PainterOsdRenderer::drawText(painter, QRectF(getPos(size, widget), size));
// }
// 
// void OsdRenderer::drawTimeLine(QPainter *painter, const QSizeF &visual, const QSizeF &widget) {
// 	const QSizeF size = timeLineSize(visual);
// 	PainterOsdRenderer::drawTimeLine(painter, QRectF(getPos(size, widget), size));
// }
// 
// void OsdRenderer::renderContents(QPainter *painter, const QSizeF &visual, const QSizeF &widget) {
// 	if (d->clear)
// 		return;
// 	if (!text().isEmpty())
// 		drawText(painter, visual, widget);
// 	if (timeLineRate() >= 0.0) {
// 		drawTimeLine(painter, visual, widget);
// 	}
// }
// 
// void OsdRenderer::clear() {
// 	if (!d->clear) {
// 		d->clear = true;
// 		d->video->rerender();
// 	}
// }
// 
// }
// 
