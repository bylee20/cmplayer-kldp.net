#ifndef APPSTATE_HPP
#define APPSTATE_HPP

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QVector>

class AppState {
public:
	typedef QMap<QString, QVariant> Map;
	enum Type {
		AspectRatio = 0,
		Crop,
		PlaySpeed,
//		VideoRenderer,
//		AudioRenderer,
		Volume,
		Muted,
		Amp,
		SubPos,
		SubSync,
		LastOpenFile,
//		ToolBoxRect,
		TrayFirst,
//		PanelLayout,
		SoftEq,
		VolNorm,
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

#endif // APPSTATE_HPP
