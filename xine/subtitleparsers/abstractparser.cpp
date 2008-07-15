#include "abstractparser.h"
#include <QTextCodec>
#include <QTextStream>

namespace MPlayer {

namespace SubtitleParsers {

AbstractParser::AbstractParser()
: m_interval(1), m_stream(new QTextStream) {
}

AbstractParser::~AbstractParser() {
	delete m_stream;
}

void AbstractParser::setDevice(QIODevice *device) const {
	m_stream->setDevice(device);
	m_stream->setCodec(m_encoding.toLocal8Bit());
}

void AbstractParser::setEncoding(const QString &enc) {
	m_encoding = enc;
}

}

}

