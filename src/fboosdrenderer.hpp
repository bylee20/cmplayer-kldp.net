#ifndef FBOOSDRENDERER_HPP
#define FBOOSDRENDERER_HPP

#include "osdrenderer.hpp"
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLContext>
#include <QtOpenGL/QGLFramebufferObject>

class FboOsdRenderer : public OsdRenderer {
	Q_OBJECT
public:
	FboOsdRenderer(OsdRenderer *osd, QGLWidget *renderer);
	~FboOsdRenderer();
	QGLFramebufferObject *fbo() const;
	GLuint texture() const;
	void render(QPainter *painter, const QPointF &pos);
	QPointF posHint() const;
	QSizeF size() const;
	void cache();
	void setBackgroundSize(const QSize &size);
	void setStyle(const OsdStyle &style);
	const OsdStyle &style() const;
	void blit(QGLFramebufferObject *target, const QRect &dest, const QRect &source = QRect());
	QRect rect() const;
private slots:
	void slotSizeChanged(const QSizeF &size);
	void slotNeedToRerender();
private:
	void realloc();
	static QSize roundUp(const QSizeF &size) {
		return QSize(size.width() + 0.5, size.height() + 0.5);
	}
	struct Data;
	Data *d;
};

#endif // FBOOSDRENDERER_HPP
