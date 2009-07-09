#ifndef XVIDEO_CREATOR_H
#define XVIDEO_CREATOR_H

#include <core/softwarerenderercreator.h>

namespace XVideo {

class Creator : public QObject, public Core::SoftwareRendererCreator {
	Q_OBJECT
	Q_INTERFACES(Core::SoftwareRendererCreator);
public:
	virtual Core::SoftwareRendererIface *create() const;
};
} // namespace XVideo

#endif // CREATOR_H
