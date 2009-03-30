#include "dummyengine.h"
#include <core/info.h>
#include <core/painterosdrenderer.h>
#include <core/videorendereriface.h>
#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QLinearGradient>
#include <QtCore/QDebug>

class DummyEngine::Osd : public Core::PainterOsdRenderer {
public:
	Osd(QWidget *widget): widget(widget), cleared(false) {}
	void render() {cleared = false; widget->update();}
	void clear() {if (!cleared) {cleared = true; widget->update();}}
	void renderContents(QPainter *painter, const QSizeF &widget) {
		if (cleared)
			return;
		if (prev != widget.toSize()) {
			prev = widget.toSize();
			updateFontSize(widget.width(), widget.height());
		}
		if (!text().isEmpty()) {
			const QSizeF size = textSize(widget);
			if (size.height() > 0.5 && size.width() > 0.5)
				drawText(painter, QRectF(getPos(size, widget), size));
		} else if (timeLineRate() > 0.0) {
			const QSizeF size = timeLineSize(widget);
			if (size.height() > 0.5 && size.width() > 0.5)
				drawTimeLine(painter, QRectF(getPos(size, widget), size));
		}
	}
private:
	QWidget *widget;
	QSize prev;
	bool cleared;
};

class DummyEngine::Renderer : public QWidget, public Core::VideoRendererIface {
public:
	Renderer(): VideoRendererIface(this) {
		m_logo.load(":/img/cmplayer-logo-center.png");
		m_smallLogo.load(":/img/cmplayer-logo-center-small.png");
		QLinearGradient grad(0.5, 1.0, 0.75, 0.13);
		grad.setColorAt(0.0, qRgb(51, 131, 230));
		grad.setColorAt(1.0, qRgb(110, 202, 247));
		m_bgBrush = QBrush(grad);
		
		grad = QLinearGradient(0.3, 0.1, 0.5, 0.9);
		grad.setColorAt(0.0, Qt::white);
		grad.setColorAt(1.0, Qt::transparent);
		m_lightBrush = QBrush(grad);
		
		const double oh = 0.6;
		m_lightPath.moveTo(0.0, 0.0);
		m_lightPath.lineTo(0.0, oh*0.8);
		m_lightPath.cubicTo(0.1, oh*0.9, 0.3, oh, 0.4, oh);
		m_lightPath.cubicTo(0.6, oh, 0.8, oh*0.9, 1.0, oh*0.6);
		m_lightPath.lineTo(1.0, 0.0);
		m_lightPath.closeSubpath();
		
		setMinimumSize(200, 150);
	}
	~Renderer() {
		for (int i=0; i<m_osd.size(); ++i)
			delete m_osd[i];
	}
	Core::AbstractOsdRenderer* createOsd() {
		Osd *osd = new Osd(this);
		m_osd.append(osd);
		return osd;
	}
	QSize sizeHint() const {return QSize(600, 450);}
	Core::VideoType type() const {return Core::AutoVideo;}
	QImage grabCurrentFrame() const {return QImage();}
private:
	void paintEvent(QPaintEvent */*event*/) {
		QPainter painter(this);
		const double w = width();
		const double h = height();
		
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.scale(w, h);
		painter.setPen(Qt::NoPen);
		painter.fillRect(rect(), m_bgBrush);
		painter.scale(1.0/w, 1.0/h);
		
		const int len = qMin(qRound(qMin(w, h)*0.5), m_logo.width());
		QRect rect;
		rect.setX((w-len)*0.5 + 0.5);
		rect.setY((h-len)*0.5 + 0.5);
		rect.setWidth(len + 0.5);
		rect.setHeight(len + 0.5);
		if (len != m_logo.width())
			painter.drawPixmap(rect, m_logo.scaled(len, len
					, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		else
			painter.drawPixmap(rect, m_logo);
		painter.scale(w, h);
		painter.setOpacity(0.2);
		painter.setBrush(m_lightBrush);
		painter.drawPath(m_lightPath);
		painter.restore();
		
		for (int i=0; i<m_osd.size(); ++i)
			m_osd[i]->renderContents(&painter, this->size());
	}
	QList<Osd*> m_osd;
	QPixmap m_logo, m_smallLogo;
	QBrush m_bgBrush, m_lightBrush;
	QPainterPath m_lightPath;
};

DummyEngine::Info::Data DummyEngine::Info::d;

DummyEngine::Info::Data::Data() {
	vr = ar = QStringList() << "auto";
	name = "dummy";
	maxAmp = 10.0;
}

DummyEngine::DummyEngine(QObject *parent)
: PlayEngine(parent), m_renderer(new Renderer) {
	setMessageOsd(m_renderer->createOsd());
	setTimeLineOsd(m_renderer->createOsd());
	setVideoRenderer(m_renderer);
}

DummyEngine::~DummyEngine() {
	setVideoRenderer(0);
}

