#ifndef MRL_HPP
#define MRL_HPP

#include <QtCore/QUrl>

class Mrl {
public:
	Mrl() {}
	Mrl(const QUrl &url): m_url(url) {}
	bool operator == (const Mrl &rhs) const {return m_url == rhs.m_url;}
	bool operator != (const Mrl &rhs) const {return !(*this == rhs);}
	const QUrl &url() const {return m_url;}
	QString toString() const {return m_url.toString();}
	static Mrl fromLocalFile(const QString &file) {return Mrl(QUrl::fromLocalFile(file));}
private:
	QUrl m_url;
};

#endif // MRL_HPP
