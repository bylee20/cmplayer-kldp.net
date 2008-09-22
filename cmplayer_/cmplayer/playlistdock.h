#ifndef PLAYLISTDOCK_H
#define PLAYLISTDOCK_H

#include <QDockWidget>

class QModelIndex;				class PlayListModel;

class PlayListDock : public QDockWidget {
	Q_OBJECT
public:
	PlayListDock(PlayListModel *model, QWidget *parent = 0);
	~PlayListDock();
public slots:
	void checkRoot(bool checked);
	void setAutoShutdown(bool shut);
protected:
	void showEvent(QShowEvent *event);
private slots:
	void add();
	void erase();
	void up();
	void down();
	void checkShutdown();
	void open();
	void save();
	void slotActivated(const QModelIndex &index);
	void adjustCellSize();
private:
	struct Data;
	friend struct Data;
	Data *d;
};

#endif
