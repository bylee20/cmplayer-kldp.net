#include "info.h"
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include "config.h"

namespace Backend {

namespace MPlayer {

Info::Data Info::d;
	
Info::Data::Data() {
	cv = QObject::trUtf8("정보 없음");
	name = QObject::trUtf8("MPlayer");
	des = QObject::trUtf8("MPlayer입니다.");
	ves << "asf" << "avi" << "dvix" << "mkv" << "mov" << "mp4" << "mpeg" << "mpg"
			<< "vob" << "ogg" << "ogm"<< "qt" << "rm" << "wmv";
	aes << "mp3" << "ogg" << "ra" << "wav" << "wma";
	ses << "smi" << "srt";
}

void Info::getInfo() {
	Config config;
	QProcess proc;
	QStringList args;
	args << "-identify" << "-vo" << "help" << "-ao" << "help";
	proc.start(config.mplayer(), args);
	if (!proc.waitForFinished())
		proc.kill();
	static QRegExp rxOuts("(\\s+)(\\S+)(.*)");
	static QRegExp rxVO("^ID_VIDEO_OUTPUTS");
	static QRegExp rxAO("^ID_AUDIO_OUTPUTS");
	static QRegExp rxVer("^MPlayer\\s+(\\S+).*");
	enum What {Nothing = 0, VO = 1, AO = 2};
	What what = Nothing;
	d.vos.clear();
	d.aos.clear();
	d.vos << "auto";
	d.aos << "auto";
	while (proc.canReadLine()) {
		QString line = QString::fromLocal8Bit(proc.readLine());
		line.replace("\n", "");
		line.replace("\r", "");
		if (rxVer.indexIn(line) != -1)
			d.rv = rxVer.cap(1);
		else if (rxVO.indexIn(line) != -1)
			what = VO;
		else if (rxAO.indexIn(line) != -1)
			what = AO;
		else if (line.isEmpty())
			what = Nothing;
		else if (what == VO) {
			if (rxOuts.indexIn(line) != -1)
				d.vos.append(rxOuts.cap(2));
		} else if (what == AO) {
			if (rxOuts.indexIn(line) != -1)
				d.aos.append(rxOuts.cap(2));
		}
	}
}

}

}
