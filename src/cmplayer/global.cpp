#include "global.hpp"
#include <QtGui/QFileDialog>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QX11Info>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDialog>

namespace Global {

struct Data {
	Data(): zero() {

	}
	const QTime zero;
};

static Data *data() {
	static Data d;
	return &d;
}

QTime secsToTime(int secs) {
	return data()->zero.addSecs(secs);
}

QTime msecsToTime(qint64 msecs) {
	return data()->zero.addMSecs(msecs);
}

QString msecsToString(qint64 msecs, const QString &format) {
	return msecsToTime(msecs).toString(format);
}

QString secsToString(int secs, const QString &format) {
	return secsToTime(secs).toString(format);
}

qint64 timeToMSecs(int h, int m, int s, int ms) {
	return h*3600000 + m*60000 + s*1000 + ms;
}

qint64 timeToMSecs(const QTime &time) {
	return data()->zero.msecsTo(time);
}

qint64 stringToMSecs(const QString &str, const QString &format) {
	return timeToMSecs(QTime::fromString(str, format));
}

const QTime &nullTime() {
	return data()->zero;
}

QStringList getOpenFileNames(QWidget *p, const QString &t, const QString &dir, const QString &f) {
	return QFileDialog::getOpenFileNames(p, t, dir, f, 0, QFileDialog::DontUseNativeDialog);
}

QString getOpenFileName(QWidget *p, const QString &t, const QString &dir, const QString &f) {
	return QFileDialog::getOpenFileName(p, t, dir, f, 0, QFileDialog::DontUseNativeDialog);
}

QString getSaveFileName(QWidget *p, const QString &t, const QString &dir, const QString &f) {
	return QFileDialog::getSaveFileName(p, t, dir, f, 0, QFileDialog::DontUseNativeDialog);
}

QDialogButtonBox *makeButtonBox(QDialog *dlg) {
	QDialogButtonBox *dbb = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, dlg);
	dlg->connect(dbb, SIGNAL(accepted()), SLOT(accept()));
	dlg->connect(dbb, SIGNAL(rejected()), SLOT(reject()));
	return dbb;
}


}
