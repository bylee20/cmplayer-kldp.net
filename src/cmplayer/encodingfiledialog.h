#ifndef ENCODINGFILEDIALOG_H
#define ENCODINGFILEDIALOG_H

#include <QtGui/QFileDialog>

class EncodingComboBox;

class EncodingFileDialog : public QFileDialog {
public:
	EncodingFileDialog(QWidget *parent = 0, const QString &caption = QString::null
			, const QString &directory = QString::null
			, const QString &filter = QString::null
			, const QString &encoding = QString::null);
	void setEncoding(const QString &encoding);
	QString encoding() const;
private:
	EncodingComboBox *combo;
};

#endif
