#include "textoverlay.h"
#include <QtCore/QSize>
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QRect>
#include <cmath>

namespace Gst {

struct TextOverlay::Data {
	GstElement *element;
	int xpad, ypad;
	int px, y;
	QRegExp rxTag;
	QRect rect;
// 	QFontInfo font;
};

TextOverlay::TextOverlay()
: d(new Data) {
	d->element = gst_element_factory_make("textoverlay", 0);
	gst_object_ref(GST_OBJECT(d->element));
	gst_object_sink(GST_OBJECT(d->element));
	d->px = 10;
	d->y = 0;
	g_object_set(G_OBJECT(d->element), "xpad", 0, NULL);
	d->rxTag = QRegExp("<[^>]+>");
	d->rxTag.setMinimal(true);
}

TextOverlay::~TextOverlay() {
	gst_object_unref(d->element);
	d->element = 0;
	delete d;
}

GstElement *TextOverlay::element() {
	return d->element;
}

void TextOverlay::render() {
	QString t = text();
	t.replace("<br>", "\n", Qt::CaseInsensitive);
	t.remove(d->rxTag);
	updatePos();
	g_object_set(G_OBJECT(d->element), "text", t.toLocal8Bit().constData(), NULL);
}

void TextOverlay::clear() {
	g_object_set(G_OBJECT(d->element), "text", "", NULL);
}

void TextOverlay::updateStyle(const Core::OsdStyle &style) {
	int halign = 1;
	if (style.alignment & Qt::AlignLeft)
		halign = 0;
	else if (style.alignment & Qt::AlignRight)
		halign = 2;
	g_object_set(G_OBJECT(d->element), "halignment", halign, NULL);

	int valign = 0;
	if (style.alignment & Qt::AlignTop)
		valign = 2;
	else if (style.alignment & Qt::AlignBottom)
		valign = 1;
	g_object_set(G_OBJECT(d->element), "valignment", valign, NULL);
	updateFont();
}

void TextOverlay::updatePos() {
	const int align = style().alignment;
	int y = 0;
	if (align & Qt::AlignTop)
		y = d->rect.top() + d->rect.height()*topMargin();
	else if (align & Qt::AlignBottom)
		y = d->rect.top() + d->rect.height()*bottomMargin();
	if (y < 0)
		y = 0;
	if (y != d->y)
		g_object_set(G_OBJECT(d->element), "ypad", d->y = y, NULL);
}

void TextOverlay::updateFont() {
	const QString desc = QString("%1, %2px").arg(style().font.family()).arg(d->px);
	g_object_set(G_OBJECT(d->element), "font-desc", qPrintable(desc), NULL);
}

void TextOverlay::updateFontSize(const QSize &size) {
	int px = 0;
	const double h = size.height(), w = size.width();
	if (style().scale == Core::OsdStyle::FitToDiagonal)
		px = qRound(std::sqrt(h*h + w*w) * style().textSize);
	else if (style().scale == Core::OsdStyle::FitToWidth)
		px = qRound(w * style().textSize);
	else
		px = qRound(h * style().textSize);
	if (d->px != px) {
		d->px = px;
		updateFont();
	}
}

void TextOverlay::setRect(const QRect &rect) {
	if (d->rect != rect) {
		updateFontSize(rect.size());
		d->rect = rect;
// 		if (!d->cleared)
			render();
	}
}


} // namespace Gst
