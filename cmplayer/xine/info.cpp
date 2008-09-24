#include "info.h"
#include "xineengine.h"

namespace Backend {

namespace Xine {

Info::Data Info::d;
	
Info::Data::Data() {
	name = "xine";
	des = QObject::trUtf8("xine입니다.");
	cv = XINE_VERSION;
	rv = xine_get_version_string();
	ves << "asf" << "avi" << "dvix" << "mkv" << "mov" << "mp4" << "mpeg" << "mpg"
		<< "vob" << "ogg" << "ogm"<< "qt" << "rm" << "wmv";
	aes << "mp3" << "ogg" << "ra" << "wav" << "wma";
	ses << "smi" << "srt";
}

const QStringList &Info::getVideoDrivers() {
	d.vos.clear();
	d.vos.append("auto");
	const char *const *vos
			= xine_list_video_output_plugins(XineEngine::get()->xine());
	for (int i=0; vos[i]; ++i)
		d.vos.append(vos[i]);
	return d.vos;
}

const QStringList &Info::getAudioDrivers() {
	d.aos.clear();
	d.aos.append("auto");
	const char *const *aos
			= xine_list_audio_output_plugins(XineEngine::get()->xine());
	for (int i=0; aos[i]; ++i)
		d.aos.append(aos[i]);
	return d.aos;
}

}

}
