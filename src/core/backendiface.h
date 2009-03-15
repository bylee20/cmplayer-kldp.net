#ifndef CORE_BACKENDIFACE_H
#define CORE_BACKENDIFACE_H

#include <QtCore/QtPlugin>

namespace Core {

class PlayEngine;			class Info;

class BackendIface {
public:
	virtual ~BackendIface() {}
	virtual PlayEngine *createPlayEngine(QObject *parent = 0) const = 0;
	virtual Core::Info *info() const = 0;
};

}

Q_DECLARE_INTERFACE(Core::BackendIface, "CMPlayer.Backend.BackendIface/3.1");

#endif
