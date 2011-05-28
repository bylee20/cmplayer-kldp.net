#ifndef SKINHELPER_HPP
#define SKINHELPER_HPP

#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativePropertyMap>
#include "global.hpp"

class PlaylistModel;		class Mrl;
class QDeclarativePropertyMap;

namespace Skin {

class Frame;	class Media;	class Spacer;	class Storage;
class RectItem;	class ButtonItem;

class Helper : public QDeclarativeItem {
	Q_OBJECT
	Q_ENUMS(PlayerState)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
	Q_PROPERTY(int position READ position NOTIFY positionChanged)
	Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
	Q_PROPERTY(QSizeF size READ size NOTIFY sizeChanged)
	Q_PROPERTY(bool muted READ isMuted NOTIFY mutedChanged)
	Q_PROPERTY(PlayerState playerState READ playerState NOTIFY playerStateChanged)
	Q_PROPERTY(bool fullscreen READ isFullscreen NOTIFY fullscreenChanged)
	Q_PROPERTY(Skin::Storage *storage READ storage DESIGNABLE false CONSTANT FINAL)
//	Q_PROPERTY(Skin::Spacer *screen READ screen DESIGNABLE false CONSTANT FINAL)
	Q_PROPERTY(QRectF screenGeometry READ screenGeometry WRITE setScreenGeometry NOTIFY screenGeometryChanged)
	Q_PROPERTY(Skin::Spacer *box READ box DESIGNABLE false CONSTANT FINAL)
	Q_PROPERTY(Skin::Media *currentMedia READ currentMedia NOTIFY currentMediaChanged)
	Q_PROPERTY(int mediaCount READ mediaCount NOTIFY mediaCountChanged)
	Q_PROPERTY(Skin::Frame *frame READ frame WRITE setFrame NOTIFY frameChanged)
	Q_PROPERTY(QDeclarativeItem *autoHide READ autoHide WRITE setAutoHide NOTIFY autoHideChanged)
	Q_PROPERTY(Skin::ButtonItem *hider READ hider CONSTANT FINAL)
public:
	enum PlayerState {
		PlayingState = Global::PlayingState,
		PausedState = Global::PausedState,
		StoppedState = Global::StoppedState
	};
	Helper();
	~Helper();
	QRectF screenGeometry() const {return m_screenGeometry;}
	Frame *frame() const {return m_frame;}
	void setFrame(Frame *frame) {if (m_frame != frame) {m_frame = frame; emit frameChanged();}}
	QSizeF size() const {return QSizeF(width(), height());}
	QString name() const {return m_name;}
	int duration() const {return m_duration;}
	int position() const {return m_position;}
	int volume() const;
	QSizeF sizeHint() const;
	QDeclarativeItem *autoHide() const {return m_autoHide;}
	void setAutoHide(QDeclarativeItem *item);
	bool isMuted() const;
	PlayerState playerState() const;
	int mediaCount() const;
	bool isFullscreen() const {return m_fullscreen;}
	Media *currentMedia() const {return m_media;}
	void resize(const QSizeF &size);
	void setName(const QString &name);
//	Spacer *screen() const {return m_screen;}
	Spacer *box() const {return m_box;}
	Storage *storage() const {return m_storage;}
	ButtonItem *hider() const {return m_hider;}
	Q_INVOKABLE QAction *action(const QString &id);
	Q_INVOKABLE bool exec(const QString &id);
	Q_INVOKABLE void seek(int msec);
	Q_INVOKABLE void setVolume(int volume);
	Q_INVOKABLE QString formatMSec(int msec, const QString &format) const;
//	Q_INVOKABLE bool contains(double x, double y) const {
//		return boundingRect().contains(x, y) && !m_box->geometry().contains(x, y);
//	}
	void setScreenGeometry(const QRectF &rect);
	QRectF renderableArea() const {return m_renderable;}
signals:
	void autoHideChanged();
	void frameChanged();
	void storageCreated();
	void fullscreenChanged();
	void durationChanged();
	void positionChanged();
	void volumeChanged();
	void screenChanged();
	void sizeChanged();
	void mutedChanged();
	void playerStateChanged();
	void mediaCountChanged();
	void currentMediaChanged();
	void screenGeometryChanged();
	void renderableAreaChanged();
	void nameChanged();
private slots:
	void __updateDuration(int duration);
	void __updatePosition(int position);
	void __updateMrl(const Mrl &mrl);
//	void __updateScreenGeometry();
	void __updateFullscreen(bool full);
	void __updateRenderableArea();
	void onHiderToggled(bool hide);
private:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void componentComplete();
	const PlaylistModel *playlist() const;
	int m_duration, m_position, m_mediaCount;
	QRectF m_screenGeometry, m_renderable;
	Spacer *m_box;
	bool m_fullscreen;
	Storage *m_storage;
	QString m_name;
	Media *m_media;
	Frame *m_frame;
	QDeclarativeItem *m_autoHide;
	ButtonItem *m_hider;
};

class Manager {
public:
	static QString defaultSkinName();
	static QStringList avaiableSkinNames();
	static QList<QUrl> avaiableSkinUrls();
	static Helper *load(const QString &name);
	static Helper *load(const QUrl &url, const QString &nameHint = QString());
private:
	struct Data;
	static Data &d();
};

}

Q_DECLARE_METATYPE(Skin::Helper::PlayerState)

#endif // SKINHELPER_HPP
