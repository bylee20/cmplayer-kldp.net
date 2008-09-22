#ifndef PREFERENCESPREFGENERAL_H
#define PREFERENCESPREFGENERAL_H

#include <QString>
#include <QSizeF>
#include <QStringList>

class QSettings;

namespace Pref {

class General {
public:
	enum AutoAddFiles {
		SimilarFiles = 0,
		AllFiles,
		DoNotAddFiles
	};
	void save(QSettings *set) const;
	void load(QSettings *set);
public:
	QString videoOutput, audioOutput;
	bool playWhenRestored, pauseWhenMinimized, resetVolume, playFromStoppedPoint;
	double volumeAmplification;
	int initialVolume;
	AutoAddFiles autoAddFiles;
};

}

#endif
