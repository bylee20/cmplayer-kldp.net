#include "helper.h"
#include <core/info.h>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

struct Helper::Data {
	Data(Helper *parent) : p(parent) {
		const QString pp = Core::Info::privatePath();
		configFile = pp + "/config.ini";
		recentFile = pp + "/recent.ini";
		stateFile = pp + "/state.ini";
	}
	Helper *p;
	QString configFile;
	QString recentFile;
	QString stateFile;
};

Helper::Helper(): d(new Data(this)) {}

Helper::~Helper() {
	delete d;
}

const QString &Helper::configFile() {
	return get()->d->configFile;
}

const QString &Helper::recentFile() {
	return get()->d->recentFile;
}

const QString &Helper::stateFile() {
	return get()->d->stateFile;
}

QString Helper::mediaExtensionFilter() {
	return  QCoreApplication::translate("Helper", "Video Files") +' '
			+ Core::Info::videoExtension().toFilter() + ";;"
			+ QCoreApplication::translate("Helper", "Audio Files") + ' '
			+ Core::Info::audioExtension().toFilter() + ";;"
			+ QCoreApplication::translate("Helper", "All Files") + ' ' + "(*.*)";
}
