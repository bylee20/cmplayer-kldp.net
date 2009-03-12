#ifndef PLAYINFOWIDGET_H
#define PLAYINFOWIDGET_H

#include <QWidget>
#include <core/namespace.h>

namespace Core {class MediaSource;}

class PlayInfoWidget : public QWidget {
	Q_OBJECT
public:
	PlayInfoWidget(QWidget *parent = 0);
	~PlayInfoWidget();
public slots:
	void setCurrentSource(const Core::MediaSource &source);
	void setState(Core::State state);
	void setDuration(int duration);
	void setPlayTime(int time);
	void setTrackNumber(int nth, int total);
private:
	struct Data;
	Data *d;
};

#endif
