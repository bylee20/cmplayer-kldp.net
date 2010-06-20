#ifndef RECENTINFO_HPP
#define RECENTINFO_HPP

#include <QtCore/QObject>
#include "mrl.hpp"

class RecentInfo : public QObject {
	Q_OBJECT
public:
	~RecentInfo();
	QList<Mrl> open() const;
	int stoppedTime(const Mrl &mrl);
	static RecentInfo &get();
private:
	friend class HistoryView;
	void setStopped(const Mrl &mrl, int time);
	void setFinished(const Mrl &mrl);
	RecentInfo();
	struct Data;
	Data *d;
};

#endif // RECENTINFO_HPP
