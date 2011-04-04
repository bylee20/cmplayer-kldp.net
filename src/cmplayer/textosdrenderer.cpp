#include "textosdrenderer.hpp"
#include "richstring.hpp"
#include <QtGui/QTextDocument>
#include <QtCore/QRegExp>
#include <QtCore/QVector>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <qmath.h>
#include "osdstyle.hpp"
#include <QtCore/QMutex>
#include <QtGui/QImage>

class TextOsdRenderer::SineCosine {
public:
	SineCosine(int count = 12): m_count(count) {
		m_sine = new double[m_count];
		m_cosine = new double[m_count];
		const double factor = M_PI*2.0/double(m_count);
		for (int i=0; i<m_count; ++i) {
			const double theta = double(i)*factor;
			m_sine[i] = qSin(theta);
			m_cosine[i] = qCos(theta);
		}
	}
	~SineCosine() {
		delete [] m_sine;
		delete [] m_cosine;
	}
	double cosine(int i) const {return m_cosine[i];}
	double sine(int i) const {return m_sine[i];}
	int count() const {return m_count;}
private:
	double *m_sine;
	double *m_cosine;
	const int m_count;
};


struct TextOsdRenderer::Data {
	static SineCosine sc;
	OsdStyle style;
	QTextDocument doc;
	RichString text;
	QVector<QPointF> points;
	double bw, top, bottom, left, right;
	QTimer clearer;
	QSize bg;
	QSizeF size;
	QPixmap cached, interm;
};

TextOsdRenderer::SineCosine TextOsdRenderer::Data::sc;

TextOsdRenderer::TextOsdRenderer(Qt::Alignment align): d(new Data) {
	d->bw = 1.0;
	d->top = d->bottom = d->left = d->right = 0.0;
	d->points.resize(d->sc.count());
	OsdStyle style = this->style();
	style.alignment = align;
	style.bgColor.setAlphaF(0.7);
	style.scale = OsdStyle::FitToWidth;
	setStyle(style);
	d->clearer.setSingleShot(true);
	connect(&d->clearer, SIGNAL(timeout()), this, SLOT(clear()));
}

TextOsdRenderer::~TextOsdRenderer() {
	delete d;
}

const OsdStyle &TextOsdRenderer::style() const {
	return d->style;
}

void TextOsdRenderer::setStyle(const OsdStyle &style) {
	d->style = style;
	const Qt::Alignment align = style.alignment;
	QTextOption option(align);
	option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	option.setUseDesignMetrics(true);
	d->doc.setDefaultTextOption(option);
	updateFont();
	updateSize();
	cache();
	emit sizeChanged(d->size);
	emit styleChanged(d->style);
}

QString TextOsdRenderer::bgHtml() const {
	static const QRegExp rxColor("\\s+[cC][oO][lL][oO][rR]\\s*=\\s*[^>\\s\\t]+");
	QString html = QString("<font color='%1'>").arg(style().bgColor.name());
	html += QString(text().toString()).remove(rxColor);
	html += "</font>";
	return html;
}

QString TextOsdRenderer::fgHtml() const {
	QString html = QString("<font color='%1'>").arg(style().fgColor.name());
	html += text().toString();
	html += "</font>";
	return html;
}

QPointF TextOsdRenderer::getOrigin() const {
	const Qt::Alignment align = style().alignment;
	double x = 0.0;
	if (align & Qt::AlignHCenter)
		x = -(d->doc.textWidth() - d->doc.idealWidth())*0.5;
	else if (align & Qt::AlignRight)
		x = -(d->doc.textWidth() - d->doc.idealWidth());
	return QPointF(x, 0);
}

void TextOsdRenderer::cache() {
	const QSize size = cachedSize(this->size());
	if (size != d->cached.size()) {
		d->cached = QPixmap(size);
		d->interm = QPixmap(size);
	}
	d->cached.fill(Qt::transparent);
	d->interm.fill(Qt::transparent);
	if (d->cached.isNull() || d->bg.isEmpty() || d->text.isEmpty())
		return;
#define CMPLAYER_USE_INTERM_PIXMAP
#ifdef CMPLAYER_USE_INTERM_PIXMAP
	const QPointF origin = getOrigin();
	QPainter painter(&d->interm);
	d->doc.setHtml(bgHtml());
	painter.setOpacity(d->style.bgColor.alphaF());
	painter.translate(origin);
	d->doc.drawContents(&painter);
	painter.end();

	painter.begin(&d->cached);
	for (int i=0; i<d->points.size(); ++i) {
		painter.drawPixmap(d->points[i], d->interm);
	}
	painter.end();

	d->interm.fill(Qt::transparent);
	painter.begin(&d->interm);
	d->doc.setHtml(fgHtml());
	painter.setOpacity(d->style.fgColor.alphaF());
	painter.translate(origin);
	d->doc.drawContents(&painter);
	painter.end();

	painter.begin(&d->cached);
	painter.drawPixmap(QPointF(d->bw, d->bw), d->interm);
	painter.end();
#else
	QPainter painter(&d->cached);
	renderDirectly(&painter, QPointF(0.0, 0.0));
#endif
}

void TextOsdRenderer::renderDirectly(QPainter *painter, const QPointF &pos) {
	painter->save();
	const QPointF origin = getOrigin() + pos;
	painter->setOpacity(style().bgColor.alphaF());
	d->doc.setHtml(bgHtml());
	for (int i=0; i<d->points.size(); ++i) {
		painter->save();
		painter->translate(origin + d->points[i]);
		d->doc.drawContents(painter);
		painter->restore();
	}

	painter->setOpacity(style().fgColor.alphaF());
	d->doc.setHtml(fgHtml());
	painter->translate(origin + QPointF(d->bw, d->bw));
	d->doc.drawContents(painter);
	painter->restore();
}

void TextOsdRenderer::render(QPainter *painter, const QPointF &pos) {
	painter->drawPixmap(pos, d->cached);
}

void TextOsdRenderer::showText(const RichString &text, int last) {
	d->clearer.stop();
	d->text = text;
	updateSize();
	cache();
	emit sizeChanged(size());
	if (last >= 0)
		d->clearer.start(last);
}

RichString TextOsdRenderer::text() const {
	return d->text;
}

QPointF TextOsdRenderer::posHint() const {
	const QSizeF s = size();
	Qt::Alignment align = style().alignment;
	double x = 0.0;
	double y = 0.0;
	if (align & Qt::AlignBottom)
		y = d->bg.height()*(1.0 - d->bottom) - s.height();
	else if (align & Qt::AlignVCenter)
		y += (d->bg.height() - s.height())*0.5;
	else
		y += d->bg.height()*d->top;
	if (align & Qt::AlignHCenter)
		x += (d->bg.width() - s.width())*0.5;
	else if (align & Qt::AlignRight)
		x = d->bg.width()*(1.0 - d->right) - s.width();
	else
		x += d->bg.width()*d->left;
	return QPointF(x, y);
}

void TextOsdRenderer::updateSize() {
	d->doc.setHtml(d->text.toString());
	d->doc.setTextWidth(d->bg.width() - 2.0*d->bw - d->left - d->right);
	d->size.setWidth(d->doc.idealWidth() + 2.0*d->bw);
	d->size.setHeight(d->doc.size().height() + 2.0*d->bw);
}

QSizeF TextOsdRenderer::size() const {
	return d->size;
}

void TextOsdRenderer::updateFont() {
	int px = 0;
	const OsdStyle::Scale scale = style().scale;
	if (scale == OsdStyle::FitToDiagonal)
		px = qRound(qSqrt(d->bg.height()*d->bg.height() + d->bg.width()*d->bg.width()) * style().textSize);
	else if (scale == OsdStyle::FitToWidth)
		px = qRound(d->bg.width()*style().textSize);
	else
		px = qRound(d->bg.height()*style().textSize);
	px = qMax(1, px);
	d->bw = qMax(style().borderWidth*px, 1.0);
	for (int i=0; i<d->sc.count(); ++i) {
		d->points[i].setX(d->bw*(1 + d->sc.sine(i)));
		d->points[i].setY(d->bw*(1 + d->sc.cosine(i)));
	}
	QFont font = style().font;
	font.setPixelSize(px);
	d->doc.setDefaultFont(font);
}

void TextOsdRenderer::setBackgroundSize(const QSize &bg) {
	if (d->bg == bg)
		return;
	d->bg = bg;
	updateFont();
	updateSize();
	cache();
	emit sizeChanged(d->size);
}

void TextOsdRenderer::clear() {
	showText(RichString());
}

void TextOsdRenderer::setMargin(double top, double bottom, double right, double left) {
	d->top = top;
	d->bottom = bottom;
	d->right = right;
	d->left = left;
	updateSize();
	cache();
	emit sizeChanged(d->size);
}
