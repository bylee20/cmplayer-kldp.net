/****************************************************************************
** Meta object code from reading C++ file 'playmenubar.h'
**
** Created: Thu Jul 17 00:18:39 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../playmenubar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playmenubar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlayMenuBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      18,   13,   12,   12, 0x0a,
      39,   13,   12,   12, 0x0a,
      67,   62,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PlayMenuBar[] = {
    "PlayMenuBar\0\0msec\0setTotalTime(qint64)\0"
    "setCurrentTime(qint64)\0text\0"
    "setPlayText(QString)\0"
};

const QMetaObject PlayMenuBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PlayMenuBar,
      qt_meta_data_PlayMenuBar, 0 }
};

const QMetaObject *PlayMenuBar::metaObject() const
{
    return &staticMetaObject;
}

void *PlayMenuBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlayMenuBar))
	return static_cast<void*>(const_cast< PlayMenuBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int PlayMenuBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setTotalTime((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 1: setCurrentTime((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 2: setPlayText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
