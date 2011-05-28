#include "skinhelper.hpp"
#include <QtDeclarative/QDeclarativeProperty>
#include <QtGui/QGraphicsSceneHoverEvent>
#include <QtGui/QGraphicsSceneMouseEvent>
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
#include <QtGui/QPainter>
#include "skinmisc.hpp"

namespace Skin {


Helper::Helper() {
	m_duration = m_position = m_mediaCount = 0;
	m_fullscreen = false;
	m_autoHide = 0;

	m_media = new Media(this);

	PlayEngine *engine = LibVLC::engine();
	connect(engine, SIGNAL(durationChanged(int)), this, SLOT(__updateDuration(int)));
	connect(engine, SIGNAL(tick(int)), this, SLOT(__updatePosition(int)));
	connect(engine, SIGNAL(stateChanged(MediaState,MediaState)), this, SIGNAL(playerStateChanged()));
	connect(engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(__updateMrl(Mrl)));

	AudioController *audio = LibVLC::audio();
	connect(audio, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged()));
	connect(audio, SIGNAL(mutedChanged(bool)), this, SIGNAL(mutedChanged()));

	connect(&MainWindow::get(), SIGNAL(fullScreenChanged(bool)), this, SLOT(__updateFullscreen(bool)));
	m_storage = new Storage;

	m_renderable = m_screenGeometry = QRectF(0, 0, width(), height());
	m_box = new Spacer(this);
	m_frame = 0;
	m_hider = new ButtonItem(this);

	connect(m_hider, SIGNAL(clicked()), m_hider, SLOT(toggle()));
	connect(m_hider, SIGNAL(toggled(bool)), this, SLOT(onHiderToggled(bool)));
//	connect(m_screen, SIGNAL(geometryChanged()), this, SLOT(__updateScreenGeometry()));
}

void Helper::onHiderToggled(bool hide) {
	if (m_autoHide)
		m_autoHide->setVisible(!hide);
}

void Helper::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
	if (m_autoHide && m_fullscreen) {
		m_autoHide->setVisible(boundingRect().contains(event->pos()) && !m_screenGeometry.contains(event->pos()));
		m_hider->setVisible(m_autoHide->isVisible());
	}
}

Helper::~Helper() {
	m_storage->insert("hide", m_hider->isChecked());
	m_storage->save();
	delete m_storage;
}

QSizeF Helper::sizeHint() const {
	if (m_autoHide && !m_autoHide->isVisible() && !m_fullscreen)
		return QSizeF(0, 0);
	const QSizeF size = m_autoHide ? m_autoHide->boundingRect().size() : this->size();
	return size - m_screenGeometry.size();
}

void Helper::__updateRenderableArea() {
	const QRectF rect = m_fullscreen || (m_autoHide && !m_autoHide->isVisible()) ? boundingRect() : m_screenGeometry;
	if (rect != m_renderable) {
		m_renderable = rect;
		emit renderableAreaChanged();
	}
}

void Helper::setName(const QString &name) {
	if (m_name != name) {
		m_storage->setName(m_name = name);
		emit nameChanged();
	}
}

void Helper::componentComplete() {
	QDeclarativeItem::componentComplete();
	m_storage->insert("hide", false);

	PlayEngine *engine = LibVLC::engine();
	__updateDuration(engine->duration());
	__updatePosition(engine->position());
	__updateMrl(engine->mrl());
//	__updateScreenGeometry();
	__updateFullscreen(MainWindow::get().isFullScreen());
	emit playerStateChanged();
	emit volumeChanged();
	emit mutedChanged();
	emit storageCreated();
	m_storage->load();

	m_hider->setChecked(m_storage->value("hide").toBool());
}

void Helper::setScreenGeometry(const QRectF &rect) {
	if (m_screenGeometry != rect) {
		m_screenGeometry = rect;
		emit screenGeometryChanged();
		if (!m_fullscreen)
			__updateRenderableArea();
//			emit renderableAreaChanged();
	}
}

int Helper::mediaCount() const {return playlist()->rowCount();}
//QString Helper::currentMediaInfo() const {return LibVLC::engine()->mrl().displayName();}
QString Helper::formatMSec(int msec, const QString &format) const {return msecToString(msec, format);}
void Helper::seek(int msec) {LibVLC::engine()->seek(msec);}
int Helper::volume() const {return LibVLC::audio()->volume();}
bool Helper::isMuted() const {return LibVLC::audio()->isMuted();}
const PlaylistModel *Helper::playlist() const {return PlaylistView::get().model();}
Helper::PlayerState Helper::playerState() const {return (PlayerState)LibVLC::engine()->state();}

void Helper::__updateDuration(int duration) {
	if (m_duration != duration) {
		m_duration = duration;
		emit durationChanged();
	}
}

void Helper::__updatePosition(int position) {
	if (m_position/1000 != position/1000) {
		m_position = position;
		emit positionChanged();
	}
}

void Helper::__updateFullscreen(bool full) {
	if (m_fullscreen != full) {
		m_fullscreen = full;
		__updateRenderableArea();
//		emit renderableAreaChanged();

		m_hider->setVisible(!m_fullscreen);
		setAcceptsHoverEvents(m_fullscreen);
		if (m_autoHide)
			m_autoHide->setVisible(!m_fullscreen && !m_hider->isChecked());

		emit fullscreenChanged();
	}
}

//hider.onClicked: {hider.checked = !hider.checked}

//hider.onCheckedChanged: {
//	if (autoHide)
//		autoHide.visible = !hider.checked
//}

void Helper::setAutoHide(QDeclarativeItem *item) {
	if (m_autoHide != item) {
		if (m_autoHide)
			disconnect(m_autoHide, 0, this, 0);
		m_autoHide = item;
		connect(m_autoHide, SIGNAL(visibleChanged()), this, SLOT(__updateRenderableArea()));
		emit autoHideChanged();
	}
}

void Helper::setVolume(int volume) {
	AudioController *audio = LibVLC::audio();
	if (audio->volume() != volume)
		audio->setVolume(volume);
}

void Helper::resize(const QSizeF &size) {
	if (size.width() != width() || size.height() != height()) {
		setSize(size);
		emit sizeChanged();
	}
}

//void SkinHelper::save(const QString &name, const QString &key, const QVariant &value) const {
//	Record r(name);
//	r.setValue(key, value);

//}

//QVariant SkinHelper::load(const QString &name, const QString &key, const QVariant &def) const {
//	Record r(name);
//	return r.value(key, def);
//}

QAction *Helper::action(const QString &id) {
	QAction *action = RootMenu::get().action(id);
	if (!action)
		qWarning("SkinHelper: no action or menu %s", qPrintable(id));
	return action;
}

bool Helper::exec(const QString &id) {
	QAction *action = this->action(id);
	if (!action)
		return false;
	if (action->menu())
		action->menu()->exec(QCursor::pos());
	else
		action->trigger();
	return true;
}

void Helper::__updateMrl(const Mrl &mrl) {
	m_media->setIndex(playlist()->rowOf(mrl));
	m_media->setInfo(mrl.displayName());
	emit currentMediaChanged();
	if (m_mediaCount != playlist()->rowCount()) {
		m_mediaCount = playlist()->rowCount();
		emit mediaCountChanged();
	}
}

/*******************************************************************/

struct Manager::Data {
	Data();
	void check(const QString &path);

	QMap<QString, QUrl> skins;
};

void Manager::Data::check(const QString &path) {
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

Manager::Data::Data() {
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

Manager::Data &Manager::d() {
	static Data d;
	return d;
}

QStringList Manager::avaiableSkinNames() {
	return d().skins.keys();
}

QList<QUrl> Manager::avaiableSkinUrls() {
	return d().skins.values();
}

QString Manager::defaultSkinName() {
	if (d().skins.isEmpty())
		return QString();
	if (d().skins.contains("default"))
		return "default";
	return d().skins.begin().key();
}

Helper *Manager::load(const QString &name) {
	const QUrl url = d().skins.value(name, QUrl());
	return url.isEmpty() ? 0 : load(url, name);
}

Helper *Manager::load(const QUrl &url, const QString &nameHint) {
	static QDeclarativeEngine engine;
	static QDeclarativeComponent comp(&engine, 0);
	comp.loadUrl(url);
	if (comp.isError()) {
		qWarning("Cannot load skin %s", qPrintable(url.toString()));
		qWarning("%s", qPrintable(comp.errorString()));
		return 0;
	} else {
		Helper *skin = qobject_cast<Helper*>(comp.create());
		if (!skin)
			return 0;
		if (skin->name().isEmpty())
			skin->setName(nameHint);
		return skin;
	}
}

}
