#ifndef OPENGL_OPENGLIFACE_H
#define OPENGL_OPENGLIFACE_H

#include <core/opengliface.h>

namespace OpenGL {

class OpenGLIface : public QObject, public Core::OpenGLIface {
Q_OBJECT
Q_INTERFACES(Core::OpenGLIface);
public:
	virtual Core::OpenGLVideoRendererIface *createVideoRenderer() const;
	virtual Core::AbstractOsdRenderer *createOsdRenderer() const;
};

}

#endif
