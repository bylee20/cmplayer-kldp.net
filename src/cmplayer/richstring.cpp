#include "richstring.hpp"
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

struct RichStringRx {
	RichStringRx()
	: tag("</?\\s*[a-zA-Z]+[^>]*>")
	, newLine("(\\r\\n|\\n|\\r)")
	, whiteSpaces("\\s+")
	, entity("&(.+);")
	, trim("(^([\\s\\t\\r\\n]|&[nN][bB][sS][pP];|<\\s*[bB][rR]\\s*>)+|"
	       "([\\s\\t\\r\\n]|&[nN][bB][sS][pP];|<\\s*[bB][rR]\\s*>)+$)") {
		tag.setMinimal(true);
		newLine.setMinimal(false);
		whiteSpaces.setMinimal(true);
		entity.setMinimal(true);
	}
	QRegExp tag, newLine, whiteSpaces, entity, trim;
};

static const RichStringRx rx;

RichString::RichString()
: m_cached(false) {
}

RichString::RichString(const char *string)
: m_string(string), m_cached(false) {
	m_string.remove(rx.newLine);
	m_string.replace(rx.whiteSpaces, " ");
	m_string.remove(rx.trim);
}

RichString::RichString(const QString &string)
: m_string(string), m_cached(false) {
	m_string.remove(rx.newLine);
	m_string.replace(rx.whiteSpaces, " ");
	m_string.remove(rx.trim);
}

RichString::RichString(const RichString &other)
: m_string(other.m_string), m_plain(other.m_plain), m_cached(other.m_cached) {
	m_cached = other.m_cached;
	m_plain = other.m_plain;
	m_string = other.m_string;
}

RichString::~RichString() {

}

RichString &RichString::operator = (const RichString &rhs) {
	if (this != &rhs) {
		m_cached = rhs.m_cached;
		m_plain = rhs.m_plain;
		m_string = rhs.m_string;
	}
	return *this;
}

bool RichString::hasWords() const {
	cachePlain();
	return !m_plain.trimmed().isEmpty();
}

bool RichString::isEmpty() const {
	cachePlain();
	return m_plain.isEmpty();
}

int RichString::size() const {
	cachePlain();
	return m_plain.size();
}

void RichString::cachePlain() const {
	if (!m_cached) {
		m_plain = m_string;
		m_plain.replace("<br>", "\n", Qt::CaseInsensitive);
		m_plain.remove(rx.tag).remove(rx.newLine);
		m_plain.replace(rx.whiteSpaces, " ");
		m_plain.replace("&nbsp;", " ", Qt::CaseInsensitive);
		m_plain.replace("&gt;", ">", Qt::CaseInsensitive);
		m_plain.replace("&lt;", "<", Qt::CaseInsensitive);
		m_plain.replace("&amp;", "&", Qt::CaseInsensitive);
		m_cached = true;
	}
}

QString RichString::toPlain() const {
	cachePlain();
	return m_plain;
}

RichString RichString::fromPlain(const QString &plain) {
	QString rich = plain;
	rich.replace("&", "&amp;").replace(" ", "&nbsp;");
	rich.replace(">", "&gt;").replace("<", "&lt;");
	rich.replace(rx.newLine, "<br>");
	return RichString(rich);
}

RichString &RichString::merge(const RichString &other) {
	if (!hasWords()) {
		*this = other;
	} else if (other.hasWords()) {
		m_string += "<br>";
		m_string += other.string();
		m_cached = false;
	}
	return *this;
}

RichString RichString::merged(const RichString &other) const {
	return RichString(*this).merge(other);
}

void RichString::clear() {
	m_string.clear();
	m_plain.clear();
	m_cached = true;
}

