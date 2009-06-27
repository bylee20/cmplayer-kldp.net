#ifndef XINE_CONFIG_H
#define XINE_CONFIG_H

#include <core/config.h>
#include <QtCore/QObject>

namespace Xine {

class Config : public Core::Config {
public:
	virtual const Map &item() const;
	virtual void setData(const QString &name, const QVariant &data) const;
	virtual Obj *obj() const {return &d.obj;}
private:
	virtual QString id() const;
	struct Data {
		Data();
		Map map;
		Obj obj;
	};
	static Data d;
};

}

#endif // CONFIG_H
