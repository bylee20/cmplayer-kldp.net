#ifndef PREF_BACKEND_H
#define PREF_BACKEND_H

#include <QtCore/QString>

class QSettings;

namespace Pref {

class BackendPref {
public:
	void save(QSettings *set) const;
	void load(QSettings *set);
	void setFileName(const QString &fileName) {m_fileName = fileName;}
	const QString &fileName() const {return m_fileName;}
private:
	QString m_fileName;
};

}

#endif
