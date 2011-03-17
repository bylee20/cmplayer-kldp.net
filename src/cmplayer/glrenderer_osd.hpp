#ifndef GLRENDERER_OSD_HPP
#define GLRENDERER_OSD_HPP

#include "osdrenderer.hpp"
#include "glrenderer.hpp"
#include <QtCore/QObject>

class GLRenderer::Osd : public QObject {
	Q_OBJECT
public:
	Osd(OsdRenderer *osd, GLRenderer *renderer);
	~Osd();
	void add(OsdRenderer *renderer);
	void setBackgroundSize(const QSize &bg);
	void render();
	void render(QPainter *painter);
	GLuint texture() const;
private slots:
	void cache();
	void updateSize(const QSizeF &size);
private:
	QSize cachingSize(const QSizeF &size);
	struct Data;
	Data *d;
};

#endif // GLRENDERER_OSD_HPP
