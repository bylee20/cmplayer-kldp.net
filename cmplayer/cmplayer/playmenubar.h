#ifndef PLAYMENUBAR_H
#define PLAYMENUBAR_H

#include <QtGui/QWidget>

namespace Backend {class PlayEngine;}

class PlayMenuBar : public QWidget {
	Q_OBJECT
public:
	PlayMenuBar(const QList<QAction*> &acts, QWidget *parent = 0);
	~PlayMenuBar();
	void setPlayEngine(Backend::PlayEngine *engine);
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
