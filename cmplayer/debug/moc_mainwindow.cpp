/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Tue Jul 15 23:33:35 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      53,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x0a,
      37,   11,   11,   11, 0x08,
      48,   11,   11,   11, 0x08,
      58,   11,   11,   11, 0x08,
      75,   71,   11,   11, 0x08,
     103,   71,   11,   11, 0x08,
     130,   71,   11,   11, 0x08,
     153,   71,   11,   11, 0x08,
     184,  178,   11,   11, 0x08,
     214,   11,   11,   11, 0x08,
     233,  228,   11,   11, 0x08,
     254,   71,   11,   11, 0x08,
     280,   71,   11,   11, 0x08,
     319,  313,   11,   11, 0x08,
     356,  348,   11,   11, 0x08,
     397,   11,   11,   11, 0x08,
     421,   11,   11,   11, 0x08,
     441,   11,   11,   11, 0x08,
     456,   11,   11,   11, 0x08,
     472,   71,   11,   11, 0x08,
     492,  487,   11,   11, 0x08,
     519,  513,   11,   11, 0x08,
     557,  552,   11,   11, 0x08,
     579,   11,   11,   11, 0x08,
     602,  598,   11,   11, 0x08,
     619,   11,   11,   11, 0x08,
     637,   11,   11,   11, 0x08,
     655,   11,   11,   11, 0x08,
     676,   11,   11,   11, 0x08,
     711,  703,   11,   11, 0x08,
     741,  735,   11,   11, 0x08,
     761,   11,   11,   11, 0x08,
     773,   11,   11,   11, 0x08,
     790,   11,   11,   11, 0x08,
     807,   11,   11,   11, 0x08,
     817,   11,   11,   11, 0x08,
     831,   11,   11,   11, 0x08,
     849,   11,   11,   11, 0x08,
     860,   11,   11,   11, 0x08,
     875,   11,   11,   11, 0x08,
     894,   11,   11,   11, 0x08,
     910,   11,   11,   11, 0x08,
     926,   11,   11,   11, 0x08,
     941,   11,   11,   11, 0x08,
     955,   11,   11,   11, 0x08,
     967,   11,   11,   11, 0x08,
     987,   11,   11,   11, 0x08,
    1007,   11,   11,   11, 0x08,
    1024,   11,   11,   11, 0x08,
    1044,   11,   11,   11, 0x08,
    1060,   11,   11,   11, 0x08,
    1074,   11,   11,   11, 0x08,
    1096, 1091,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0full\0setFullScreen(bool)\0"
    "openFile()\0openDVD()\0openRecent()\0act\0"
    "changeAspectRatio(QAction*)\0"
    "changeStaysOnTop(QAction*)\0"
    "seekDVDTitle(QAction*)\0seekDVDChapter(QAction*)\0"
    "state\0slotStateChanged(Xine::State)\0"
    "slotResized()\0rate\0setVideoSize(double)\0"
    "changeVideoSize(QAction*)\0"
    "changeCurrentSubtitles(QAction*)\0names\0"
    "updateSubtitles(QStringList)\0indexes\0"
    "updateCurrentSubtitleIndexes(QList<int>)\0"
    "showPreferencesDialog()\0clearSubtitleList()\0"
    "addSubtitles()\0showEqualizer()\0"
    "crop(QAction*)\0msec\0updateSyncDelay(int)\0"
    "files\0updateRecentActions(RecentStack)\0"
    "size\0updateRecentSize(int)\0"
    "clearRecentFiles()\0pos\0showMenu(QPoint)\0"
    "showAboutDialog()\0selectABSection()\0"
    "showABRepeatDialog()\0togglePlayListVisibility()\0"
    "visible\0adjustSizeForDock(bool)\0speed\0"
    "updateSpeed(double)\0playPause()\0"
    "increaseVolume()\0decreaseVolume()\0"
    "forward()\0forwardMore()\0forwardMuchMore()\0"
    "backward()\0backwardMore()\0backwardMuchMore()\0"
    "increaseSpeed()\0decreaseSpeed()\0"
    "restoreSpeed()\0doubleSpeed()\0halfSpeed()\0"
    "increaseSyncDelay()\0decreaseSyncDelay()\0"
    "updatePlayText()\0updatePreferences()\0"
    "executeWizard()\0slotStarted()\0"
    "updateFinished()\0time\0updateStopped(qint64)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
	return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setFullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: openFile(); break;
        case 2: openDVD(); break;
        case 3: openRecent(); break;
        case 4: changeAspectRatio((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 5: changeStaysOnTop((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 6: seekDVDTitle((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 7: seekDVDChapter((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 8: slotStateChanged((*reinterpret_cast< Xine::State(*)>(_a[1]))); break;
        case 9: slotResized(); break;
        case 10: setVideoSize((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: changeVideoSize((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 12: changeCurrentSubtitles((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 13: updateSubtitles((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 14: updateCurrentSubtitleIndexes((*reinterpret_cast< const QList<int>(*)>(_a[1]))); break;
        case 15: showPreferencesDialog(); break;
        case 16: clearSubtitleList(); break;
        case 17: addSubtitles(); break;
        case 18: showEqualizer(); break;
        case 19: crop((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 20: updateSyncDelay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: updateRecentActions((*reinterpret_cast< const RecentStack(*)>(_a[1]))); break;
        case 22: updateRecentSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: clearRecentFiles(); break;
        case 24: showMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 25: showAboutDialog(); break;
        case 26: selectABSection(); break;
        case 27: showABRepeatDialog(); break;
        case 28: togglePlayListVisibility(); break;
        case 29: adjustSizeForDock((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: updateSpeed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 31: playPause(); break;
        case 32: increaseVolume(); break;
        case 33: decreaseVolume(); break;
        case 34: forward(); break;
        case 35: forwardMore(); break;
        case 36: forwardMuchMore(); break;
        case 37: backward(); break;
        case 38: backwardMore(); break;
        case 39: backwardMuchMore(); break;
        case 40: increaseSpeed(); break;
        case 41: decreaseSpeed(); break;
        case 42: restoreSpeed(); break;
        case 43: doubleSpeed(); break;
        case 44: halfSpeed(); break;
        case 45: increaseSyncDelay(); break;
        case 46: decreaseSyncDelay(); break;
        case 47: updatePlayText(); break;
        case 48: updatePreferences(); break;
        case 49: executeWizard(); break;
        case 50: slotStarted(); break;
        case 51: updateFinished(); break;
        case 52: updateStopped((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        }
        _id -= 53;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
