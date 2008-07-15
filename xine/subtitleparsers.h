#include "subtitleparsers/abstractparser.h"

namespace Xine {

class Subtitle;

namespace SubtitleParsers {

class AbstractParser;

bool parse(const QString &file, QList<Subtitle> *subs, const QString &enc, int minimumInterval = 10);
bool save(const QString &file, const Subtitle &sub, const QString &enc);

}

}
