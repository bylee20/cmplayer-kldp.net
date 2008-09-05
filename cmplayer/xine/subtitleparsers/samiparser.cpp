#include "samiparser.h"
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QTextCodec>

namespace Xine {

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
	QString text;
	while(it.hasNext()) {
		it.next();
		text = it.value();
		text.replace("\n", "<br>");
		text.replace("  ", "&nbsp;&nbsp;");
		out << "<SYNC Start=" << it.key() << ">" << text << endl;
	}
	out << "</BODY>" << endl << "</SAMI>" << endl;
	file.close();
	return true;
}

bool SamiParser::parse(const QString &path, QList<Subtitle> *subs) {
	static bool minimal = false;
	static QRegExp rxTag("<\\s*([a-zA-Z]+).*>");
	static QRegExp rxSync("<[sS][yY][nN][cC]\\s+[sS][tT][aA][rR][tT]\\s*=\\s*\"?(\\d+)\"?\\s*>");
	static QRegExp rxClass("<([a-zA-Z]+)\\s+[cC][lL][aA][sS][sS]\\s*=\\s*\"?(.*)\"?.*>");
	static QRegExp rxP("(<\\s*)([pP])(\\s*>|\\s+.*>)");
	static QRegExp rxEnd("<\\s*/([dB][oO][dD][yY]|[sS][aA][mM][iI])\\s*>");
	static QRegExp rxBr("<\\s*[bB][rR]\\s*>");
	static QRegExp rxColor("(\\s+[cC][oO][lL][oO][rR]\\s*=\\s*['\"]?)([0-9a-fA-F]{6}['\"]?)");
	if (!minimal) {
		rxTag.setMinimal(true);
		rxSync.setMinimal(true);
		rxClass.setMinimal(true);
		rxP.setMinimal(true);
		rxEnd.setMinimal(true);
		rxBr.setMinimal(true);
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
		text.replace(rxP, "\\1span\\3");
		text.replace(rxColor, "\\1#\\2");
		(*subs)[idxes[lang]].insert(time, (end == -1 ? text : text.mid(0, end)).trimmed());
	}
	file.close();
	return true;
}

}

}
