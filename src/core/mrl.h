#ifndef CORE_MRL_H
#define CORE_MRL_H

#include <QtCore/QUrl>

namespace Core {

class Mrl : public QUrl {
public:
	Mrl() {}
	Mrl(const QString &location);
	Mrl(const QUrl &url): QUrl(url) {}
	bool isLocalFile() const {return scheme().toLower() == "file";}
	QString fileName() const;
	QString location() const {return isLocalFile() ? toLocalFile() : toString();}
	bool isPlaylist() const;
	static Mrl fromLocalFile(const QString &file) {return QUrl::fromLocalFile(file);}
};

}

#endif // MRL_H
