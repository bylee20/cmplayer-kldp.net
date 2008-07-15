#include "informations.h"
#include <QRegExp>
#include <QProcess>

namespace MPlayer {

QString Informations::Extensions::toFilter() const {
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

QStringList Informations::Extensions::toNameFilter() const {
	QStringList nameFilter;
	for (QStringList::const_iterator it = begin(); it != end(); ++it)
		nameFilter << "*." + *it;
	return nameFilter;
}

struct Informations::Data {
	mutable QStringList vos, aos;
	mutable QString version;
	Extensions ves, aes, ses;
	QString mplayer, privatePath;
};

Informations::Informations()
: d(new Data) {
	d->mplayer = "mplayer";
	d->privatePath = ".";
	d->ves << "asf" << "avi" << "dvix" << "mkv" << "mov" << "mp4" << "mpeg" << "mpg"
		<< "vob" << "ogg" << "ogm"<< "qt" << "rm" << "wmv";
	d->aes << "mp3" << "ogg" << "ra" << "wav" << "wma";
	d->ses << "smi" << "srt";
}

void Informations::getInfo() const {
	QProcess proc;
	QStringList args;
	args << "-identify" << "-vo" << "help" << "-ao" << "help";
	proc.start(d->mplayer, args);
	if (!proc.waitForFinished())
		proc.kill();
	static QRegExp rxOuts("(\\s+)(\\S+)(.*)");
	static QRegExp rxVO("^ID_VIDEO_OUTPUTS");
	static QRegExp rxAO("^ID_AUDIO_OUTPUTS");
	static QRegExp rxVer("^MPlayer\\s+(\\S+).*");
	enum What {Nothing = 0, VO = 1, AO = 2};
	What what = Nothing;
	d->vos.clear();
	d->aos.clear();
	while (proc.canReadLine()) {
		QString line = QString::fromLocal8Bit(proc.readLine());
		line.replace("\n", "");
		line.replace("\r", "");
		if (rxVer.indexIn(line) != -1)
			d->version = rxVer.cap(1);
		else if (rxVO.indexIn(line) != -1)
			what = VO;
		else if (rxAO.indexIn(line) != -1)
			what = AO;
		else if (line.isEmpty())
			what = Nothing;
		else if (what == VO) {
			if (rxOuts.indexIn(line) != -1)
				d->vos.append(rxOuts.cap(2));
		} else if (what == AO) {
			if (rxOuts.indexIn(line) != -1)
				d->aos.append(rxOuts.cap(2));
		}
	}
}

void Informations::setMPlayerPath(const QString &path) {
	d->mplayer = path;
	d->version.clear();
	d->vos.clear();
	d->aos.clear();
}

void Informations::setPrivatePath(const QString &path) {
	d->privatePath = path;
}

const QString &Informations::version() const {
	if (d->version.isEmpty())
		getInfo();
	return d->version;
}

const QString &Informations::privatePath() const {
	return d->privatePath;
}

const QString &Informations::mplayerPath() const {
	return d->mplayer;
}

const QStringList &Informations::videoOutputs() const {
	if (d->vos.isEmpty()) getInfo(); return d->vos;
}

const QStringList &Informations::audioOutputs() const {
	if (d->aos.isEmpty()) getInfo(); return d->aos;
}

const Informations::Extensions &Informations::videoExtensions() const {
	return d->ves;
}

const Informations::Extensions &Informations::audioExtensions() const {
	return d->aes;
}

const Informations::Extensions &Informations::subtitleFilter() const {
	return d->ses;
}

bool Informations::isPlayable(const QString ex) const {
	return d->ves.contains(ex) || d->aes.contains(ex);
}

}
