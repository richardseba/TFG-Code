/****************************************************************************
** Meta object code from reading C++ file 'vrfullscreenviewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vrfullscreenviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vrfullscreenviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VrFullscreenViewer_t {
    QByteArrayData data[11];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VrFullscreenViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VrFullscreenViewer_t qt_meta_stringdata_VrFullscreenViewer = {
    {
QT_MOC_LITERAL(0, 0, 18), // "VrFullscreenViewer"
QT_MOC_LITERAL(1, 19, 12), // "setUpdatingR"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 6), // "update"
QT_MOC_LITERAL(4, 40, 12), // "setUpdatingL"
QT_MOC_LITERAL(5, 53, 6), // "zoomIn"
QT_MOC_LITERAL(6, 60, 4), // "zoom"
QT_MOC_LITERAL(7, 65, 7), // "zoomOut"
QT_MOC_LITERAL(8, 73, 14), // "showFullScreen"
QT_MOC_LITERAL(9, 88, 14), // "screenSelector"
QT_MOC_LITERAL(10, 103, 16) // "frameUpdateEvent"

    },
    "VrFullscreenViewer\0setUpdatingR\0\0"
    "update\0setUpdatingL\0zoomIn\0zoom\0zoomOut\0"
    "showFullScreen\0screenSelector\0"
    "frameUpdateEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VrFullscreenViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       5,    1,   50,    2, 0x06 /* Public */,
       7,    1,   53,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   56,    2, 0x0a /* Public */,
      10,    0,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Float,    6,
    QMetaType::Void, QMetaType::Float,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,

       0        // eod
};

void VrFullscreenViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VrFullscreenViewer *_t = static_cast<VrFullscreenViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setUpdatingR((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setUpdatingL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->zoomIn((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->zoomOut((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->showFullScreen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->frameUpdateEvent(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VrFullscreenViewer::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VrFullscreenViewer::setUpdatingR)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VrFullscreenViewer::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VrFullscreenViewer::setUpdatingL)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VrFullscreenViewer::*_t)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VrFullscreenViewer::zoomIn)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VrFullscreenViewer::*_t)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VrFullscreenViewer::zoomOut)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VrFullscreenViewer::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_VrFullscreenViewer.data,
      qt_meta_data_VrFullscreenViewer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VrFullscreenViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VrFullscreenViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VrFullscreenViewer.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int VrFullscreenViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void VrFullscreenViewer::setUpdatingR(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VrFullscreenViewer::setUpdatingL(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VrFullscreenViewer::zoomIn(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VrFullscreenViewer::zoomOut(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
