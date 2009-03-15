#ifndef RECENTINFO_H
#define RECENTINFO_H

#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <core/mediasource.h>

namespace Core {class Playlist;}

class RecentStack : private QList<Core::MediaSource> {
public:
	RecentStack(const int size = 0);
	inline void setSize(int size) {m_size = size; checkSize();}
	inline void stack(const Core::MediaSource &source) {
		removeAll(source); push_front(source); checkSize();}
	inline int size() const {return QList<Core::MediaSource>::size();}
	inline void clear() {for (int i=0; i< m_size; ++i) (*this)[i] = Core::MediaSource();}
	inline Core::MediaSource &operator[] (int i) {return QList<Core::MediaSource>::operator[](i);}
	inline const Core::MediaSource &operator[] (int i) const {
		return QList<Core::MediaSource>::operator[](i);}
	inline RecentStack &operator = (const QList<Core::MediaSource> &other) {
		QList<Core::MediaSource>::operator=(other); checkSize(); return *this;}
	inline Core::MediaSource value(int i) const {return QList<Core::MediaSource>::value(i);}
	inline const QList<Core::MediaSource> &toList() const {return *this;}
private:
	inline void checkSize() {
		while(size() != m_size) {if (size() > m_size) pop_back(); else push_back(Core::MediaSource());}}
	int m_size;
};

class RecentInfo : public QObject {
	Q_OBJECT
public:
	static const int DefaultRememberCount = 10;
	static RecentInfo *get();
	~RecentInfo();
	void setLastPlaylist(const Core::Playlist &list);
	const Core::Playlist &lastPlaylist() const;
	void setLastSource(const Core::MediaSource &source);
	const Core::MediaSource &lastSource() const;
	const RecentStack &sources() const;
	Core::MediaSource source(int index) const;
	void setRememberCount(int count);
	void stackSource(const Core::MediaSource &source);
	void load();
	void save() const;
	int stoppedTime(const Core::MediaSource &source) const;
public slots:
	void clearStack();
	void setStopped(const Core::MediaSource &source, int time);
	void setFinished(const Core::MediaSource &source);
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
