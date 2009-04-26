#ifndef FAVORITESWIDGET_H
#define FAVORITESWIDGET_H

#include <QtGui/QWidget>

class VideoPlayer;			class QTreeWidgetItem;
class QSettings;

namespace Core {class Mrl;}

class FavoritesWidget : public QWidget {
	Q_OBJECT
private:
	class Item;
public:
	FavoritesWidget(VideoPlayer *player, QWidget *parent = 0);
	~FavoritesWidget();
	void save() const;
	void load();
signals:
	void openRequested(const Core::Mrl &mrl);
private slots:
	void addNew();
	void addFolder();
	void addCurrent();
	void erase();
	void showContext(const QPoint &pos);
	void slotDblClick(QTreeWidgetItem *item);
	void modify(Item *item = 0);
private:
	class ItemDialog;
	class Item;
	class FolderItem;
	class MrlItem;
	void save(QTreeWidgetItem *item, QSettings *set) const;
	void load(QTreeWidgetItem *parent, QSettings *set);
	void addItem(Item *item, bool topLevel);
	struct Data;
	Data *d;
};

#endif
