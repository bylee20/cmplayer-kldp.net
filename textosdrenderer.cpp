#include "textosdrenderer.hpp"
#include "richstring.hpp"
#include <QtGui/QTextDocument>
#include <QtCore/QRegExp>
#include <QtCore/QVector>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <qmath.h>

class SineCosine {
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

static SineCosine sc;


struct TextOsdRenderer::Data {
	QTextDocument doc;
	RichString text;
	QVector<QPointF> points;
	double bw;
	double top, bottom, left, right;
	QPoint pos;
	QTimer clearer;
};

TextOsdRenderer::TextOsdRenderer(Qt::Alignment align): d(new Data) {
	d->bw = 1.0;
	d->top = d->bottom = d->left = d->right = 0.0;
	d->points.resize(sc.count());
	OsdStyle style = this->style();
	style.alignment = align;
	style.bgColor.setAlphaF(0.7);
	style.scale = OsdStyle::FitToWidth;
	setStyle(style);
	d->pos = getPos();
	d->clearer.setSingleShot(true);
	connect(&d->clearer, SIGNAL(timeout()), this, SLOT(clear()));
	invokeRerender();
}

TextOsdRenderer::~TextOsdRenderer() {

}

void TextOsdRenderer::render(QPainter *painter) {
	const QRect area = this->area();
	if (area.isEmpty() || d->text.isEmpty())
		return;
	painter->save();
	static QRegExp rxColor("\\s+[cC][oO][lL][oO][rR]\\s*=\\s*[^>\\s\\t]+");
	QString bgText = text().string();
	d->doc.setHtml(QString("<font color='%1'>").arg(style().bgColor.name())
			+ bgText.remove(rxColor) + "</font>");

	painter->setOpacity(style().bgColor.alphaF());
//	QPointF pos = rect.topLeft();
	for (int i=0; i<d->points.size(); ++i) {
		painter->translate(d->points[i]/* + pos*/);
		d->doc.drawContents(painter);
		painter->resetTransform();
	}

	d->doc.setHtml(QString("<font color='%1'>").arg(style().fgColor.name())+text().string()+"</font>");

	painter->setOpacity(style().fgColor.alphaF());
	painter->translate(d->bw, d->bw);
	d->doc.drawContents(painter);
	painter->restore();
}

QPoint TextOsdRenderer::posHint() const {
	return d->pos;
}

QPoint TextOsdRenderer::getPos() const {
	Qt::Alignment align = style().alignment;
	const QRect area = this->area();
	const QSize size = sizeHint();
	int x = area.left();
	int y = area.top();
	if (align & Qt::AlignBottom)
		y = area.bottom() - size.height() - size.height()*d->bottom;
	else if (align & Qt::AlignVCenter)
		y += (area.height() - size.height())*0.5;
	else
		y += size.height()*d->top;
	if (align & Qt::AlignHCenter)
		x += (area.width() - size.width())*0.5;
	else if (align & Qt::AlignRight)
		x = area.right() - size.width() - size.width()*d->right;
	else
		x += size.width()*d->left;
	return QPoint(x, y);

}

void TextOsdRenderer::showText(const RichString &text, int last) {
	d->clearer.stop();
	d->text = text;
	d->doc.setHtml(text.string());
	d->pos = getPos();
	invokeRerender();
	if (last >= 0)
		d->clearer.start(last);
}

RichString TextOsdRenderer::text() const {
	return d->text;
}

QSize TextOsdRenderer::sizeHint() const {
	QSize size = d->doc.size().toSize();
	size.rheight() += 2*d->bw;
	return size;
}

void TextOsdRenderer::updateFontSize() {
	const QSize s = area().size();
	int px = 0;
	if (style().scale == OsdStyle::FitToDiagonal)
		px = qRound(qSqrt(s.height()*s.height() + s.width()*s.width()) * style().textSize);
	else if (style().scale == OsdStyle::FitToWidth)
		px = qRound(s.width()*style().textSize);
	else
		px = qRound(s.height()*style().textSize);
	d->bw = qMax(style().borderWidth*px, 1.0);
	for (int i=0; i<sc.count(); ++i) {
		d->points[i].setX(d->bw*(1 + sc.sine(i)));
		d->points[i].setY(d->bw*(1 + sc.cosine(i)));
	}
	QFont font = style().font;
	font.setPixelSize(px);
	d->doc.setDefaultFont(font);
}

void TextOsdRenderer::areaChanged(const QRect &area) {
	d->doc.setTextWidth(area.width() - 2.0*d->bw - d->left - d->right);
	d->pos = getPos();
	invokeRerender();
}

void TextOsdRenderer::styleChanged(const OsdStyle &style) {
	updateFontSize();

	QTextOption option(style.alignment);
	option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	option.setUseDesignMetrics(true);
	d->doc.setDefaultTextOption(option);

	d->pos = getPos();
	invokeRerender();
}

void TextOsdRenderer::clear() {
	showText(RichString());
}

void TextOsdRenderer::setMargin(double top, double bottom, double right, double left) {
	d->top = top;
	d->bottom = bottom;
	d->right = right;
	d->left = left;
	areaChanged(area());
}
