#include "mediainfo.h"
#include "info.h"
#include <QRegExp>
#include <QUrl>
#include <QDebug>
#include "mplayerprocess.h"
#include <backend/mediasource.h>
#include "config.h"

namespace Backend {

namespace MPlayer {

MediaInfo::MediaInfo()
: m_length(0), m_videoSize(-1, -1), m_valid(false), m_hasVideo(true)
, m_disc(false), m_dvd(new DVDInfo) {}

MediaInfo::MediaInfo(const MediaInfo &other)
: m_length(other.m_length), m_videoSize(other.m_videoSize), m_valid(other.m_valid)
, m_hasVideo(other.m_hasVideo), m_disc(other.m_disc), m_dvd(new DVDInfo(*other.m_dvd)) {}

MediaInfo &MediaInfo::operator = (const MediaInfo &rhs) {
	if (this != &rhs) {
		m_length = rhs.m_length;
		m_videoSize = rhs.m_videoSize;
		m_valid = rhs.m_valid;
		m_hasVideo = rhs.m_hasVideo;
		m_disc = rhs.m_disc;
		*m_dvd = *rhs.m_dvd;
	}
	return *this;
}

MediaInfo::~MediaInfo() {
	delete m_dvd;
}

bool MediaInfo::get(const MediaSource &source) {
	m_valid = false;
	QStringList args = QStringList() << "-slave" << "-quiet" << "-identify"
		<< "-vo" << "null" << "-ao" << "null" << source.url().toString();
	MPlayerProcess proc;
	proc.setMediaInfo(this);
	static Info info;
	static Config config;
	proc.start(config.mplayer(), args);
	if (proc.waitForStarted(1000)) {
		proc.write("quit\n");
		if (!proc.waitForFinished(200000))
			proc.kill();
	}
	return m_valid;
}

}

}
