//#ifndef MEDIAINFO_H_
//#define MEDIAINFO_H_
//
//#include <QSize>
//
//class QSize;					class QProcess;
//
//namespace MPlayer {
//
//class MediaSource;				class DVDInfo;
//
//class MediaInfo {
//public:
//	~MediaInfo();
//	MediaInfo(const MediaInfo &other);
//	MediaInfo &operator = (const MediaInfo &rhs);
//	const QSize &videoSize() const {return m_videoSize;}
//	qreal aspectRatio() const {return double(m_videoSize.width())/double(m_videoSize.height());}
//	qint64 length() const {return m_length;}
//	bool hasVideo() const {return m_hasVideo;}
//	bool isValid() const {return m_valid;}
//	bool isDisc() const {return m_disc;}
//	const DVDInfo &dvd() const {return *m_dvd;}
//private:
//	MediaInfo();
//	bool get(const MediaSource &source);
//	bool get(const QString &parse, const MediaSource &source, QProcess *proc = 0);
//	friend class MediaSource;
//	friend class PlayEngine;
//	friend class MPlayerProcess;
//	qint64 m_length;
//	QSize m_videoSize;
//	bool m_valid, m_hasVideo, m_disc;
//	DVDInfo *m_dvd;
//};
//
//}
//
//#endif /* MEDIAINFO_H_ */
