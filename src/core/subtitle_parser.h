#ifndef CORE_SUBTITLE_PARSER_H
#define CORE_SUBTITLE_PARSER_H

#include "subtitle.h"
#include <QtCore/QTextStream>

namespace Core {

class Subtitle::Parser {
public:
	virtual ~Parser() {}
	virtual Subtitle parse(const QString &file) = 0;
	virtual bool save(const QString &/*file*/, const Subtitle &/*sub*/
			, double /*frameRate*/) const {return false;}
	void setEncoding(const QString &enc) {m_enc = enc;}
	static Parser *create(const QString &ext);
	static void setMsPerCharactor(int msPerChar) {Parser::msPerChar = msPerChar;}
protected:
	static void appendLastTime(Subtitle *sub);
	QTextStream &stream() const {return m_stream;}
	void setDevice(QIODevice *device) const;
	static int predictEndTime(const Component::const_iterator &it);
	static QString &replaceEntity(QString &str);
private:
	class Sami;
	class SubRip;
	class SubStationAlpha;
	class TMPlayer;
	class MicroDVD;
	static int msPerChar;
	mutable QTextStream m_stream;
	QString m_enc;
};

}

#endif // SUBTITLE_PARSER_H
