#ifndef ABREPEATER_H
#define ABREPEATER_H

#include <QObject>

namespace Xine {

class XineStream;

class ABRepeater : public QObject {
	Q_OBJECT
public:
	bool repeat(int a, int b, int times = 0) {m_a = a; m_b = b; return start(times);}
	bool isRepeating() {return m_repeating;}
	int a() const {return m_a;}
	int b() const {return m_b;}
public slots:
	void stop();
	bool start(int times = 0);
	int setAToCurrentTime();
	int setBToCurrentTime();
	int setAToSubtitleTime();
	int setBToSubtitleTime();
	void setA(int a) {m_a = a;}
	void setB(int b) {m_b = b;}
private slots:
	void slotTick(int time);
private:
	ABRepeater(XineStream *parent);
	friend class XineStream;
	XineStream *m_stream;
	int m_a, m_b;
	bool m_repeating;
	int m_times, m_nth;
};

}

#endif
