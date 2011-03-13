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
	double scaler;
	QTimer clearer;
	QSize bg;
};

TextOsdRenderer::TextOsdRenderer(Qt::Alignment align): d(new Data) {
	d->scaler = 1.0;
	d->bw = 1.0;
	d->top = d->bottom = d->left = d->right = 0.0;
	d->points.resize(sc.count());
	OsdStyle style = this->style();
	style.alignment = align;
	style.bgColor.setAlphaF(0.7);
	style.scale = OsdStyle::FitToWidth;
	setStyle(style);
	updateStyle(style);
	d->clearer.setSingleShot(true);
	connect(&d->clearer, SIGNAL(timeout()), this, SLOT(clear()));
	connect(this, SIGNAL(styleChanged(OsdStyle)), this, SLOT(updateStyle(OsdStyle)));
}

TextOsdRenderer::~TextOsdRenderer() {
	delete d;
}

double TextOsdRenderer::scaler() const {
	return d->scaler;
}

void TextOsdRenderer::updateStyle(const OsdStyle &style) {
	const Qt::Alignment align = style.alignment;
	QTextOption option(align);
	option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	option.setUseDesignMetrics(true);
	d->doc.setDefaultTextOption(option);
	updateFontSize();
}

void TextOsdRenderer::render(QPainter *painter, const QPointF &pos) {
	if (d->bg.isEmpty() || d->bg.isNull() || d->text.isEmpty())
		return;

	painter->save();
	painter->translate(pos);
	const Qt::Alignment align = style().alignment;
	double x = 0.0;
	if (align & Qt::AlignHCenter)
		x = -(d->doc.textWidth() - d->doc.idealWidth())*0.5;
	else if (align & Qt::AlignRight)
		x = -(d->doc.textWidth() - d->doc.idealWidth());
	painter->translate(x, 0);

	static QRegExp rxColor("\\s+[cC][oO][lL][oO][rR]\\s*=\\s*[^>\\s\\t]+");
	QString bgText = text().string();
	d->doc.setHtml(QString("<font color='%1'>").arg(style().bgColor.name())
			+ bgText.remove(rxColor) + "</font>");
	painter->setOpacity(style().bgColor.alphaF());
	for (int i=0; i<d->points.size(); ++i) {
		painter->save();
		painter->translate(d->points[i]);
		d->doc.drawContents(painter);
		painter->restore();
	}
	d->doc.setHtml(QString("<font color='%1'>").arg(style().fgColor.name())+text().string()+"</font>");
	painter->setOpacity(style().fgColor.alphaF());
	painter->translate(d->bw, d->bw);
	d->doc.drawContents(painter);
	painter->restore();
}

void TextOsdRenderer::showText(const RichString &text, int last) {
	d->clearer.stop();
	d->text = text;
	d->doc.setHtml(text.string());
	emit sizeChanged(size());
	if (last >= 0)
		d->clearer.start(last);
}

RichString TextOsdRenderer::text() const {
	return d->text;
}

QPointF TextOsdRenderer::posHint() const {
	const QSizeF s = size()*d->scaler;
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

QSizeF TextOsdRenderer::size() const {
	const double width = d->doc.idealWidth();
	const double height = (d->doc.size().height() + 2.0*d->bw);
	return QSizeF(width, height);
}

void TextOsdRenderer::updateFontSize() {
	int px = 0;
	const OsdStyle::Scale scale = style().scale;
	if (scale == OsdStyle::FitToDiagonal)
		px = qRound(qSqrt(d->bg.height()*d->bg.height() + d->bg.width()*d->bg.width()) * style().textSize);
	else if (scale == OsdStyle::FitToWidth)
		px = qRound(d->bg.width()*style().textSize);
	else
		px = qRound(d->bg.height()*style().textSize);
	static const int max = 60;
	if (px > max) {
		d->scaler = (double)px/(double)max;
		px = max;
	} else
		d->scaler = 1.0;
	d->bw = qMax(style().borderWidth*px, 1.0);
	for (int i=0; i<sc.count(); ++i) {
		d->points[i].setX(d->bw*(1 + sc.sine(i)));
		d->points[i].setY(d->bw*(1 + sc.cosine(i)));
	}
	QFont font = style().font;
	font.setPixelSize(px);
	d->doc.setDefaultFont(font);
	d->doc.setTextWidth(d->bg.width()/d->scaler - 2.0*d->bw - d->left - d->right);
	emit sizeChanged(size());
}

void TextOsdRenderer::setBackgroundSize(const QSize &bg) {
	if (d->bg == bg)
		return;
	d->bg = bg;
	updateFontSize();
}

void TextOsdRenderer::clear() {
	showText(RichString());
}

void TextOsdRenderer::setMargin(double top, double bottom, double right, double left) {
	d->top = top;
	d->bottom = bottom;
	d->right = right;
	d->left = left;
	d->doc.setTextWidth(d->bg.width()/d->scaler - 2.0*d->bw - d->left - d->right);
	emit sizeChanged(size());
}
