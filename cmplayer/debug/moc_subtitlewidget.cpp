/****************************************************************************
** Meta object code from reading C++ file 'subtitlewidget.h'
**
** Created: Tue Jul 15 21:22:40 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pref/subtitlewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'subtitlewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Pref__SubtitleWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_Pref__SubtitleWidget[] = {
    "Pref::SubtitleWidget\0"
};

const QMetaObject Pref::SubtitleWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Pref__SubtitleWidget,
      qt_meta_data_Pref__SubtitleWidget, 0 }
};

const QMetaObject *Pref::SubtitleWidget::metaObject() const
{
    return &staticMetaObject;
}

void *Pref::SubtitleWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Pref__SubtitleWidget))
	return static_cast<void*>(const_cast< SubtitleWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Pref::SubtitleWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
