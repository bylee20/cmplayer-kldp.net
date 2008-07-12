#include "samiparser.h"
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QTextCodec>

namespace MPlayer {

namespace SubtitleParsers {

SamiParser::SamiParser() : AbstractParser() {}

bool SamiParser::save(const QString& path, const Subtitle &sub) const {
	QFile file(path);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return false;
	setDevice(&file);
	QTextStream &out = textStream();
	out << "<SAMI>" << endl << "<BODY>" << endl;
	SubtitleIterator it(sub);
	while(it.hasNext()) {
		it.next();
		out << "<SYNC Start=" << it.key() << ">" << it.value() << endl;
	}
	out << "</BODY>" << endl << "</SAMI>" << endl;
	file.close();
	return true;
}

bool SamiParser::parse(const QString &path, QList<Subtitle> *subs) {
	static bool minimal = false;
	static QRegExp rxTag("<([a-zA-Z]+)(\\s*)>");
	static QRegExp rxSync("<[sS][yY][nN][cC]\\s+[sS][tT][aA][rR][tT]\\s*=\\s*\"?(\\d+)\"?\\s*>");
	static QRegExp rxClass("<([a-zA-Z]+)\\s+[cC][lL][aA][sS][sS]\\s*=\\s*\"?(.*)\"?.*>");
	static QRegExp rxP("<\\s*[pP](\\s*>|\\s+.*>)");
	static QRegExp rxEnd("<\\s*/([dB][oO][dD][yY]|[sS][aA][mM][iI])\\s*>");
	if (!minimal) {
		rxTag.setMinimal(true);
		rxSync.setMinimal(true);
		rxClass.setMinimal(true);
		rxP.setMinimal(true);
		rxEnd.setMinimal(true);
		minimal = true;
	}
	QFile file(path);
	if (!file.open(QFile::ReadOnly))
		return false;
	setDevice(&file);
	QTextStream &in = textStream();
	QMap<QString, int> idxes;
	subs->clear();
	QString line;
	while (!in.atEnd()) {
		int sync = rxSync.indexIn(line);
		if (sync == -1) {
			line = in.readLine();
			continue;
		}
		qlonglong time = rxSync.cap(1).toInt();
		int end = sync + rxSync.matchedLength();
		QString text;
		sync = rxSync.indexIn(line, end);
		if (sync != -1) {
			text = line.mid(end, sync - end);
			line = line.mid(sync, line.size() - sync);
		} else {
			text = line.mid(end, line.size() - end);
			while (!in.atEnd()) {
				line = in.readLine();
				sync = rxSync.indexIn(line);
				if (sync != -1) {
					text += line.mid(0, sync);
					break;
				} else
					text += line;
			}
		}
		time = (time/minimumInterval())*minimumInterval();
		QString lang;
		if (rxClass.indexIn(text) != -1)
			lang = rxClass.cap(2);
		end = rxEnd.indexIn(text);

		if (!idxes.contains(lang)) {
			int idx = idxes.size();
			idxes.insert(lang, idx);
			subs->append(Subtitle(path));
			Subtitle &sub = (*subs)[idx];
			sub.setLanguage(lang);
		}
		(*subs)[idxes[lang]].insert(time, (end == -1 ? text : text.mid(0, end)).trimmed());
	}
	file.close();
	return true;
}

}

}
