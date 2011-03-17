#ifndef PIXMAPOVERLAY_HPP
#define PIXMAPOVERLAY_HPP

#include "overlay.hpp"

class PixmapOverlay : public Overlay {
	Q_OBJECT
public:
	PixmapOverlay(QGLWidget *video);
	~PixmapOverlay();
	void setArea(const QRect &area);
	qint64 addOsd(OsdRenderer *osd);
	void render(QPainter *painter);
private slots:
	void cache();
private:
	struct Data;
	Data *d;
};

#endif // PIXMAPOVERLAY_HPP
