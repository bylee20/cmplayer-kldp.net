#include "backendiface.h"
#include "playengine.h"
#include "info.h"
#include "config.h"

namespace MPlayer {

Core::PlayEngine *BackendIface::createPlayEngine(QObject *parent) const {
	return new PlayEngine(parent);
}

Core::Info *BackendIface::info() const {
	static Info info;
	return &info;
}

// Core::Config *BackendIface::config() const {
// 	static Config config;
// 	return &config;
// }

}

Q_EXPORT_PLUGIN2(cmplayer_backend_mplayer, MPlayer::BackendIface);
