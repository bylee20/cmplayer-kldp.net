#ifndef PREFERENCESPREFSUBTITLE_H
#define PREFERENCESPREFSUBTITLE_H

#include <QString>
#include <QStringList>
#include <xine/subtitleoutput.h>
#include <xine/xineosd.h>

class QSettings;

namespace Pref {

class Subtitle {
public:
	enum AutoLoad {NoAutoLoad = 0, Matched = 1, Contain = 2, SamePath = 3};
	Xine::XineOsd::Style osdStyle;
	QString encoding;
	AutoLoad autoLoad;
	Xine::SubtitleOutput::AutoSelect autoSelect;
	int initialPos;
	QStringList priority;
	bool displayOnMarginWhenFullScreen;
	void load(QSettings *set);
	void save(QSettings *set) const;
};

}

#endif
