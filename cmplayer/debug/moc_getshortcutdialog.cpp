/****************************************************************************
** Meta object code from reading C++ file 'getshortcutdialog.h'
**
** Created: Thu Jul 17 00:18:44 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pref/getshortcutdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'getshortcutdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Pref__GetShortcutDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      28,   25,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Pref__GetShortcutDialog[] = {
    "Pref::GetShortcutDialog\0\0on\0"
    "setGetting(bool)\0"
};

const QMetaObject Pref::GetShortcutDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Pref__GetShortcutDialog,
      qt_meta_data_Pref__GetShortcutDialog, 0 }
};

const QMetaObject *Pref::GetShortcutDialog::metaObject() const
{
    return &staticMetaObject;
}

void *Pref::GetShortcutDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Pref__GetShortcutDialog))
	return static_cast<void*>(const_cast< GetShortcutDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Pref::GetShortcutDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setGetting((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
