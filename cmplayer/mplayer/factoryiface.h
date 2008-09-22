#ifndef BACKEND_MPLAYER_FACTORYIFACE_H
#define BACKEND_MPLAYER_FACTORYIFACE_H

#include <backend/factoryiface.h>

namespace Backend {

namespace MPlayer {

class FactoryIface : public QObject, public Backend::FactoryIface {
	Q_OBJECT
	Q_INTERFACES(Backend::FactoryIface)
public:
	PlayEngine *createPlayEngine(QObject *parent) const;
	Config *config() const;
	Info *info() const;
};

}

}

#endif
