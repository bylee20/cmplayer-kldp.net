#ifndef RECENTINFO_H
#define RECENTINFO_H

#include <QStringList>
#include <QObject>
#include <backend/mediasource.h>

namespace Backend {class PlayList;}

class RecentStack : private QList<Backend::MediaSource> {
public:
	RecentStack(const int size = 0);
	inline void setSize(int size) {m_size = size; checkSize();}
	inline void stack(const Backend::MediaSource &source) {
		removeAll(source); push_front(source); checkSize();}
	inline int size() const {return QList<Backend::MediaSource>::size();}
	inline void clear() {for (int i=0; i< m_size; ++i) (*this)[i] = Backend::MediaSource();}
	inline Backend::MediaSource &operator[] (int i) {return QList<Backend::MediaSource>::operator[](i);}
	inline const Backend::MediaSource &operator[] (int i) const {
		return QList<Backend::MediaSource>::operator[](i);}
	inline RecentStack &operator = (const QList<Backend::MediaSource> &other) {
		QList<Backend::MediaSource>::operator=(other); checkSize(); return *this;}
	inline Backend::MediaSource value(int i) const {return QList<Backend::MediaSource>::value(i);}
	inline const QList<Backend::MediaSource> &toList() const {return *this;}
private:
	inline void checkSize() {
		while(size() != m_size) {if (size() > m_size) pop_back(); else push_back(Backend::MediaSource());}}
	int m_size;
};

class RecentInfo : public QObject {
	Q_OBJECT
public:
	static const int DefaultRememberCount = 5;
	static RecentInfo *get() {static RecentInfo obj; return &obj;}
	~RecentInfo();
	void setLastPlayList(const Backend::PlayList &list);
	const Backend::PlayList &lastPlayList() const;
	void setLastSource(const Backend::MediaSource &source);
	const Backend::MediaSource &lastSource() const;
	const RecentStack &sources() const;
	Backend::MediaSource source(int index) const;
	void clearStack();
	void setRememberCount(int count);
	void stackSource(const Backend::MediaSource &source);
	void load();
	void save() const;
	void setStopped(const Backend::MediaSource &source, qint64 time);
	void setFinished(const Backend::MediaSource &source);
	qint64 stoppedTime(const Backend::MediaSource &source) const;
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
