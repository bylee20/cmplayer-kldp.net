#ifndef SUBTITLEPARSERSSUBRIPPARSER_H
#define SUBTITLEPARSERSSUBRIPPARSER_H

#include "abstractparser.h"

namespace MPlayer {

namespace SubtitleParsers {

class SubRipParser : public AbstractParser {
public:
	SubRipParser();
	virtual bool parse(const QString &path, QList<Subtitle> *subs);
	virtual bool save(const QString &path, const Subtitle &sub) const;
};

}

}

#endif
