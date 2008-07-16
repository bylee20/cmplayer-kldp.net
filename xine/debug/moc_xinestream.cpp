/****************************************************************************
** Meta object code from reading C++ file 'xinestream.h'
**
** Created: Thu Jul 17 00:16:00 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../xinestream.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'xinestream.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Xine__XineStream[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      23,   18,   17,   17, 0x05,
      36,   18,   17,   17, 0x05,
      79,   61,   17,   17, 0x05,
     114,  105,   17,   17, 0x05,
     145,  136,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     172,   17,  167,   17, 0x0a,
     179,   17,   17,   17, 0x0a,
     187,   17,   17,   17, 0x0a,
     194,   18,   17,   17, 0x0a,
     207,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Xine__XineStream[] = {
    "Xine::XineStream\0\0time\0tick(qint64)\0"
    "totalTimeChanged(qint64)\0newState,oldState\0"
    "stateChanged(State,State)\0hasVideo\0"
    "hasVideoChanged(bool)\0seekable\0"
    "seekableChanged(bool)\0bool\0play()\0"
    "pause()\0stop()\0seek(qint64)\0emitTick()\0"
};

const QMetaObject Xine::XineStream::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Xine__XineStream,
      qt_meta_data_Xine__XineStream, 0 }
};

const QMetaObject *Xine::XineStream::metaObject() const
{
    return &staticMetaObject;
}

void *Xine::XineStream::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Xine__XineStream))
	return static_cast<void*>(const_cast< XineStream*>(this));
    return QObject::qt_metacast(_clname);
}

int Xine::XineStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: tick((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 1: totalTimeChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 2: stateChanged((*reinterpret_cast< State(*)>(_a[1])),(*reinterpret_cast< State(*)>(_a[2]))); break;
        case 3: hasVideoChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: seekableChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: { bool _r = play();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: pause(); break;
        case 7: stop(); break;
        case 8: seek((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 9: emitTick(); break;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Xine::XineStream::tick(qint64 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Xine::XineStream::totalTimeChanged(qint64 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Xine::XineStream::stateChanged(State _t1, State _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Xine::XineStream::hasVideoChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Xine::XineStream::seekableChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
