#ifndef BACKEND_FILEREQUESTER_H
#define BACKEND_FILEREQUESTER_H

#include <QtGui/QWidget>

namespace Backend {

class FileRequester : public QWidget {
	Q_OBJECT
public:
	FileRequester(const QString &fileName, QWidget *parent = 0);
	~FileRequester();
	bool isValidFile() const;
	QString filePath() const;
signals:
	void validityChanged(bool valid);
private slots:
	void find();
	void open();
	void checkFileExists(const QString &filePath);
private:
	struct Data;
	Data *d;
};

}

#endif
