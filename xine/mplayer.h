#ifndef MPLAYER_H
#define MPLAYER_H

namespace MPlayer {

enum State {
	StoppedState = 1,
	PlayingState = 2,
	PausedState = 4
};

enum DiscType {
	NoDisc = 0,
	DVD = 1,
	VCD = 2
};

}

#endif
