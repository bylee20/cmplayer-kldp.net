#ifndef TEXTOVERLAY_H
#define TEXTOVERLAY_H

#include <core/abstractosdrenderer.h>
#include <gst/gst.h>

namespace Gst {

class TextOverlay : public Core::AbstractOsdRenderer {
	Q_OBJECT
public:
	TextOverlay();
	~TextOverlay();
	GstElement *bin();
	void render();
	void clear();
	void updateFontSize(const QSize &size);
public slots:
	void setRect(const QRect &rect);
private:
	void updateFont();
	void updatePos();
	void updateStyle(const Core::OsdStyle &style);
	struct Data;
	Data *d;
};

} // namespace Gst
#endif // TEXTOVERLAY_H
