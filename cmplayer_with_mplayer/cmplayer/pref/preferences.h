#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QStringList>

namespace Pref {

class General;			class Subtitle;
class Interface;

class Preferences {
public:
	static Preferences *get() {static Preferences obj; return &obj;}
	void load();
	void save() const;
	void setGeneral(const General &general);
	void setSubtitle(const Subtitle &subtitle);
	void setInterface(const Interface &interface);
	inline const General &general() const {return *m_general;}
	inline const Subtitle &subtitle() const {return *m_subtitle;}
	inline const Interface &interface() const {return *m_interface;}
private:
	Preferences();
	General *m_general;
	Subtitle *m_subtitle;
	Interface *m_interface;
};

Preferences *get();

}

#endif
