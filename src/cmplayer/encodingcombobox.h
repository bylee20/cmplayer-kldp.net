#ifndef ENCODINGCOMBOBOX_H
#define ENCODINGCOMBOBOX_H

#include <QtGui/QComboBox>

class EncodingComboBox : public QComboBox {
	Q_OBJECT
public:
	EncodingComboBox(QWidget *parent = 0);
	QString encoding() const;
	void setEncoding(const QString &encoding);
private:
	QStringList enc;
};

#endif
