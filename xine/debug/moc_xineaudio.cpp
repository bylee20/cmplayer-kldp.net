/****************************************************************************
** Meta object code from reading C++ file 'xineaudio.h'
**
** Created: Tue Jul 15 23:30:27 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../xineaudio.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'xineaudio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Xine__XineAudio[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      23,   17,   16,   16, 0x05,
      48,   42,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   17,   16,   16, 0x0a,
      96,   89,   16,   16, 0x0a,
     111,   42,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Xine__XineAudio[] = {
    "Xine::XineAudio\0\0value\0volumeChanged(int)\0"
    "muted\0mutedChanged(bool)\0setInitialVolume(int)\0"
    "volume\0setVolume(int)\0setMuted(bool)\0"
};

const QMetaObject Xine::XineAudio::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Xine__XineAudio,
      qt_meta_data_Xine__XineAudio, 0 }
};

const QMetaObject *Xine::XineAudio::metaObject() const
{
    return &staticMetaObject;
}

void *Xine::XineAudio::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Xine__XineAudio))
	return static_cast<void*>(const_cast< XineAudio*>(this));
    return QObject::qt_metacast(_clname);
}

int Xine::XineAudio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: volumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: mutedChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: setInitialVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setMuted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Xine::XineAudio::volumeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Xine::XineAudio::mutedChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
