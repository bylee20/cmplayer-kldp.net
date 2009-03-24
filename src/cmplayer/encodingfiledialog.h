#ifndef ENCODINGFILEDIALOG_H
#define ENCODINGFILEDIALOG_H

#include <QtGui/QFileDialog>

class EncodingComboBox;

class EncodingFileDialog : public QFileDialog {
public:
	static QString getOpenFileName(QWidget *parent = 0
			, const QString &caption = QString()
			, const QString &dir = QString()
			, const QString &filter = QString()
			, QString *enc = 0);
	static QStringList getOpenFileNames(QWidget *parent = 0
			, const QString &caption = QString()
			, const QString &dir = QString()
			, const QString &filter = QString()
			, QString *enc = 0, FileMode = ExistingFiles);
private:
	EncodingFileDialog(QWidget *parent = 0
			, const QString &caption = QString::null
			, const QString &directory = QString::null
			, const QString &filter = QString::null
			, const QString &encoding = QString::null);
	void setEncoding(const QString &encoding);
	QString encoding() const;
	EncodingComboBox *combo;
};

#endif
