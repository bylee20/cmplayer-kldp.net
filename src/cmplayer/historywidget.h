#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QtGui/QWidget>
#include <core/mediasource.h>

class VideoPlayer;			class QTreeWidgetItem;

namespace Core {class MediaSource;}

class HistoryWidget : public QWidget {
	Q_OBJECT
public:
	HistoryWidget(VideoPlayer *player, QWidget *parent = 0);
	~HistoryWidget();
	void save() const;
	void load();
signals:
	void openRequested(const Core::Mrl &mrl);
private slots:
	void slotStarted();
	void slotItemDoubleClicked(QTreeWidgetItem *item);
	void updateSize();
private:
	class Item;
	struct Data;
	Data *d;
};

#endif // HISTORYWIDGET_H
