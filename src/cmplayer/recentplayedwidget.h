#ifndef RECENTPLAYEDWIDGET_H
#define RECENTPLAYEDWIDGET_H

#include <QWidget>
#include <core/mediasource.h>

class VideoPlayer;			class QTreeWidgetItem;

namespace Core {class MediaSource;}

class RecentPlayedWidget : public QWidget {
	Q_OBJECT
public:
// 	class Model;
// 	class Item;
	RecentPlayedWidget(VideoPlayer *player, QWidget *parent = 0);
	~RecentPlayedWidget();
	void save() const;
	void load();
signals:
	void openRequested(const QUrl &url);
private slots:
	void slotStarted();
	void slotItemDoubleClicked(QTreeWidgetItem *item);
	void updateSize();
private:
	QTreeWidgetItem *makeItem(const QUrl &url);
	struct Data;
	Data *d;
};

// class RecentPlayedWidget::Model : public QWidget {
// 
// };
// 
// class RecentPlayedWidget::Item {
// public:
// 	Item(const QUrl &url, Item *parent = 0) {
// 		m_parent = parent;
// 		url = url;
// 	}
// 	~Item() {
// 		qDeleteAll(m_children);
// 	}
// 	void append(Item *item) {
// 		m_children.append(item);
// 	}
// 	Item *child(int row) {
// 		return m_children.value(row);
// 	}
// 	int childCount() const {
// 		return m_children.size();
// 	}
// 	int columnCount() const {
// 		return 2;
// 	}
// 	int row() const {
// 		return m_parent ? m_parent->children.indexOf(this) : 0;
// 	}
// 	QUrl url() const {return m_url;}
// 	Item *parent() {return m_parent;}
// private:
// 	QList<Item*> m_children;
// 	QUrl m_url;
// 	Item *m_parent;
// };

#endif
