#ifndef FRAMEBUFFEROBJECTOVERLAY_HPP
#define FRAMEBUFFEROBJECTOVERLAY_HPP

#include "overlay.hpp"

class FramebufferObjectOverlay : public Overlay {
	Q_OBJECT
public:
	FramebufferObjectOverlay(QGLWidget *video);
	~FramebufferObjectOverlay();
	void setArea(const QRect &area);
	qint64 addOsd(OsdRenderer *osd);
	void render(QPainter *painter);
private slots:
	void cache();
private:
	struct Data;
	Data *d;
};

#endif // FRAMEBUFFEROBJECTOVERLAY_HPP
