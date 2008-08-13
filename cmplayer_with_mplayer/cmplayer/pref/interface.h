#ifndef PREFERENCESPREFINTERFACE_H
#define PREFERENCESPREFINTERFACE_H

#include <QString>
#include <QKeySequence>
#include <QMap>

class QStringList;
class QSettings;

namespace Pref {

class Interface {
public:
	static const int DefaultSeekingStep = 5000;
	static const int DefaultSeekingMoreStep = 30000;
	static const int DefaultSeekingMuchMoreStep = 60000;
	static const int DefaultVolumeStep = 1;
	static const int DefualtSyncDelayStep = 500;
	enum MouseClickAction {
		ToggleFullScreen = 0,
		TogglePlayPause = 1
	};
	enum WheelScrollAction {
		ForwardBackward = 0,
		ForwardBackwardMore = 1,
		ForwardBackwardMuchMore = 2,
		NextPrevious = 3,
		VolumeUpDown = 4
	};
	static QStringList mouseClickActions();
	static QStringList wheelScrollActions();
	int seekingStep;
	int seekingMoreStep;
	int seekingMuchMoreStep;
	int volumeStep;
	int syncDelayStep;
	MouseClickAction doubleClickAction;
	MouseClickAction middleClickAction;
	WheelScrollAction wheelScrollAction;
	QMap<QString, QKeySequence> shortcuts;
	void load(QSettings *set);
	void save(QSettings *set) const;
};

}

#endif
