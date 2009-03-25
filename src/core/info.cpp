#include "info.h"
#include <QtCore/QDir>
#include <cstdlib>
#include <QtCore/QDebug>

namespace Core {

Info::Data Info::d;

Info::Info() {}
Info::~Info() {}

QString Info::ExtensionList::toFilter() const {
	QString filter;
	for (QStringList::const_iterator it = begin(); it != end(); ++it)
		filter += "*." + *it + ' ';
	const int size = filter.size();
	if (size) {
		filter.remove(size-1, 1);
		return '(' + filter + ')';
	} else
		return QString();
}

QStringList Info::ExtensionList::toNameFilter() const {
	QStringList nameFilter;
	for (QStringList::const_iterator it = begin(); it != end(); ++it)
		nameFilter << "*." + *it;
	return nameFilter;
}

Info::Data::Data() {
	pluginPath = QString::fromLocal8Bit(qgetenv("CMPLAYER_PLUGIN_PATH"));
	privatePath = QString::fromLocal8Bit(qgetenv("CMPLAYER_PRIVATE_PATH"));
	if (pluginPath.isEmpty())
		pluginPath = QString(CMPLAYER_PLUGIN_DIR);
	if (privatePath.isEmpty()) {
		QDir dir = QDir::home();
		if (!dir.exists(".cmplayer"))
			dir.mkdir(".cmplayer");
		dir.cd(".cmplayer");
		privatePath = dir.absolutePath();
	}
	videoExt << "asf" << "avi" << "dvix" << "mkv" << "mov"
			<< "mp4" << "mpeg" << "mpg" << "vob"
			<< "ogg" << "ogm"<< "qt" << "rm" << "wmv";
	audioExt << "mp3" << "ogg" << "ra" << "wav" << "wma";
	subExt << "smi" << "srt" << "sub" << "txt";
}



}
