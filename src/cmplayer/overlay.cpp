#include "overlay.hpp"
#include "framebufferobjectoverlay.hpp"
#include "pixelbufferoverlay.hpp"
#include "pixmapoverlay.hpp"
#include <QtOpenGL/QGLFramebufferObject>
#include <QtOpenGL/QGLPixelBuffer>

Overlay *Overlay::create(QGLWidget *video, Type type) {
	if (type == Auto) {
		type = Pixmap;
#ifdef Q_WS_MAC
		if (QGLFramebufferObject::hasOpenGLFramebufferObjects())
			type = FramebufferObject;
//		else if (QGLPixelBuffer::hasOpenGLPbuffers())
//			type = PixelBuffer;
#endif
	}
	switch (type) {
	case FramebufferObject:
		return new FramebufferObjectOverlay(video);
//	case PixelBuffer:
//		return new PixelBufferOverlay(video);
	case Pixmap:
		return new PixmapOverlay(video);
	default:
		return 0;
	}
}

QString Overlay::typeToString(Type type) {
	switch (type) {
	case FramebufferObject:
		return QString("FramebufferObject");
//	case PixelBuffer:
//		return QString("PixelBuffer");
	case Pixmap:
		return QString("Pixmap");
	default:
		return QString("InvalidType");
	}
}
