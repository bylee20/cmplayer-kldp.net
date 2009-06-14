#ifndef SUBVIEWDIALOG_H
#define SUBVIEWDIALOG_H

#include <QtGui/QDialog>

class VideoPlayer;

namespace Core {
class Subtitle;
}

class SubViewDialog : public QDialog {
	Q_OBJECT
public:
	SubViewDialog(VideoPlayer *player, QWidget *parent = 0);
	~SubViewDialog();
public slots:
	void showCurrentSubtitle();
private slots:
	void slotTick(int time);
	void setStartEndVisible(bool visible);
private:
	struct Data;
	Data *d;
};

#endif // SUBVIEWDIALOG_H
