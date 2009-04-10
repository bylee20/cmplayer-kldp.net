#include "textoverlay.h"
#include <QtCore/QSize>
#include <cmath>

namespace Gst {

struct TextOverlay::Data {
	GstElement *element;
	int xpad, ypad;
	int px;
// 	QFontInfo font;
};

TextOverlay::TextOverlay()
: d(new Data) {
	d->element = gst_element_factory_make("textoverlay", 0);
	gst_object_ref(GST_OBJECT(d->element));
	gst_object_sink(GST_OBJECT(d->element));
	d->px = 10;
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
	g_object_set(G_OBJECT(d->element), "text", text().toLocal8Bit().constData(), NULL);
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

	g_object_set(G_OBJECT(d->element), "xpad", 0, NULL);
	g_object_set(G_OBJECT(d->element), "ypad", 0, NULL);
// 	const int xpad = style.

	updateFont();
}

void TextOverlay::updateFont() {
	static const QString desc = "%1, %2px";
	g_object_set(G_OBJECT(d->element), "font-desc", qPrintable(desc.arg(style().font.family()).arg(d->px)), NULL);
}

void TextOverlay::updateFontSize(const QSize &size) {
// 	qDebug() << "font" << size;
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
// 		d->bw = static_cast<double>(d->px) * style().borderWidth;
// 		for (int i=0; i<Data::Count; ++i) {
// 			d->points[i].setX(d->bw*(1+Data::Sines[i]));
// 			d->points[i].setY(d->bw*(1+Data::Cosines[i]));
// 		}
// 	}
}


} // namespace Gst
