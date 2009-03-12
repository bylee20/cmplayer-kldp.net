#include "subtitle.h"
#include "subtitle_parser.h"
#include <QtCore/QFileInfo>

namespace Core {

Subtitle::Subtitle(const QString &filePath)
: d(new Data) {
	d->filePath = filePath;
	updateName();
}

Subtitle::Subtitle(const Subtitle &other)
: QMap<qint64, QString>(other), d(other.d) {}

Subtitle::~Subtitle() {}

Subtitle &Subtitle::unite(const Subtitle &other) {
	if (this == &other || other.isEmpty())
		return *this;
	else if (isEmpty()) {
		QMap<qint64, QString>::operator=(other);
		return *this;
	}
	SubtitleIterator it1(*this);
	SubtitleIterator it2(other);
	qlonglong t3 = -1;
	while(it1.hasNext()) {
		qlonglong t1 = it1.next().key();
		qlonglong t2 = it1.hasNext() ? it1.peekNext().key() : -1;
		if (t3 != -1 && it2.hasPrevious())
			(*this)[t1] = it1.value() + "<br>" + it2.peekPrevious().value();
		while(it2.hasNext()) {
			t3 = it2.next().key();
			if (t2 == -1) {
				(*this)[t3] = it1.value() + "<br>" + it2.value();
			} else if (t3 >= t2) {
				it2.previous();
				break;
			} else if (t3 == t1) {
				(*this)[t1] = it1.value() + "<br>" + it2.value();
			} else if (t3 > t1)
				(*this)[t3] = it1.value() + "<br>" + it2.value();
			else if (t3 < t1)
				(*this)[t3] = it2.value();
		}
	}
	return *this;
}

void Subtitle::updateName() {
	if (d->lang.language().isEmpty())
		d->name = QFileInfo(d->filePath).fileName();
	else
		d->name = QFileInfo(d->filePath).fileName() + " (" + d->lang.language() + ')';
}

QString Subtitle::filePath() const {
	return d->filePath;
}

void Subtitle::setLanguage(const Language &lang) {
	d->lang = lang;
	updateName();
}

const Subtitle::Language &Subtitle::language() const {
	return d->lang;
}

bool Subtitle::parse(const QString &file, QList<Subtitle> *subs, const QString &enc) {
	Parser *parser = Parser::create(QFileInfo(file).suffix());
	bool result = false;
	if (parser) {
		parser->setEncoding(enc);
		result = parser->parse(file, subs);
		if (!result) {
			subs->clear();
			Subtitle sub(file);
			subs->append(sub);
		}
	}
	delete parser;
	return result;
}

bool Subtitle::save(const QString &file, const Subtitle &sub, const QString &enc) {
	Parser *parser = Parser::create(QFileInfo(file).suffix());
	bool result = false;
	if (parser) {
		parser->setEncoding(enc);
		result = parser->save(file, sub);
	}
	delete parser;
	return result;
}

}
