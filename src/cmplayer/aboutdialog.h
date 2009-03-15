#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>

class AboutDialog : public QDialog {
	Q_OBJECT
public:
	AboutDialog(QWidget *parent = 0);
	~AboutDialog();
private:
	struct Data;
	Data *d;
};

#endif
