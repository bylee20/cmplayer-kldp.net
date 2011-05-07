#include "overlay.hpp"
#include "framebufferobjectoverlay.hpp"
#include "pixelbufferoverlay.hpp"
#include "pixmapoverlay.hpp"
#include <QtOpenGL/QGLFramebufferObject>
#include <QtOpenGL/QGLPixelBuffer>

Overlay::Type Overlay::guessType(int hint) {
	if (hint == Type::FramebufferObject)
		return Type::FramebufferObject;
	if (hint == Type::Pixmap)
		return Type::Pixmap;
#ifdef Q_WS_MAC
	if (QGLFramebufferObject::hasOpenGLFramebufferObjects())
		return Type::FramebufferObject;
#endif
	return Type::Pixmap;
}

Overlay *Overlay::create(QGLWidget *video, Type type) {
	if (type == Type::FramebufferObject)
		return new FramebufferObjectOverlay(video);
	return new PixmapOverlay(video);
}
