#ifndef CORE_SUBTITLE_H
#define CORE_SUBTITLE_H

#include <QtCore/QMap>
#include <QtCore/QList>
#include "richstring.h"

namespace Core {

class Subtitle {
public:
	class Parser;
	class Language {
	public:
		Language() {}
		const QString &id() const {
			if (!m_name.isEmpty())
				return m_name;
			if (!m_locale.isEmpty())
				return m_locale;
			return m_klass;
		}
		const QString &name() const {return m_name;}
		const QString &locale() const {return m_locale;}
		const QString &klass() const {return m_klass;}
	private:
		friend class Parser;
		QString m_name, m_locale, m_klass;
	};
	class Component : public QMap<int, RichString> {
	public:
		enum Base {Time, Frame};
		Component(const QString &file = QString(), Base base = Time)
		: m_file(file), m_base(base) {insert(0, QString());}
		Component &unite(const Component &other, double frameRate);
		Component united(const Component &other, double frameRate) const {
			Component comp(*this); return comp.unite(other, frameRate);}
		bool operator == (const Component &rhs) const {return name() == rhs.name();}
		bool operator != (const Component &rhs) const {return !operator==(rhs);}
		QString name() const;
		const QString &fileName() const {return m_file;}
		Base base() const {return m_base;}
		bool isBasedOnFrame() const {return m_base == Frame;}
		const Language &language() const {return m_lang;}
		const_iterator start(int time, double frameRate) const;
		const_iterator end(int time, double frameRate) const;
		iterator end() {return QMap<int, RichString>::end();}
		const_iterator end() const {return QMap<int, RichString>::end();}
		static int convertKeyBase(int key, Base from, Base to, double frameRate) {
			if (from == to) return key;
			if (to == Time) return msec(key, frameRate);
			else return frame(key, frameRate);
		}
	private:
		friend class Parser;
		QString m_file;
		Base m_base;
		Language m_lang;
	};
	typedef QMapIterator<int, RichString> ComponentIterator;
	const Component &operator[] (int rhs) const {return m_comp[rhs];}
	Subtitle &operator += (const Subtitle &rhs) {
		m_comp += rhs.m_comp;
		return *this;
	}
	int count() const {return m_comp.size();}
	int size() const {return m_comp.size();}
	bool isEmpty() const;
	Component component(double frameRate) const;
	int start(int time, double frameRate) const;
	int end(int time, double frameRate) const;
	RichString text(int time, double frameRate) const;
	bool save(const QString &file, const QString &enc, double frameRate) const;
	bool load(const QString &file, const QString &enc);
	void clear() {m_comp.clear();}
	void append(const Component &comp) {m_comp.append(comp);}
	static Subtitle parse(const QString &file, const QString &enc);
private:
	static int msec(int frame, double frameRate) {
		return qRound(frame/frameRate*1000.0);
	}
	static int frame(int msec, double frameRate) {
		return qRound(msec*0.001*frameRate);
	}
	QList<Component> m_comp;
};

}

#endif
