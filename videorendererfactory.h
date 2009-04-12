#ifndef GSTVIDEORENDERERFACTORY_H
#define GSTVIDEORENDERERFACTORY_H

class QString;			class QStringList;

namespace Gst {

class GstVideoIface;		class PlayEngine;

class VideoRendererFactory {
public:
	enum Type {Xv, X11, Software};
	~VideoRendererFactory();
	static GstVideoIface *create(const QString &key, PlayEngine *engine);
	static QStringList keys();
private:
	VideoRendererFactory();
	struct Data;
	static Data &data();
};

}

#endif
