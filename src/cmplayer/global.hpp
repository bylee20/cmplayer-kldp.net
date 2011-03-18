#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QSize>

namespace Global {

enum MediaState {StoppedState = 0, PlayingState, PausedState, FinishedState};
enum MediaStatus {NoMediaStatus = 0, EosStatus, BufferedStatus};
enum StreamType {UnknownStream = 0, VideoStream, AudioStream, SubPicStream};
enum MediaMetaData {LanguageCode};
enum NavDVDCmd {NavAngleMenu, NavAudioMenu, NavChapterMenu
		, NavToggleMenu, NavRootMenu, NavSubPicMenu, NavTitleMenu};

namespace EnumSpace {template<typename E> class Enum;}

#define DEC_ENUM_CLASS_HEAD(_type, ...)\
namespace EnumSpace {template<> class Enum<_type> {\
	struct Map {QMap<_type, QString> name; QMap<QString, _type> value; Map() {

#define DEC_ENUM_CLASS_FOOT(_type)\
	}}; static Map &map() {static Map m; return m;} _type v;\
public: typedef _type Type; typedef Enum<_type> This;\
	typedef QMap<Type, QString> NameMap; typedef QMap<QString, Type> ValueMap;\
	Enum<_type>(_type v = valueMap().begin().value()): v(v) {}\
	operator _type () const {return v;}\
	This &operator = (const This &rhs) {v = rhs.v; return *this;}\
	This &operator = (Type rhs) {v = rhs; return *this;}\
	QString name() const {return nameMap()[v];}\
	_type value() const {return v;}\
	static QString name(_type value) {return nameMap()[value];}\
	static _type value(const QString &name, _type def = valueMap().begin().value()) {\
		return valueMap().value(name, def);}\
	static const QString &typeName() {static const QString n(#_type); return n;}\
	static const NameMap &nameMap() {return map().name;}\
	static const ValueMap &valueMap() {return map().value;} };}

#define DEC_ENUM_CLASS_BODY2(_val1, _val2)\
name[_val1] = #_val1; value[#_val1] = _val1;\
name[_val2] = #_val2; value[#_val2] = _val2;

#define DEC_ENUM_CLASS_BODY3(_val1, _val2, _val3)\
DEC_ENUM_CLASS_BODY2(_val1, _val2)\
name[_val3] = #_val3; value[#_val3] = _val3;

#define DEC_ENUM_CLASS_BODY4(_val1, _val2, _val3, _val4)\
DEC_ENUM_CLASS_BODY3(_val1, _val2, _val3)\
name[_val4] = #_val4; value[#_val4] = _val4;

#define DEC_ENUM_CLASS_BODY5(_val1, _val2, _val3, _val4, _val5)\
DEC_ENUM_CLASS_BODY4(_val1, _val2, _val3, _val4)\
name[_val5] = #_val5; value[#_val5] = _val5;

#define DEC_ENUM_CLASS_BODY6(_val1, _val2, _val3, _val4, _val5, _val6)\
DEC_ENUM_CLASS_BODY5(_val1, _val2, _val3, _val4, _val5)\
name[_val6] = #_val6; value[#_val6] = _val6;

#define DEC_ENUM_CLASS_BODY7(_val1, _val2, _val3, _val4, _val5, _val6, _val7)\
DEC_ENUM_CLASS_BODY6(_val1, _val2, _val3, _val4, _val5, _val6)\
name[_val7] = #_val7; value[#_val7] = _val7;

#define DEC_ENUM_CLASS(_type, count, ...)\
DEC_ENUM_CLASS_HEAD(_type, __VA_ARGS__)\
DEC_ENUM_CLASS_BODY##count(__VA_ARGS__)\
DEC_ENUM_CLASS_FOOT(_type)

#define DEC_ENUM(_type, count, ...)\
enum _type {__VA_ARGS__};\
DEC_ENUM_CLASS(_type, count, __VA_ARGS__)\
typedef EnumSpace::Enum<_type> _type##Enum;

// DEC_ENUM(UiLanguage, 4, SystemDefault, English, Japanese, Korean)
DEC_ENUM(AutoAddFiles, 3, AllFiles, SimilarFiles, DoNotAddFiles)
DEC_ENUM(SeekingStep, 3, SeekingStep1, SeekingStep2, SeekingStep3)
DEC_ENUM(SubtitleAutoLoad, 4, NoAutoLoad, Matched, Contain, SamePath)
DEC_ENUM(SubtitleAutoSelect, 4, FirstFile, SameName, AllLoaded, EachLanguage)
DEC_ENUM(ClickAction, 5, OpenFile, ToggleFullScreen, TogglePlayPause, ToggleMute, ToggleToolBox)
DEC_ENUM(WheelAction, 6, Seek1, Seek2, Seek3, NextPrevious, VolumeUpDown, AmpUpDown)
DEC_ENUM_CLASS(Qt::KeyboardModifier, 4, Qt::NoModifier, Qt::ShiftModifier
		, Qt::ControlModifier, Qt::AltModifier)
typedef EnumSpace::Enum<Qt::KeyboardModifier> ModifierEnum;

QTime secsToTime(int secs);
QTime msecsToTime(qint64 msecs);
QString msecsToString(qint64 msecs, const QString &format = QString("hh:mm:ss"));
QString secsToString(int secs, const QString &format = QString("hh:mm:ss"));
qint64 timeToMSecs(const QTime &time);
qint64 stringToMSecs(const QString &str, const QString &format = QString("hh:mm:ss"));
const QTime &nullTime();

}

using namespace Global;

#endif // GLOBAL_HPP
