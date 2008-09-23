#include "playlist.h"
#include "mediasource.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtCore/QUrl>

namespace Backend {

PlayList::PlayList()
: QList<MediaSource>() {}

PlayList::PlayList(const PlayList &rhs)
: QList<MediaSource>(rhs) {}

PlayList::PlayList(const QList<MediaSource> &rhs)
: QList<MediaSource>(rhs) {}

bool PlayList::save(const QString &filePath) const {
	QFile file(filePath);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return false;
	QTextStream out(&file);
	const int count = size();
	out << "[playlist]" << endl << "NumberOfEntries=" << count << endl << endl;
	for (int i=0; i<count; ++i)
		out << "File" << i+1 << '=' << at(i).url().toString() << endl
				<< "Length" << i+1 << '=' << -1 << endl << endl;
	out << "Version=2" << endl;
	return true;
}

bool PlayList::load(const QString &filePath) {
	clear();
	QFile file(filePath);
	if (!file.open(QFile::ReadOnly))
		return false;
	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.isEmpty())
			continue;
		static QRegExp rxFile("^File\\d+=(.+)$");
		if (rxFile.indexIn(line) != -1)
			append(MediaSource(QUrl(rxFile.cap(1))));
	}
	return true;
}

}
