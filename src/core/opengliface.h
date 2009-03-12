#ifndef CORE_OPENGLIFACE_H
#define CORE_OPENGLIFACE_H

#include <QtCore/QtPlugin>

namespace Core {

class AbstractOsdRenderer;			class OpenGLVideoRendererIface;
class OpenGLIface {
public:
	virtual ~OpenGLIface() {}
	virtual Core::OpenGLVideoRendererIface *createVideoRenderer() const = 0;
	virtual Core::AbstractOsdRenderer *createOsdRenderer() const = 0;
};

}

Q_DECLARE_INTERFACE(Core::OpenGLIface, "CMPlayer.Core.OpenGLIface/3.0");
#endif
