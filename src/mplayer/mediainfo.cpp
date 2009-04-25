#include "mplayerprocess.h"
#include "mediainfo.h"
#include "info.h"
#include "config.h"
#include <core/mediasource.h>
#include <QtCore/QRegExp>
#include <QtCore/QUrl>

namespace MPlayer {

MediaInfo::MediaInfo()
: m_length(0), m_videoSize(-1, -1), m_valid(false), m_hasVideo(true)
, m_disc(false), m_frameRate(-1.0) {}

MediaInfo::MediaInfo(const MediaInfo &other)
: m_length(other.m_length), m_videoSize(other.m_videoSize)
, m_valid(other.m_valid), m_hasVideo(other.m_hasVideo), m_disc(other.m_disc)
, m_frameRate(other.m_frameRate), m_dvd(other.m_dvd) {}

MediaInfo &MediaInfo::operator = (const MediaInfo &rhs) {
	if (this != &rhs) {
		m_length = rhs.m_length;
		m_videoSize = rhs.m_videoSize;
		m_valid = rhs.m_valid;
		m_hasVideo = rhs.m_hasVideo;
		m_disc = rhs.m_disc;
		m_frameRate = rhs.m_frameRate;
		m_dvd = rhs.m_dvd;
		m_tracks = rhs.m_tracks;
	}
	return *this;
}

void MediaInfo::reset() {
	m_valid = false;
	m_hasVideo = false;
	m_length = 0;
	m_videoSize = QSize(-1, -1);
	m_disc = false;
	m_frameRate = -1.0;
	m_dvd.titles.clear();
	m_dvd.channels.clear();
	m_tracks.clear();
}

bool MediaInfo::get(const Core::MediaSource &source) {
	reset();
	QStringList args;
	args << "-ao" << "null" << "-vo" << "null" << "-frames" << "0" << "-identify";
	args << (source.isDisc() ? "dvd://" : source.mrl().toString());
	MPlayerProcess proc;
	proc.setMediaInfo(this);
	static Info info;
	proc.start("mplayer", args);
	if (!proc.waitForFinished(200000))
		proc.kill();
	return m_valid;
}

}
