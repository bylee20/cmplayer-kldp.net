#ifndef MRL_HPP
#define MRL_HPP

#include <QtCore/QUrl>

class Record;

class Mrl {
public:
	Mrl() {}
	Mrl(const QString &location);
	Mrl(const QUrl &url): m_url(url) {}
	Mrl(const Mrl &other): m_url(other.m_url) {}
	bool operator == (const Mrl &rhs) const {return m_url == rhs.m_url;}
	bool operator != (const Mrl &rhs) const {return !(*this == rhs);}
	Mrl &operator=(const Mrl &other) {
		if (this != &other)
			m_url = other.m_url;
		return *this;
	}
	bool operator < (const Mrl &rhs) const {
		return m_url < rhs.m_url;
	}
	const QUrl &url() const {return m_url;}
	QString toString() const {return m_url.toString();}
	static Mrl fromLocalFile(const QString &file) {return Mrl(QUrl::fromLocalFile(file));}
	QString scheme() const {return m_url.scheme();}
	QString toLocalFile() const {return m_url.toLocalFile();}
	QString fileName() const;
	bool isPlaylist() const;
	bool isLocalFile() const {return m_url.scheme().toLower() == "file";}
	bool isDVD() const {return m_url.scheme().toLower() == "dvd";}
	QString displayName() const;
	bool isEmpty() const {return m_url.isEmpty();}
private:
	QUrl m_url;
};

#endif // MRL_HPP
