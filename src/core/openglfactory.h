#ifndef COREOPENGLFACTORY_H
#define COREOPENGLFACTORY_H

namespace Core {

class OpenGLVideoRendererIface;		class AbstractOsdRenderer;
class OpenGLIface;

class OpenGLFactory {
public:
	~OpenGLFactory();
	static OpenGLVideoRendererIface *createVideoRenderer();
	static AbstractOsdRenderer *createOsdRenderer();
	static bool isAvailable();
private:
	OpenGLFactory();
	static OpenGLFactory &get();
	OpenGLIface *iface;
};

}

#endif
