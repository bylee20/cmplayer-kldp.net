#include "mrl.h"
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>

namespace Core {

Mrl::Mrl(const QString &location) {
	static const QRegExp rx("^\\w+://");
	QUrl &url = static_cast<QUrl&>(*this);
	if (location.contains(rx))
		url = QUrl(location);
	else
		url = fromLocalFile(QFileInfo(location).absoluteFilePath());
}

bool Mrl::isPlaylist() const {
	return path().right(4).toLower() == ".pls";
}

QString Mrl::fileName() const {
	return QFileInfo(path()).fileName();
}

}
