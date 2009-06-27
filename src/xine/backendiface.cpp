#include "backendiface.h"
#include "playengine.h"
#include "config.h"
#include "info.h"

namespace Xine {

Core::PlayEngine *BackendIface::createPlayEngine(QObject *parent) const {
	return new Xine::PlayEngine(parent);
}

Core::Info *BackendIface::info() const {
	static Info info;
	return &info;
}

Core::Config *BackendIface::config() const {
	static Config config;
	return 0;
}

}

Q_EXPORT_PLUGIN2(cmplayer_engine_xine, Xine::BackendIface);
