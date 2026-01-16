/****************************************************************************
** Meta object code from reading C++ file 'sectionRdgsSurfWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/sectionRdgsSurfWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sectionRdgsSurfWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_sectionRdgsSurfWidget_t {
    QByteArrayData data[10];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sectionRdgsSurfWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sectionRdgsSurfWidget_t qt_meta_stringdata_sectionRdgsSurfWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "sectionRdgsSurfWidget"
QT_MOC_LITERAL(1, 22, 24), // "signalFixRdgsSurfSection"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 18), // "incExcRdgsSurfNorm"
QT_MOC_LITERAL(4, 67, 22), // "updateRdgsSurfSections"
QT_MOC_LITERAL(5, 90, 6), // "inPosX"
QT_MOC_LITERAL(6, 97, 6), // "inPosY"
QT_MOC_LITERAL(7, 104, 6), // "fnPosX"
QT_MOC_LITERAL(8, 111, 6), // "fnPosY"
QT_MOC_LITERAL(9, 118, 20) // "hideRdgsSurfSections"

    },
    "sectionRdgsSurfWidget\0signalFixRdgsSurfSection\0"
    "\0incExcRdgsSurfNorm\0updateRdgsSurfSections\0"
    "inPosX\0inPosY\0fnPosX\0fnPosY\0"
    "hideRdgsSurfSections"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sectionRdgsSurfWidget[] = {

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

void sectionRdgsSurfWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        sectionRdgsSurfWidget *_t = static_cast<sectionRdgsSurfWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalFixRdgsSurfSection(); break;
        case 1: _t->incExcRdgsSurfNorm(); break;
        case 2: _t->updateRdgsSurfSections((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->hideRdgsSurfSections(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (sectionRdgsSurfWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&sectionRdgsSurfWidget::signalFixRdgsSurfSection)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject sectionRdgsSurfWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sectionRdgsSurfWidget.data,
      qt_meta_data_sectionRdgsSurfWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *sectionRdgsSurfWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sectionRdgsSurfWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_sectionRdgsSurfWidget.stringdata0))
        return static_cast<void*>(const_cast< sectionRdgsSurfWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sectionRdgsSurfWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void sectionRdgsSurfWidget::signalFixRdgsSurfSection()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
