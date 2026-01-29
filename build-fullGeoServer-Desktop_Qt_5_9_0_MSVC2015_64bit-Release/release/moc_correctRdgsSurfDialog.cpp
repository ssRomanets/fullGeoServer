/****************************************************************************
** Meta object code from reading C++ file 'correctRdgsSurfDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/correctRdgsSurfDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'correctRdgsSurfDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_correctRdgsSurfDialog_t {
    QByteArrayData data[13];
    char stringdata0[184];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_correctRdgsSurfDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_correctRdgsSurfDialog_t qt_meta_stringdata_correctRdgsSurfDialog = {
    {
QT_MOC_LITERAL(0, 0, 21), // "correctRdgsSurfDialog"
QT_MOC_LITERAL(1, 22, 18), // "sendColorsRdgsSurf"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 7), // "QColor&"
QT_MOC_LITERAL(4, 50, 14), // "colorRdgsSurf1"
QT_MOC_LITERAL(5, 65, 14), // "colorRdgsSurf2"
QT_MOC_LITERAL(6, 80, 23), // "signalSendDeleteRdgName"
QT_MOC_LITERAL(7, 104, 11), // "std::string"
QT_MOC_LITERAL(8, 116, 13), // "deleteRdgName"
QT_MOC_LITERAL(9, 130, 9), // "defColor1"
QT_MOC_LITERAL(10, 140, 9), // "defColor2"
QT_MOC_LITERAL(11, 150, 10), // "sendColors"
QT_MOC_LITERAL(12, 161, 22) // "slotsSendDeleteRdgName"

    },
    "correctRdgsSurfDialog\0sendColorsRdgsSurf\0"
    "\0QColor&\0colorRdgsSurf1\0colorRdgsSurf2\0"
    "signalSendDeleteRdgName\0std::string\0"
    "deleteRdgName\0defColor1\0defColor2\0"
    "sendColors\0slotsSendDeleteRdgName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_correctRdgsSurfDialog[] = {

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
       1,    2,   44,    2, 0x06 /* Public */,
       6,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   52,    2, 0x08 /* Private */,
      10,    0,   53,    2, 0x08 /* Private */,
      11,    0,   54,    2, 0x08 /* Private */,
      12,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void correctRdgsSurfDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        correctRdgsSurfDialog *_t = static_cast<correctRdgsSurfDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendColorsRdgsSurf((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 1: _t->signalSendDeleteRdgName((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 2: _t->defColor1(); break;
        case 3: _t->defColor2(); break;
        case 4: _t->sendColors(); break;
        case 5: _t->slotsSendDeleteRdgName(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (correctRdgsSurfDialog::*_t)(QColor & , QColor & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&correctRdgsSurfDialog::sendColorsRdgsSurf)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (correctRdgsSurfDialog::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&correctRdgsSurfDialog::signalSendDeleteRdgName)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject correctRdgsSurfDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_correctRdgsSurfDialog.data,
      qt_meta_data_correctRdgsSurfDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *correctRdgsSurfDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *correctRdgsSurfDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_correctRdgsSurfDialog.stringdata0))
        return static_cast<void*>(const_cast< correctRdgsSurfDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int correctRdgsSurfDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void correctRdgsSurfDialog::sendColorsRdgsSurf(QColor & _t1, QColor & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void correctRdgsSurfDialog::signalSendDeleteRdgName(const std::string & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
