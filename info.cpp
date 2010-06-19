#include "info.hpp"
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>

Info Info::self;

Info::Info() {
	m_privPath = QString::fromLocal8Bit(qgetenv("CMPLAYER_PRIVATE_PATH"));
	if (m_privPath.isEmpty()) {
		QDir dir = QDir::home();
		if (!dir.exists(".cmplayer"))
			dir.mkdir(".cmplayer");
		dir.cd(".cmplayer");
		m_privPath = dir.absolutePath();
	}
	m_videoExt << "asf" << "avi" << "dvix" << "flv" << "mkv" << "mov"
			<< "mp4" << "mpeg" << "mpg" << "vob"
			<< "ogg" << "ogm"<< "qt" << "rm" << "rmvb" << "wmv";
	m_audioExt << "mp3" << "ogg" << "ra" << "wav" << "wma";
	m_subExt << "smi" << "srt" << "sub" << "txt";
	m_plExt << "pls" << "m3u";
}

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
		nameFilter << ("*." + *it);
	return nameFilter;
}

QString Info::mediaExtensionFilter() {
	static const QString filter = QCoreApplication::translate("Info", "Video Files") +' '
			+ Info::videoExtension().toFilter() + ";;"
			+ QCoreApplication::translate("Info", "Audio Files") + ' '
			+ Info::audioExtension().toFilter() + ";;"
			+ QCoreApplication::translate("Info", "All Files") + ' ' + "(*.*)";
	return filter;
}

