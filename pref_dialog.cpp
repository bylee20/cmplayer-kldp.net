#include "pref_dialog.hpp"
#include "ui_pref_dialog.h"

struct Pref::Dialog::Data {

};

Pref::Dialog::Dialog(QWidget *parent)
: QDialog(parent), d(new Data) {
}

Pref::Dialog::~Dialog() {
	delete d;
}


