#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include "mrl.hpp"

class QFile;

class Playlist : public QList<Mrl> {
public:
	enum Type {Unknown, PLS, M3U};
	Playlist();
	Playlist(const Playlist &rhs);
	Playlist(const QList<Mrl> &rhs);
	bool save(const QString &filePath, Type type = Unknown) const;
	bool load(const QString &filePath, const QString &enc = QString(), Type type = Unknown);
	bool load(const Mrl &url, const QString &enc = QString(), Type type = Unknown);
	bool load(QFile *file, const QString &enc = QString(), Type type = Unknown);
private:
	static Type getType(const QString &fileName);
	bool savePLS(QFile *file) const;
	bool saveM3U(QFile *file) const;
	bool loadPLS(QFile *file, const QString &enc);
	bool loadM3U(QFile *file, const QString &enc);
};

#endif // PLAYLIST_HPP
