#ifndef MPLAYERPLAYLIST_H
#define MPLAYERPLAYLIST_H

#include <QList>
#include "mediasource.h"

namespace Xine {

class PlayList : public QList<MediaSource> {
public:
	PlayList();
	PlayList(const PlayList &rhs);
	PlayList(const QList<MediaSource> &rhs);
	bool save(const QString &filePath) const;
	bool load(const QString &filePath);
};

}

#endif
