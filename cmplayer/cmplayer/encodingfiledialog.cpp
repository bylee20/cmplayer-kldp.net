#include "encodingfiledialog.h"
#include "encodingcombobox.h"
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtCore/QDebug>

struct EncodingFileDialog::Data {
	EncodingComboBox *combo;
};

EncodingFileDialog::EncodingFileDialog(QWidget *parent)
: QFileDialog(parent) {
	d = new Data;
	QLabel *label = new QLabel("Encoding:", this);
	d->combo = new EncodingComboBox(this);
	QLayout *l = layout();
	QGridLayout *grid = qobject_cast<QGridLayout*>(l);
	if (grid) {
		const int row = grid->rowCount();
		grid->addWidget(label, row, 0, 1, 1);
		grid->addWidget(d->combo, row, 1, 1, grid->columnCount()-1);
	}
}

EncodingFileDialog::~EncodingFileDialog() {
	delete d;
}

void EncodingFileDialog::setEncoding(const QString &encoding) {
	d->combo->setEncoding(encoding);
}

QString EncodingFileDialog::encoding() const {
	return d->combo->encoding();
}
