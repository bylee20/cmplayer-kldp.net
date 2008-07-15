/****************************************************************************
** Meta object code from reading C++ file 'seekslider.h'
**
** Created: Wed Jul 16 00:30:35 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../seekslider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'seekslider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Xine__SeekSlider[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      21,   18,   17,   17, 0x0a,
      42,   18,   17,   17, 0x08,
      64,   59,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Xine__SeekSlider[] = {
    "Xine::SeekSlider\0\0ms\0setTotalTime(qint64)\0"
    "slotTick(qint64)\0msec\0seek(int)\0"
};

const QMetaObject Xine::SeekSlider::staticMetaObject = {
    { &CSlider::staticMetaObject, qt_meta_stringdata_Xine__SeekSlider,
      qt_meta_data_Xine__SeekSlider, 0 }
};

const QMetaObject *Xine::SeekSlider::metaObject() const
{
    return &staticMetaObject;
}

void *Xine::SeekSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Xine__SeekSlider))
	return static_cast<void*>(const_cast< SeekSlider*>(this));
    return CSlider::qt_metacast(_clname);
}

int Xine::SeekSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setTotalTime((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 1: slotTick((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 2: seek((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
