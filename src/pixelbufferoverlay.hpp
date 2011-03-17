#ifndef PIXELBUFFEROVERLAY_HPP
#define PIXELBUFFEROVERLAY_HPP

#include "overlay.hpp"

class PixelBufferOverlay : public Overlay {
	Q_OBJECT
public:
	PixelBufferOverlay(QGLWidget *video);
	~PixelBufferOverlay();
	void setArea(const QRect &area);
	qint64 addOsd(OsdRenderer *osd);
	void render(QPainter *painter);
	GLuint texture() const;
private slots:
	void cache();
private:
	struct Data;
	Data *d;
};

#endif // PIXELBUFFEROVERLAY_HPP
