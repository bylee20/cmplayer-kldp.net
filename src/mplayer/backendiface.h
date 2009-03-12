#ifndef MPLAYER_FACTORYIFACE_H
#define MPLAYER_FACTORYIFACE_H

#include <core/backendiface.h>

namespace MPlayer {

class BackendIface : public QObject, public Core::BackendIface {
	Q_OBJECT
	Q_INTERFACES(Core::BackendIface)
public:
	virtual Core::PlayEngine *createPlayEngine(QObject *parent = 0) const;
	virtual Core::Info *info() const;
};

}

#endif
