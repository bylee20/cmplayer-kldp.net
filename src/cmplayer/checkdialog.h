#ifndef CHECKDIALOG_H
#define CHECKDIALOG_H

#include <QtGui/QDialog>

class CheckDialog : public QDialog {
	Q_OBJECT
public:
	CheckDialog(QWidget *parent = 0);
	~CheckDialog();
	void setLabelText(const QString &text);
	void setCheckBoxText(const QString &text);
	void setChecked(bool checked);
	bool isChecked() const;
private:
	struct Data;
	Data *d;
};

#endif
