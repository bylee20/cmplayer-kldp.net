#ifndef HELPER_H
#define HELPER_H

class QString;

class Helper {
public:
	~Helper();
	static const QString &configFile();
	static const QString &appPath();
	static const QString &recentFile();
	static const QString &homePath();
	static const QString &privatePath();
	static const char *version() {return "0.2.0";}
private:
	static Helper *get() {static Helper obj; return &obj;}
	struct Data;
	friend struct Data;
	Helper();
	Data *d;
};

#endif
