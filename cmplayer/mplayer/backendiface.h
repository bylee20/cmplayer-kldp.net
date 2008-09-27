#ifndef BACKEND_MPLAYER_FACTORYIFACE_H
#define BACKEND_MPLAYER_FACTORYIFACE_H

#include <backend/private/backendiface.h>

namespace Backend {

namespace MPlayer {

class BackendIface : public QObject, public Backend::BackendIface {
	Q_OBJECT
	Q_INTERFACES(Backend::BackendIface)
public:
	PlayEngine *createPlayEngine(QObject *parent) const;
	Config *config() const;
	Info *info() const;
};

}

}

#endif
