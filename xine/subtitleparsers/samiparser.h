#ifndef SUBTITLEPARSERSSAMIPARSER_H
#define SUBTITLEPARSERSSAMIPARSER_H

#include "abstractparser.h"

namespace MPlayer {

namespace SubtitleParsers {

class SamiParser : public AbstractParser {
public:
	SamiParser();
	virtual bool parse(const QString &path, QList<Subtitle> *subs);
	virtual bool save(const QString &path, const Subtitle &sub) const;
};

}

}

#endif
