#ifndef PREFERENCESPREFSUBTITLE_H
#define PREFERENCESPREFSUBTITLE_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <backend/subtitleoutput.h>
#include <backend/osdstyle.h>
#include <QtCore/QSharedDataPointer>

class QSettings;

namespace Pref {

class Subtitle {
public:
	enum AutoLoad {NoAutoLoad = 0, Matched = 1, Contain = 2, SamePath = 3};
	Subtitle(): d(new Data) {}
	void load(QSettings *set);
	void save(QSettings *set) const;
	const Backend::OsdStyle &osdStyle() const {return d->style;}
	const QString &encoding() const {return d->encoding;}
	AutoLoad autoLoad() const {return d->autoLoad;}
	Backend::SubtitleOutput::AutoSelect autoSelect() const {return d->autoSelect;}
	int initialPos() const {return d->initialPos;}
	const QStringList &priority() const {return d->priority;}
	void setOsdStyle(const Backend::OsdStyle &style) {d->style = style;}
	void setEncoding(const QString &encoding) {d->encoding = encoding;}
	void setAutoLoad(AutoLoad mode) {d->autoLoad = mode;}
	void setAutoSelect(Backend::SubtitleOutput::AutoSelect mode) {d->autoSelect = mode;}
	void setInitialPos(int pos) {d->initialPos = pos;}
	void setPriority(const QStringList &priority) {d->priority = priority;}
private:
	struct Data : public QSharedData {
		Data() {}
		Data(const Data &other): QSharedData(other)
		, style(other.style), encoding(other.encoding), autoLoad(other.autoLoad)
		, autoSelect(other.autoSelect), initialPos(other.initialPos)
		, priority(other.priority) {}
		Backend::OsdStyle style;
		QString encoding;
		AutoLoad autoLoad;
		Backend::SubtitleOutput::AutoSelect autoSelect;
		int initialPos;
		QStringList priority;

	};
	QSharedDataPointer<Data> d;
};

}

#endif
