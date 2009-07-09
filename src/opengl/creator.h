#ifndef OPENGL_CREATOR_H
#define OPENGL_CREATOR_H

#include <core/softwarerenderercreator.h>

namespace OpenGL {

class Creator : public QObject, public Core::SoftwareRendererCreator {
	Q_OBJECT
	Q_INTERFACES(Core::SoftwareRendererCreator);
public:
	virtual Core::SoftwareRendererIface *create() const;
};

} // namespace OpenGL

#endif // CREATOR_H
