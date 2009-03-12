#include "translator.h"
#include <QtCore/QTranslator>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QLocale>

struct Translator::Data {
	QTranslator trans;
	QString path;
};

Translator::Translator()
: d(new Data) {
	qApp->installTranslator(&d->trans);
	d->path = QString::fromLocal8Bit(qgetenv("CMPLAYER_TRANSLATION_PATH"));
	if (d->path.isEmpty())
		d->path = CMPLAYER_TRANSLATION_DIR"";
}

Translator::~Translator() {
	delete d;
}

Translator &Translator::get() {
	static Translator self;
	return self;
}

bool Translator::load(const QString &locale) {
	QString loc = locale.isEmpty() ? QLocale::system().name() : locale;
	const QString file = "cmplayer_" + loc;
	Translator::Data *d = get().d;
	return d->trans.load(file, d->path);
}

bool Translator::load(UiLanguage lang) {
	QString locale;
	switch (lang) {
	case English:
		locale = "en";
		break;
	case Korean:
		locale = "ko";
		break;
	default:
		break;
	}
	return load(locale);
}