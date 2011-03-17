#include "subtitle_parser.hpp"
#include "global.hpp"
#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QFileInfo>

int Subtitle::Parser::msPerChar = -1;

int Subtitle::Parser::predictEndTime(const Component::const_iterator &it) {
	if (msPerChar > 0)
		return it.value().size()*msPerChar + it.key();
	return -1;
}

void Subtitle::Parser::appendLastTime(Subtitle *sub) {
	if (msPerChar <= 0)
		return;
	for (int i=0; i<sub->size(); ++i) {
		Component &comp = sub->m_comp[i];
		if (comp.isEmpty())
			continue;
		const Component::const_iterator last = --comp.end();
		if (!last.value().isEmpty())
			comp.insert(predictEndTime(last), "");
	}
}

QString &Subtitle::Parser::replaceEntity(QString &str) {
	return str.replace("<", "&lt;").replace(">", "&gt;").replace(" ", "&nbsp;");
}

void Subtitle::Parser::setDevice(QIODevice *device) const {
	m_stream.setDevice(device);
	m_stream.setCodec(m_enc.toLocal8Bit());
}

class Subtitle::Parser::Sami : public Subtitle::Parser {
	Subtitle parse(const QString &file) {
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
		static QRegExp rxComment("<!--.*-->");
		if (!minimal) {
			rxTag.setMinimal(true);
			rxSync.setMinimal(true);
			rxClass.setMinimal(true);
			rxP.setMinimal(true);
			rxEnd.setMinimal(true);
			rxBr.setMinimal(true);
			minimal = true;
		}
		Subtitle sub;
		QFile dev(file);
		if (!dev.open(QFile::ReadOnly))
			return sub;
		setDevice(&dev);
		QTextStream &in = stream();
		QMap<QString, int> idxes;
		QString line = in.readLine();
		int sync = -1;
		while (!in.atEnd()) {
			if ((sync = rxSync.indexIn(line)) != -1)
				break;
			static QRegExp rxLang("\\s*\\.(\\w+)\\s*\\{(.+)\\}");
			if (rxLang.indexIn(line) != -1) {
				static QRegExp rxParam("\\s*(\\S+)\\s*:\\s*(\\S+)\\s*;\\s*");
				int pos = 0;
				Component comp(file);
				comp.m_lang.m_klass = rxLang.cap(1);
				const QString text = rxLang.cap(2);
				while ((pos = rxParam.indexIn(text, pos)) != -1) {
					const QString key = rxParam.cap(1);
					if (!key.compare("name", Qt::CaseInsensitive))
						comp.m_lang.m_name = rxParam.cap(2);
					else if (!key.compare("lang", Qt::CaseInsensitive))
						comp.m_lang.m_locale = rxParam.cap(2);
					pos += rxParam.matchedLength();
				}
				const int idx = idxes.size();
				idxes.insert(comp.m_lang.m_klass, idx);
				sub.m_comp.push_back(comp);
			}
			line = in.readLine();
		}
		while (!in.atEnd()) {
			int time = rxSync.cap(1).toInt();
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
			text.remove(rxComment);
			QString klass;
			if (rxClass.indexIn(text) != -1)
				klass = rxClass.cap(2);
			if ((end = rxEnd.indexIn(text)) != -1)
				text = text.left(end);
			QMap<QString, int>::const_iterator it = idxes.find(klass);
			if (it == idxes.end()) {
				const int idx = idxes.size();
				idxes.insert(klass, idx);
				Component comp(file);
				comp.m_lang.m_klass = klass;
				sub.m_comp.push_back(comp);
			}
			RichString rich(text);
			if (!rich.hasWords())
				rich.clear();
			sub.m_comp[idxes[klass]].insert(time, rich);
		}
		dev.close();
		appendLastTime(&sub);
		return sub;
	}
	bool save(const QString &file, const Subtitle &sub, double frameRate) const {
		QFile dev(file);
		if (!dev.open(QFile::WriteOnly | QFile::Truncate))
			return false;
		setDevice(&dev);
		QTextStream &out = stream();
		out << "<SAMI>" << endl << "<BODY>" << endl;
		const Component comp = sub.component(frameRate);
		ComponentIterator it(comp);
		RichString text;
		while(it.hasNext()) {
			it.next();
			text = it.value().isEmpty() ? "&nbsp;" : it.value();
			out << "<SYNC Start=" << it.key() << ">" << text.string() << endl;
		}
		out << "</BODY>" << endl << "</SAMI>" << endl;
		dev.close();
		return true;
	}
};


class Subtitle::Parser::TMPlayer : public Subtitle::Parser {
	Subtitle parse(const QString &file) {
		QFile dev(file);
		if (!dev.open(QFile::ReadOnly))
			return Subtitle();
		setDevice(&dev);
		QTextStream &in = stream();
		Subtitle sub;
		sub.append(Component(file));
		Component &comp = sub.m_comp[0];
		int predictedEnd = -1;
		while (!in.atEnd()) {
			const QString line = in.readLine().trimmed();
			static QRegExp rxLine("^\\s*(\\d?\\d)\\s*:"
					"\\s*(\\d\\d)\\s*:\\s*(\\d\\d)\\s*:\\s*(.*)$");
			if (rxLine.indexIn(line) == -1)
				continue;
			const int time = timeToMSecs(QTime(rxLine.cap(1).toInt()
					, rxLine.cap(2).toInt(), rxLine.cap(3).toInt()));
			if (predictedEnd > 0 && time > predictedEnd)
				comp.insert(predictedEnd, "");
			QString text = rxLine.cap(4);
			replaceEntity(text).replace('|', "<br>");
			predictedEnd = predictEndTime(comp.insert(time, text));
		}
		dev.close();
		return sub;
	}
};

class Subtitle::Parser::MicroDVD : public Subtitle::Parser {
	Subtitle parse(const QString &file) {
		QFile dev(file);
		if (!dev.open(QFile::ReadOnly))
			return Subtitle();
		setDevice(&dev);
		QTextStream &in = stream();
		Subtitle sub;
		sub.append(Component(file, Component::Frame));
		Component &comp = sub.m_comp[0];
		QString line;
		while (!in.atEnd()) {
			line = in.readLine().trimmed();
			static QRegExp rxLine("^\\{(\\d+)\\}\\{(\\d+)\\}(.*)$");
			if (rxLine.indexIn(line) == -1)
				continue;
			const int start = rxLine.cap(1).toInt();
			const int end = rxLine.cap(2).toInt();
			QString text = rxLine.cap(3).trimmed();
			text.replace('|', "<br>");
			comp.insert(start, text);
			comp.insert(end, QString());
		}
		dev.close();
		return sub;
	}
};

class Subtitle::Parser::SubRip : public Subtitle::Parser {
	Subtitle parse(const QString &file) {
		Subtitle sub;
		QFile dev(file);
		if (!dev.open(QFile::ReadOnly))
			return sub;
		setDevice(&dev);
		QTextStream &in = stream();
		sub.append(Component(file));
		Component &comp = sub.m_comp[0];
		QString line;
		while (!in.atEnd()) {
			line = in.readLine().trimmed();
			static QRegExp rxNum("^(\\d+)$");
			static QRegExp rxTime("^(\\d\\d):(\\d\\d):(\\d\\d),(\\d\\d\\d) --> "
					"(\\d\\d):(\\d\\d):(\\d\\d),(\\d\\d\\d)$");
			if (rxNum.indexIn(line) == -1)
				continue;
			int index = -1;
			while (!in.atEnd()) {
				line = in.readLine().trimmed();
				if ((index = rxTime.indexIn(line)) != -1)
					break;
			}
			if (index == -1)
				return Subtitle();
			const int start = timeToMSecs(
					QTime(rxTime.cap(1).toInt(), rxTime.cap(2).toInt()
					, rxTime.cap(3).toInt(), rxTime.cap(4).toInt()));
			const int end = timeToMSecs(
					QTime(rxTime.cap(5).toInt(), rxTime.cap(6).toInt()
					, rxTime.cap(7).toInt(), rxTime.cap(8).toInt()));
			QString text;
			while (!in.atEnd()) {
				line = in.readLine().trimmed();
				if (line.isEmpty())
					break;
				text += line + '\n';
			}
			text.chop(1);
			text.replace('\n', "<br>");
			comp.insert(start, text);
			comp.insert(end, QString());
		}
		dev.close();
		return sub;
	}
// 	bool save(const QString &file, const Subtitle &sub) const {
// 		QFile dev(file);
// 		if (!dev.open(QFile::WriteOnly | QFile::Truncate))
// 			return false;
// 		setDevice(&dev);
// 		QTextStream &out = stream();
//
// 		int nth = 0;
// 		SubtitleIterator it(sub);
// 		while(it.hasNext()) {
// 			it.next();
// 			out << (++nth) << endl;
// 			out << Utility::msecsToString(it.key(), "hh:mm:ss,zzz") << " --> ";
// 			const bool hasNext = it.hasNext();
// 			out << Utility::msecsToString(hasNext ? it.peekNext().key(): it.key()*2, "hh:mm:ss,zzz")
// 					<< endl << QString(it.value()).replace("<BR>", "\n", Qt::CaseInsensitive)
// 					<< endl << endl;
// 			if (hasNext && it.peekNext().value().isEmpty())
// 				it.next();
// 		}
// 		dev.close();
// 		return true;
// 	}
};

//
// class Subtitle::Parser::SubStationAlpha : public Subtitle::Parser {
// 	bool parse(const QString &path, QList<Subtitle> *subs) {
// 		Q_UNUSED(path);
// 		Q_UNUSED(subs);
// 		return false;
// 	}
//
// 	bool save(const QString &path, const Subtitle &sub) const {
// 		QFile dev(path);
// 		if (!dev.open(QFile::WriteOnly | QFile::Truncate))
// 			return false;
// 		setDevice(&dev);
// 		QTextStream &out = stream();
//
// 		const QString header = "[Script Info]\nScriptType: v4.00\nCollisions: Normal\n"
// 			"PlayResY: 1024\nPlayDepth: 0\nTimer: 100.0000\n\n"
// 			"[V4 Styles]\nFormat: Name, Fontname, Fontsize, PrimaryColour, "
// 			"SecondaryColour, TertiaryColour, BackColour, Bold, Italic, BorderStyle, "
// 			"Outline, Shadow, Alignment, MarginL, MarginR, MarginV, AlphaLevel, Encoding\n"
// 			"Style: DefaultVCD, Arial,28,11861244,11861244,11861244,-2147483640,-1,0,1,1,2,2,30,30,30,0,0\n\n"
// 			"[Events]\nFormat: Marked, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n";
// 		out << header;
// 		const QString dialogue = "Dialogue: Marked=0,%1,%2,DefaultVCD, NTP,0000,0000,0000,,{\\c&H000000&}%3\n";
// 		SubtitleIterator it(sub);
// 		QString dlg, time;
// 		while(it.hasNext()) {
// 			it.next();
// 			const QString text = it.value().trimmed();
// 			if (text.isEmpty())
// 				continue;
// 			time = Utility::msecsToString(it.key(), "h:mm:ss.zzz");
// 			time.chop(1);
// 			dlg = dialogue.arg(time);
// 			const int end = it.hasNext() ? it.peekNext().key() - 10 : it.key()*2;
// 			time = Utility::msecsToString(end, "h:mm:ss.zzz");
// 			time.chop(1);
// 			dlg = dlg.arg(time);
// 			dlg = dlg.arg(text);//.replace("<BR>", "\n", Qt::CaseInsensitive));
// 			out << dlg;
// 		}
// 		dev.close();
// 		return true;
// 	}
// };

Subtitle::Parser *Subtitle::Parser::create(const QString &fileName) {
	QFileInfo info(fileName);
	Parser *p = 0;
#define EXT_IS(_ext) (ext.compare((_ext), Qt::CaseInsensitive) == 0)
	const QString ext = info.suffix();
	if (EXT_IS("smi"))
		p = new Sami;
	else if(EXT_IS("srt"))
		p = new SubRip;
// 	else if (EXT_IS("ssa") || EXT_IS("ass"))
// 		p = new SubStationAlpha;
	else if (EXT_IS("sub") || EXT_IS("txt")) {
		QFile file(info.absoluteFilePath());
		if (file.open(QFile::ReadOnly)) {
			QTextStream in(&file);
			for (int i=0; !p && i<10 && !in.atEnd(); ++i) {
				static QRegExp rxMicro("^\\{(\\d+)\\}\\{(\\d+)\\}(.*)$");
				static QRegExp rxTmp("^\\s*(\\d?\\d)\\s*:"
						"\\s*(\\d\\d)\\s*:\\s*(\\d\\d)\\s*:\\s*(.*)$");
				const QString line = in.readLine();
				if (rxMicro.indexIn(line) != -1)
					p = new MicroDVD;
				else if (rxTmp.indexIn(line) != -1)
					p = new TMPlayer;
			}
		}
	}
#undef EXT_IS
	return p;
}

