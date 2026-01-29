/****************************************************************************
** Meta object code from reading C++ file 'accomplishmentThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/accomplishmentThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'accomplishmentThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_accomplishmentThread_t {
    QByteArrayData data[13];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_accomplishmentThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_accomplishmentThread_t qt_meta_stringdata_accomplishmentThread = {
    {
QT_MOC_LITERAL(0, 0, 20), // "accomplishmentThread"
QT_MOC_LITERAL(1, 21, 18), // "signalEndAccThread"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 48), // "std::vector<std::pair<std::st..."
QT_MOC_LITERAL(4, 90, 15), // "rdgsNamesVector"
QT_MOC_LITERAL(5, 106, 13), // "RdgFileFormat"
QT_MOC_LITERAL(6, 120, 13), // "rdgFileFormat"
QT_MOC_LITERAL(7, 134, 16), // "sendProgressData"
QT_MOC_LITERAL(8, 151, 11), // "progressPos"
QT_MOC_LITERAL(9, 163, 11), // "progressMax"
QT_MOC_LITERAL(10, 175, 20), // "sendSurfRdgsWorkData"
QT_MOC_LITERAL(11, 196, 13), // "sendRdgsZData"
QT_MOC_LITERAL(12, 210, 24) // "sendProgressRdgsWorkData"

    },
    "accomplishmentThread\0signalEndAccThread\0"
    "\0std::vector<std::pair<std::string,std::string> >\0"
    "rdgsNamesVector\0RdgFileFormat\0"
    "rdgFileFormat\0sendProgressData\0"
    "progressPos\0progressMax\0sendSurfRdgsWorkData\0"
    "sendRdgsZData\0sendProgressRdgsWorkData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_accomplishmentThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       7,    2,   44,    2, 0x06 /* Public */,
      10,    0,   49,    2, 0x06 /* Public */,
      11,    0,   50,    2, 0x06 /* Public */,
      12,    2,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,

       0        // eod
};

void accomplishmentThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        accomplishmentThread *_t = static_cast<accomplishmentThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalEndAccThread((*reinterpret_cast< std::vector<std::pair<std::string,std::string> >(*)>(_a[1])),(*reinterpret_cast< RdgFileFormat(*)>(_a[2]))); break;
        case 1: _t->sendProgressData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->sendSurfRdgsWorkData(); break;
        case 3: _t->sendRdgsZData(); break;
        case 4: _t->sendProgressRdgsWorkData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (accomplishmentThread::*_t)(std::vector<std::pair<std::string,std::string>> , RdgFileFormat );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&accomplishmentThread::signalEndAccThread)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (accomplishmentThread::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&accomplishmentThread::sendProgressData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (accomplishmentThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&accomplishmentThread::sendSurfRdgsWorkData)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (accomplishmentThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&accomplishmentThread::sendRdgsZData)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (accomplishmentThread::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&accomplishmentThread::sendProgressRdgsWorkData)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject accomplishmentThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_accomplishmentThread.data,
      qt_meta_data_accomplishmentThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *accomplishmentThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *accomplishmentThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_accomplishmentThread.stringdata0))
        return static_cast<void*>(const_cast< accomplishmentThread*>(this));
    return QThread::qt_metacast(_clname);
}

int accomplishmentThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void accomplishmentThread::signalEndAccThread(std::vector<std::pair<std::string,std::string>> _t1, RdgFileFormat _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void accomplishmentThread::sendProgressData(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void accomplishmentThread::sendSurfRdgsWorkData()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void accomplishmentThread::sendRdgsZData()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void accomplishmentThread::sendProgressRdgsWorkData(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
