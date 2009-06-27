#ifndef CORE_MRL_H
#define CORE_MRL_H

#include <QtCore/QUrl>

namespace Core {

class Mrl {
public:
	Mrl() {}
	Mrl(const Mrl &mrl): m_url(mrl.m_url) {}
	Mrl(const QString &location);
	Mrl(const QUrl &url): m_url(url) {}
	bool operator == (const Mrl &rhs) const {return m_url == rhs.m_url;}
	bool operator != (const Mrl &rhs) const {return m_url != rhs.m_url;}
	QString scheme() const {return m_url.scheme().toLower();}
	QString toLocalFile() const {return m_url.toLocalFile();}
	QString toString() const {return m_url.toString();}
	bool isLocalFile() const {return scheme().toLower() == "file";}
	bool isEmpty() const {return m_url.isEmpty();}
	QString fileName() const;
	QString location() const {return isLocalFile() ? toLocalFile() : toString();}
	bool isPlaylist() const;
	bool isValid() const {return m_url.isValid();}
	const QUrl &url() const {return m_url;}
	static Mrl fromLocalFile(const QString &file) {return QUrl::fromLocalFile(file);}
private:
	QUrl m_url;
};

}

#endif // MRL_H
