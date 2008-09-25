#ifndef ENCODINGFILEDIALOG_H
#define ENCODINGFILEDIALOG_H

#include <QtGui/QFileDialog>

class EncodingFileDialog : public QFileDialog {
public:
	EncodingFileDialog(QWidget *parent = 0);
	~EncodingFileDialog();
	void setEncoding(const QString &encoding);
	QString encoding() const;
private:
	struct Data;
	Data *d;
};

#endif
