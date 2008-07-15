#ifndef SUBTITLEPARSERSABSTRACTPARSER_H
#define SUBTITLEPARSERSABSTRACTPARSER_H

#include "../subtitle.h"

class QTextStream;
class QIODevice;

namespace Xine {

namespace SubtitleParsers {

class AbstractParser{
public:
	AbstractParser();
	virtual ~AbstractParser();
	virtual bool parse(const QString &path, QList<Subtitle> *subs) = 0;
	virtual bool save(const QString &path, const Subtitle &sub) const = 0;
	inline int minimumInterval() const {return m_interval;}
	inline void setMinimumInterval(int interval) {m_interval = interval;}
	void setEncoding(const QString &enc);
protected:
	inline QTextStream &textStream() const {return *m_stream;}
	void setDevice(QIODevice *device) const;
private:
	int m_interval;
	mutable QTextStream *m_stream;
	QString m_encoding;
};

}

}

#endif
