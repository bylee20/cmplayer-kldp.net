#ifndef RECENTINFO_H
#define RECENTINFO_H

#include <QStringList>
#include <QObject>
#include <mplayer/mediasource.h>

namespace MPlayer {class PlayList;}

class RecentStack : private QList<MPlayer::MediaSource> {
public:
	RecentStack(const int size = 0);
	inline void setSize(int size) {m_size = size; checkSize();}
	inline void stack(const MPlayer::MediaSource &source) {
		removeAll(source); push_front(source); checkSize();}
	inline int size() const {return QList<MPlayer::MediaSource>::size();}
	inline void clear() {for (int i=0; i< m_size; ++i) (*this)[i] = MPlayer::MediaSource();}
	inline MPlayer::MediaSource &operator[] (int i) {return QList<MPlayer::MediaSource>::operator[](i);}
	inline const MPlayer::MediaSource &operator[] (int i) const {
		return QList<MPlayer::MediaSource>::operator[](i);}
	inline RecentStack &operator = (const QList<MPlayer::MediaSource> &other) {
		QList<MPlayer::MediaSource>::operator=(other); checkSize(); return *this;}
	inline MPlayer::MediaSource value(int i) const {return QList<MPlayer::MediaSource>::value(i);}
	inline const QList<MPlayer::MediaSource> &toList() const {return *this;}
private:
	inline void checkSize() {
		while(size() != m_size) {if (size() > m_size) pop_back(); else push_back(MPlayer::MediaSource());}}
	int m_size;
};

class RecentInfo : public QObject {
	Q_OBJECT
public:
	static const int DefaultRememberCount = 5;
	static RecentInfo *get() {static RecentInfo obj; return &obj;}
	~RecentInfo();
	void setLastPlayList(const MPlayer::PlayList &list);
	const MPlayer::PlayList &lastPlayList() const;
	void setLastSource(const MPlayer::MediaSource &source);
	const MPlayer::MediaSource &lastSource() const;
	const RecentStack &sources() const;
	MPlayer::MediaSource source(int index) const;
	void clearStack();
	void setRememberCount(int count);
	void stackSource(const MPlayer::MediaSource &source);
	void load();
	void save() const;
	void setStopped(const MPlayer::MediaSource &source, qint64 time);
	void setFinished(const MPlayer::MediaSource &source);
	qint64 stoppedTime(const MPlayer::MediaSource &source) const;
signals:
	void sourcesChanged(const RecentStack &stack);
	void rememberCountChanged(int count);
private:
	struct Data;
	friend struct Data;
	Data *d;
	RecentInfo();
};

#endif
