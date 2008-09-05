#include "informations.h"
#include "xineengine.h"
#include <QRegExp>
#include <xine.h>

namespace Xine {

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
	QString cv, rv;
	Extensions ves, aes, ses;
	QString privatePath;
};

Informations::Informations()
: d(new Data) {
	d->cv = XINE_VERSION;
	d->rv = xine_get_version_string();
	d->privatePath = ".";
	d->ves << "asf" << "avi" << "dvix" << "mkv" << "mov" << "mp4" << "mpeg" << "mpg"
		<< "vob" << "ogg" << "ogm"<< "qt" << "rm" << "wmv";
	d->aes << "mp3" << "ogg" << "ra" << "wav" << "wma";
	d->ses << "smi" << "srt";
	videoOutputs();
}

void Informations::setPrivatePath(const QString &path) {
	d->privatePath = path;
}

const QString &Informations::compileVersion() const {
	return d->cv;
}

const QString &Informations::runtimeVersion() const {
	return d->rv;
}

const QString &Informations::privatePath() const {
	return d->privatePath;
}

const QStringList &Informations::videoOutputs() const {
	if (d->vos.isEmpty()) {
		d->vos.append("auto");
		const char *const *vos = xine_list_video_output_plugins(XineEngine::get()->xine());
		for (int i=0; vos[i]; ++i)
			d->vos.append(vos[i]);
	}
	return d->vos;
}

const QStringList &Informations::audioOutputs() const {
	if (d->aos.isEmpty()) {
		d->aos.append("auto");
		const char *const *aos = xine_list_audio_output_plugins(XineEngine::get()->xine());
		for (int i=0; aos[i]; ++i)
			d->aos.append(aos[i]);
	}
	return d->aos;
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
