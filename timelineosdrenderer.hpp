#ifndef TIMELINEOSDRENDERER_HPP
#define TIMELINEOSDRENDERER_HPP

#include "osdrenderer.hpp"

class TimeLineOsdRenderer : public OsdRenderer {
	Q_OBJECT
public:
	TimeLineOsdRenderer();
	~TimeLineOsdRenderer();
	void show(int pos, int duration, int last = 2500);
	void render(QPainter *painter);
public slots:
	void clear();
private:
	void areaChanged(const QRect &area);
	void styleChanged(const OsdStyle &style);

	QPoint posHint() const;
	QSize sizeHint() const;
	struct Data;
	Data *d;

};

#endif // TIMELINEOSDRENDERER_HPP
