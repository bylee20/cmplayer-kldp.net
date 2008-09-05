#ifndef XINEINFORMATIONS_H
#define XINEINFORMATIONS_H

#include <QStringList>

namespace Xine {

class Informations {
	class Extensions : public QStringList {
	public:
		QString toFilter() const;
		QStringList toNameFilter() const;
	};
public:
	inline static Informations *get() {static Informations obj; return &obj;}
	void setPrivatePath(const QString &path);
	const QString &compileVersion() const;
	const QString &runtimeVersion() const;
	const QString &privatePath() const;
	const QStringList &videoOutputs() const;
	const QStringList &audioOutputs() const;
	const Extensions &videoExtensions() const;
	const Extensions &audioExtensions() const;
	const Extensions &subtitleFilter() const;
	bool isPlayable(const QString ex) const;
private:
	Q_DISABLE_COPY(Informations)
	Informations();
	struct Data;
	friend struct Data;
	Data *d;
};

}

#endif
