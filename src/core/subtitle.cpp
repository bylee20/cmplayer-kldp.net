#include "subtitle.h"
#include "subtitle_parser.h"
#include "utility.h"
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

namespace Core {

QString Subtitle::Component::name() const {
	if (m_lang.id().isEmpty())
		return QFileInfo(m_file).fileName();
	else
		return QFileInfo(m_file).fileName() + " (" + m_lang.id() + ')';
}

Subtitle::Component Subtitle::component(double frameRate) const {
	if (m_comp.isEmpty())
		return Component();
	Component comp;
	for (int i=0; i<m_comp.size(); ++i)
		comp.unite(m_comp[i], frameRate);
	return comp;
}

Subtitle::Component::const_iterator Subtitle::Component::start(int time, double frameRate) const {
	if (isEmpty() || time < 0)
		return end();
	return --end(time, frameRate);
}

Subtitle::Component::const_iterator Subtitle::Component::end(int time, double frameRate) const {
	if (isEmpty() || time < 0)
		return end();
	int key = time;
	if (m_base == Frame) {
		if (frameRate < 0.0)
			return end();
		key = qRound(time*0.001*frameRate);
	}
	return upperBound(time);
}

Subtitle::Component &Subtitle::Component::unite(const Component &other, double frameRate) {
	if (this == &other || other.isEmpty())
		return *this;
	else if (isEmpty()) {
		*this = other;
		return *this;
	}
	ComponentIterator it1(*this);
	ComponentIterator it2(other);
	int k3 = -1;
	while(it1.hasNext()) {
		int k1 = it1.next().key();
		int k2 = it1.hasNext() ? it1.peekNext().key() : -1;
		if (k3 != -1 && it2.hasPrevious())
			(*this)[k1] = merged(it1.value(), it2.peekPrevious().value());
		while(it2.hasNext()) {
			k3 = convertKeyBase(it2.next().key(), other.base(), m_base, frameRate);
			if (k2 == -1)
				(*this)[k3] = merged(it1.value(), it2.value());
			else if (k3 >= k2) {
				it2.previous();
				break;
			} else if (k3 == k1)
				(*this)[k1] = merged(it1.value(), it2.value());
			else if (k3 > k1)
				(*this)[k3] = merged(it1.value(), it2.value());
			else if (k3 < k1)
				(*this)[k3] = it2.value();
		}
	}
	return *this;
}

int Subtitle::start(int time, double frameRate) const {
	int s = -1;
	for (int i=0; i<m_comp.size(); ++i) {
		const Component::const_iterator it = m_comp[i].start(time, frameRate);
		if (it != m_comp[i].end())
			s = qMax(s, m_comp[i].isBasedOnFrame() ? msec(it.key(), frameRate) : it.key());
	}
	return s;
}

int Subtitle::end(int time, double frameRate) const {
	int e = -1;
	for (int i=0; i<m_comp.size(); ++i) {
		const Component::const_iterator it = m_comp[i].end(time, frameRate);
		if (it != m_comp[i].end()) {
			const int t = m_comp[i].isBasedOnFrame() ? msec(it.key(), frameRate) : it.key();
			e = e == -1 ? t : qMin(e, t);
		}
	}
	return e;
}

QString Subtitle::text(int time, double frameRate) const {
	if (m_comp.isEmpty())
		return QString();
	QString text;
	for (int i=0; i<m_comp.size(); ++i) {
		const Component::const_iterator it = m_comp[i].start(time, frameRate);
		if (it != m_comp[i].end())
			text = merged(text, it.value());
	}
	return text;
}

bool Subtitle::save(const QString &file, const QString &enc, double frameRate) const {
	Parser *parser = Parser::create(QFileInfo(file).suffix());
	bool result = false;
	if (parser) {
		parser->setEncoding(enc);
		result = parser->save(file, *this, frameRate);
	}
	delete parser;
	return result;
}

bool Subtitle::load(const QString &file, const QString &enc) {
	*this = parse(file, enc);
	return !isEmpty();
}

Subtitle Subtitle::parse(const QString &file, const QString &enc) {
	Parser *parser = Parser::create(file);
	Subtitle sub;
	if (parser) {
		parser->setEncoding(enc);
		sub = parser->parse(file);
	}
	delete parser;
	return sub;
}

bool Subtitle::isEmpty() const {
	if (m_comp.isEmpty())
		return true;
	for (int i=0; i<m_comp.size(); ++i) {
		if (!m_comp[i].isEmpty())
			return false;
	}
	return true;
}

}
