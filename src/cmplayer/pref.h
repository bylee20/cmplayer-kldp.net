#ifndef PREFPREF_H
#define PREFPREF_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <core/mediasource.h>
#include "enum.h"

class QLocale;

namespace Core {class OsdStyle;}

class Pref {
public:
	typedef QPair<bool, ClickActionEnum> ClickActionPair;
	typedef QPair<bool, WheelActionEnum> WheelActionPair;
	typedef QMap<ModifierEnum, ClickActionPair> ClickActionMap;
	typedef QMap<ModifierEnum, WheelActionPair> WheelActionMap;
	static Pref *get();
	~Pref();
	
	SubtitleAutoLoad subtitleAutoLoad() const;
	SubtitleAutoSelect subtitleAutoSelect() const;
	const Core::OsdStyle &subtitleStyle() const;
	const QString &subtitleEncoding() const;
	const QStringList &subtitlePriority() const;
	
	int seekingStep(SeekingStep step) const;
	int seekingStep1() const;
	int seekingStep2() const;
	int seekingStep3() const;
	int speedStep() const;
	int volumeStep() const;
	int ampStep() const;
	int subtitlePosStep() const;
	int syncDelayStep() const;
	int brightnessStep() const;
	int saturationStep() const;
	int contrastStep() const;
	int hueStep() const;

	const ClickActionMap &doubleClickMap() const;
	const ClickActionMap &middleClickMap() const;
	const WheelActionMap &wheelScrollMap() const;
	
	bool rememberStopped() const;
	AutoAddFiles autoAddFiles() const;
	bool pauseWhenMinimized() const;
	bool pauseVideoOnly() const;
	bool playWhenRestored() const;
	bool hideCursor() const;
	int hideDelay() const;
	bool hideInFullScreen() const;
	const QLocale &locale() const;
	bool isVolumeNormalized() const;
	bool useSoftwareEqualizer() const;
	bool isSystemTrayEnabled() const;
	bool hideWhenClosed() const;
	bool singleApplication() const;
	
	const QString &backendName(Core::MediaType media) const;
	
	void load();
private:
	void setSubtitleAutoLoad(SubtitleAutoLoad mode);
	void setSubtitleAutoSelect(SubtitleAutoSelect mode);
	void setSubtitleStyle(const Core::OsdStyle &style);
	void setSubtitleEncoding(const QString &encoding);
	void setSubtitlePriority(const QStringList &priority);
	
	void setSeekingStep1(int step);
	void setSeekingStep2(int step);
	void setSeekingStep3(int step);
	void setVolumeStep(int step);
	void setSyncDelayStep(int step);
	void setSpeedStep(int step);
	void setAmpStep(int step);
	void setSubtitlePosStep(int step);
	void setBrightnessStep(int step);
	void setSaturationStep(int step);
	void setContrastStep(int step);
	void setHueStep(int step);
	void setDoubleClickMap(const ClickActionMap &map);
	void setMiddleClickMap(const ClickActionMap &map);
	void setWheelScrollMap(const WheelActionMap &map);
		
	void setRememberStopped(bool remember);
	void setAutoAddFiles(AutoAddFiles mode);
	void setPauseWhenMinimized(bool enabled);
	void setPauseVideoOnly(bool enabled);
	void setPlayWhenRestored(bool enabled);
	void setHideCursor(bool enabled);
	void setHideDelay(int delay);
	void setHideInFullScreen(bool enabled);
	void setLocale(const QLocale &locale);
	void setVolumeNormalized(bool on);
	void setUseSoftwareEqualizer(bool use);
	void setSystemTrayEnabled(bool enabled);
	void setHideWhenClosed(bool enabled);
	void setSingleApplication(bool enabled);
		
	void setBackendName(Core::MediaType media, const QString &name);
	
	void save() const;
	friend class PrefDialog;
	Pref();
	struct Backend; struct Subtitle; struct Interface; struct General;
	Subtitle *sub; Interface *iface; General *gen; Backend *back;
};

#endif
