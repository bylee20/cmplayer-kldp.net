#include "skinhelper.hpp"
#include "record.hpp"
#include "mainwindow.hpp"
#include "playlistmodel.hpp"
#include "playlistview.hpp"
#include "menu.hpp"
#include "playengine.hpp"
#include "audiocontroller.hpp"
#include "libvlc.hpp"
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>

void SkinStorage::save() {
	if (m_name.isEmpty()) {
		qWarning("Skin name is empty. Cannot save storage.");
		return;
	}
	const QStringList keys = this->keys();
	Record r("skin");
	r.beginGroup(m_name);
	for (int i=0; i<keys.size(); ++i)
		r.setValue(keys[i], this->value(keys[i]));
	r.endGroup();
}

void SkinStorage::load() {
	if (m_name.isEmpty()) {
		qWarning("Skin name is empty. Cannot load storage.");
		return;
	}
	Record r("skin");
	r.beginGroup(m_name);
	const QStringList keys = this->keys();
	for (int i=0; i<keys.size(); ++i) {
		QVariant &data = (*this)[keys[i]];
		data = r.value(keys[i], data);
	}
	r.endGroup();
}

SkinScreen::SkinScreen() {}

void SkinScreen::geometryChanged(const QRectF &newOne, const QRectF &old) {
	QDeclarativeItem::geometryChanged(newOne, old);
	if (newOne != m_rect) {
		m_rect = newOne;
		emit geometryChanged();
	}
}

SkinHelper::SkinHelper() {
	m_mediaIndex = m_duration = m_position = m_mediaCount = 0;
	m_fullscreen = false;

	PlayEngine *engine = LibVLC::engine();
	connect(engine, SIGNAL(durationChanged(int)), this, SLOT(__updateDuration(int)));
	connect(engine, SIGNAL(tick(int)), this, SLOT(__updatePosition(int)));
	connect(engine, SIGNAL(stateChanged(MediaState,MediaState)), this, SIGNAL(playerStateChanged()));
	connect(engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(__updateMrl(Mrl)));

	AudioController *audio = LibVLC::audio();
	connect(audio, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged()));
	connect(audio, SIGNAL(mutedChanged(bool)), this, SIGNAL(mutedChanged()));

	connect(&MainWindow::get(), SIGNAL(fullScreenChanged(bool)), this, SLOT(__updateFullscreen(bool)));
	m_storage = new SkinStorage;
}

SkinHelper::~SkinHelper() {
	m_storage->save();
	delete m_storage;
}

void SkinHelper::setName(const QString &name) {
	if (m_name != name) {
		m_storage->setName(m_name = name);
		emit nameChanged();
	}
}

SkinScreen *findScreen(QGraphicsItem *parent) {
	const QList<QGraphicsItem*> items = parent->childItems();
	for (int i=0; i<items.size(); ++i) {
		SkinScreen *screen = qgraphicsitem_cast<SkinScreen*>(items[i]);
		if (screen)
			return screen;
		screen = findScreen(items[i]);
		if (screen)
			return screen;
	}
	return 0;
}

void SkinHelper::componentComplete() {
	QDeclarativeItem::componentComplete();
	m_screen = findScreen(this);
	if (m_screen)
		connect(m_screen, SIGNAL(geometryChanged()), this, SLOT(__updateScreenGeometry()));

	PlayEngine *engine = LibVLC::engine();
	__updateDuration(engine->duration());
	__updatePosition(engine->position());
	__updateMrl(engine->mrl());
	__updateScreenGeometry();
	__updateFullscreen(MainWindow::get().isFullScreen());
	emit playerStateChanged();
	emit volumeChanged();
	emit mutedChanged();
	emit storageCreated();
	m_storage->load();
}

void SkinHelper::__updateScreenGeometry() {
	if (m_screen && m_screenGeometry != m_screen->geometry()) {
		m_screenGeometry = m_screen->geometry();
		emit screenGeometryChanged();
	}
}

int SkinHelper::mediaCount() const {return playlist()->rowCount();}
QString SkinHelper::currentMediaInfo() const {return LibVLC::engine()->mrl().displayName();}
QString SkinHelper::formatMSec(int msec, const QString &format) const {return msecToString(msec, format);}
void SkinHelper::seek(int msec) {LibVLC::engine()->seek(msec);}
int SkinHelper::volume() const {return LibVLC::audio()->volume();}
bool SkinHelper::isMuted() const {return LibVLC::audio()->isMuted();}
const PlaylistModel *SkinHelper::playlist() const {return PlaylistView::get().model();}
SkinHelper::PlayerState SkinHelper::playerState() const {return (PlayerState)LibVLC::engine()->state();}

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

void SkinHelper::__updateFullscreen(bool full) {
	if (m_fullscreen != full) {
		m_fullscreen = full;
		emit fullscreenChanged();
	}
}

void SkinHelper::setVolume(int volume) {
	AudioController *audio = LibVLC::audio();
	if (audio->volume() != volume)
		audio->setVolume(volume);
}

void SkinHelper::resize(const QSizeF &size) {
	if (size.width() != width() || size.height() != height()) {
		setSize(size);
		emit sizeChanged();
	}
}

void SkinHelper::updateScreenGeometry(const QRectF &rect) {
	if (rect != m_screenGeometry) {
		m_screenGeometry = rect;
		emit screenGeometryChanged();
	}
}

void SkinHelper::updateScreenGeometry(double x, double y, double w, double h) {
	updateScreenGeometry(QRectF(x, y, w, h));
}

//void SkinHelper::save(const QString &name, const QString &key, const QVariant &value) const {
//	Record r(name);
//	r.setValue(key, value);

//}

//QVariant SkinHelper::load(const QString &name, const QString &key, const QVariant &def) const {
//	Record r(name);
//	return r.value(key, def);
//}

bool SkinHelper::exec(const QString &id) {
	QAction *action = RootMenu::get().action(id);
	if (action) {
		if (action->menu())
			action->menu()->exec(QCursor::pos());
		else
			action->trigger();
		return true;
	} else {
		qWarning("SkinHelper::exec(): no action or menu %s", qPrintable(id));
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

/*******************************************************************/

struct SkinManager::Data {
	Data();
	void check(const QString &path);

	QMap<QString, QUrl> skins;
};

void SkinManager::Data::check(const QString &path) {
	QDir dir(path);
	qDebug() << "checking skin path:" << dir.absolutePath();
	if (!dir.exists())
		return;
	QStringList skins = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (int i=0; i<skins.size(); ++i) {
		dir.cd(skins[i]);
		const QFileInfoList files = dir.entryInfoList(QStringList(_LS("skin.qml")), QDir::Files);
		if (files.size() == 1)
			this->skins[skins[i].toLower()] = QUrl::fromLocalFile(files.first().absoluteFilePath());
	}
}

SkinManager::Data::Data() {
	const QString app = QCoreApplication::applicationDirPath();
	if (app.isEmpty())
		check("./skin");
	else
		check(app + "/skin");
#ifdef CMPLAYER_SKIN_PATH
	check(CMPLAYER_SKIN_PATH);
#endif
	check(QDir::homePath() + "/.cmplayer/skin");
	const QByteArray path = qgetenv("CMPLAYER_SKIN_PATH");
	if (!path.isEmpty())
		check(QString::fromLocal8Bit(path));
}

SkinManager::Data &SkinManager::d() {
	static Data d;
	return d;
}

QStringList SkinManager::avaiableSkinNames() {
	return d().skins.keys();
}

QList<QUrl> SkinManager::avaiableSkinUrls() {
	return d().skins.values();
}

QString SkinManager::defaultSkinName() {
	if (d().skins.isEmpty())
		return QString();
	if (d().skins.contains("default"))
		return "default";
	return d().skins.begin().key();
}

SkinHelper *SkinManager::load(const QString &name) {
	const QUrl url = d().skins.value(name, QUrl());
	return url.isEmpty() ? 0 : load(url, name);
}

SkinHelper *SkinManager::load(const QUrl &url, const QString &nameHint) {
	static QDeclarativeEngine engine;
	static QDeclarativeComponent comp(&engine, 0);
	comp.loadUrl(url);
	if (comp.isError()) {
		qWarning("Cannot load skin %s", qPrintable(url.toString()));
		qWarning("%s", qPrintable(comp.errorString()));
		return 0;
	} else {
		SkinHelper *skin = qobject_cast<SkinHelper*>(comp.create());
		if (!skin)
			return 0;
		if (skin->name().isEmpty())
			skin->setName(nameHint);
		return skin;
	}
}
