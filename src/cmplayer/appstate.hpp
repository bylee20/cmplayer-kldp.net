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
		Volume,
		Muted,
		Amp,
		SubPos,
		SubSync,
		LastOpenFile,
		OpenUrlList,
		UrlEncoding,
		TrayFirst,
		VolNorm,
		StaysOnTop,
		SubLetterbox,
		SubAlignTop,
		OverlayType,
		TypeMax
	};
	const QString &key(Type type) const {return d->keys[type];}
	QVariant &operator[](Type type) {return d->values[type];}
	const QVariant &operator[](Type type) const {return d->values[type];}
	void save() const;
	void load();
	static void init();
	static void fin() {delete d;}
private:
	struct Data {
		QVector<QString> keys;
		QVector<QVariant> values;
	};
	static Data *d;
};

#endif // APPSTATE_HPP
