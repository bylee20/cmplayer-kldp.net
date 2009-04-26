#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QtGui/QDialog>
#include "designedwidgetiface.h"

class PlaylistModel;			class VideoPlayer;
class MainWindow;

class ToolBox : public QDialog, public DesignedWidgetIface {
	Q_OBJECT
public:
	ToolBox(VideoPlayer *player, PlaylistModel *model, MainWindow *mainWindow);
	~ToolBox();
signals:
	void hidingRequested();
	void snapRequested();
private slots:
	void slotStarted();
private:
	QRect boxRect() const {return QRect(3, 3, width()-6, height() - 6);}
	void addPage(QWidget *widget, const QString &name, const QString &iconHolder);
	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	class Frame;
	class ButtonWidget;
	struct Data;
	Data *d;
};

#endif
