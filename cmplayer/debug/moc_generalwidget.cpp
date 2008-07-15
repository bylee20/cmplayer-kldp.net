/****************************************************************************
** Meta object code from reading C++ file 'generalwidget.h'
**
** Created: Tue Jul 15 21:22:42 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pref/generalwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'generalwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Pref__GeneralWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Pref__GeneralWidget[] = {
    "Pref::GeneralWidget\0\0findMPlayer()\0"
};

const QMetaObject Pref::GeneralWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Pref__GeneralWidget,
      qt_meta_data_Pref__GeneralWidget, 0 }
};

const QMetaObject *Pref::GeneralWidget::metaObject() const
{
    return &staticMetaObject;
}

void *Pref::GeneralWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Pref__GeneralWidget))
	return static_cast<void*>(const_cast< GeneralWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Pref::GeneralWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: findMPlayer(); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
