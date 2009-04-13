#include "textoverlay.h"
#include <QtCore/QSize>
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QRect>
#include <cmath>
#include "textsource.h"

namespace Gst {

struct TextOverlay::Data {
	GstElement *bin;
	GstElement *overlay;
// 	TextSource *source;
	int xpad, ypad;
	int px, y;
	QRegExp rxTag;
	QRect rect;
// 	QFontInfo font;
};

TextOverlay::TextOverlay()
: d(new Data) {
	d->bin = gst_bin_new(0);
	gst_object_ref(GST_OBJECT(d->bin));
	gst_object_sink(GST_OBJECT(d->bin));
	d->overlay = gst_element_factory_make("textoverlay", 0);
// 	GstElement *source = GST_ELEMENT(g_object_new(TextSource::gtype(), 0));
// 	d->source = reinterpret_cast<TextSource*>(source);
	gst_bin_add(GST_BIN(d->bin), d->overlay);
// 	gst_element_link(source, d->overlay);
	
	GstPad *pad = gst_element_get_pad(d->overlay, "video_sink");
	gst_element_add_pad(d->bin, gst_ghost_pad_new("sink", pad));
	gst_object_unref(pad);
	pad = gst_element_get_pad(d->overlay, "src");
	gst_element_add_pad(d->bin, gst_ghost_pad_new("src", pad));
	gst_object_unref(pad);
	
	d->px = 10;
	d->y = 0;
	g_object_set(G_OBJECT(d->overlay), "xpad", 0, NULL);
	g_object_set(G_OBJECT(d->overlay), "ypad", 0, NULL);
	d->rxTag = QRegExp("<[^>]+>");
	d->rxTag.setMinimal(true);
}

TextOverlay::~TextOverlay() {
	gst_object_unref(d->bin);
	delete d;
}

GstElement *TextOverlay::bin() {
	return d->bin;
}

void TextOverlay::render() {
	QString t = text();
	t.replace("<br>", "\n", Qt::CaseInsensitive);
	t.remove(d->rxTag);
// 	t = "<span underline='error' underline_color='red'>" + t + "</span>";
	updatePos();
	g_object_set(G_OBJECT(d->overlay), "text", t.toUtf8().constData(), NULL);
}

void TextOverlay::clear() {
	g_object_set(G_OBJECT(d->overlay), "text", "", NULL);
}

void TextOverlay::updateStyle(const Core::OsdStyle &style) {
	int halign = 1;
	if (style.alignment & Qt::AlignLeft)
		halign = 0;
	else if (style.alignment & Qt::AlignRight)
		halign = 2;
	g_object_set(G_OBJECT(d->overlay), "halignment", halign, NULL);

	int valign = 0;
	if (style.alignment & Qt::AlignTop)
		valign = 2;
	else if (style.alignment & Qt::AlignBottom)
		valign = 1;
	g_object_set(G_OBJECT(d->overlay), "valignment", valign, NULL);
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
		g_object_set(G_OBJECT(d->overlay), "ypad", d->y = y, NULL);
}

void TextOverlay::updateFont() {
	const QFont font = style().font;
	QString desc = font.family();
	if (font.bold())
		desc += " bold";
	if (font.italic())
		desc += " italic";
	desc += QString(" %1px").arg(d->px);
	g_object_set(G_OBJECT(d->overlay), "font-desc", qPrintable(desc), NULL);
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
