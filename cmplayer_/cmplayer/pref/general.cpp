#include "general.h"
#include <QSettings>

namespace Pref {

void General::save(QSettings *set) const {
	set->beginGroup("General");
	set->setValue("VideoOutput", videoOutput);
	set->setValue("AudioOutput", audioOutput);
	set->setValue("VolumeAmplification", volumeAmplification);
	set->setValue("ResetVolume", resetVolume);
	set->setValue("InitialVolume", initialVolume);
	set->setValue("AutoAddFiles", autoAddFiles);
	set->setValue("PauseWhenMinimized", pauseWhenMinimized);
	set->setValue("PlayWhenRestored", playWhenRestored);
	set->setValue("PlayFromStoppedPoint", playFromStoppedPoint);
	set->endGroup();
}

void General::load(QSettings *set) {
	set->beginGroup("General");
	videoOutput = set->value("VideoOutput", "auto").toString();
	audioOutput = set->value("AudioOutput", "auto").toString();
	volumeAmplification = set->value("VolumeAmplification", 110.0).toDouble();
	resetVolume = set->value("ResetVolume", true).toBool();
	initialVolume = set->value("InitialVolume", 100).toInt();
	autoAddFiles = AutoAddFiles(set->value("AutoAddFiles", SimilarFiles).toInt());
	pauseWhenMinimized = set->value("PauseWhenMinimized", false).toBool();
	playWhenRestored = set->value("PlayWhenRestored", true).toBool();
	playFromStoppedPoint = set->value("PlayFromStoppedPoint", true).toBool();
	set->endGroup();
}

}
