#ifndef BACKEND_SUBTITLEPARSERS_SAMIPARSER_H
#define BACKEND_SUBTITLEPARSERS_SAMIPARSER_H

#include "abstractparser.h"

namespace Backend {

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
