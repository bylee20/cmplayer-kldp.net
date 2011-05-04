#ifndef RECENTINFO_HPP
#define RECENTINFO_HPP

#include <QtCore/QObject>
#include "mrl.hpp"

class Playlist;		class QDateTime;

class RecentInfo : public QObject {
	Q_OBJECT
public:
	static void init();
	static void fin();
	~RecentInfo();
	QList<Mrl> openList() const;
	int stoppedTime(const Mrl &mrl) const;
	QDateTime stoppedDate(const Mrl &mrl)const ;
	static RecentInfo &get() {Q_ASSERT(obj != 0); return *obj;}
	void save() const;
	void load();
	void stack(const Mrl &mrl);
	void setLastPlaylist(const Playlist &list);
	void setLastMrl(const Mrl &mrl);
	Mrl lastMrl() const;
	Playlist lastPlaylist() const;
public slots:
	void clear();
signals:
	void openListChanged(const QList<Mrl> &list);
private:
	friend class HistoryView;
	void setStopped(const Mrl &mrl, int time, const QDateTime &date);
	void setFinished(const Mrl &mrl);
	RecentInfo();
	struct Data;
	Data *d;
	static RecentInfo *obj;
};

#endif // RECENTINFO_HPP
