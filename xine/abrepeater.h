//#ifndef ABREPEATER_H
//#define ABREPEATER_H
//
//#include <QObject>
//
//namespace MPlayer {
//
//class PlayEngine;
//
//class ABRepeater : public QObject {
//	Q_OBJECT
//public:
//	~ABRepeater();
//	bool repeat(qint64 a, qint64 b, int times = 0);
//	bool isRepeating();
//	qint64 a() const;
//	qint64 b() const;
//public slots:
//	void stop();
//	bool start(int times = 0);
//	qint64 setAToCurrentTime();
//	qint64 setBToCurrentTime();
//	qint64 setAToSubtitleTime();
//	qint64 setBToSubtitleTime();
//	void setA(qint64 a);
//	void setB(qint64 b);
//private slots:
//	void slotTick(qint64 time);
//private:
//	ABRepeater(PlayEngine *parent);
//	friend class PlayEngine;
//	struct Data;
//	Data *d;
//
//};
//
//}
//
//#endif
