#ifndef ABREPEATDIALOG_H
#define ABREPEATDIALOG_H

#include <QtGui/QDialog>

namespace Core {class ABRepeater;}

class ABRepeatDialog : public QDialog {
	Q_OBJECT
public:
	ABRepeatDialog(QWidget *parent = 0);
	~ABRepeatDialog();
	void setRepeater(Core::ABRepeater *repeater);
public slots:
	void quit();
private slots:
	void getAFromTime();
	void getAFromSubtitle();
	void getBFromTime();
	void getBFromSubtitle();
	void start();
	void setRest(int rest);
	void setStopped(bool stopped = true);
private:
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	struct Data;
	friend struct Data;
	Data *d;
};

#endif
