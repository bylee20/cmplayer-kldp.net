#include "encodingfiledialog.h"
#include "encodingcombobox.h"
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>

EncodingFileDialog::EncodingFileDialog(QWidget *parent, const QString &caption
		, const QString &directory, const QString &filter, const QString &encoding)
: QFileDialog(parent, caption, directory, filter), combo(new EncodingComboBox(this)) {
	QGridLayout *grid = qobject_cast<QGridLayout*>(layout());
	if (grid) {
		const int row = grid->rowCount();
		grid->addWidget(new QLabel("Encoding:", this), row, 0, 1, 1);
		grid->addWidget(combo, row, 1, 1, grid->columnCount()-1);
	}
	if (!encoding.isEmpty())
		setEncoding(encoding);
}

void EncodingFileDialog::setEncoding(const QString &encoding) {
	combo->setEncoding(encoding);
}

QString EncodingFileDialog::encoding() const {
	return combo->encoding();
}

QString EncodingFileDialog::getOpenFileName(QWidget *parent
		, const QString &caption, const QString &dir
		, const QString &filter, QString *enc) {
	const QStringList files = getOpenFileNames(parent
			, caption, dir, filter, enc, ExistingFile);
	return files.isEmpty() ? QString() : files[0];
}

QStringList EncodingFileDialog::getOpenFileNames(QWidget *parent
		, const QString &caption
		, const QString &dir
		, const QString &filter
		, QString *enc, FileMode mode) {
	EncodingFileDialog dlg(parent, caption, dir, filter);
	if (enc && !enc->isEmpty())
		dlg.setEncoding(*enc);
	dlg.setFileMode(mode);
	if (dlg.exec()) {
		if (enc)
			*enc = dlg.encoding();
		return dlg.selectedFiles();
	}
	return QStringList();
}

