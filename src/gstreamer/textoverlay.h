#ifndef TEXTOVERLAY_H
#define TEXTOVERLAY_H

#include <core/abstractosdrenderer.h>
#include <gst/gst.h>

namespace Gst {

class TextOverlay : public Core::AbstractOsdRenderer {
public:
	TextOverlay();
	~TextOverlay();
	GstElement *element();
	void render();
	void clear();
	void updateFontSize(const QSize &size);
private:
	void updateFont();
	void updateStyle(const Core::OsdStyle &style);
	struct Data;
	Data *d;
};

} // namespace Gst
#endif // TEXTOVERLAY_H
