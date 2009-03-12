#ifndef BACKEND_SUBTITLE_H
#define BACKEND_SUBTITLE_H

#include <QMap>
#include <QMapIterator>
#include <QSharedDataPointer>
#include <QString>

namespace Core {

class Subtitle : private QMap<qint64, QString> {
public:
	class Parser;
	struct Language {
		Language(const QString &klass): klass(klass) {}
		Language() {}
		bool operator<(const Language &rhs) const {return klass < rhs.klass;}
		const QString &language() const {
			if (!name.isEmpty())
				return name;
			if (!code.isEmpty())
				return code;
			return klass;
		}
		QString name, code, klass;
	};
	typedef QMap<qint64, QString>::iterator Iterator;
	typedef QMap<qint64, QString>::const_iterator ConstIterator;
	static Iterator iterator() {
		return QMap<qint64, QString>::iterator();
	}
	static ConstIterator const_iterator() {
		return QMap<qint64, QString>::const_iterator();
	}
	Subtitle(const QString &filePath = QString::null);
	Subtitle(const Subtitle &other);
	~Subtitle();
	Subtitle united(const Subtitle &other) const {Subtitle sub(*this); return sub.unite(other);}
	Subtitle &unite(const Subtitle &other);
	bool operator == (const Subtitle &rhs) const {return name() == rhs.name();}
	bool operator != (const Subtitle &rhs) const {return !operator==(rhs);}
	Subtitle operator | (const Subtitle &rhs) const {return united(rhs);}
	Subtitle &operator |= (const Subtitle &rhs) {return unite(rhs);}
	QString filePath() const;
	void setLanguage(const Language &lang);
	const Language &language() const;
	const QString &name() const {return d->name;}
	QString &operator[](qint64 time) {return QMap<qint64, QString>::operator[](time);}
	void clear() {QMap<qint64, QString>::clear();}
	Iterator insert(qint64 t, const QString &txt) {return QMap<qint64, QString>::insert(t, txt);}
	ConstIterator find(qint64 time) const {return QMap<qint64, QString>::find(time);}
	Iterator begin() {return QMap<qint64, QString>::begin();}
	ConstIterator begin() const {return QMap<qint64, QString>::begin();}
	Iterator end() {return QMap<qint64, QString>::end();}
	ConstIterator end() const {return QMap<qint64, QString>::end();}
	Iterator lowerBound(qint64 time) {return QMap<qint64, QString>::lowerBound(time);}
	ConstIterator lowerBound(qint64 time) const {return QMap<qint64, QString>::lowerBound(time);}
	Iterator upperBound(qint64 time) {return QMap<qint64, QString>::upperBound(time);}
	ConstIterator upperBound(qint64 time) const {return QMap<qint64, QString>::upperBound(time);}
	int count() const {return QMap<qint64, QString>::count();}
	bool isEmpty() const {return QMap<qint64, QString>::isEmpty();}
	bool contains(qint64 time) const {return QMap<qint64, QString>::contains(time);}
	static bool parse(const QString &file, QList<Subtitle> *subs, const QString &enc);
	static bool save(const QString &file, const Subtitle &sub, const QString &enc);
private:
	void updateName();
	friend class SubtitleIterator;
	struct Data : public QSharedData {
		Data() {}
		Data(const Data &other)
		: QSharedData(other), filePath(other.filePath), lang(other.lang) {}
		~Data() {}
		QString filePath, name;
		Language lang;
	};
	QSharedDataPointer<Data> d;
};

class SubtitleIterator : public QMapIterator<qint64, QString> {
public:
	SubtitleIterator(const Subtitle &sub): QMapIterator<qint64, QString>(sub) {}
};

}

#endif
