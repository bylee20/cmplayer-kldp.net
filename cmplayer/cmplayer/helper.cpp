#include "helper.h"
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

struct Helper::Data {
	Data(Helper *parent) : p(parent) {
		appPath = QCoreApplication::applicationDirPath();
		QDir home = QDir::home();
		homePath = home.path();
		if (!home.exists(".cmplayer"))
			home.mkdir(".cmplayer");
		privatePath = homePath + "/.cmplayer";
		configFile = privatePath + "/config.ini";
		recentFile = privatePath + "/recent.ini";
	}
	Helper *p;
	QString appPath;
	QString configFile;
	QString homePath;
	QString recentFile;
	QString privatePath;
};

Helper::Helper() {
	d = new Data(this);
	f = 0;
}

Helper::~Helper() {
	delete d;
}

const QString &Helper::privatePath() {
	return get()->d->privatePath;
}

const QString &Helper::configFile() {
	return get()->d->configFile;
}

const QString &Helper::appPath() {
	return get()->d->appPath;
}

const QString &Helper::recentFile() {
	return get()->d->recentFile;
}

const QString &Helper::homePath() {
	return get()->d->homePath;
}

