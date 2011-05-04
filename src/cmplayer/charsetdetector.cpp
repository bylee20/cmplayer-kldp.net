#include "charsetdetector.hpp"
#include <chardet.h>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QFile>

struct CharsetDetector::Data {
	DetectObj *obj;
	bool detected;
};

CharsetDetector::CharsetDetector(const QByteArray &data)
: d(new Data) {
	d->obj = detect_obj_init();
	d->detected = (::detect(data.data(), &d->obj) == CHARDET_SUCCESS);
}

CharsetDetector::~CharsetDetector() {
	detect_obj_free(&d->obj);
	delete d;
}

bool CharsetDetector::isDetected() const {
	return d->detected;
}

QString CharsetDetector::encoding() const {
	if (d->detected) {
		const QString enc(d->obj->encoding);
		if (enc == "EUC-KR")
			return QString("CP949");
		return enc;
	}
	return QString();
}

double CharsetDetector::accuracy() const {
	return d->detected ? d->obj->confidence : 0.0;
}


QString CharsetDetector::detect(const QByteArray &data, double accuracy) {
	CharsetDetector chardet(data);
	if (chardet.isDetected() && chardet.accuracy() > accuracy)
		return chardet.encoding();
	return QString();
}

QString CharsetDetector::detect(const QString &fileName, double accuracy, int size) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return QString();
	return detect(file.read(size), accuracy);
}
