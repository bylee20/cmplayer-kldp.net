#include "general.h"
#include <QSettings>

namespace Pref {

void General::save(QSettings *set) const {
	set->beginGroup("General");
	set->setValue("VideoOutput", videoOutput);
	set->setValue("AudioOutput", audioOutput);
	set->setValue("MPlayerPath", mplayerPath);
	set->setValue("UseSoftwareVolume", useSoftwareVolume);
	set->setValue("VolumeAmplification", volumeAmplification);
	set->setValue("AdditionalOptions", additionalOptions);
	set->setValue("ResetVolume", resetVolume);
	set->setValue("InitialVolume", initialVolume);
	set->setValue("UseSoftwareEqualizer", useSoftwareEqualizer);
	set->setValue("AutoAddFiles", autoAddFiles);
	set->setValue("PauseWhenMinimized", pauseWhenMinimized);
	set->setValue("PlayWhenRestored", playWhenRestored);
	set->setValue("AutoPitch", autoPitch);
	set->setValue("PlayFromStoppedPoint", playFromStoppedPoint);
	set->setValue("AddAllDVDTitles", addAllDVDTitles);
	set->endGroup();
}

void General::load(QSettings *set) {
	set->beginGroup("General");
	videoOutput = set->value("VideoOutput", "").toString();
	audioOutput = set->value("AudioOutput", "").toString();
	mplayerPath = set->value("MPlayerPath", "mplayer").toString();
	useSoftwareVolume = set->value("UseSoftwareVolume", true).toBool();
	volumeAmplification = set->value("VolumeAmplification", 110.0).toDouble();
	additionalOptions = set->value("AdditionalOptions", QStringList()).toStringList();
	resetVolume = set->value("ResetVolume", true).toBool();
	initialVolume = set->value("InitialVolume", 100).toInt();
	useSoftwareEqualizer = set->value("UseSoftwareEqualizer", false).toBool();
	autoAddFiles = AutoAddFiles(set->value("AutoAddFiles", SimilarFiles).toInt());
	pauseWhenMinimized = set->value("PauseWhenMinimized", false).toBool();
	playWhenRestored = set->value("PlayWhenRestored", true).toBool();
	autoPitch = set->value("AutoPitch", true).toBool();
	playFromStoppedPoint = set->value("PlayFromStoppedPoint", true).toBool();
	addAllDVDTitles = set->value("AddAllDVDTitles", true).toBool();
	set->endGroup();
}

}
