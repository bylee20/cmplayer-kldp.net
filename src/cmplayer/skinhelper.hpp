#ifndef SKINHELPER_HPP
#define SKINHELPER_HPP

#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativePropertyMap>
#include "global.hpp"

class PlaylistModel;		class Mrl;
class QDeclarativePropertyMap;

class SkinStorage : public QDeclarativePropertyMap {
	Q_OBJECT
public:
	void setName(const QString &name) {m_name = name;}
	Q_INVOKABLE void save();
	Q_INVOKABLE void load();
private:
	QString m_name;
};

class SkinScreen : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY(QRectF geometry READ geometry NOTIFY geometryChanged)
public:
	static const int Type = UserType+1;
	SkinScreen();
	QRectF geometry() const {return m_rect;}
	int type() const {return Type;}
signals:
	void geometryChanged();
private:
	void geometryChanged(const QRectF &newOne, const QRectF &old);
	QRectF m_rect;
};

class SkinHelper : public QDeclarativeItem {
	Q_OBJECT
	Q_ENUMS(PlayerState)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
	Q_PROPERTY(int position READ position NOTIFY positionChanged)
	Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
	Q_PROPERTY(QSizeF size READ size NOTIFY sizeChanged)
	Q_PROPERTY(bool muted READ isMuted NOTIFY mutedChanged)
	Q_PROPERTY(PlayerState playerState READ playerState NOTIFY playerStateChanged)
	Q_PROPERTY(int mediaCount READ mediaCount NOTIFY mediaCountChanged)
	Q_PROPERTY(int currentMediaIndex READ currentMediaIndex NOTIFY currentMediaIndexChanged)
	Q_PROPERTY(QString currentMediaInfo READ currentMediaInfo NOTIFY currentMediaInfoChanged)
	Q_PROPERTY(bool fullscreen READ isFullscreen NOTIFY fullscreenChanged)
	Q_PROPERTY(QRectF screenGeometry READ screenGeometry WRITE updateScreenGeometry NOTIFY screenGeometryChanged)
	Q_PROPERTY(SkinStorage *storage READ storage NOTIFY storageChanged)
public:
	enum PlayerState {
		PlayingState = Global::PlayingState,
		PausedState = Global::PausedState,
		StoppedState = Global::StoppedState
	};
	SkinHelper();
	~SkinHelper();
	QRectF screenGeometry() const {return m_screenGeometry;}
	QSizeF size() const {return QSizeF(width(), height());}
	void resize(const QSizeF &size);
	SkinStorage *storage() const {return m_storage;}
	QString name() const {return m_name;}

	int duration() const {return m_duration;}
	int position() const {return m_position;}
	int volume() const;
	bool isMuted() const;
	PlayerState playerState() const;
	int mediaCount() const;
	int currentMediaIndex() const {return m_mediaIndex;}
	QString currentMediaInfo() const;
	bool isFullscreen() const {return m_fullscreen;}
	void setName(const QString &name);
	Q_INVOKABLE bool exec(const QString &id);
	Q_INVOKABLE void seek(int msec);
	Q_INVOKABLE void setVolume(int volume);
	Q_INVOKABLE void updateScreenGeometry(double x, double y, double w, double h);
	void updateScreenGeometry(const QRectF &rect);
	Q_INVOKABLE QString formatMSec(int msec, const QString &format) const;
signals:
	void storageCreated();
	void storageChanged();
	void fullscreenChanged();
	void durationChanged();
	void positionChanged();
	void volumeChanged();
	void screenChanged();
	void sizeChanged();
	void mutedChanged();
	void playerStateChanged();
	void mediaCountChanged();
	void currentMediaIndexChanged();
	void currentMediaInfoChanged();
	void screenGeometryChanged();
	void nameChanged();
private slots:
	void __updateDuration(int duration);
	void __updatePosition(int position);
	void __updateMrl(const Mrl &mrl);
	void __updateScreenGeometry();
	void __updateFullscreen(bool full);
private:
	void componentComplete();
	const PlaylistModel *playlist() const;
	int m_duration, m_position, m_mediaCount, m_mediaIndex;
	QRectF m_screenGeometry;
	SkinScreen *m_screen;
	bool m_fullscreen;
	SkinStorage *m_storage;
	QString m_name;
};

Q_DECLARE_METATYPE(SkinHelper::PlayerState)

class SkinManager {
public:
	static QString defaultSkinName();
	static QStringList avaiableSkinNames();
	static QList<QUrl> avaiableSkinUrls();
	static SkinHelper *load(const QString &name);
	static SkinHelper *load(const QUrl &url, const QString &nameHint = QString());
private:
	struct Data;
	static Data &d();
};


#endif // SKINHELPER_HPP
