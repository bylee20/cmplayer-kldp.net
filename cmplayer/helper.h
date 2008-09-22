#ifndef HELPER_H
#define HELPER_H

class QString;

namespace Backend {class FactoryIface;}

class Helper {
public:
	~Helper();
	static const QString &configFile();
	static const QString &appPath();
	static const QString &recentFile();
	static const QString &homePath();
	static const QString &privatePath();
	static const char *version() {return "0.3.0";}
	static const Backend::FactoryIface *currentFactory() {return get()->f;}
	static void setCurrentFactory(Backend::FactoryIface *factory) {get()->f = factory;}
private:
	static Helper *get() {static Helper obj; return &obj;}
	struct Data;
	friend struct Data;
	Helper();
	Data *d;
	Backend::FactoryIface *f;
};

#endif
