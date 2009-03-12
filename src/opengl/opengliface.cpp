#include "opengliface.h"
#include "osdrenderer.h"
#include "videorenderer.h"
#include "pixelbufferosdrenderer.h"

namespace OpenGL {

Core::OpenGLVideoRendererIface *OpenGLIface::createVideoRenderer() const {
	return new VideoRenderer;
}

Core::AbstractOsdRenderer *OpenGLIface::createOsdRenderer() const {
	return new PixelBufferOsdRenderer;
}

}

Q_EXPORT_PLUGIN2(cmplayer_opengl, OpenGL::OpenGLIface);