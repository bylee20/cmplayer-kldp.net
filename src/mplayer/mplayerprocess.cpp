#include "mplayerprocess.h"
#include "playengine.h"
#include "mediainfo.h"
#include <core/utility.h>

namespace MPlayer {

struct MPlayerProcess::Data {
	PlayEngine *engine;
	MediaInfo *info;
	int dvdIdx;
};

MPlayerProcess::MPlayerProcess(PlayEngine *engine)
: QProcess(engine), d(new Data) {
	d->engine = engine;
	d->info = 0;
	d->dvdIdx = -1;
	connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(interpretMessages()));
}

MPlayerProcess::~MPlayerProcess() {
	delete d;
}

void MPlayerProcess::interpretMessages() {
	static QRegExp rxLineBreak("[\\n\\r]");
	bool skip = false;

	QStringList lines = QString::fromLocal8Bit(readAllStandardOutput()).split(rxLineBreak);
	for (int i=0; i<lines.size(); ++i) {
		const QString &msg = lines[i];
		if (msg.isEmpty())
			continue;
		bool matched = false;
		if (d->engine) {
			static QRegExp rxAV("^[AV]: *([0-9,:.-]+)");
			static QRegExp rxStated("^Playing (.+)");
			static QRegExp rxFinished("^Exiting\\.+\\s+\\(End of file\\)");
			if ((matched = (rxAV.indexIn(msg) != -1))) {
				skip = true;
				const qint64 msec = static_cast<int>(rxAV.cap(1).toDouble()*1000);
				d->engine->setState(Core::Playing);
				d->engine->setCurrentTime(msec);
			} else if ((matched = msg.contains(" PAUSE "))) {
				d->engine->setState(Core::Paused);
			} else if ((matched = (rxStated.indexIn(msg) != -1 && (!d->info || d->info->isValid())))) {
				emit d->engine->started();
			} else if ((matched = (rxFinished.indexIn(msg) != -1))) {
				d->engine->exiting();
			}
		} else if (!matched && d->info && !d->info->isValid()) {
			static QRegExp rxID("^ID_(.*)=(.*)");
			static QRegExp rxNoVideo("^Video:\\s+no video");
			static QRegExp rxVO("^VO: \\[(.*)\\] (\\d+)x(\\d+) => (\\d+)x(\\d+)");
			static QRegExp rxAudio("^AID_(\\d+)_(LANG|NAME)=(.*)");
			if (rxID.indexIn(msg) != -1) {
				const QString id = rxID.cap(1);
				if (id == "LENGTH") {
					d->info->m_length = rxID.cap(2).toDouble()*1000.0;
				} else if (id == "AUDIO_ID") {
					d->info->m_tracks[rxID.cap(2).toInt()] = QString();
				} else if (rxAudio.indexIn(id) != -1) {
					d->info->m_tracks[rxAudio.cap(1).toInt()] = rxAudio.cap(2);
				} else if (d->info->isDisc()) {
					static QRegExp rxDVDTitle("^DVD_TITLE_(\\d+)_([A-Z]+)$");
					if (id == "DVD_TITLES") {
						d->info->m_dvd.titles.resize(rxID.cap(2).toInt());
					} else if (id == "DVD_CURRENT_TITLE") {
						d->dvdIdx = rxID.cap(2).toInt()-1;
					} else if (rxDVDTitle.indexIn(id) != -1) {
						const QString text = rxDVDTitle.cap(2);
						const int idx = rxDVDTitle.cap(1).toInt() - 1;
						if (text == "CHAPTERS") {
							d->info->m_dvd.titles[idx].chapters.resize(rxID.cap(2).toInt());
						} else if (text == "ANGLES") {
							d->info->m_dvd.titles[idx].angles = rxID.cap(2).toInt();
						} else if (text == "LENGTH") {
							d->info->m_dvd.titles[idx].number = idx+1;
							d->info->m_dvd.titles[idx].length = rxID.cap(2).toDouble()*1000.0;
						}
					}
				}
			} else if (rxNoVideo.indexIn(msg) != -1) {
				d->info->m_hasVideo = false;
				d->info->m_valid = true;
			} else if (rxVO.indexIn(msg) != -1) {
				d->info->m_videoSize.setWidth(rxVO.cap(4).toInt());
				d->info->m_videoSize.setHeight(rxVO.cap(5).toInt());
				d->info->m_hasVideo = true;
				d->info->m_valid = true;
			} else if (d->info->isDisc()) {
				static QRegExp rxSID("^subtitle \\( sid \\): (\\d+) language: (.*)");
				static QRegExp rxChapters("^CHAPTERS: ([\\d:,]+),$");
				if (rxSID.indexIn(msg) != -1) {
					d->info->m_dvd.channels.append(tr("Channel%1:%2").arg(rxSID.cap(1).toInt()).arg(rxSID.cap(2)));
				} else if (rxChapters.indexIn(msg) != -1) {
					QStringList times = rxChapters.cap(1).split(',');
					QVector<int> &chapters = d->info->m_dvd.titles[d->dvdIdx].chapters;
					const int size = times.size();
					if (chapters.size() != size)
						chapters.resize(size);
					for (int i=0; i<size; ++i)
						chapters[i] = Core::Utility::stringToMSecs(times[i]);
				}
			}
		}
		if (!skip)
			qDebug("%s", qPrintable(msg));
	}
}

void MPlayerProcess::setMediaInfo(MediaInfo *info) {
	d->info = info;
}

}
