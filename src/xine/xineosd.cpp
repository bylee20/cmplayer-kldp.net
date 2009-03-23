#include "xineosd.h"
#include "xinestream.h"
#include <core/utility.h>
#include <QtGui/QPainter>
#include <QtCore/QVector>
#include <QtCore/QDebug>

namespace Xine {

class XineOsd::Clut {
public:
	Clut();
	inline const uint32_t *colorMap() const {return m_colorMap;}
	inline const uint8_t *transMap() const {return m_transMap;}
	inline const QVector<QRgb> &clut() const {return m_clut;}
private:
	static int toYuv(int r, int g, int b) {
	#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	#define Yuv(R,G,B) ((((((66*R+129*G+25*B+128)>>8)+16)<<8)\
		|(((112*R-94*G-18*B+128)>>8)+128))<<8|(((-38*R-74*G+112*B+128)>>8)+128))
	#else
	#define Yuv(R,G,B) (((((((-38*R-74*G+112*B+128)>>8)+128)<<8)\
		|(((112*R-94*G-18*B+128)>>8)+128))<<8|(((66*R+129*G+25*B+128)>>8)+16))<<8)
	#endif
		return Yuv(r, g, b);
	#undef Yuv
	}
	static int toYuv(const QRgb &rgb) {return toYuv(qRed(rgb), qGreen(rgb), qBlue(rgb));}
	uint32_t m_colorMap[256];
	uint8_t m_transMap[256];
	QVector<QRgb> m_clut;
};

XineOsd::Clut::Clut(): m_clut(256) {
	int i=0;
	for (int r=0; r<8; ++r) {for (int g=0; g<8; ++g) {
	for (int b=0; b<4; ++b) {
		static const QRgb key = qRgb(0x92, 0xff, 0x00);
		static const double RFactor = 255.0/7.0;
		static const double GFactor = 255.0/7.0;
		static const double BFactor = 255.0/3.0;
		const int red = qMin(255, qRound(r*RFactor));
		const int green = qMin(255, qRound(g*GFactor));
		const int blue = qMin(255, qRound(b*BFactor));
		const QRgb rgb = qRgb(red, green, blue);
		m_colorMap[i] = toYuv(rgb);
		if (rgb == key) {
			m_transMap[i] = 0;
			m_clut[i++] = qRgba(0, 0, 0, 0);
		} else {
			m_transMap[i] = 255;
			m_clut[i++] = rgb;
		}
	}}}
}

struct XineOsd::Data {
	struct TimeLineBox {
		void setSize(const QSizeF &out, int border) {
			setSize(out.width(), out.height(), border);
		}
		void setSize(double outWidth, double outHeight, int border) {
			out.setWidth(outWidth);
			out.setHeight(outHeight);
			in.setWidth(out.width() - 2*border);
			in.setHeight(out.height() - 2*border);
			this->border = border;
		}
		void draw(QPainter *painter, const QPointF &pos) {
			QRectF rect(pos, out);
			painter->fillRect(rect, bg);
			rect.translate(border, border);
			rect.setSize(in);
			painter->fillRect(rect, fg);
		}
		QPointF draw(QPainter *painter, const QPointF &pos, int count) {
			QPointF p = pos;
			for (int i=0; i<count; ++i) {
				draw(painter, p);
				p.rx() += out.width() + space;
			}
			return p;
		}
		QSizeF out, in;
		double space;
		int border;
		QBrush fg, bg;
	};
	static const Clut clut;
	xine_osd_t *osd;
	QRect rect;
	XineStream *stream;
	bool cleared;
	TimeLineBox bar, dot;
};

const XineOsd::Clut XineOsd::Data::clut;

XineOsd::XineOsd(XineStream *stream)
: d(new Data) {
	d->osd = 0;
        d->cleared = false;
	d->stream = stream;
	connect(stream, SIGNAL(aboutToClose()), this, SLOT(free()));
	connect(stream, SIGNAL(opened()), this, SLOT(alloc()));
	if (d->stream->stream)
		alloc();
}

XineOsd::~XineOsd() {
	free();
	delete d;
}

void XineOsd::alloc() {
	if (d->osd)
		free();
	if (d->stream->stream) {
		static const QSize size = Core::Utility::desktopSize();
		d->osd = xine_osd_new(d->stream->stream, 0, 0, size.width(), size.height());
		xine_osd_set_palette(d->osd, d->clut.colorMap(), d->clut.transMap());
	}
}
void XineOsd::free() {
	if (d->osd) {
		xine_osd_clear(d->osd);
		xine_osd_free(d->osd);
		d->osd = 0;
	}
}

void XineOsd::updateStyle(const Core::OsdStyle &style) {
	Core::PainterOsdRenderer::updateStyle(style);
	QColor color = style.bgColor;
	color.setAlpha(255);
	d->bar.bg = d->dot.bg = color;
	color = style.fgColor;
	color.setAlpha(255);
	d->bar.fg = d->dot.fg = color;
	updateFontSize(d->rect.width(), d->rect.height());
}

void XineOsd::drawTimeLine(QPainter *painter, const QRectF &rect) {
	QPointF pos = rect.topLeft();
	painter->save();
	painter->setOpacity(1.0);
	const int barCount = qRound(timeLineRate()*TimeLineBoxCount);
	const int dotCount = TimeLineBoxCount - barCount;
	pos = d->bar.draw(painter, pos, barCount);
	pos.rx() += (d->bar.out.width() - d->dot.out.width())/2.0;
	pos.ry() += (d->bar.out.height() - d->dot.out.height())/2.0;
	d->dot.draw(painter, pos, dotCount);
	painter->restore();
}

void XineOsd::drawPixmap(const QPixmap &pixmap) {
	if (!d->osd || d->cleared)
		return;
	const QImage img
		= pixmap.toImage().convertToFormat(QImage::Format_Indexed8, d->clut.clut());
	const int width = img.width();
	const int height = img.height();
	const int length = width * height;
	uint8_t bitmap[length];
	for (int y=0; y<height; ++y) for (int x=0; x<width; ++x)
		bitmap[y*width+x] = img.pixelIndex(x, y);
	xine_osd_clear(d->osd);
	const QPoint pos = getPos(img.size(), d->rect.size()).toPoint() + d->rect.topLeft();
	xine_osd_draw_bitmap(d->osd, bitmap, qMax(pos.x(), 0), qMax(pos.y(), 0), width, height, 0);
}

void XineOsd::render() {
	if (!d->osd)
		return;
	d->cleared = false;
	if (!text().isEmpty()) {
		const QSizeF size = textSize(d->rect.size());
		if (size.height() < 1.0 || size.width() < 1.0)
			return;
		QPixmap pixmap(size.toSize());
		pixmap.fill(Qt::transparent);
		QPainter painter(&pixmap);
		drawText(&painter, QRectF(QPointF(0.0, 0.0), size));
		painter.end();
		drawPixmap(pixmap);
	}
	if (timeLineRate() >= 0.0) {
		const QSizeF size = timeLineSize(d->rect.size());
		if (size.height() < 1.0 || size.width() < 1.0)
			return;
		QPixmap pixmap(size.toSize());
		pixmap.fill(Qt::transparent);
		QPainter painter(&pixmap);
		drawTimeLine(&painter, pixmap.rect());
		painter.end();
		drawPixmap(pixmap);
	}
	xine_osd_show(d->osd, 0);
}

void XineOsd::setRect(const QRect &rect) {
	if (d->rect != rect) {
		updateFontSize(rect.width(), rect.height());
		d->rect = rect;
		const int count = TimeLineBoxCount;
		const QSizeF time = timeLineSize(d->rect.size());
		const int b = border(time.height());
		d->bar.space = qRound(time.width() * 0.005);
		d->bar.setSize((time.width() - d->bar.space*(count-1))/count, time.height(), b);
		d->dot.setSize(d->bar.out.width()*0.8, d->bar.out.width()*0.8, b);
		d->dot.space = d->bar.space + d->bar.out.width() - d->dot.out.width();
		if (!d->cleared)
			render();
	}
}

void XineOsd::clear() {
	if (d->osd) {
		xine_osd_hide(d->osd, 0);
                xine_osd_clear(d->osd);
	}
	d->cleared = true;
}

}
