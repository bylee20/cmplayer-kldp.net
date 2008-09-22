#ifndef PREFERENCESPREFGENERAL_H
#define PREFERENCESPREFGENERAL_H

#include <QString>
#include <QSizeF>
#include <QStringList>
#include <QSharedDataPointer>

class QSettings;

namespace Pref {

class General {
public:
	enum AutoAddFiles {
		SimilarFiles = 0,
		AllFiles,
		DoNotAddFiles
	};
	General(): d(new Data) {}
	void save(QSettings *set) const;
	void load(QSettings *set);
	bool playWhenRestored() const {return d->playWhenRestored;}
	bool pauseWhenMinimized() const {return d->pauseWhenMinimized;}
	bool resetVolume() const {return d->resetVolume;}
	bool playFromStoppedPoint() const {return d->playFromStoppedPoint;}
	int initialVolume() const {return d->initialVolume;}
	AutoAddFiles autoAddFiles() const {return d->autoAddFiles;}
	void setPlayWhenRestored(bool enabled) {d->playWhenRestored = enabled;}
	void setPauseWhenMinimized(bool enabled) {d->pauseWhenMinimized = enabled;}
	void setResetVolume(bool enabled) {d->resetVolume = enabled;}
	void setPlayFromStoppedPoint(bool enabled) {d->playFromStoppedPoint = enabled;}
	void setInitialVolume(int value) {d->initialVolume = value;}
	void setAutoAddFiles(AutoAddFiles mode) {d->autoAddFiles = mode;}
private:
	struct Data : public QSharedData {
		Data() {}
		Data(const Data &other): QSharedData(other)
		, playWhenRestored(other.playWhenRestored)
		, pauseWhenMinimized(other.pauseWhenMinimized)
		, playFromStoppedPoint(other.playFromStoppedPoint)
		, resetVolume(other.resetVolume), initialVolume(other.initialVolume) {}
		bool playWhenRestored, pauseWhenMinimized, playFromStoppedPoint, resetVolume;
		int initialVolume;
		AutoAddFiles autoAddFiles;		
	};
	QSharedDataPointer<Data> d;

};

}

#endif
