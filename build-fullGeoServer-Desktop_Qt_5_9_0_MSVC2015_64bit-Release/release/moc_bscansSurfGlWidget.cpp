/****************************************************************************
** Meta object code from reading C++ file 'bscansSurfGlWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/bscansSurfGlWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bscansSurfGlWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_bscansSurfGlWidget_t {
    QByteArrayData data[25];
    char stringdata0[434];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_bscansSurfGlWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_bscansSurfGlWidget_t qt_meta_stringdata_bscansSurfGlWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "bscansSurfGlWidget"
QT_MOC_LITERAL(1, 19, 24), // "updateBscansSurfSections"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 6), // "inPosX"
QT_MOC_LITERAL(4, 52, 6), // "inPosY"
QT_MOC_LITERAL(5, 59, 6), // "fnPosX"
QT_MOC_LITERAL(6, 66, 6), // "fnPosY"
QT_MOC_LITERAL(7, 73, 22), // "hideBscansSurfSections"
QT_MOC_LITERAL(8, 96, 21), // "sendOutBscansSurfInfo"
QT_MOC_LITERAL(9, 118, 17), // "outBscansSurfInfo"
QT_MOC_LITERAL(10, 136, 25), // "signalFixBscansSurfPixels"
QT_MOC_LITERAL(11, 162, 22), // "signalSetupScrollHData"
QT_MOC_LITERAL(12, 185, 14), // "minScrollHData"
QT_MOC_LITERAL(13, 200, 11), // "scrollHData"
QT_MOC_LITERAL(14, 212, 14), // "maxScrollHData"
QT_MOC_LITERAL(15, 227, 22), // "signalSetupScrollVData"
QT_MOC_LITERAL(16, 250, 14), // "minScrollVData"
QT_MOC_LITERAL(17, 265, 11), // "scrollVData"
QT_MOC_LITERAL(18, 277, 14), // "maxScrollVData"
QT_MOC_LITERAL(19, 292, 24), // "slotCloseSection2dAction"
QT_MOC_LITERAL(20, 317, 16), // "section2dVisible"
QT_MOC_LITERAL(21, 334, 29), // "slotChangeBscansSurfPixelsFnX"
QT_MOC_LITERAL(22, 364, 19), // "bscansSurfPixelsFnX"
QT_MOC_LITERAL(23, 384, 29), // "slotChangeBscansSurfPixelsFnY"
QT_MOC_LITERAL(24, 414, 19) // "bscansSurfPixelsFnY"

    },
    "bscansSurfGlWidget\0updateBscansSurfSections\0"
    "\0inPosX\0inPosY\0fnPosX\0fnPosY\0"
    "hideBscansSurfSections\0sendOutBscansSurfInfo\0"
    "outBscansSurfInfo\0signalFixBscansSurfPixels\0"
    "signalSetupScrollHData\0minScrollHData\0"
    "scrollHData\0maxScrollHData\0"
    "signalSetupScrollVData\0minScrollVData\0"
    "scrollVData\0maxScrollVData\0"
    "slotCloseSection2dAction\0section2dVisible\0"
    "slotChangeBscansSurfPixelsFnX\0"
    "bscansSurfPixelsFnX\0slotChangeBscansSurfPixelsFnY\0"
    "bscansSurfPixelsFnY"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_bscansSurfGlWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   59,    2, 0x06 /* Public */,
       7,    0,   68,    2, 0x06 /* Public */,
       8,    1,   69,    2, 0x06 /* Public */,
      10,    0,   72,    2, 0x06 /* Public */,
      11,    3,   73,    2, 0x06 /* Public */,
      15,    3,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    1,   87,    2, 0x0a /* Public */,
      21,    1,   90,    2, 0x0a /* Public */,
      23,    1,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   12,   13,   14,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   16,   17,   18,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Int,   24,

       0        // eod
};

void bscansSurfGlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        bscansSurfGlWidget *_t = static_cast<bscansSurfGlWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateBscansSurfSections((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->hideBscansSurfSections(); break;
        case 2: _t->sendOutBscansSurfInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->signalFixBscansSurfPixels(); break;
        case 4: _t->signalSetupScrollHData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->signalSetupScrollVData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->slotCloseSection2dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->slotChangeBscansSurfPixelsFnX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->slotChangeBscansSurfPixelsFnY((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (bscansSurfGlWidget::*_t)(int , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscansSurfGlWidget::updateBscansSurfSections)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (bscansSurfGlWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscansSurfGlWidget::hideBscansSurfSections)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (bscansSurfGlWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscansSurfGlWidget::sendOutBscansSurfInfo)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (bscansSurfGlWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscansSurfGlWidget::signalFixBscansSurfPixels)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (bscansSurfGlWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscansSurfGlWidget::signalSetupScrollHData)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (bscansSurfGlWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscansSurfGlWidget::signalSetupScrollVData)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject bscansSurfGlWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_bscansSurfGlWidget.data,
      qt_meta_data_bscansSurfGlWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *bscansSurfGlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *bscansSurfGlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_bscansSurfGlWidget.stringdata0))
        return static_cast<void*>(const_cast< bscansSurfGlWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions_1_1"))
        return static_cast< QOpenGLFunctions_1_1*>(const_cast< bscansSurfGlWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int bscansSurfGlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void bscansSurfGlWidget::updateBscansSurfSections(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void bscansSurfGlWidget::hideBscansSurfSections()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void bscansSurfGlWidget::sendOutBscansSurfInfo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void bscansSurfGlWidget::signalFixBscansSurfPixels()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void bscansSurfGlWidget::signalSetupScrollHData(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void bscansSurfGlWidget::signalSetupScrollVData(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
