/****************************************************************************
** Meta object code from reading C++ file 'tableBscanLogAutoDataWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/tableBscanLogAutoDataWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tableBscanLogAutoDataWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_tableBscanLogAutoDataWidget_t {
    QByteArrayData data[13];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tableBscanLogAutoDataWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tableBscanLogAutoDataWidget_t qt_meta_stringdata_tableBscanLogAutoDataWidget = {
    {
QT_MOC_LITERAL(0, 0, 27), // "tableBscanLogAutoDataWidget"
QT_MOC_LITERAL(1, 28, 19), // "receiveVectorPairXY"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 9), // "pixelsInX"
QT_MOC_LITERAL(4, 59, 9), // "pixelsInY"
QT_MOC_LITERAL(5, 69, 9), // "pixelsFnX"
QT_MOC_LITERAL(6, 79, 9), // "pixelsFnY"
QT_MOC_LITERAL(7, 89, 8), // "filterId"
QT_MOC_LITERAL(8, 98, 16), // "st_bscanInfoData"
QT_MOC_LITERAL(9, 115, 13), // "bscanInfoData"
QT_MOC_LITERAL(10, 129, 20), // "receiveLoadMapPairXY"
QT_MOC_LITERAL(11, 150, 43), // "std::map<int,std::vector<QPai..."
QT_MOC_LITERAL(12, 194, 9) // "mapPairXY"

    },
    "tableBscanLogAutoDataWidget\0"
    "receiveVectorPairXY\0\0pixelsInX\0pixelsInY\0"
    "pixelsFnX\0pixelsFnY\0filterId\0"
    "st_bscanInfoData\0bscanInfoData\0"
    "receiveLoadMapPairXY\0"
    "std::map<int,std::vector<QPair<int,int> > >\0"
    "mapPairXY"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tableBscanLogAutoDataWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    6,   24,    2, 0x0a /* Public */,
      10,    6,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 8,    3,    4,    5,    6,    7,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 8, 0x80000000 | 11,    3,    4,    5,    6,    9,   12,

       0        // eod
};

void tableBscanLogAutoDataWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        tableBscanLogAutoDataWidget *_t = static_cast<tableBscanLogAutoDataWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->receiveVectorPairXY((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< const st_bscanInfoData(*)>(_a[6]))); break;
        case 1: _t->receiveLoadMapPairXY((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< const st_bscanInfoData(*)>(_a[5])),(*reinterpret_cast< std::map<int,std::vector<QPair<int,int> > >(*)>(_a[6]))); break;
        default: ;
        }
    }
}

const QMetaObject tableBscanLogAutoDataWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_tableBscanLogAutoDataWidget.data,
      qt_meta_data_tableBscanLogAutoDataWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *tableBscanLogAutoDataWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tableBscanLogAutoDataWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_tableBscanLogAutoDataWidget.stringdata0))
        return static_cast<void*>(const_cast< tableBscanLogAutoDataWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int tableBscanLogAutoDataWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
