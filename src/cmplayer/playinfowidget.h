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
	void showMessage(const QString &msg, int time = 3000);
	void setCurrentSource(const Core::MediaSource &source);
	void setState(Core::State state);
	void setDuration(int duration);
	void setPlayTime(int time);
	void setTrackNumber(int nth, int total);
private slots:
	void hideMessage();
private:
	static const int Info = 0;
	static const int Msg = 1;
	struct Data;
	Data *d;
};

#endif
