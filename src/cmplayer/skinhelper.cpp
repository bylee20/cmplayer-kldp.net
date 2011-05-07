#include "skinhelper.hpp"
#include "playlistmodel.hpp"
#include "playlist.hpp"
#include "playlistview.hpp"
#include "menu.hpp"
#include "playengine.hpp"
#include "audiocontroller.hpp"
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>

SkinHelper::SkinHelper() {

	m_mediaIndex = m_duration = m_position = m_mediaCount = 0;

	PlayEngine *engine = &PlayEngine::get();
	connect(engine, SIGNAL(durationChanged(int)), this, SLOT(__updateDuration(int)));
	connect(engine, SIGNAL(tick(int)), this, SLOT(__updatePosition(int)));
	connect(engine, SIGNAL(stateChanged(MediaState,MediaState)), this, SIGNAL(playerStateChanged()));
	connect(engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(__updateMrl(Mrl)));

	AudioController *audio = &AudioController::get();
	connect(audio, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged()));
	connect(audio, SIGNAL(mutedChanged(bool)), this, SIGNAL(mutedChanged()));
}

void SkinHelper::__updateDuration(int duration) {
	if (m_duration != duration) {
		m_duration = duration;
		emit durationChanged();
	}
}

void SkinHelper::__updatePosition(int position) {
	if (m_position/1000 != position/1000) {
		m_position = position;
		emit positionChanged();
	}
}

QString SkinHelper::msecToString(int msec, const QString &format) const {
	return QTime().addMSecs(msec).toString(format);
}

void SkinHelper::seek(int msec) {
	PlayEngine::get().seek(msec);
}

void SkinHelper::setVolume(int volume) {
	AudioController &audio = AudioController::get();
	if (audio.volume() != volume)
		audio.setVolume(volume);
}

int SkinHelper::volume() const {
	return AudioController::get().volume();
}

void SkinHelper::resize(const QSizeF &size) {
	if (size.width() != width() || size.height() != height()) {
		setSize(size);
		emit sizeChanged();
	}
}

void SkinHelper::updateScreen(double x, double y, double w, double h) {
	const QRectF rect(x, y, w, h);
	if (m_screen != rect) {
		m_screen = rect;
		qDebug() << m_screen;
		emit screenChanged();
	}
}

bool SkinHelper::isMuted() const {
	return AudioController::get().isMuted();
}

const PlaylistModel *SkinHelper::playlist() const {
	return PlaylistView::get().model();
}

SkinHelper::PlayerState SkinHelper::playerState() const {
	return (PlayerState)PlayEngine::get().state();
}

bool SkinHelper::exec(const QString &id) {
	QAction *action = RootMenu::get().action(id);
	if (action) {
		action->trigger();
		return true;
	} else {
		qWarning("SkinHelper::exec(): no action %s", qPrintable(id));
		return false;
	}
}

void SkinHelper::__updateMrl(const Mrl &mrl) {
	emit currentMediaInfoChanged();
	const int idx = playlist()->rowOf(mrl);
	if (m_mediaIndex != idx) {
		m_mediaIndex = idx;
		emit currentMediaIndexChanged();
	}
	if (m_mediaCount != playlist()->rowCount()) {
		m_mediaCount = playlist()->rowCount();
		emit mediaCountChanged();
	}
}

int SkinHelper::mediaCount() const {
	return playlist()->rowCount();
}

QString SkinHelper::currentMediaInfo() const {
	return PlayEngine::get().mrl().displayName();
}


SkinHelper *SkinManager::load(const QUrl &url) {
	static QDeclarativeEngine engine;
	static QDeclarativeComponent comp(&engine, 0);
	comp.loadUrl(url);
	if (comp.isError()) {
		qWarning("Cannot load skin %s", qPrintable(url.toString()));
		qWarning("%s", qPrintable(comp.errorString()));
		return 0;
	} else
		return qobject_cast<SkinHelper*>(comp.create());
}
