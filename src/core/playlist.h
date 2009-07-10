#ifndef CORE_PLAYLIST_H
#define CORE_PLAYLIST_H

#include <QtCore/QList>
#include "mediasource.h"

class QFile;

namespace Core {

class Mrl;

class Playlist : public QList<MediaSource> {
public:
	Playlist();
	Playlist(const Playlist &rhs);
	Playlist(const QList<MediaSource> &rhs);
	bool save(const QString &filePath) const;
	bool load(const QString &filePath, const QString &enc = QString());
	bool load(const Mrl &url, const QString &enc = QString());
	bool load(QFile *file, const QString &enc = QString());
};

}

#endif
