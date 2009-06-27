#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QObject>

namespace Core {

class Config {
public:
	class Obj;
	class Item {
	public:
		Item(const QString &name, const QString &desc, const QVariant &data)
		: m_name(name), m_desc(desc), m_data(data) {}
		const QString &name() const {return m_name;}
		const QString &description() const {return m_desc;}
		const QVariant &data() const {return m_data;}
		void setData(const QVariant &data) {m_data = data;}
	private:
		Item() {}
		friend class QMap<QString, Item>;
		QString m_name, m_desc;
		QVariant m_data;
	};
	typedef QMap<QString, Item> Map;
	virtual ~Config() {}
	virtual const Map &item() const = 0;
	QVariant data(const QString &name) const {return item()[name].data();}
	virtual void setData(const QString &name, const QVariant &data) const = 0;
	virtual void apply() const;
	void load() const;
	void save() const;
protected:
	virtual QString id() const = 0;
	virtual Obj *obj() const = 0;
};

class Config::Obj : public QObject {
	Q_OBJECT
signals:
	void needToBeUpdated();
private:
	void update() {emit needToBeUpdated();}
	friend class Config;
};

}

#endif // CONFIG_H
