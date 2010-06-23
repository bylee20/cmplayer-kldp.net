#ifndef SUBTITLEVIEW_HPP
#define SUBTITLEVIEW_HPP

#include <QtGui/QWidget>

class PlayEngine;		class Subtitle;

class SubtitleView : public QWidget {
	Q_OBJECT
public:
	SubtitleView(const PlayEngine *engine, QWidget *parent = 0);
	~SubtitleView();
	void setSubtitle(const Subtitle &sub);
	void setFrameRate(double fps);
private slots:
	void updateCurrent(int time);
private:
	class CompView;
	struct Data;
	Data *d;
};

#endif // SUBTITLEVIEW_HPP
