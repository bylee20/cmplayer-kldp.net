#ifndef PREF_DIALOG_HPP
#define PREF_DIALOG_HPP

#include "pref.hpp"
#include <QtGui/QDialog>

class Pref::Dialog : public QDialog {
	Q_OBJECT
public:
	Dialog(QWidget *parent = 0);
	~Dialog();

private:
//	void updatePref();
	struct Data;
	Data *d;
};

#endif // PREF_DIALOG_HPP
