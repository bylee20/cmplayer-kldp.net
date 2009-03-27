#ifndef BACKEND_INFO_H
#define BACKEND_INFO_H

#include <QtCore/QStringList>
#include "namespace.h"

namespace Core {

class Info {
public:
	class ExtensionList : public QStringList {
	public:
		ExtensionList() {}
		ExtensionList(const QStringList &other): QStringList(other) {}
		ExtensionList(const ExtensionList &other): QStringList(other) {}
		QString toFilter() const;
		QStringList toNameFilter() const;
	};
	Info();
	virtual ~Info();
	static const QString &privatePath() {return d.privatePath;}
	static const QString &pluginPath() {return d.pluginPath;}
	static const ExtensionList &videoExtension() {return d.videoExt;}
	static const ExtensionList &audioExtension() {return d.audioExt;}
	static const ExtensionList &subtitleExtension() {return d.subExt;}
	static QString coreVersion() {return "0.2.7";}
	virtual const QString &defaultVideoRenderer() const {return videoRenderer()[0];}
	virtual const QString &defaultAudioRenderer() const {return audioRenderer()[0];}
	virtual const QStringList &videoRenderer() const = 0;
	virtual const QStringList &audioRenderer() const = 0;
	virtual QString name() const = 0;
	virtual double maximumAmplifyingRate() const = 0;
	virtual QString compileVersion() const = 0;
	virtual QString runtimeVersion() const = 0;
private:
	struct Data {
		Data();
		ExtensionList videoExt, audioExt, subExt;
		QString privatePath, pluginPath;
	};
	static Data d;
};

}

#endif
