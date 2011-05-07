#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QSize>

#define CONNECT(a, b, c, d) (QObject::connect(a, SIGNAL(b), c, SLOT(d)))

class QDialogButtonBox;		class QDialog;

namespace Global {

enum MediaState {StoppedState = 0, PlayingState, PausedState, FinishedState};
enum MediaStatus {NoMediaStatus = 0, EosStatus, BufferedStatus};
enum StreamType {UnknownStream = 0, VideoStream, AudioStream, SubPicStream};
enum MediaMetaData {LanguageCode};

class FrameRateMeasure {
public:
	FrameRateMeasure() {m_drawn = 0; m_prev = 0;}
	void reset() {m_time.restart(); m_drawn = 0; m_prev = 0;}
	int elapsed() const {return m_time.elapsed();}
	void frameDrawn(int id) {
		if (m_prev != id) {
			++m_drawn;
			m_prev = id;
		}
	}
	int drawnFrames() const {return m_drawn;}
	double frameRate() const {return (double)m_drawn/(double)m_time.elapsed()*1e3;}
private:
	int m_drawn;
	int m_prev;
	QTime m_time;
};

QTime secsToTime(int secs);
QTime msecsToTime(qint64 msecs);
QString msecsToString(qint64 msecs, const QString &format = QLatin1String("hh:mm:ss"));
QString secsToString(int secs, const QString &format = QLatin1String("hh:mm:ss"));
qint64 timeToMSecs(const QTime &time);
qint64 timeToMSecs(int h, int m, int s, int ms = 0);
qint64 stringToMSecs(const QString &str, const QString &format = QLatin1String("hh:mm:ss"));
const QTime &nullTime();

QStringList getOpenFileNames(QWidget *p, const QString &t, const QString &dir, const QString &f);
QString getOpenFileName(QWidget *p, const QString &t, const QString &dir, const QString &f);
QString getSaveFileName(QWidget *p, const QString &t, const QString &dir, const QString &f);

static inline QString toString(int value, bool sign = true) {
	QString ret;
	if (sign && value >= 0)
		ret = (value ? QLatin1String("+") : QString::fromUtf8("±"));
	return (ret += QString::number(value));
}

static inline QString toString(double value, bool sign = true) {
	QString ret;
	if (sign && value >= 0)
		ret = (value ? QLatin1String("+") : QString::fromUtf8("±"));
	return (ret += QString::number(value));
}

QDialogButtonBox *makeButtonBox(QDialog *dlg);

}

using namespace Global;

#endif // GLOBAL_HPP
