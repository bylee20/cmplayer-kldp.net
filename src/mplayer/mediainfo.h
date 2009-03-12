#ifndef MPLAYER_MEDIAINFO_H
#define MPLAYER_MEDIAINFO_H

#include <QtCore/QSize>
#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtCore/QMap>

class QSize;					class QProcess;

namespace Core {class MediaSource;}

namespace MPlayer {

struct DVDInfo {
public:
	struct Title {
		Title() : number(1), angles(0), length(0) {}
		QVector<int> chapters;
		int number, angles,  length;
	};
	QVector<Title> titles;
	QStringList channels;
};

class MediaInfo {
public:
	MediaInfo(const MediaInfo &other);
	MediaInfo &operator = (const MediaInfo &rhs);
	const QSize &videoSize() const {return m_videoSize;}
	qreal aspectRatio() const {return double(m_videoSize.width())/double(m_videoSize.height());}
	qint64 length() const {return m_length;}
	bool hasVideo() const {return m_hasVideo;}
	bool isValid() const {return m_valid;}
	bool isDisc() const {return m_disc;}
	const DVDInfo &dvd() const {return m_dvd;}
	const QMap<int, QString> &tracks() const {return m_tracks;}
	bool get(const Core::MediaSource &source);
	bool get(const QString &parse, const Core::MediaSource &source, QProcess *proc = 0);
private:
	MediaInfo();
	friend class MediaSource;
	friend class PlayEngine;
	friend class MPlayerProcess;
	qint64 m_length;
	QSize m_videoSize;
	bool m_valid, m_hasVideo, m_disc;
	QMap<int, QString> m_tracks;
	DVDInfo m_dvd;
};

}

#endif /* MEDIAINFO_H_ */
