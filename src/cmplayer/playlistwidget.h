#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QtGui/QWidget>

class QModelIndex;			class PlaylistModel;

class PlaylistWidget : public QWidget {
	Q_OBJECT
public:
	PlaylistWidget(PlaylistModel *model, QWidget *parent = 0);
	~PlaylistWidget();
	void retranslateUi();
	int controlHeight() const;
	QWidget *buttons();
public slots:
	void checkRoot(bool checked);
	void setAutoShutdown(bool shut);
	void adjustCellSize();
private slots:
	void add();
	void erase();
	void up();
	void down();
	void checkShutdown();
	void open();
	void save();
	void slotDblClicked(const QModelIndex &index);
private:
	void move(bool up);
	class ShutdownDialog;
	struct Data;
	Data *d;
};

#endif
