#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QtGui/QWidget>
#include <core/namespace.h>

class VideoPlayer;			class QToolButton;

namespace Core {class MediaSource;}

class ControlWidget : public QWidget {
	Q_OBJECT
public:
	ControlWidget(VideoPlayer *player, QWidget *parent = 0);
	~ControlWidget();
	void connectMute(QAction *action);
	void connectPlay(QAction *action);
	void connectStop(QAction *action);
	void connectPrevious(QAction *action);
	void connectNext(QAction *action);
	void connectForward(QAction *action);
	void connectBackward(QAction *action);
	void connectOpenFile(QAction *action);
	void connectOpenUrl(QAction *action);
	void connectFullScreen(QAction *action);
	void connectPlaylist(QAction *action);
// 	void connectPlay(QAction *action);
public slots:
	void showMessage(const QString &msg, int time = 3000);
	void setCurrentSource(const Core::MediaSource &source);
	void setState(Core::State state);
	void setDuration(int duration);
	void setPlayTime(int time);
	void setTrackNumber(int nth, int total);
private slots:
	void hideMessage();
	void togglePlaylist(bool visible);
	void toggleFullScreen(bool full);
private:
	void retranslateUi();
	void changeEvent(QEvent *event);
	static QToolButton *createButton(QWidget *parent = 0, int size = 25);
	void paintEvent(QPaintEvent *event);
	class Bottom;
	class Middle;
	class Top;
	struct Data;
	Data *d;
};

#endif
