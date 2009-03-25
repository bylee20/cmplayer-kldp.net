#ifndef CORE_SUBTITLERENDERER_H
#define CORE_SUBTITLERENDERER_H

namespace Core {

class Subtitle;				class AbstractOsdRenderer;

class SubtitleRenderer {
public:
	SubtitleRenderer(AbstractOsdRenderer *renderer = 0);
	~SubtitleRenderer();
	void setRenderer(AbstractOsdRenderer *renderer);
	void setSubtitle(const Subtitle *sub, double frameRate);
	void clear();
	void show(int time);
	int delay() const;
	void setDelay(int delay);
	void setFrameRate(double frameRate);
	AbstractOsdRenderer *renderer();
private:
	struct Data;
	Data *d;
};

}

#endif
