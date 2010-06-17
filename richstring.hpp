#ifndef RICHSTRING_HPP
#define RICHSTRING_HPP

#include <QtCore/QString>

class RichString {
public:
	RichString();
	RichString(const char *string);
	RichString(const QString &string);
	RichString(const RichString &other);
	~RichString();
	RichString &operator = (const RichString &rhs);
	const QString &string() const {return m_string;}
	QString toPlain() const;
	bool isEmpty() const;
	bool hasWords() const;
	int size() const;
	void clear();
	RichString &merge(const RichString &other);
	RichString merged(const RichString &other) const;
//	RichString &trim();
//	RichString trimmed() const;
	static RichString fromPlain(const QString &plain);
private:
	void cachePlain() const;
	QString m_string;
	mutable QString m_plain;
	mutable bool m_cached;
};

#endif // RICHSTRING_HPP
