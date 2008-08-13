#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <QMap>
#include <QMapIterator>
#include <QSharedDataPointer>
#include <QString>
#include "xinenamespace.h"

namespace Xine {

class Subtitle : private QMap<qint64, QString> {
public:
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
	Subtitle &operator=(const Subtitle &rhs);
	Subtitle united(const Subtitle &other) const {Subtitle sub(*this); return sub.unite(other);}
	Subtitle &unite(const Subtitle &other);
	Subtitle operator | (const Subtitle &other) const {return united(other);}
	Subtitle &operator |= (const Subtitle &other) {return unite(other);}
	QString filePath() const;
	void setLanguage(const QString &lang);
	const QString &language() const;
	QString name() const;

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
private:
	friend class SubtitleIterator;
	struct Data;
	QSharedDataPointer<Data> d;
};

class SubtitleIterator : public QMapIterator<qint64, QString> {
public:
	SubtitleIterator(const Subtitle &sub): QMapIterator<qint64, QString>(sub) {}
};

}

#endif
