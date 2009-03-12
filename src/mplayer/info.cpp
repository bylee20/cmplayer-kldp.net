#include "info.h"
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include "config.h"
#include <QtCore/QDebug>

namespace MPlayer {

Info::Data Info::d;
	
Info::Data::Data() {
	
}

void Info::getInfo() const {
// 	Config config;
	QProcess proc;
	QStringList args;
	args << "-identify" << "-vo" << "help" << "-ao" << "help"
		<< "-vf" << "help" << "-af" << "help";
	proc.start("mplayer", args);
	if (!proc.waitForFinished())
		proc.kill();
	QRegExp rxOut("^\\s+(\\S+)\\s+(.*)$");
	QRegExp rxFilter("^\\s+(\\S+)\\s+:\\s+(.*)$");;
	QRegExp rxVf("^ID_VIDEO_FILTERS");
	QRegExp rxAf("^Available audio filters:");
	QRegExp rxVo("^ID_VIDEO_OUTPUTS");
	QRegExp rxAo("^ID_AUDIO_OUTPUTS");
	QRegExp rxVer("^MPlayer\\s+(\\S+).*");
	enum What {Nothing = 0, Vo = 1, Ao = 2, Vf, Af};
	d.vf.clear();
	d.af.clear();
	d.vo.clear();
	d.ao.clear();
	d.vo << "auto";
	d.ao << "auto";
	What what = Nothing;
	while (proc.canReadLine()) {
		QString line = QString::fromLocal8Bit(proc.readLine());
		line.replace("\n", "");
		line.replace("\r", "");
		if (line.isEmpty()) {
			what = Nothing;
			continue;
		}
// 		if (rxVer.indexIn(line) != -1)
// 			d.rv = rxVer.cap(1);
		if (what == Nothing) {
			if (rxVo.indexIn(line) != -1)
				what = Vo;
			else if (rxAo.indexIn(line) != -1)
				what = Ao;
			else if (rxVf.indexIn(line) != -1)
				what = Vf;
			else if (rxAf.indexIn(line) != -1)
				what = Af;
			continue;
		}
		switch(what) {
		case Vo:
		case Ao:
			if (rxOut.indexIn(line) == -1)
				break;
			(what == Vo ? d.vo : d.ao).append(rxOut.cap(1));
			continue;
		case Vf:
		case Af:
			if (rxFilter.indexIn(line) == -1)
				break;
			(what == Vf ? d.vf : d.af).append(rxFilter.cap(1));
			continue;
		default:
			break;
		}
		what = Nothing;
	}
}

}
