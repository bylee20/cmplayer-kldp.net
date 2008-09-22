#ifndef BACKEND_SUBTITLEPARSERS_ABSTRACTPARSER_H
#define BACKEND_SUBTITLEPARSERS_ABSTRACTPARSER_H

#include "../subtitle.h"

class QTextStream;
class QIODevice;

namespace Backend {

namespace SubtitleParsers {

class AbstractParser{
public:
	AbstractParser();
	virtual ~AbstractParser();
	virtual bool parse(const QString &path, QList<Subtitle> *subs) = 0;
	virtual bool save(const QString &path, const Subtitle &sub) const = 0;
	void setEncoding(const QString &enc);
protected:
	inline QTextStream &textStream() const {return *m_stream;}
	void setDevice(QIODevice *device) const;
private:
	mutable QTextStream *m_stream;
	QString m_encoding;
};

}

}

#endif
