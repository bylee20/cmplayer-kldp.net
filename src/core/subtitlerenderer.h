#ifndef CORE_SUBTITLERENDERER_H
#define CORE_SUBTITLERENDERER_H

namespace Core {

class Subtitle;				class AbstractOsdRenderer;

class SubtitleRenderer {
public:
	SubtitleRenderer(AbstractOsdRenderer *renderer = 0);
	~SubtitleRenderer();
	void setRenderer(AbstractOsdRenderer *renderer);
	void setSubtitle(const Subtitle *subtitle);
	void clear();
	void show(int time);
	int delay() const;
	void setDelay(int delay);
	AbstractOsdRenderer *renderer();
private:
	struct Data;
	Data *d;
};

}

#endif
