#ifndef OPENGL_PIXELBUFFEROSDRENDERER_H
#define OPENGL_PIXELBUFFEROSDRENDERER_H

#include <core/painterosdrenderer.h>

namespace OpenGL {

class PixelBufferOsdRenderer : public Core::PainterOsdRenderer {
public:
	PixelBufferOsdRenderer();
	~PixelBufferOsdRenderer();
	void clear();
	void renderContents(QPainter *painter);
	void renderContents(QPixmap *pixmap);
	void setSize(const QSizeF &visual, const QSizeF &widget);
	virtual bool grab(QImage &image) const;
	virtual void setRect(const QRect &rect);
private:
	void render();
	void alloc();
	void free();
	struct Data;
	Data *d;
};

}

#endif
