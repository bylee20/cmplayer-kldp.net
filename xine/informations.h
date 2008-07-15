//#ifndef MPLAYERINFORMATIONS_H
//#define MPLAYERINFORMATIONS_H
//
//#include <QStringList>
//
//namespace MPlayer {
//
//class Informations {
//	class Extensions : public QStringList {
//	public:
//		QString toFilter() const;
//		QStringList toNameFilter() const;
//	};
//public:
//	inline static Informations *get() {static Informations obj; return &obj;}
//	void setMPlayerPath(const QString &path);
//	void setPrivatePath(const QString &path);
//	const QString &version() const;
//	const QString &privatePath() const;
//	const QString &mplayerPath() const;
//	const QStringList &videoOutputs() const;
//	const QStringList &audioOutputs() const;
//	const Extensions &videoExtensions() const;
//	const Extensions &audioExtensions() const;
//	const Extensions &subtitleFilter() const;
//	bool isPlayable(const QString ex) const;
//private:
//	Q_DISABLE_COPY(Informations)
//	void getInfo() const;
//	Informations();
//	struct Data;
//	friend struct Data;
//	Data *d;
//};
//
//}
//
//#endif
