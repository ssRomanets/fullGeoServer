/****************************************************************************
** Meta object code from reading C++ file 'colorsPaletteDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/colorsPaletteDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'colorsPaletteDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_colorsPaletteDialog_t {
    QByteArrayData data[9];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_colorsPaletteDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_colorsPaletteDialog_t qt_meta_stringdata_colorsPaletteDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "colorsPaletteDialog"
QT_MOC_LITERAL(1, 20, 17), // "sendColorsPalette"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 7), // "QColor&"
QT_MOC_LITERAL(4, 47, 7), // "colorUp"
QT_MOC_LITERAL(5, 55, 8), // "colorLow"
QT_MOC_LITERAL(6, 64, 10), // "defColorUp"
QT_MOC_LITERAL(7, 75, 11), // "defColorLow"
QT_MOC_LITERAL(8, 87, 10) // "sendColors"

    },
    "colorsPaletteDialog\0sendColorsPalette\0"
    "\0QColor&\0colorUp\0colorLow\0defColorUp\0"
    "defColorLow\0sendColors"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_colorsPaletteDialog[] = {

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
       1,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   39,    2, 0x08 /* Private */,
       7,    0,   40,    2, 0x08 /* Private */,
       8,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void colorsPaletteDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        colorsPaletteDialog *_t = static_cast<colorsPaletteDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendColorsPalette((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 1: _t->defColorUp(); break;
        case 2: _t->defColorLow(); break;
        case 3: _t->sendColors(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (colorsPaletteDialog::*_t)(QColor & , QColor & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&colorsPaletteDialog::sendColorsPalette)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject colorsPaletteDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_colorsPaletteDialog.data,
      qt_meta_data_colorsPaletteDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *colorsPaletteDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *colorsPaletteDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_colorsPaletteDialog.stringdata0))
        return static_cast<void*>(const_cast< colorsPaletteDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int colorsPaletteDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void colorsPaletteDialog::sendColorsPalette(QColor & _t1, QColor & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
