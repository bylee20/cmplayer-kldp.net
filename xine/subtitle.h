#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <QMap>
#include <QMapIterator>
#include <QSharedDataPointer>
#include <QString>
#include "mplayer.h"

namespace MPlayer {

class Subtitle : private QMap<qint64, QString> {
public:
	class iterator : public QMap<qint64, QString>::iterator {
	public:
		iterator(const QMap<qint64, QString>::iterator &other)
		: QMap<qint64, QString>::iterator(other) {}
	};
	class const_iterator : public QMap<qint64, QString>::const_iterator {
	public:
		const_iterator(const QMap<qint64, QString>::const_iterator &other)
		: QMap<qint64, QString>::const_iterator(other) {}
	};
	enum Type {Invalid = 0, LocalFile = 1, Disc = 2};
	Subtitle(const QString &filePath);
	Subtitle(DiscType discType);
	Subtitle(const Subtitle &other);
	~Subtitle();
	Subtitle &operator=(const Subtitle &rhs);
	Subtitle united(const Subtitle &other) const {Subtitle sub(*this); return sub.unite(other);}
	Subtitle &unite(const Subtitle &other);
	Subtitle operator | (const Subtitle &other) const {return united(other);}
	Subtitle &operator |= (const Subtitle &other) {return unite(other);}
	QString filePath() const;
	void setLanguage(const QString &lang);
	void setInternalID(int id);
	const QString &language() const;
	Type type() const;
	DiscType discType() const;
	int internalID() const;
	QString name() const;

	void clear() {QMap<qint64, QString>::clear();}
	iterator insert(qint64 t, const QString &txt) {return QMap<qint64, QString>::insert(t, txt);}
	const_iterator find(qint64 time) const {return QMap<qint64, QString>::find(time);}
	iterator begin() {return QMap<qint64, QString>::begin();}
	const_iterator begin() const {return QMap<qint64, QString>::begin();}
	iterator end() {return QMap<qint64, QString>::end();}
	const_iterator end() const {return QMap<qint64, QString>::end();}
	iterator lowerBound(qint64 time) {return QMap<qint64, QString>::lowerBound(time);}
	const_iterator lowerBound(qint64 time) const {return QMap<qint64, QString>::lowerBound(time);}
	iterator upperBound(qint64 time) {return QMap<qint64, QString>::upperBound(time);}
	const_iterator upperBound(qint64 time) const {return QMap<qint64, QString>::upperBound(time);}
	int count() const {return QMap<qint64, QString>::count();}
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
