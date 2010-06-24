#ifndef PLAYLISTVIEW_HPP
#define PLAYLISTVIEW_HPP

#include <QtGui/QWidget>

class PlayEngine;		class Mrl;
class Playlist;			class QTableWidgetItem;

class PlaylistView : public QWidget {
	Q_OBJECT
public:
	PlaylistView(PlayEngine *engine, QWidget *parent);
	~PlaylistView();
	void load(const Mrl &mrl);
	const Playlist &playlist() const;
	void setPlaylist(const Playlist &list);
	void play(const Mrl &mrl);
public slots:
	void playNext();
	void playPrevious();
signals:
	void finished();
private slots:
	void updateCurrentMrl(const Mrl &mrl);
	void handleFinished();
	void handleDoubleClick(int row, int column);
	void showContextMenu(const QPoint &pos);
	void add();
	void erase();
	void up();
	void down();
	void clear();
	void open();
	void save();
private:
	bool isValidRow(int row);
	void move(bool up);
	void setCurrentIndex(int idx);
	class Item;
	class Table;
	struct Data;
	Data *d;
};

#endif // PLAYLISTVIEW_HPP
