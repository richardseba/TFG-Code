/****************************************************************************
** Meta object code from reading C++ file 'vrimageupdater.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vrimageupdater.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vrimageupdater.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VRimageUpdater_t {
    QByteArrayData data[8];
    char stringdata0[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VRimageUpdater_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VRimageUpdater_t qt_meta_stringdata_VRimageUpdater = {
    {
QT_MOC_LITERAL(0, 0, 14), // "VRimageUpdater"
QT_MOC_LITERAL(1, 15, 16), // "setUpdatingEvent"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "updating"
QT_MOC_LITERAL(4, 42, 6), // "zoomIn"
QT_MOC_LITERAL(5, 49, 4), // "zoom"
QT_MOC_LITERAL(6, 54, 7), // "zoomOut"
QT_MOC_LITERAL(7, 62, 16) // "frameUpdateEvent"

    },
    "VRimageUpdater\0setUpdatingEvent\0\0"
    "updating\0zoomIn\0zoom\0zoomOut\0"
    "frameUpdateEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VRimageUpdater[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       4,    1,   37,    2, 0x0a /* Public */,
       6,    1,   40,    2, 0x0a /* Public */,
       7,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Float,    5,
    QMetaType::Void, QMetaType::Float,    5,
    QMetaType::Void,

       0        // eod
};

void VRimageUpdater::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VRimageUpdater *_t = static_cast<VRimageUpdater *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setUpdatingEvent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->zoomIn((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->zoomOut((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->frameUpdateEvent(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VRimageUpdater::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VRimageUpdater.data,
      qt_meta_data_VRimageUpdater,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VRimageUpdater::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VRimageUpdater::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VRimageUpdater.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VRimageUpdater::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE