/****************************************************************************
** Meta object code from reading C++ file 'sectionBscansSurfWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/sectionBscansSurfWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sectionBscansSurfWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_sectionBscansSurfWidget_t {
    QByteArrayData data[10];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sectionBscansSurfWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sectionBscansSurfWidget_t qt_meta_stringdata_sectionBscansSurfWidget = {
    {
QT_MOC_LITERAL(0, 0, 23), // "sectionBscansSurfWidget"
QT_MOC_LITERAL(1, 24, 26), // "signalFixBscansSurfSection"
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 20), // "incExcBscansSurfNorm"
QT_MOC_LITERAL(4, 73, 24), // "updateBscansSurfSections"
QT_MOC_LITERAL(5, 98, 6), // "inPosX"
QT_MOC_LITERAL(6, 105, 6), // "inPosY"
QT_MOC_LITERAL(7, 112, 6), // "fnPosX"
QT_MOC_LITERAL(8, 119, 6), // "fnPosY"
QT_MOC_LITERAL(9, 126, 22) // "hideBscansSurfSections"

    },
    "sectionBscansSurfWidget\0"
    "signalFixBscansSurfSection\0\0"
    "incExcBscansSurfNorm\0updateBscansSurfSections\0"
    "inPosX\0inPosY\0fnPosX\0fnPosY\0"
    "hideBscansSurfSections"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sectionBscansSurfWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x0a /* Public */,
       4,    4,   36,    2, 0x0a /* Public */,
       9,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    5,    6,    7,    8,
    QMetaType::Void,

       0        // eod
};

void sectionBscansSurfWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        sectionBscansSurfWidget *_t = static_cast<sectionBscansSurfWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalFixBscansSurfSection(); break;
        case 1: _t->incExcBscansSurfNorm(); break;
        case 2: _t->updateBscansSurfSections((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->hideBscansSurfSections(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (sectionBscansSurfWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&sectionBscansSurfWidget::signalFixBscansSurfSection)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject sectionBscansSurfWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sectionBscansSurfWidget.data,
      qt_meta_data_sectionBscansSurfWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *sectionBscansSurfWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sectionBscansSurfWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_sectionBscansSurfWidget.stringdata0))
        return static_cast<void*>(const_cast< sectionBscansSurfWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sectionBscansSurfWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void sectionBscansSurfWidget::signalFixBscansSurfSection()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
