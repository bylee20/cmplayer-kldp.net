#ifndef SUBTITLERENDERER_HPP
#define SUBTITLERENDERER_HPP

#include "subtitle.hpp"
#include "textosdrenderer.hpp"
#include <QtCore/QObject>

class SubtitleRenderer : public QObject {
	Q_OBJECT
public:
	SubtitleRenderer();
	~SubtitleRenderer();
	void setOsd(TextOsdRenderer *osd);
	TextOsdRenderer *osd() const;
	void setSubtitle(const Subtitle &subtitle);
	const Subtitle &subtitle() const;
	double frameRate() const;
	int delay() const;
	int start(int pos) const;
	int end(int pos) const;
	void setDelay(int delay);
	bool hasSubtitle() const;
	double pos() const;
	void setPos(double pos);
public slots:
	void clear();
	void setFrameRate(double frameRate);
	void render(int ms);
	void setVisible(bool visible);
	void setHidden(bool hidden) {setVisible(!hidden);}
private:
	struct Data;
	Data *d;
};

#endif // SUBTITLERENDERER_HPP
