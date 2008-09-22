#ifndef PLAYMENUBAR_H
#define PLAYMENUBAR_H

#include <QWidget>

class PlayMenuBar : public QWidget {
	Q_OBJECT
public:
	PlayMenuBar(QWidget *parent = 0);
	~PlayMenuBar();
	void init(const QList<QWidget *> &tools);
public slots:
	void setTotalTime(int msec);
	void setCurrentTime(int msec);
	void setPlayText(const QString &text);
private:
	struct Data;
	friend struct Data;
	Data *d;
};

#endif
