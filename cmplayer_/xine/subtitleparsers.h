#include "subtitleparsers/abstractparser.h"

namespace Xine {

class Subtitle;

namespace SubtitleParsers {

class AbstractParser;

bool parse(const QString &file, QList<Subtitle> *subs, const QString &enc);
bool save(const QString &file, const Subtitle &sub, const QString &enc);

}

}
