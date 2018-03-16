/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[16];
    char stringdata0[402];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 26), // "on_recordingButton_clicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 24), // "on_captureButton_clicked"
QT_MOC_LITERAL(4, 64, 14), // "frameTimeEvent"
QT_MOC_LITERAL(5, 79, 24), // "acceptedCalibParamsEvent"
QT_MOC_LITERAL(6, 104, 20), // "on_exeButton_clicked"
QT_MOC_LITERAL(7, 125, 37), // "on_pushButton_loadCalibration..."
QT_MOC_LITERAL(8, 163, 32), // "on_pushButton_Fullscreen_clicked"
QT_MOC_LITERAL(9, 196, 24), // "on_pushButton_VR_clicked"
QT_MOC_LITERAL(10, 221, 18), // "fullscreen_closing"
QT_MOC_LITERAL(11, 240, 35), // "on_changeParamsL_pushButton_c..."
QT_MOC_LITERAL(12, 276, 26), // "acceptedCamParamsLeftEvent"
QT_MOC_LITERAL(13, 303, 35), // "on_changeParamsR_pushButton_c..."
QT_MOC_LITERAL(14, 339, 27), // "acceptedCamParamsRightEvent"
QT_MOC_LITERAL(15, 367, 34) // "on_switchCamera_pushButton_cl..."

    },
    "MainWindow\0on_recordingButton_clicked\0"
    "\0on_captureButton_clicked\0frameTimeEvent\0"
    "acceptedCalibParamsEvent\0on_exeButton_clicked\0"
    "on_pushButton_loadCalibration_clicked\0"
    "on_pushButton_Fullscreen_clicked\0"
    "on_pushButton_VR_clicked\0fullscreen_closing\0"
    "on_changeParamsL_pushButton_clicked\0"
    "acceptedCamParamsLeftEvent\0"
    "on_changeParamsR_pushButton_clicked\0"
    "acceptedCamParamsRightEvent\0"
    "on_switchCamera_pushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    0,   96,    2, 0x08 /* Private */,
      15,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_recordingButton_clicked(); break;
        case 1: _t->on_captureButton_clicked(); break;
        case 2: _t->frameTimeEvent(); break;
        case 3: _t->acceptedCalibParamsEvent(); break;
        case 4: _t->on_exeButton_clicked(); break;
        case 5: _t->on_pushButton_loadCalibration_clicked(); break;
        case 6: _t->on_pushButton_Fullscreen_clicked(); break;
        case 7: _t->on_pushButton_VR_clicked(); break;
        case 8: _t->fullscreen_closing(); break;
        case 9: _t->on_changeParamsL_pushButton_clicked(); break;
        case 10: _t->acceptedCamParamsLeftEvent(); break;
        case 11: _t->on_changeParamsR_pushButton_clicked(); break;
        case 12: _t->acceptedCamParamsRightEvent(); break;
        case 13: _t->on_switchCamera_pushButton_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
