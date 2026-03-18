/****************************************************************************
** Meta object code from reading C++ file 'bscansTransGlWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/bscansTransGlWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bscansTransGlWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_bscansTransGlWidget_t {
    QByteArrayData data[23];
    char stringdata0[495];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_bscansTransGlWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_bscansTransGlWidget_t qt_meta_stringdata_bscansTransGlWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "bscansTransGlWidget"
QT_MOC_LITERAL(1, 20, 22), // "signalSetupScrollHData"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 14), // "minScrollHData"
QT_MOC_LITERAL(4, 59, 11), // "scrollHData"
QT_MOC_LITERAL(5, 71, 14), // "maxScrollHData"
QT_MOC_LITERAL(6, 86, 22), // "signalSetupScrollVData"
QT_MOC_LITERAL(7, 109, 14), // "minScrollVData"
QT_MOC_LITERAL(8, 124, 11), // "scrollVData"
QT_MOC_LITERAL(9, 136, 14), // "maxScrollVData"
QT_MOC_LITERAL(10, 151, 32), // "receiveBscansTransGlWidgetPoints"
QT_MOC_LITERAL(11, 184, 38), // "std::map<std::string,st_bscan..."
QT_MOC_LITERAL(12, 223, 17), // "bscansInfoDataMap"
QT_MOC_LITERAL(13, 241, 17), // "st_bscansWorkData"
QT_MOC_LITERAL(14, 259, 14), // "bscansWorkData"
QT_MOC_LITERAL(15, 274, 32), // "std::vector<std::pair<int,int> >"
QT_MOC_LITERAL(16, 307, 39), // "vectorBscansTransLineSectionF..."
QT_MOC_LITERAL(17, 347, 14), // "highLowOnBscan"
QT_MOC_LITERAL(18, 362, 28), // "slotClearBscansTransGlWidget"
QT_MOC_LITERAL(19, 391, 30), // "slotChangeBscansTransPixelsFnX"
QT_MOC_LITERAL(20, 422, 20), // "bscansTransPixelsFnX"
QT_MOC_LITERAL(21, 443, 30), // "slotChangeBscansTransPixelsFnY"
QT_MOC_LITERAL(22, 474, 20) // "bscansTransPixelsFnY"

    },
    "bscansTransGlWidget\0signalSetupScrollHData\0"
    "\0minScrollHData\0scrollHData\0maxScrollHData\0"
    "signalSetupScrollVData\0minScrollVData\0"
    "scrollVData\0maxScrollVData\0"
    "receiveBscansTransGlWidgetPoints\0"
    "std::map<std::string,st_bscanInfoData>\0"
    "bscansInfoDataMap\0st_bscansWorkData\0"
    "bscansWorkData\0std::vector<std::pair<int,int> >\0"
    "vectorBscansTransLineSectionFinalPoints\0"
    "highLowOnBscan\0slotClearBscansTransGlWidget\0"
    "slotChangeBscansTransPixelsFnX\0"
    "bscansTransPixelsFnX\0"
    "slotChangeBscansTransPixelsFnY\0"
    "bscansTransPixelsFnY"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_bscansTransGlWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   44,    2, 0x06 /* Public */,
       6,    3,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    4,   58,    2, 0x0a /* Public */,
      18,    0,   67,    2, 0x0a /* Public */,
      19,    1,   68,    2, 0x0a /* Public */,
      21,    1,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    7,    8,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 13, 0x80000000 | 15, QMetaType::Bool,   12,   14,   16,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Int,   22,

       0        // eod
};

void bscansTransGlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        bscansTransGlWidget *_t = static_cast<bscansTransGlWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalSetupScrollHData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->signalSetupScrollVData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->receiveBscansTransGlWidgetPoints((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2])),(*reinterpret_cast< const std::vector<std::pair<int,int> >(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 3: _t->slotClearBscansTransGlWidget(); break;
        case 4: _t->slotChangeBscansTransPixelsFnX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slotChangeBscansTransPixelsFnY((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (bscansTransGlWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscansTransGlWidget::signalSetupScrollHData)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (bscansTransGlWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscansTransGlWidget::signalSetupScrollVData)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject bscansTransGlWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_bscansTransGlWidget.data,
      qt_meta_data_bscansTransGlWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *bscansTransGlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *bscansTransGlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_bscansTransGlWidget.stringdata0))
        return static_cast<void*>(const_cast< bscansTransGlWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions_1_1"))
        return static_cast< QOpenGLFunctions_1_1*>(const_cast< bscansTransGlWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int bscansTransGlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
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
void bscansTransGlWidget::signalSetupScrollHData(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void bscansTransGlWidget::signalSetupScrollVData(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
