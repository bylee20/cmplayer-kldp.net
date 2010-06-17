#ifndef SUBTITLERENDERER_HPP
#define SUBTITLERENDERER_HPP

#include "subtitle.hpp"
#include <QtCore/QObject>

class OsdRenderer;

class SubtitleRenderer : public QObject {
	Q_OBJECT
public:
	SubtitleRenderer();
	~SubtitleRenderer();
	OsdRenderer *osd() const;
	void setSubtitle(const Subtitle &subtitle);
public slots:
	void setFrameRate(double frameRate);
	void render(int ms);
private:
	struct Data;
	Data *d;
};

#endif // SUBTITLERENDERER_HPP
