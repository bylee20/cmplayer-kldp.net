#ifndef PREFERENCESPREFINTERFACE_H
#define PREFERENCESPREFINTERFACE_H

#include <QString>
#include <QKeySequence>
#include <QMap>
#include <QSharedDataPointer>

class QStringList;
class QSettings;

namespace Pref {

class Interface {
public:
	typedef QMap<QString, QKeySequence> KeyMap;
	static const int DefaultSeekingStep = 10000;
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
	Interface(): d(new Data) {}
	static QStringList mouseClickActions();
	static QStringList wheelScrollActions();
	void load(QSettings *set);
	void save(QSettings *set) const;
	int seekingStep() const {return d->seekingStep;}
	int seekingMoreStep() const {return d->seekingMoreStep;}
	int seekingMuchMoreStep() const {return d->seekingMuchMoreStep;}
	int volumeStep() const {return d->volumeStep;}
	int syncDelayStep() const {return d->syncDelayStep;}
	MouseClickAction doubleClickAction() const {return d->doubleClickAction;}
	MouseClickAction middleClickAction() const {return d->middleClickAction;}
	WheelScrollAction wheelScrollAction() const {return d->wheelScrollAction;}
	const KeyMap &shortcuts() const {return d->shortcuts;}
	void setSeekingStep(int step) {d->seekingStep = step;}
	void setSeekingMoreStep(int step) {d->seekingMoreStep = step;}
	void setSeekingMuchMoreStep(int step) {d->seekingMuchMoreStep = step;}
	void setVolumeStep(int step) {d->volumeStep = step;}
	void setSyncDelayStep(int step) {d->syncDelayStep = step;}
	void setDoubleClickAction(MouseClickAction action) {d->doubleClickAction = action;}
	void setMiddleClickAction(MouseClickAction action) {d->middleClickAction = action;}
	void setWheelScrollAction(WheelScrollAction action) {d->wheelScrollAction = action;}
	void setShortcuts(const KeyMap &map) {d->shortcuts = map;}
private:
	struct Data : public QSharedData {
		Data() {}
		Data(const Data &other): QSharedData(other)
		, seekingStep(other.seekingStep), seekingMoreStep(other.seekingMoreStep)
		, seekingMuchMoreStep(other.seekingMuchMoreStep)
		, volumeStep(other.volumeStep), syncDelayStep(other.syncDelayStep)
		, doubleClickAction(other.doubleClickAction)
		, middleClickAction(other.middleClickAction)
		, shortcuts(other.shortcuts) {}
		int seekingStep, seekingMoreStep, seekingMuchMoreStep;
		int volumeStep, syncDelayStep;
		MouseClickAction doubleClickAction, middleClickAction;
		WheelScrollAction wheelScrollAction;
		KeyMap shortcuts;
	};
	QSharedDataPointer<Data> d;
};

}

#endif
