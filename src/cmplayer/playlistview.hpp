#ifndef PLAYLISTVIEW_HPP
#define PLAYLISTVIEW_HPP

#include <QtGui/QWidget>

class PlayEngine;		class Mrl;
class Playlist;			class QModelIndex;

class PlaylistView : public QWidget {
	Q_OBJECT
public:
	PlaylistView(PlayEngine *engine, QWidget *parent);
	~PlaylistView();
	void load(const Mrl &mrl, const QString &enc = QString());
	const Playlist &playlist() const;
	void setPlaylist(const Playlist &list);
	void play(const Mrl &mrl);
	void append(const Playlist &list);
public slots:
	void toggleVisibility() {setVisible(!isVisible());}
	void playNext();
	void playPrevious();
signals:
	void finished();
private slots:
	void updateCurrentMrl(const Mrl &mrl);
	void handleFinished();
	void showContextMenu(const QPoint &pos);
	void addFile();
	void addUrl();
	void erase();
	void up();
	void down();
	void clear();
	void open();
	void save();
	void handleDoubleClick(const QModelIndex &index);
private:
	void move(bool up);
	void setCurrentIndex(int idx);
	class Item;
	class Table;
	struct Data;
	Data *d;
};

#endif // PLAYLISTVIEW_HPP
