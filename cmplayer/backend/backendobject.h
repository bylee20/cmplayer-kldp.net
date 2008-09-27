#ifndef BACKEND_BACKENDOBJECT_H
#define BACKEND_BACKENDOBJECT_H

#include <QtCore/QString>

class QObject;

namespace Backend {

class Info;								class Config;
class PlayEngine;

class BackendObject {
public:
	static void setCurrentBackend(BackendObject *obj) {current = obj;}
	static BackendObject *currentBackend() {return current;}
	~BackendObject();
	Info *info() const;
	Config *config() const;
	PlayEngine *createPlayEngine(QObject *parent = 0) const;
	const QString &name() const;
	const QString &fileName() const;
	const QString &filePath() const;
	const QString &description() const;
	bool isValid() const;
private:
	static BackendObject *current;
	BackendObject(const QString &filePath);
	friend class Manager;
	struct Data;
	Data *d;
};

}

#endif
