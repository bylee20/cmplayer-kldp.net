#ifndef CORE_DOWNLOADER_H
#define CORE_DOWNLOADER_H

#include <QtCore/QObject>

class QUrl;					class QFile;

namespace Core {

class Downloader : public QObject {
	Q_OBJECT
public:
	Downloader();
	~Downloader();
	bool download(const QUrl &url, QFile *file, int timeout = -1);
	static bool get(const QUrl &url, QFile *file, int timeout = -1) {
		return Downloader().download(url, file, timeout);
	}
private slots:
	void slotTimeout();
	void slotHttpFinished(int id);
private:
	struct Data;
	Data *d;
};

}

#endif
