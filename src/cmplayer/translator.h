#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QtCore/QString>
#include "enum.h"

class Translator {
public:
	~Translator();
	static bool load(const QString &locale = QString());
	static bool load(UiLanguage lang = SystemDefault);
private:
	Translator();
	static Translator &get();
	struct Data;
	Data *d;
};

#endif
