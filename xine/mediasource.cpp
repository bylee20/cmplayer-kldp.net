#include "mediasource.h"
#include <QFileInfo>
#include <QRegExp>
#include <QUrl>
#include <QDebug>
#include <QString>
#include <QTime>
#include <QSize>

namespace Xine {

struct MediaSource::Data : public QSharedData {
	Data(): type(Invalid), discType(NoDisc) {}
	Data(const Data &rhs)
	: QSharedData(rhs), type(rhs.type), url(rhs.url), discType(rhs.discType) {}
	~Data() {}
	Type type;
	QUrl url;
	DiscType discType;
};

MediaSource::MediaSource()
: d(new Data) {}

MediaSource::MediaSource(DiscType dt)
: d(new Data) {
	d->type = Disc;
	d->discType = dt;
	if (dt == DVD)
		d->url.setUrl("dvd://1");
	else if (dt == VCD)
		d->url.setUrl("vcd://1");
	else
		d->type = Invalid;
}

MediaSource::MediaSource(const QString &filePath)
: d(new Data) {
	d->url = QUrl::fromLocalFile(filePath);
	if (!d->url.isEmpty()) {
		d->type = LocalFile;
	}
}

MediaSource::MediaSource(const QUrl &url)
: d(new Data) {
	d->url = url;
	if (!url.isEmpty()) {
		const QString scheme = url.scheme();
		if (scheme == "file")
			d->type = LocalFile;
		else if (scheme == "dvd") {
			d->type = Disc;
			d->discType = DVD;

		} else if (scheme == "vcd") {
			d->type = Disc;
			d->discType = VCD;
		} else
			d->type = Url;
		if (d->type == Disc) {
			static QRegExp rxDigit("^\\d+$");
			if (d->url.authority().indexOf(rxDigit) == -1)
				d->url.setAuthority("1");
		}
	}
}

MediaSource::MediaSource(const MediaSource &rhs)
: d(rhs.d) {
}

MediaSource::~MediaSource() {
}

MediaSource &MediaSource::operator = (const MediaSource &rhs) {
	if (this != &rhs)
		d = rhs.d;
	return *this;
}

bool MediaSource::operator != (const MediaSource &rhs) const {
	return d->type != rhs.d->type || d->url != rhs.d->url;
}

bool MediaSource::operator == (const MediaSource &rhs) const {
	return !operator!=(rhs);
}

bool MediaSource::operator < (const MediaSource &rhs) const {
	return (isLocalFile() ? filePath() : d->url.toString())
			< (rhs.isLocalFile() ? rhs.filePath() : rhs.d->url.toString());
}

QString MediaSource::displayName() const {
	if (d->type == LocalFile)
		return QFileInfo(filePath()).fileName();
	else if (isDisc())
		return QObject::trUtf8("%1 타이틀  %2")
			.arg(d->discType == DVD ? "DVD" : "VCD").arg(titleNumber());
	else
		return d->url.toString();
}

QString MediaSource::filePath() const {
	return d->url.toLocalFile();
}

MediaSource::Type MediaSource::type () const {
	return d->type;
}

bool MediaSource::isValid() const {
	return d->type != Invalid;
}

bool MediaSource::isLocalFile() const {
	return d->type == LocalFile;
}

QUrl MediaSource::url () const {
	return d->type != Invalid ? d->url : QUrl();
}

DiscType MediaSource::discType() const {
	return d->discType;
}

bool MediaSource::isDisc() const {
	return d->type == Disc && d->discType != NoDisc;
}

int MediaSource::titleNumber() const {
	if (!isDisc())
		return -1;
	return d->url.authority().toInt();
}

}
