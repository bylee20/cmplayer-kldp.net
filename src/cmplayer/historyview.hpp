#ifndef HISTORYVIEW_HPP
#define HISTORYVIEW_HPP

#include <QtGui/QTreeWidget>
#include "global.hpp"
#include "mrl.hpp"

class PlayEngine;

class HistoryView : public QTreeWidget {
	Q_OBJECT
public:
	HistoryView(PlayEngine *engine, QWidget *parent);
	~HistoryView();
	QList<Mrl> top(int count = 10) const;
	int stoppedTime(const Mrl &mrl) const;
	void save() const;
signals:
	void historyChanged();
private slots:
	void handleStopped(Mrl mrl, int time, int duration);
	void handleFinished(Mrl mrl);
	void handleStateChanged(MediaState state, MediaState old);
	void play(QTreeWidgetItem *treeItem);
	void clearAll();
	void showContextMenu();
	void erase();
private:
	void load();
	int findIndex(const Mrl &mrl) const;
	struct Item;
	Item *item(int index) const;
	Item *findItem(const Mrl &mrl) const {return item(findIndex(mrl));}
	struct Data;
	Data *d;
};

#endif // HISTORYVIEW_HPP
