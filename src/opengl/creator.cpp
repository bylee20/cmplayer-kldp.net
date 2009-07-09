#include "creator.h"
#include "videorenderer.h"

namespace OpenGL {

Core::SoftwareRendererIface *Creator::create() const {
	return new VideoRenderer;
}

}

Q_EXPORT_PLUGIN2(cmplayer_opengl, OpenGL::Creator);
