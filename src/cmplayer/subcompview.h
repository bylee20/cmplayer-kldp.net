#ifndef SUBCOMPVIEW_H
#define SUBCOMPVIEW_H

#include <QtGui/QWidget>
#include <core/subtitle.h>

class QTreeWidgetItem;

class SubCompView : public QWidget {
	Q_OBJECT
public:
	enum Column {Text = 2, Start = 0, End = 1};
	SubCompView(QWidget *parent);
	void setComponent(const Core::Subtitle::Component &comp);
	~SubCompView();
	void setCurrent(int time, double frameRate, bool scroll);
	void setStartEndVisible(bool visible);
private slots:
	void slotEntered(QTreeWidgetItem *item, int column);
	void slotTextSelected();
private:
	typedef Core::Subtitle::Component::const_iterator CompConstIt;
	struct Data;
	Data *d;
};

#endif // SUBCOMPVIEW_H
