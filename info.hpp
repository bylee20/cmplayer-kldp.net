#ifndef INFO_HPP
#define INFO_HPP

#include <QtCore/QStringList>
#include <QtCore/QSettings>

class Info {
public:
	class ExtensionList : public QStringList {
	public:
		ExtensionList() {}
		ExtensionList(const QStringList &other): QStringList(other) {}
		ExtensionList(const ExtensionList &other): QStringList(other) {}
		QString toFilter() const;
		QStringList toNameFilter() const;
	};
	~Info();
	static const QString &privatePath() {return self.m_privPath;}
	static const ExtensionList &videoExtension() {return self.m_videoExt;}
	static const ExtensionList &audioExtension() {return self.m_audioExt;}
	static const ExtensionList &subtitleExtension() {return self.m_subExt;}
	static const ExtensionList &playlistExtension() {return self.m_plExt;}
	static const char *version() {return "0.3.99";}
	static QString mediaExtensionFilter();
private:
	Info();
	ExtensionList m_videoExt, m_audioExt, m_subExt, m_plExt;
	QString m_privPath;
	static Info self;
};

#endif // INFO_HPP
