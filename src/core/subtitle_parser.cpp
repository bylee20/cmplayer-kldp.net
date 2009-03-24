#include "subtitle_parser.h"
#include "utility.h"
#include <QtCore/QTextCodec>
#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QDebug>

namespace Core {

class Subtitle::Parser::TMPlayer : public Subtitle::Parser {
	bool parse(const QString &file, QList<Subtitle> *subs) {
		QFile dev(file);
		if (!dev.open(QFile::ReadOnly))
			return false;
		setDevice(&dev);
		QTextStream &in = stream();

		subs->clear();
		QString line;
		subs->append(Subtitle(file));
		Subtitle &sub = (*subs)[0];
		while (!in.atEnd()) {
			line = in.readLine().trimmed();
			static QRegExp rxLine("^\\s*(\\d?\\d)\\s*:\\s*(\\d\\d)\\s*:\\s*(\\d\\d)\\s*:\\s*(.*)$");
			if (rxLine.indexIn(line) == -1)
				continue;
			const int time = Utility::timeToMSecs(QTime(rxLine.cap(1).toInt()
					, rxLine.cap(2).toInt(), rxLine.cap(3).toInt()));
			QString text = rxLine.cap(4);
			text.replace('|', "<BR>");
			sub.insert(time, text);
		}
		dev.close();
		return true;
	}
};
class Subtitle::Parser::MicroDVD {
};

class Subtitle::Parser::Sami : public Subtitle::Parser {
	bool parse(const QString &path, QList<Subtitle> *subs) {
		static bool minimal = false;
		static QRegExp rxTag("<\\s*([a-zA-Z]+).*>");
		static QRegExp rxSync("<[sS][yY][nN][cC]\\s+[sS][tT][aA][rR][tT]"
				"\\s*=\\s*\"?(\\d+)\"?\\s*>");
		static QRegExp rxClass("<([a-zA-Z]+)\\s+[cC][lL][aA][sS][sS]\\s*=\\s*\"?(\\w+)\"?.*>");
		static QRegExp rxP("(<\\s*)([pP])(\\s*>|\\s+.*>)");
		static QRegExp rxEnd("<\\s*/([dB][oO][dD][yY]|[sS][aA][mM][iI])\\s*>");
		static QRegExp rxBr("<\\s*[bB][rR]\\s*>");
		static QRegExp rxColor("(\\s+[cC][oO][lL][oO][rR]\\s*=\\s*['\"]?)"
				"([0-9a-fA-F]{6}['\"]?)");
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
		QTextStream &in = stream();
		QMap<Subtitle::Language, int> idxes;
		subs->clear();
		QString line = in.readLine();
		int sync = -1;
		while (!in.atEnd()) {
			if ((sync = rxSync.indexIn(line)) != -1)
				break;
			static QRegExp rxLang("\\s*\\.(\\w+)\\s*\\{(.+)\\}");
			if (rxLang.indexIn(line) != -1) {
				static QRegExp rxParam("\\s*(\\S+)\\s*:\\s*(\\S+)\\s*;\\s*");
				int pos = 0;
				Subtitle::Language lang;
				lang.klass = rxLang.cap(1);
				const QString text = rxLang.cap(2);
				while ((pos = rxParam.indexIn(text, pos)) != -1) {
					const QString key = rxParam.cap(1);
					if (!key.compare("name", Qt::CaseInsensitive))
						lang.name = rxParam.cap(2);
					else if (!key.compare("lang", Qt::CaseInsensitive))
						lang.code = rxParam.cap(2);
					pos += rxParam.matchedLength();
				}
				const int idx = idxes.size();
				idxes.insert(lang, idx);
				subs->append(Subtitle(path));
				(*subs)[idx].setLanguage(lang);;
			}
			line = in.readLine();
		}
		while (!in.atEnd()) {
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
			text.replace(rxP, "\\1span\\3").replace(rxColor, "\\1#\\2");
			Subtitle::Language lang;
			if (rxClass.indexIn(text) != -1)
				lang.klass = rxClass.cap(2);
			if ((end = rxEnd.indexIn(text)) != -1)
				text = text.left(end);
			if (!idxes.contains(lang)) {
				const int idx = idxes.size();
				idxes.insert(lang, idx);
				subs->append(Subtitle(path));
				(*subs)[idx].setLanguage(lang);
			}
			text.replace("&nbsp;", " ");
			text.replace("  ", "&nbsp;&nbsp;");
	// 		text.replace(rxBr, "\n");
	// 		text.remove(rxTag);
			(*subs)[idxes[lang]].insert(time, text);
		}
		file.close();
		return true;
	}

	bool save(const QString &path, const Subtitle &sub) const {
		QFile file(path);
		if (!file.open(QFile::WriteOnly | QFile::Truncate))
			return false;
		setDevice(&file);
		QTextStream &out = stream();
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
};

class Subtitle::Parser::SubRip : public Subtitle::Parser {
	bool parse(const QString &file, QList<Subtitle> *subs) {
		QFile dev(file);
		if (!dev.open(QFile::ReadOnly))
			return false;
		setDevice(&dev);
		QTextStream &in = stream();

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
			start = Utility::timeToMSecs(QTime(rxTime.cap(1).toInt()
					, rxTime.cap(2).toInt(), rxTime.cap(3).toInt(), rxTime.cap(4).toInt()));
			end = Utility::timeToMSecs(QTime(rxTime.cap(5).toInt()
					, rxTime.cap(6).toInt(), rxTime.cap(7).toInt(), rxTime.cap(8).toInt()));
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

	bool save(const QString &file, const Subtitle &sub) const {
		QFile dev(file);
		if (!dev.open(QFile::WriteOnly | QFile::Truncate))
			return false;
		setDevice(&dev);
		QTextStream &out = stream();

		int nth = 0;
		SubtitleIterator it(sub);
		while(it.hasNext()) {
			it.next();
			out << (++nth) << endl;
			out << Utility::msecsToString(it.key(), "hh:mm:ss,zzz") << " --> ";
			const bool hasNext = it.hasNext();
			out << Utility::msecsToString(hasNext ? it.peekNext().key(): it.key()*2, "hh:mm:ss,zzz")
					<< endl << QString(it.value()).replace("<BR>", "\n", Qt::CaseInsensitive)
					<< endl << endl;
			if (hasNext && it.peekNext().value().isEmpty())
				it.next();
		}
		dev.close();
		return true;
	}
};

class Subtitle::Parser::SubStationAlpha : public Subtitle::Parser {
	bool parse(const QString &path, QList<Subtitle> *subs) {
		Q_UNUSED(path);
		Q_UNUSED(subs);
		return false;
	}

	bool save(const QString &path, const Subtitle &sub) const {
		QFile dev(path);
		if (!dev.open(QFile::WriteOnly | QFile::Truncate))
			return false;
		setDevice(&dev);
		QTextStream &out = stream();

		const QString header = "[Script Info]\nScriptType: v4.00\nCollisions: Normal\n"
			"PlayResY: 1024\nPlayDepth: 0\nTimer: 100.0000\n\n"
			"[V4 Styles]\nFormat: Name, Fontname, Fontsize, PrimaryColour, "
			"SecondaryColour, TertiaryColour, BackColour, Bold, Italic, BorderStyle, "
			"Outline, Shadow, Alignment, MarginL, MarginR, MarginV, AlphaLevel, Encoding\n"
			"Style: DefaultVCD, Arial,28,11861244,11861244,11861244,-2147483640,-1,0,1,1,2,2,30,30,30,0,0\n\n"
			"[Events]\nFormat: Marked, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n";
		out << header;
		const QString dialogue = "Dialogue: Marked=0,%1,%2,DefaultVCD, NTP,0000,0000,0000,,{\\c&H000000&}%3\n";
		SubtitleIterator it(sub);
		QString dlg, time;
		while(it.hasNext()) {
			it.next();
			const QString text = it.value().trimmed();
			if (text.isEmpty())
				continue;
			time = Utility::msecsToString(it.key(), "h:mm:ss.zzz");
			time.chop(1);
			dlg = dialogue.arg(time);
			const int end = it.hasNext() ? it.peekNext().key() - 10 : it.key()*2;
			time = Utility::msecsToString(end, "h:mm:ss.zzz");
			time.chop(1);
			dlg = dlg.arg(time);
			dlg = dlg.arg(text);//.replace("<BR>", "\n", Qt::CaseInsensitive));
			out << dlg;
		}
		dev.close();
		return true;
	}
};

void Subtitle::Parser::setDevice(QIODevice *device) const {
	m_stream.setDevice(device);
	m_stream.setCodec(m_enc.toLocal8Bit());
}

Subtitle::Parser *Subtitle::Parser::create(const QString &ext) {
	Parser *p = 0;
#define EXT_IS(_ext) (ext.compare((_ext), Qt::CaseInsensitive) == 0)
	if (EXT_IS("smi"))
		p = new Sami;
	else if(EXT_IS("srt"))
		p = new SubRip;
	else if (EXT_IS("ssa") || EXT_IS("ass"))
		p = new SubStationAlpha;
	else if (EXT_IS("txt"))
		p = new TMPlayer;
#undef EXT_IS
	return p;
}

}
