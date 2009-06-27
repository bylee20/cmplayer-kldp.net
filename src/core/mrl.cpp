#include "mrl.h"
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>

namespace Core {

Mrl::Mrl(const QString &location) {
	static const QRegExp rx("^\\w+://");
	if (location.contains(rx))
		m_url = QUrl(location);
	else
		m_url = QUrl::fromLocalFile(QFileInfo(location).absoluteFilePath());
}

bool Mrl::isPlaylist() const {
	return m_url.path().right(4).toLower() == ".pls";
}

QString Mrl::fileName() const {
	return QFileInfo(m_url.path()).fileName();
}

}
