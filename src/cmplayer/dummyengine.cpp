#include "dummyengine.h"
#include <core/info.h>
#include <core/painterosdrenderer.h>
#include <core/videorendereriface.h>
#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QImage>

class DummyEngine::Osd : public Core::PainterOsdRenderer {
public:
	Osd(QWidget *widget): widget(widget), cleared(false) {}
	void render() {cleared = false; widget->update();}
	void clear() {if (!cleared) {cleared = true; widget->update();}}
	void renderContents(QPainter *painter, const QSizeF &widget) {
		if (cleared)
			return;
		if (!text().isEmpty()) {
			if (prev != widget.toSize()) {
				prev = widget.toSize();
				updateFontSize(widget.width(), widget.height());
			}
			const QSizeF size = textSize(widget);
			if (size.height() < 1.0 || size.width() < 1.0)
				return;
			drawText(painter, QRectF(getPos(size, widget), size));
		}
	}
private:
	QWidget *widget;
	QSize prev;
	bool cleared;
};

class DummyEngine::Renderer : public QWidget, public Core::VideoRendererIface {
public:
	Renderer(): VideoRendererIface(this) {}
	~Renderer() {
		for (int i=0; i<m_osd.size(); ++i)
			delete m_osd[i];
	}
	Core::AbstractOsdRenderer* createOsd() {
		Osd *osd = new Osd(this);
		m_osd.append(osd);
		return osd;
	}
	Core::VideoType type() const {return Core::AutoVideo;}
	QImage grabCurrentFrame() const {return QImage();}
private:
	void paintEvent(QPaintEvent */*event*/) {
		QPainter painter(this);
		painter.fillRect(rect(), Qt::black);
		for (int i=0; i<m_osd.size(); ++i)
			m_osd[i]->renderContents(&painter, size());
	}
	QList<Osd*> m_osd;
};

DummyEngine::Info::Data DummyEngine::Info::d;

DummyEngine::Info::Data::Data() {
	vr = ar = QStringList() << "auto";
	name = "dummy";
	maxAmp = 10.0;
}

DummyEngine::DummyEngine(QObject *parent)
: PlayEngine(parent), m_renderer(new Renderer) {
	setSubtitleOsd(m_renderer->createOsd());
	setMessageOsd(m_renderer->createOsd());
	setTimeLineOsd(m_renderer->createOsd());
	setVideoRenderer(m_renderer);
}

DummyEngine::~DummyEngine() {
	setVideoRenderer(0);
}

