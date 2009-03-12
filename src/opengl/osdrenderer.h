#ifndef OPENGL_OSDRENDERER_H
#define OPENGL_OSDRENDERER_H

#include <core/painterosdrenderer.h>

namespace OpenGL {

class VideoRenderer;

class OsdRenderer : public Core::PainterOsdRenderer {
public:
	OsdRenderer(VideoRenderer *renderer = 0);
	~OsdRenderer();
	void clear();
	void renderContents(QPainter *painter);
	void renderContents(QPixmap *pixmap);
	void setSize(const QSizeF &visual, const QSizeF &widget);
	virtual bool grab(QImage &image) const;
	virtual void setRect(const QRect &rect);
protected:
	void render();
private:
	void alloc();
	void free();
	struct Data;
	Data *d;
};

}

#endif
