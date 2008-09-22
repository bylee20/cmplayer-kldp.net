#ifndef ABREPEATDIALOG_H
#define ABREPEATDIALOG_H

#include <QDialog>

namespace Backend {class ABRepeater;}

class ABRepeatDialog : public QDialog {
	Q_OBJECT
public:
	ABRepeatDialog(Backend::ABRepeater *repeater, QWidget *parent = 0);
	~ABRepeatDialog();
private slots:
	void getAFromTime();
	void getAFromSubtitle();
	void getBFromTime();
	void getBFromSubtitle();
	void start();
private:
	struct Data;
	friend struct Data;
	Data *d;
};

#endif
