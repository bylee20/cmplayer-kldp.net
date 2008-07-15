//#ifndef RECENTINFO_H
//#define RECENTINFO_H
//
//#include <QStringList>
//#include <QObject>
//#include <xine/mediasource.h>
//
//namespace Xine {class PlayList;}
//
//class RecentStack : private QList<Xine::MediaSource> {
//public:
//	RecentStack(const int size = 0);
//	inline void setSize(int size) {m_size = size; checkSize();}
//	inline void stack(const Xine::MediaSource &source) {
//		removeAll(source); push_front(source); checkSize();}
//	inline int size() const {return QList<Xine::MediaSource>::size();}
//	inline void clear() {for (int i=0; i< m_size; ++i) (*this)[i] = Xine::MediaSource();}
//	inline Xine::MediaSource &operator[] (int i) {return QList<Xine::MediaSource>::operator[](i);}
//	inline const Xine::MediaSource &operator[] (int i) const {
//		return QList<Xine::MediaSource>::operator[](i);}
//	inline RecentStack &operator = (const QList<Xine::MediaSource> &other) {
//		QList<Xine::MediaSource>::operator=(other); checkSize(); return *this;}
//	inline Xine::MediaSource value(int i) const {return QList<Xine::MediaSource>::value(i);}
//	inline const QList<Xine::MediaSource> &toList() const {return *this;}
//private:
//	inline void checkSize() {
//		while(size() != m_size) {if (size() > m_size) pop_back(); else push_back(Xine::MediaSource());}}
//	int m_size;
//};
//
//class RecentInfo : public QObject {
//	Q_OBJECT
//public:
//	static const int DefaultRememberCount = 5;
//	static RecentInfo *get() {static RecentInfo obj; return &obj;}
//	~RecentInfo();
//	void setLastPlayList(const Xine::PlayList &list);
//	const Xine::PlayList &lastPlayList() const;
//	void setLastSource(const Xine::MediaSource &source);
//	const Xine::MediaSource &lastSource() const;
//	const RecentStack &sources() const;
//	Xine::MediaSource source(int index) const;
//	void clearStack();
//	void setRememberCount(int count);
//	void stackSource(const Xine::MediaSource &source);
//	void load();
//	void save() const;
//	void setStopped(const Xine::MediaSource &source, qint64 time);
//	void setFinished(const Xine::MediaSource &source);
//	qint64 stoppedTime(const Xine::MediaSource &source) const;
//signals:
//	void sourcesChanged(const RecentStack &stack);
//	void rememberCountChanged(int count);
//private:
//	struct Data;
//	friend struct Data;
//	Data *d;
//	RecentInfo();
//};
//
//#endif
