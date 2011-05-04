#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QLocale>

class Translator : public QObject {
	Q_OBJECT
public:
	~Translator();
	static bool load(const QLocale &locale = QLocale::system());
	static QList<QLocale> availableLocales();
private:
	static QSet<QLocale> getLocales(const QString &p, const QString &f, const QString &rx);
	Translator();
	static Translator &get();
	struct Data;
	Data *d;
};

#endif // TRANSLATOR_HPP
