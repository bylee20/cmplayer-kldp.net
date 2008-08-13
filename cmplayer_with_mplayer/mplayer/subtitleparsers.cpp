#include "subtitleparsers.h"
#include "subtitleparsers/samiparser.h"
#include "subtitleparsers/subripparser.h"
#include "subtitle.h"
#include <QFileInfo>

namespace MPlayer {

namespace SubtitleParsers {

AbstractParser *getNewParser(const QString &extension) {
	AbstractParser *p = 0;
	if (extension.compare("smi", Qt::CaseInsensitive) == 0)
		p = new SamiParser;
	else if(extension.compare("srt", Qt::CaseInsensitive)==0)
		p = new SubRipParser;
	return p;
}

bool parse(const QString &file, QList<Subtitle> *subs, const QString &enc, int minimumInterval) {
	AbstractParser *parser = getNewParser(QFileInfo(file).suffix());
	bool result = false;
	if (parser) {
		parser->setMinimumInterval(minimumInterval);
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

bool save(const QString &file, const Subtitle &sub, const QString &enc) {
	AbstractParser *parser = getNewParser(QFileInfo(file).suffix());
	bool result = false;
	if (parser) {
		parser->setEncoding(enc);
		result = parser->save(file, sub);
	}
	delete parser;
	return result;
}

}

}
