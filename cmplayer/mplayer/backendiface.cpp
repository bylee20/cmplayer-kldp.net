#include "backendiface.h"
#include "info.h"
#include "playengine.h"
#include "config.h"

namespace Backend {

namespace MPlayer {

Backend::PlayEngine *BackendIface::createPlayEngine(QObject *parent) const {
	return new PlayEngine(this, parent);
}

Backend::Info *BackendIface::info() const {
	static Info info;
	return &info;
}

Backend::Config *BackendIface::config() const {
	static Config config;
	return &config;
}

}

}

Q_EXPORT_PLUGIN2(cmplayer_mplayer, Backend::MPlayer::BackendIface);
