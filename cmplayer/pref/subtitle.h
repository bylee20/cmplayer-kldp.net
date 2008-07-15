#ifndef PREFERENCESPREFSUBTITLE_H
#define PREFERENCESPREFSUBTITLE_H

#include <QString>
#include <QStringList>
//#include <mplayer/subtitleoutput.h>

class QSettings;

namespace Pref {

class Subtitle {
public:
	enum AutoLoad {NoAutoLoad = 0, Matched = 1, Contain = 2, SamePath = 3};
//	MPlayer::SubtitleOutput::Font font;
	QString encoding;
//	MPlayer::SubtitleOutput::AutoScale autoScale;
	AutoLoad autoLoad;
//	MPlayer::SubtitleOutput::AutoSelect autoSelect;
	int initialPos;
	QStringList priority;
	bool displayOnMarginWhenFullScreen;
	void load(QSettings *set);
	void save(QSettings *set) const;
};

}

#endif
