#ifndef AVCODEC_W_HPP
#define AVCODEC_W_HPP

#include <QtCore/QtGlobal>
#ifndef UINT64_C
#define UINT64_C Q_UINT64_C
#endif
extern "C" {
#include <libavcodec/avcodec.h>
}
#undef UINT64_C

#endif // AVCODEC_W_HPP
