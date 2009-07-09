#ifndef CORE_SOFTWARERENDERERCREATOR_H
#define CORE_SOFTWARERENDERERCREATOR_H

#include <QtCore/QtPlugin>

namespace Core {

class SoftwareRendererIface;

class SoftwareRendererCreator {
public:
	virtual ~SoftwareRendererCreator() {}
	virtual Core::SoftwareRendererIface *create() const = 0;
};

}

Q_DECLARE_INTERFACE(Core::SoftwareRendererCreator, "net.xylosper.CMPlayer.Core.SoftwareRendererCreator/0.0.1");

#endif
