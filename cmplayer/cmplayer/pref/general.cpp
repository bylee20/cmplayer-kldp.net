#include "general.h"
#include <QtCore/QSettings>

namespace Pref {

void General::save(QSettings *set) const {
	set->beginGroup("General");
	set->setValue("ResetVolume", d->resetVolume);
	set->setValue("InitialVolume", d->initialVolume);
	set->setValue("AutoAddFiles", d->autoAddFiles);
	set->setValue("PauseWhenMinimized", d->pauseWhenMinimized);
	set->setValue("PlayWhenRestored", d->playWhenRestored);
	set->setValue("PlayFromStoppedPoint", d->playFromStoppedPoint);
	set->endGroup();
}

void General::load(QSettings *set) {
	set->beginGroup("General");
	d->resetVolume = set->value("ResetVolume", true).toBool();
	d->initialVolume = set->value("InitialVolume", 100).toInt();
	d->autoAddFiles = AutoAddFiles(set->value("AutoAddFiles", SimilarFiles).toInt());
	d->pauseWhenMinimized = set->value("PauseWhenMinimized", false).toBool();
	d->playWhenRestored = set->value("PlayWhenRestored", true).toBool();
	d->playFromStoppedPoint = set->value("PlayFromStoppedPoint", true).toBool();
	set->endGroup();
}

}
