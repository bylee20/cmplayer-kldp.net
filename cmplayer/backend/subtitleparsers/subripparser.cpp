#include "subripparser.h"
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QTime>

namespace Backend {

namespace SubtitleParsers {

static const QTime zero;

SubRipParser::SubRipParser()
: AbstractParser() {}

bool SubRipParser::parse(const QString &file, QList<Subtitle> *subs) {
	QFile dev(file);
	if (!dev.open(QFile::ReadOnly))
		return false;

	setDevice(&dev);
	QTextStream &in = textStream();

	subs->clear();
	QString line;
	subs->append(Subtitle(file));
	Subtitle &sub = (*subs)[0];
	while (!in.atEnd()) {
		line = in.readLine().trimmed();
		static QRegExp rxNum("^(\\d+)$");
		static QRegExp rxTime("^(\\d\\d):(\\d\\d):(\\d\\d),(\\d\\d\\d) --> "
				"(\\d\\d):(\\d\\d):(\\d\\d),(\\d\\d\\d)$");
		if (rxNum.indexIn(line) == -1)
			continue;
		qlonglong start = -1;
		qlonglong end = -1;
		QString text;
		int index = -1;
		while (!in.atEnd()) {
			line = in.readLine().trimmed();
			if ((index = rxTime.indexIn(line)) != -1)
				break;
		}
		if (index == -1)
			return false;
		start = zero.msecsTo(QTime(rxTime.cap(1).toInt(), rxTime.cap(2).toInt(),
									rxTime.cap(3).toInt(), rxTime.cap(4).toInt()));
		end = zero.msecsTo(QTime(rxTime.cap(5).toInt(), rxTime.cap(6).toInt(),
								rxTime.cap(7).toInt(), rxTime.cap(8).toInt()));
		while (!in.atEnd()) {
			line = in.readLine().trimmed();
			if (line.isEmpty())
				break;
			text += line + '\n';
		}
		if (text.isEmpty())
			return false;
		text.remove(text.length()-1, 1);
		text.replace('\n', "<BR>");
		sub.insert(start, text);
		sub.insert(end, QString());
	}
	dev.close();
	return true;
}

bool SubRipParser::save(const QString &file, const Subtitle &sub) const {
	QFile dev(file);
	if (!dev.open(QFile::WriteOnly | QFile::Truncate)) {
		return false;
	}

	setDevice(&dev);
	QTextStream &out = textStream();

	int nth = 0;
	SubtitleIterator it(sub);
	while(it.hasNext()) {
		it.next();
		out << (++nth) << endl;
		out << zero.addMSecs(it.key()).toString("hh:mm:ss,zzz") << " --> ";
		const bool hasNext = it.hasNext();
		out << zero.addMSecs(hasNext ? it.key()*2 : it.peekNext().key()).toString("hh:mm:ss,zzz")
				<< endl << QString(it.value()).replace("<BR>", "\n", Qt::CaseInsensitive)
				<< endl << endl;
		if (hasNext && it.peekNext().value().isEmpty())
			it.next();
	}
	dev.close();
	return true;
}

}

}
