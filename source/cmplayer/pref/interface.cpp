#include "interface.h"
#include <QStringList>
#include <QSettings>

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
	seekingStep = set->value("SeekingStep", DefaultSeekingStep).toInt();
	seekingMoreStep = set->value("SeekingMoreStep", DefaultSeekingMoreStep).toInt();
	seekingMuchMoreStep = set->value("SeekingMuchMoreStep", DefaultSeekingMuchMoreStep).toInt();
	syncDelayStep = set->value("SyncDelayStep", DefualtSyncDelayStep).toInt();
	volumeStep = set->value("VolumeStep", DefaultVolumeStep).toInt();
	doubleClickAction = MouseClickAction(set->value("DoubleClickAction", ToggleFullScreen).toInt());
	middleClickAction = MouseClickAction(set->value("MiddleClickAction", TogglePlayPause).toInt());
	wheelScrollAction = WheelScrollAction(set->value("WheelScrollAction", VolumeUpDown).toInt());
	shortcuts.clear();
	int size = set->beginReadArray("Shortcuts");
	for (int i=0; i<size; ++i) {
		set->setArrayIndex(i);
		QString name = set->value("Name", QString()).toString();
		QKeySequence key = set->value("Key", QKeySequence()).value<QKeySequence>();
		shortcuts[name] = key;
	}
	set->endArray();
	set->endGroup();
}

void Interface::save(QSettings *set) const {
	set->beginGroup("Interface");
	set->setValue("SeekingStep", seekingStep);
	set->setValue("SeekingMoreStep", seekingMoreStep);
	set->setValue("SeekingMuchMoreStep", seekingMuchMoreStep);
	set->setValue("SyncDelayStep", syncDelayStep);
	set->setValue("VolumeStep", volumeStep);
	set->setValue("DoubleClickAction", doubleClickAction);
	set->setValue("MiddleClickAction", middleClickAction);
	set->setValue("WheelScrollAction", wheelScrollAction);
	set->beginWriteArray("Shortcuts", shortcuts.size());
	int i = -1;
	for (QMap<QString, QKeySequence>::const_iterator it = shortcuts.begin();
			it != shortcuts.end(); ++it) {
		set->setArrayIndex(++i);
		set->setValue("Name", it.key());
		set->setValue("Key", it.value());
	}
	set->endArray();
	set->endGroup();
}

}
