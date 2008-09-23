#ifndef BACKEND_AVOUTPUT_H
#define BACKEND_AVOUTPUT_H

#include <QtCore/QObject>

namespace Backend {

class PlayEngine;

class AVOutput : public QObject {
	Q_OBJECT
public:
	AVOutput(PlayEngine *engine);
	~AVOutput() {}
	PlayEngine *engine() {return m_engine;}
	const PlayEngine *engine() const {return m_engine;}
private:
	PlayEngine *m_engine;
};

}

#endif
