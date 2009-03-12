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
