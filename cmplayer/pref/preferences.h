#ifndef PREF_PREFERENCES_H
#define PREF_PREFERENCES_H

#include <QStringList>

namespace Pref {

class General;				class Subtitle;
class Interface;			class BackendPref;

class Preferences {
public:
	~Preferences();
	static Preferences *get() {static Preferences obj; return &obj;}
	void load();
	void save() const;
	void setGeneral(const General &general);
	void setSubtitle(const Subtitle &subtitle);
	void setInterface(const Interface &interface);
	void setBackend(const BackendPref &backend);
	inline const General &general() const {return m_gen;}
	inline const Subtitle &subtitle() const {return m_sub;}
	inline const Interface &interface() const {return m_iface;}
	inline const BackendPref &backend() const {return m_back;}
private:
	Preferences();
	struct Data;
	Data *d;
	General &m_gen;
	Subtitle &m_sub;
	Interface &m_iface;
	BackendPref &m_back;
	static const int version = 1;
};

Preferences *get();

}

#endif
