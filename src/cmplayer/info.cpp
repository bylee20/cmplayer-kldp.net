#include "info.hpp"
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringBuilder>
#include <QtCore/QDebug>

typedef QLatin1String _LS;
typedef QLatin1Char _LC;

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
	m_videoExt << _LS("asf") << _LS("avi") << _LS("dvix") << _LS("flv") << _LS("mkv") << _LS("mov")
			<< _LS("mp4") << _LS("mpeg") << _LS("mpg") << _LS("vob")
			<< _LS("ogg") << _LS("ogm") << _LS("qt") << _LS("rm") << _LS("rmvb") << _LS("wmv");
	m_audioExt << _LS("mp3") << _LS("ogg") << _LS("ra") << _LS("wav") << _LS("wma");
	m_subExt << _LS("smi") << _LS("srt") << _LS("sub") << _LS("txt");
	m_plExt << _LS("pls") << _LS("m3u");
}

Info::~Info() {}

QString Info::ExtList::toFilter() const {
	QString filter;
	for (QStringList::const_iterator it = begin(); it != end(); ++it)
		filter += _LS("*.") % *it % _LC(' ');
	const int size = filter.size();
	if (size) {
		filter.remove(size-1, 1);
		return _LC('(') % filter % _LC(')');
	} else
		return QString();
}

QStringList Info::ExtList::toNameFilter() const {
	QStringList nameFilter;
	for (QStringList::const_iterator it = begin(); it != end(); ++it)
		nameFilter.append(_LS("*.") % *it);
	return nameFilter;
}

QString Info::mediaExtFilter() {
	return videoExtFilter() % _LS(";;") % audioExtFilter() % _LS(";;") % allExtFilter();
}

QString Info::allExtFilter() {
	return tr("All Files") % _LS(" (*.*)");
}

QString Info::videoExtFilter() {
	return tr("Video Files") % QLatin1Char(' ') % videoExt().toFilter();
}

QString Info::audioExtFilter() {
	return tr("Audio Files") % QLatin1Char(' ') % audioExt().toFilter();
}

QString Info::subtitleExtFilter() {
	return tr("Subtitle Files") % QLatin1Char(' ') % subtitleExt().toFilter();
}

const char *Info::pluginPath() {
	static QByteArray path;
	if (!path.isEmpty())
		return path.constData();
	path = qgetenv("CMPLAYER_PLUGIN_PATH");
	if (!path.isEmpty() && QDir(QString::fromLocal8Bit(path.data())).exists())
		return path.constData();
#ifdef CMPLAYER_VLC_PLUGINS_PATH
	path = CMPLAYER_VLC_PLUGINS_PATH;
	if (!path.isEmpty() && QDir(QString::fromLocal8Bit(path.data())).exists())
		return path.constData();
#endif
	path = QCoreApplication::applicationDirPath().toLocal8Bit();
	if (path.isEmpty())
		path = "./vlc-plugins";
	else
		path += "/vlc-plugins";
	return path.constData();
}

//const char *Info::skinPath() {
//	static QByteArray path;
//	if (!path.isEmpty())
//		return path.constData();
//	path = qgetenv("CMPLAYER_VLC_PLUGIN_PATH");
//	if (!path.isEmpty() && QDir(QString::fromLocal8Bit(path.data())).exists())
//		return path.constData();
//#ifdef CMPLAYER_VLC_PLUGIN_PATH
//	path = CMPLAYER_VLC_PLUGIN_PATH;
//	if (!path.isEmpty() && QDir(QString::fromLocal8Bit(path.data())).exists())
//		return path.constData();
//#endif
//	path = "./plugin";
//	return path.constData();
//}

