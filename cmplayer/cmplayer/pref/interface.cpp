#include "interface.h"
#include <QtCore/QStringList>
#include <QtCore/QSettings>

namespace Pref {

QStringList Interface::mouseClickActions() {
	return QStringList() << QObject::trUtf8("전체화면 전환하기")
				<< QObject::trUtf8("재생/일시정지 전환하기");
}

QStringList Interface::wheelScrollActions() {
	return QStringList() << QObject::trUtf8("앞으로/뒤로")
			<< QObject::trUtf8("많이 앞으로/뒤로")
			<< QObject::trUtf8("더 많이 앞으로/뒤로")
			<< QObject::trUtf8("다음 파일/이전 파일")
			<< QObject::trUtf8("소리 크게/작게");
}

void Interface::load(QSettings *set) {
	set->beginGroup("Interface");
	d->seekingStep = set->value("SeekingStep", DefaultSeekingStep).toInt();
	d->seekingMoreStep = set->value("SeekingMoreStep", DefaultSeekingMoreStep).toInt();
	d->seekingMuchMoreStep = set->value("SeekingMuchMoreStep", DefaultSeekingMuchMoreStep).toInt();
	d->syncDelayStep = set->value("SyncDelayStep", DefualtSyncDelayStep).toInt();
	d->volumeStep = set->value("VolumeStep", DefaultVolumeStep).toInt();
	d->doubleClickAction = MouseClickAction(set->value("DoubleClickAction", ToggleFullScreen).toInt());
	d->middleClickAction = MouseClickAction(set->value("MiddleClickAction", TogglePlayPause).toInt());
	d->wheelScrollAction = WheelScrollAction(set->value("WheelScrollAction", VolumeUpDown).toInt());
	d->shortcuts.clear();
	int size = set->beginReadArray("Shortcuts");
	for (int i=0; i<size; ++i) {
		set->setArrayIndex(i);
		QString name = set->value("Name", QString()).toString();
		QKeySequence key = set->value("Key", QKeySequence()).value<QKeySequence>();
		d->shortcuts[name] = key;
	}
	set->endArray();
	set->endGroup();
}

void Interface::save(QSettings *set) const {
	set->beginGroup("Interface");
	set->setValue("SeekingStep", d->seekingStep);
	set->setValue("SeekingMoreStep", d->seekingMoreStep);
	set->setValue("SeekingMuchMoreStep", d->seekingMuchMoreStep);
	set->setValue("SyncDelayStep", d->syncDelayStep);
	set->setValue("VolumeStep", d->volumeStep);
	set->setValue("DoubleClickAction", d->doubleClickAction);
	set->setValue("MiddleClickAction", d->middleClickAction);
	set->setValue("WheelScrollAction", d->wheelScrollAction);
	set->beginWriteArray("Shortcuts", d->shortcuts.size());
	int i = -1;
	QMap<QString, QKeySequence>::const_iterator it = d->shortcuts.begin();
	for (; it != d->shortcuts.end(); ++it) {
		set->setArrayIndex(++i);
		set->setValue("Name", it.key());
		set->setValue("Key", it.value());
	}
	set->endArray();
	set->endGroup();
}

}
