#ifndef HELPER_H
#define HELPER_H

class QString;

class Helper {
public:
	~Helper();
	static const QString &configFile();
	static const QString &recentFile();
	static const QString &stateFile();
	static QString mediaExtensionFilter();
// 	static void drawBackground(QPainter *painter, int title);
private:
	static Helper *get() {static Helper obj; return &obj;}
	struct Data;
	friend struct Data;
	Helper();
	Data *d;
};

#endif
