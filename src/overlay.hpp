#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include <QtCore/QObject>
#include <QtOpenGL/QGLWidget>
#include "osdrenderer.hpp"

class Overlay : public QObject {
	Q_OBJECT
public:
	Overlay(QGLWidget *video): m_video(video) {}
	~Overlay() {}
	QGLWidget *video() const {return m_video;}
	virtual void setArea(const QRect &area) = 0;
	virtual qint64 addOsd(OsdRenderer *osd) = 0;
	virtual void render(QPainter *painter) = 0;
protected:
	QSize cachedSize(const QSize &size) {return OsdRenderer::cachedSize(size);}
private:
	QGLWidget *m_video;
};

#endif // OVERLAY_HPP
