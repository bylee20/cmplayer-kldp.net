#ifndef CORE_SUBTITLE_PARSER_H
#define CORE_SUBTITLE_PARSER_H

#include "subtitle.h"
#include <QtCore/QTextStream>

namespace Core {

class Subtitle::Parser {
public:
	virtual Subtitle parse(const QString &file) = 0;
	virtual bool save(const QString &/*file*/, const Subtitle &/*sub*/
			, double /*frameRate*/) const {return false;}
	void setEncoding(const QString &enc) {m_enc = enc;}
	static Parser *create(const QString &ext);
protected:
	QTextStream &stream() const {return m_stream;}
	void setDevice(QIODevice *device) const;
private:
	class Sami;
	class SubRip;
	class SubStationAlpha;
	class TMPlayer;
	class MicroDVD;
	mutable QTextStream m_stream;
	QString m_enc;
};

}

#endif // SUBTITLE_PARSER_H
