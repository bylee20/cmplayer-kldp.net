/****************************************************************************
** Meta object code from reading C++ file 'xineengine.h'
**
** Created: Tue Jul 15 23:30:27 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../xineengine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'xineengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Xine__XineEngine[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      25,   18,   17,   17, 0x05,
      65,   17,   17,   17, 0x05,
      83,   17,   17,   17, 0x05,
      94,   17,   17,   17, 0x05,
     113,  104,   17,   17, 0x05,
     146,  135,   17,   17, 0x05,
     174,  168,   17,   17, 0x05,
     200,  195,   17,   17, 0x05,
     216,   17,   17,   17, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Xine__XineEngine[] = {
    "Xine::XineEngine\0\0source\0"
    "currentSourceChanged(Xine::MediaSource)\0"
    "aboutToFinished()\0finished()\0started()\0"
    "hasVideo\0hasVideoChanged(bool)\0"
    "isSeekable\0seekableChanged(bool)\0speed\0"
    "speedChanged(double)\0time\0stopped(qint64)\0"
    "aboutToStarted()\0"
};

const QMetaObject Xine::XineEngine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Xine__XineEngine,
      qt_meta_data_Xine__XineEngine, 0 }
};

const QMetaObject *Xine::XineEngine::metaObject() const
{
    return &staticMetaObject;
}

void *Xine::XineEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Xine__XineEngine))
	return static_cast<void*>(const_cast< XineEngine*>(this));
    return QObject::qt_metacast(_clname);
}

int Xine::XineEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentSourceChanged((*reinterpret_cast< const Xine::MediaSource(*)>(_a[1]))); break;
        case 1: aboutToFinished(); break;
        case 2: finished(); break;
        case 3: started(); break;
        case 4: hasVideoChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: seekableChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: speedChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: stopped((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 8: aboutToStarted(); break;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Xine::XineEngine::currentSourceChanged(const Xine::MediaSource & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Xine::XineEngine::aboutToFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Xine::XineEngine::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Xine::XineEngine::started()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Xine::XineEngine::hasVideoChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Xine::XineEngine::seekableChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Xine::XineEngine::speedChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Xine::XineEngine::stopped(qint64 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Xine::XineEngine::aboutToStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}
QT_END_MOC_NAMESPACE
