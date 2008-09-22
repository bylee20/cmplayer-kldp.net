#include "info.h"
// #ifndef _UNICODE
// #define _UNICODE
// #endif
// #include <MediaInfo/MediaInfo.h>
// #include <sstream>
// #include "mediasource.h"

namespace Backend {

QString Info::m_privatePath;

// bool Info::get(MediaSource *source) {
// 	MediaInfoLib::MediaInfo info;
// 	if (!info.Open(source->url().toLocalFile().toStdWString()))
// 		return false;
// 	std::wistringstream sin;
// 	sin.str(info.Get(MediaInfoLib::Stream_General, 0, _T("Duration"), MediaInfoLib::Info_Text, MediaInfoLib::Info_Name));
// 	sin >> source->d->duration;
// 	sin.clear();
// 	source->d->hasVideo = info.Count_Get(MediaInfoLib::Stream_Video) > 0;
// 	if (source->d->hasVideo) {
// 		sin.str(info.Get(MediaInfoLib::Stream_Video, 0, _T("Width"), MediaInfoLib::Info_Text, MediaInfoLib::Info_Name));
// 		sin >> source->d->videoSize.rwidth();
// 		sin.clear();
// 		sin.str(info.Get(MediaInfoLib::Stream_Video, 0, _T("Height"), MediaInfoLib::Info_Text, MediaInfoLib::Info_Name));
// 		sin >> source->d->videoSize.rheight();
// 		sin.clear();
// 	}
// 	info.Close();
// 	source->d->gotInfo = true;
// 	return true;
// }

}
