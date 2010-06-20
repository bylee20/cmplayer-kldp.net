#ifndef HISTORY_HPP
#define HISTORY_HPP

#include "mrl.hpp"
#include <QtCore/QObject>

class PlayEngine;

class History : public QObject {
	Q_OBJECT
public:
	static const int DefaultMaximumCount = 100;
	History(const PlayEngine *engine, QObject *parent);
	~History();
//	static History &get();
private:
	struct Item;
	struct Data;
	Data *d;
};

#endif // HISTORY_HPP
