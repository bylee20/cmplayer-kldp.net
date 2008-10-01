#include "xineosd.h"
#include "videooutput.h"
#include "xinestream.h"
#include <backend/utility.h>
#include <QtCore/QRegExp>
#include <QtGui/QPainter>
#include <QtGui/QTextDocument>
#include <cmath>

namespace Backend {

namespace Xine {

struct XineOsd::Data {
	xine_osd_t *createOsd() {
		if (video->stream()->isOpen()) {
			static const QSize size = Utility::desktopSize();
			xine_osd_t *osd = xine_osd_new(video->stream()->stream(), 0, 0, size.width(), size.height());
			if (osd)
				xine_osd_set_palette(osd, clut->colorMap(), clut->transMap());
			return osd;
		}
		return 0;
	}
	void deleteOsd(xine_osd_t *&osd) {
		if (!osd)
			return;
		xine_osd_clear(osd);
		xine_osd_free(osd);
		osd = 0;
	}
	VideoOutput *video;
	xine_osd_t *osd, *buffer;
	OsdStyle::Clut *clut;
	QTextDocument *doc;
	QRect rect;
	QString last;
	double bw;
	static const int Count = 20;
	QPointF points[Count];
	static double Sines[Count];
	static double Cosines[Count];
};

double XineOsd::Data::Sines[XineOsd::Data::Count];
double XineOsd::Data::Cosines[XineOsd::Data::Count];

XineOsd::XineOsd(VideoOutput *video) {
	d = new Data;
	d->video = video;
	d->osd = d->createOsd();
	d->buffer = d->createOsd();
	d->doc = new QTextDocument;
	d->clut = OsdStyle::Clut::get();
	m_align = Qt::AlignBottom;
	setAlignment(Qt::AlignCenter);
	m_visible = false;
	d->bw = 1.0;
	slotStreamOpenChanged(video->stream()->isOpen());
	connect(video->stream(), SIGNAL(openStateChanged(bool)), this, SLOT(slotStreamOpenChanged(bool)));
	connect(video, SIGNAL(widgetSizeHintChanged(const QSize&)), this, SLOT(updateRect()));
	connect(video, SIGNAL(widgetResized(const QSize&)), this, SLOT(updateRect()));
	connect(video, SIGNAL(sizeUpdated()), this, SLOT(updateRect()));
	m_margins[MLeft] = m_margins[MRight] = m_margins[MBottom] = m_margins[MRight] = 0.0;

	static bool init = false;
	if (!init) {
		const double pi = 3.14159265;
		const double factor = pi*2.0/double(Data::Count);
		for (int i=0; i<Data::Count; ++i) {
			const double theta = double(i)*factor;
			Data::Sines[i] = std::sin(theta);
			Data::Cosines[i] = std::cos(theta);
		}
		init = true;
	}
}

XineOsd::~XineOsd() {
	slotStreamOpenChanged(false);
	d->video = 0;
	delete d->doc;
	delete d;
}

bool XineOsd::isValid() const {
	return d->osd != 0 && d->buffer != 0;
}

void XineOsd::drawText(const QString &text) {
	if (!isValid() || m_style.font.pixelSize() <= 0)
		return;
	d->last = text.trimmed();
	if (d->last.isEmpty())
		xine_osd_clear(d->buffer);
	else {
		d->doc->setDefaultFont(m_style.font);
		d->doc->setTextWidth(d->rect.width() - (2*d->bw));
		static QRegExp rxColor("\\s+[cC][oO][lL][oO][rR]\\s*=\\s*[^>\\s\\t]+");
		QString bgText = d->last;
		d->doc->setHtml(QString("<font color='%1'>").arg(m_style.border_color.name())
				+ bgText.remove(rxColor) + "</font>");
		QSize size = d->doc->size().toSize();
		size.rheight() += (2*d->bw);
		size.rwidth() += (2*d->bw);
		if (size.height() > d->rect.height())
			size.setHeight(d->rect.height());
		if (size.width() > d->rect.width())
			size.setWidth(d->rect.width());

		if (size.isEmpty()) {
			xine_osd_clear(d->buffer);
			return;
		}
		QPixmap pixmap(size);
		pixmap.fill(Qt::transparent);
		QRectF rect = pixmap.rect();

		QPainter p(&pixmap);

		p.save();
		p.setOpacity(m_style.border_color.alphaF());
		for (int i=0; i<Data::Count; ++i) {
			p.translate(d->points[i]);
			d->doc->drawContents(&p, rect);
			p.resetTransform();
		}
		p.restore();

		d->doc->setHtml(QString("<font color='%1'>").arg(m_style.text_color.name())+d->last+"</font>");

		p.save();
		p.setOpacity(m_style.text_color.alphaF());
		p.translate(d->bw, d->bw);
		d->doc->drawContents(&p, rect);
		p.restore();
		drawImage(pixmap.toImage());
	}
}

void XineOsd::drawImage(const QImage &image) {
	QImage img = image.convertToFormat(QImage::Format_Indexed8, d->clut->clut());
	const int width = img.width();
	const int height = img.height();
	const int length = width * height;
	uint8_t bitmap[length];
	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			bitmap[y*width+x] = img.pixelIndex(x, y);
		}
	}
	xine_osd_clear(d->buffer);
	QPoint pos = getPos(img.size());
	xine_osd_draw_bitmap(d->buffer, bitmap, pos.x(), pos.y(), width, height, 0);
}

void XineOsd::repaint() {
	if (!isValid() || !m_visible)
		return;
	drawText(d->last);
	update();
}

void XineOsd::update() {
	if (!isValid())
		return;
	qSwap(d->osd, d->buffer);
	xine_osd_hide(d->buffer, 0);
	m_style.high_quality ? xine_osd_show_unscaled(d->osd, 0) : xine_osd_show(d->osd, 0);
}

void XineOsd::setVisible(bool visible) {
	if ((m_visible = visible)) {
		if (m_style.high_quality)
			xine_osd_show_unscaled(d->osd, 0);
		else
			xine_osd_show(d->osd, 0);
	} else
		xine_osd_hide(d->osd, 0);
}

void XineOsd::setStyle(const OsdStyle &style) {
	if (style != m_style) {
		m_style = style;
		updateFontSize();
		repaint();
	}
}

void XineOsd::slotStreamOpenChanged(bool open) {
	d->deleteOsd(d->osd);
	d->deleteOsd(d->buffer);
	if (open) {
		d->osd = d->createOsd();
		d->buffer = d->createOsd();
	}
}

QPoint XineOsd::getPos(const QSize &size) const {
	int x, y;
	if (m_align & Qt::AlignHCenter)
		x = d->rect.left() + (d->rect.width() - size.width())/2;
	else if (m_align & Qt::AlignRight)
		x = d->rect.right() - size.width() - qRound(m_margins[MRight]*d->rect.width());
	else
		x = d->rect.left() + qRound(m_margins[MLeft]*d->rect.width());
	if (m_align & Qt::AlignBottom)
		y = d->rect.bottom() - size.height() - qRound(m_margins[MBottom]*d->rect.height());
	else if (m_align & Qt::AlignVCenter)
		y = d->rect.top() + (d->rect.height() - size.height())/2;
	else
		y = d->rect.top() + qRound(m_margins[MTop]*d->rect.height());
	return QPoint(qMax(0, x), qMax(0, y));
}

void XineOsd::updateFontSize() {
	double size = -1;
	if (m_style.scale == OsdStyle::FitToDiagonal) {
		const double h = d->rect.height();
		const double w = d->rect.width();
		size = std::sqrt(h*h + w*w);
	} else if (m_style.scale == OsdStyle::FitToWidth)
		size = d->rect.width();
	else
		size = d->rect.height();
	size *= m_style.size;
	m_style.font.setPixelSize(qMax(1, qRound(size)));
	d->bw = qMax(1.0, size*0.04);
	for (int i=0; i<Data::Count; ++i) {
		d->points[i].setX(d->bw*(1+Data::Sines[i]));
		d->points[i].setY(d->bw*(1+Data::Cosines[i]));
	}
}

void XineOsd::updateRect() {
	QRect rect = d->video->osdRect(!m_style.high_quality);
	if (rect != d->rect) {
		d->rect = rect;
		updateFontSize();
		repaint();
	}
}

void XineOsd::setMargin(MarginPos pos, double value) {
	double val = qBound(0.0, value, 1.0);
	if (val != m_margins[pos]) {
		m_margins[pos] = qBound(0.0, value, 1.0);
		repaint();
	}
}

void XineOsd::setAlignment(Qt::Alignment align) {
	if (m_align != align) {
		QTextOption option(m_align = align);
		option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
		d->doc->setDefaultTextOption(option);
		repaint();
	}
}

}

}
