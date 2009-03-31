#ifndef STATE_H
#define STATE_H

#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QVariant>

class State {
public:
	typedef QMap<QString, QVariant> Map;
	enum Type {
		ScreenAspectRatio = 0,
		BackendName,
		ScreenCrop,
		ScreenOnTop,
		PlaySpeed,
		VideoRenderer,
		AudioRenderer,
		AudioVolume,
		AudioMuted,
		AudioAmp,
		SubtitlePos,
		SubtitleSync,
		LastOpenFile,
		DockWidth,
		TrayFirst,
		TypeMax
	};
	const QString &key(Type type) const {return d.keys[type];}
	QVariant &operator[](Type type) {return d.values[type];}
	const QVariant &operator[](Type type) const {return d.values[type];}
	void save() const;
	void load();
private:
	struct Data {
		Data();
		QVector<QString> keys;
		QVector<QVariant> values;
	};
	static Data d;
};

#endif
