#ifndef SKINHELPER_HPP
#define SKINHELPER_HPP

#include <QtDeclarative/QDeclarativeItem>
#include "global.hpp"

class PlaylistModel;		class Mrl;

class SkinHelper : public QDeclarativeItem {
	Q_OBJECT
	Q_ENUMS(PlayerState)
	Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
	Q_PROPERTY(int position READ position NOTIFY positionChanged)
	Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
	Q_PROPERTY(QRectF screen READ screen NOTIFY screenChanged)
	Q_PROPERTY(QSizeF size READ size WRITE resize NOTIFY sizeChanged)
	Q_PROPERTY(bool muted READ isMuted NOTIFY mutedChanged)
	Q_PROPERTY(PlayerState playerState READ playerState NOTIFY playerStateChanged)
	Q_PROPERTY(int mediaCount READ mediaCount NOTIFY mediaCountChanged)
	Q_PROPERTY(int currentMediaIndex READ currentMediaIndex NOTIFY currentMediaIndexChanged)
	Q_PROPERTY(QString currentMediaInfo READ currentMediaInfo NOTIFY currentMediaInfoChanged)
public:
	enum PlayerState {
		PlayingState = Global::PlayingState,
		PausedState = Global::PausedState,
		StoppedState = Global::StoppedState
	};
	SkinHelper();

	QRectF screen() const {return m_screen;}
	QSizeF size() const {return QSizeF(width(), height());}
	void resize(const QSizeF &size);

	int duration() const {return m_duration;}
	int position() const {return m_position;}
	int volume() const;
	bool isMuted() const;
	PlayerState playerState() const;
	int mediaCount() const;
	int currentMediaIndex() const {return m_mediaIndex;}
	QString currentMediaInfo() const;
//	QSizeF sizeHint() const {return m_sizeHint;}

	Q_INVOKABLE bool exec(const QString &id);
	Q_INVOKABLE void seek(int msec);
	Q_INVOKABLE void setVolume(int volume);
	Q_INVOKABLE void updateScreen(double x, double y, double w, double h);
//	Q_INVOKABLE void updateSizeHint(double width, double height);
	Q_INVOKABLE QString msecToString(int msec, const QString &format) const;
signals:
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
private slots:
	void __updateDuration(int duration);
	void __updatePosition(int position);
	void __updateMrl(const Mrl &mrl);
private:
	const PlaylistModel *playlist() const;
	int m_duration;
	int m_position;
	QRectF m_screen;
	int m_mediaCount;
	int m_mediaIndex;
//	QSizeF m_sizeHint;
};

Q_DECLARE_METATYPE(SkinHelper::PlayerState)

class SkinManager {
public:
	static SkinHelper *load(const QUrl &url);
};


#endif // SKINHELPER_HPP
