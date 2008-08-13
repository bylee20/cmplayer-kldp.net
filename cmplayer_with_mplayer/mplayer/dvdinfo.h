#ifndef MPLAYER_DVDINFO_H
#define MPLAYER_DVDINFO_H

#include <QVector>
#include "subtitle.h"

namespace MPlayer {

struct DVDInfo {
public:
	struct Title {
		Title() : number(1), angles(0), length(0) {}
		QVector<qint64> chapters;
		int number, angles;
		qint64 length;
	};
	QVector<Title> titles;
	QList<Subtitle> subtitles;
};

}

#endif
