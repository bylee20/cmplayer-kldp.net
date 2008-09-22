#include "avoutput.h"
#include "../playengine.h"

namespace Backend {

AVOutput::AVOutput(PlayEngine *engine)
: QObject(engine) {
	m_engine = engine;
}

}
