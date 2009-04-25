#ifndef RECENTPLAYEDWIDGET_H
#define RECENTPLAYEDWIDGET_H

#include <QWidget>
#include <core/mediasource.h>

class VideoPlayer;			class QTreeWidgetItem;

namespace Core {class MediaSource;}

class RecentPlayedWidget : public QWidget {
	Q_OBJECT
public:
	RecentPlayedWidget(VideoPlayer *player, QWidget *parent = 0);
	~RecentPlayedWidget();
	void save() const;
	void load();
signals:
	void openRequested(const Core::Mrl &mrl);
private slots:
	void slotStarted();
	void slotItemDoubleClicked(QTreeWidgetItem *item);
	void updateSize();
private:
	QTreeWidgetItem *makeItem(const Core::Mrl &mrl);
	struct Data;
	Data *d;
};

#endif
