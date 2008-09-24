#ifndef BACKEND_INFO_H
#define BACKEND_INFO_H

#include <QtCore/QStringList>

namespace Backend {

// class MediaSource;

class Info {
public:
	class Extensions : public QStringList {
	public:
		QString toFilter() const {
			QString filter;
			for (QStringList::const_iterator it = begin(); it != end(); ++it)
				filter += "*." + *it + ' ';
			if (!filter.isEmpty()) {
				filter.remove(filter.size() - 1, 1);
				return '(' + filter + ')';
			} else
				return QString();
		}
		QStringList toNameFilter() const {
			QStringList nameFilter;
			for (QStringList::const_iterator it = begin(); it != end(); ++it)
				nameFilter << "*." + *it;
			return nameFilter;
		}
	};
	Info() {}
	~Info() {}
// 	static bool get(MediaSource *soure);
	virtual bool supportsVolumeAmplification() const {return false;}
	virtual double maximumVolumeAmplification() const {return 1.0;}
	virtual bool supportsExpansion() const {return false;}
	virtual bool supportsSeekingByBytes() const {return false;}
	virtual bool supportsSeekingByDrag() const {return false;}
	virtual bool supportsDvdMenu() const {return false;}
	virtual bool supportsColorSubtitle() const {return false;}
	static void setPrivatePath(const QString &path) {m_privatePath = path;}
	static const QString &privatePath() {return m_privatePath;}
	virtual const QString &name() const = 0;
	virtual const QString &description() const = 0;
// 	virtual const QImage &logo() const = 0;
	virtual const QString &compileVersion() const = 0;
	virtual const QString &runtimeVersion() const = 0;
	virtual const QStringList &videoDrivers() const = 0;
	virtual const QStringList &audioDrivers() const = 0;
	virtual const Extensions &videoExtensions() const = 0;
	virtual const Extensions &audioExtensions() const = 0;
	virtual const Extensions &subtitleExtensions() const = 0;
	bool isPlayable(const QString ex) const  {
		return videoExtensions().contains(ex) || audioExtensions().contains(ex);}
private:
	static QString m_privatePath;
};

}

#endif
