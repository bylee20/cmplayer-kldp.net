#ifndef BACKEND_FACTORYIFACE_H
#define BACKEND_FACTORYIFACE_H

#include <QtPlugin>

namespace Backend {

class Info;						class PlayEngine;
class Config;

class FactoryIface {
public:
	virtual ~FactoryIface() {}
	virtual PlayEngine *createPlayEngine(QObject *parent) const = 0;
	virtual Info *info() const = 0;
	virtual Config *config() const = 0;
};

}

Q_DECLARE_INTERFACE(Backend::FactoryIface, "CMPlayer.Backend.FactoryIface/1.0")

#endif
