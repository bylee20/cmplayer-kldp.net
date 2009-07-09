#ifndef CHECKDIALOG_H
#define CHECKDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>

class CheckDialog : public QDialog {
	Q_OBJECT
public:
	CheckDialog(QWidget *parent = 0
		    , QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Ok);
	~CheckDialog();
	void setButtonBox(QDialogButtonBox::StandardButtons buttons);
	void setLabelText(const QString &text);
	void setCheckBoxText(const QString &text);
	void setChecked(bool checked);
	bool isChecked() const;
public slots:
	int exec();
private slots:
	void slotButtonClicked(QAbstractButton *button);
private:
	struct Data;
	Data *d;
};

#endif
