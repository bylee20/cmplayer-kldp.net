#ifndef CORE_BACKENDIFACE_H
#define CORE_BACKENDIFACE_H

#include <QtCore/QtPlugin>

namespace Core {

class PlayEngine;			class Info;
class Config;

class BackendIface {
public:
	virtual ~BackendIface() {}
	virtual PlayEngine *createPlayEngine(QObject *parent = 0) const = 0;
	virtual Core::Info *info() const = 0;
	virtual Core::Config *config() const =0;
};

}

Q_DECLARE_INTERFACE(Core::BackendIface, "net.xylosper.CMPlayer.Core.BackendIface/0.3.0");

#endif
