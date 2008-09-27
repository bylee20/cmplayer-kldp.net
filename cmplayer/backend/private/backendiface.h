#ifndef BACKEND_FACTORYIFACE_H
#define BACKEND_FACTORYIFACE_H

#include <QtCore/QtPlugin>

namespace Backend {

class Info;						class PlayEngine;
class Config;

class BackendIface {
public:
	virtual ~BackendIface() {}
	virtual PlayEngine *createPlayEngine(QObject *parent) const = 0;
	virtual Info *info() const = 0;
	virtual Config *config() const = 0;
};

}

Q_DECLARE_INTERFACE(Backend::BackendIface, "CMPlayer.Backend.BackendIface/1.0");

#endif
