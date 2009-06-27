#include "playlist.h"
#include "mediasource.h"
#include "downloader.h"
#include "info.h"
#include "mrl.h"
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtCore/QTextCodec>

namespace Core {

Playlist::Playlist()
: QList<MediaSource>() {}

Playlist::Playlist(const Playlist &rhs)
: QList<MediaSource>(rhs) {}

Playlist::Playlist(const QList<MediaSource> &rhs)
: QList<MediaSource>(rhs) {}

bool Playlist::save(const QString &filePath) const {
	QFile file(filePath);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return false;
	QTextStream out(&file);
	const int count = size();
	out << "[playlist]" << endl << "NumberOfEntries=" << count << endl << endl;
	for (int i=0; i<count; ++i)
		out << "File" << i+1 << '=' << at(i).mrl().toString() << endl
				<< "Length" << i+1 << '=' << -1 << endl << endl;
	out << "Version=2" << endl;
	return true;
}

bool Playlist::load(const QString &filePath, const QString &enc) {
	QFile file(filePath);
	return load(&file, enc);
}

bool Playlist::load(QFile *file, const QString &enc) {
	clear();
	if (!file->isOpen() && !file->open(QFile::ReadOnly))
		return false;
	QTextStream in(file);
	if (!enc.isEmpty())
		in.setCodec(QTextCodec::codecForName(enc.toLocal8Bit()));
	in.seek(0);
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.isEmpty())
			continue;
		static QRegExp rxFile("^File\\d+=(.+)$");
		if (rxFile.indexIn(line) != -1)
			append(MediaSource(Mrl(rxFile.cap(1))));
	}
	return true;
}

bool Playlist::load(const Mrl &mrl, const QString &enc) {
	if (mrl.scheme().toLower() == "file")
		return load(mrl.toLocalFile(), enc);
	QTemporaryFile file(Core::Info::privatePath() + "/temp_XXXXXX.pls");
	if (!file.open() || !Downloader::get(mrl.url(), &file, 30000))
		return false;
	return load(&file, enc);
}

}
