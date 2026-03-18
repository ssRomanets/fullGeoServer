/****************************************************************************
** Meta object code from reading C++ file 'structureBscansGraph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/structureBscansGraph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'structureBscansGraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_structureBscansGraph_t {
    QByteArrayData data[28];
    char stringdata0[577];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_structureBscansGraph_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_structureBscansGraph_t qt_meta_stringdata_structureBscansGraph = {
    {
QT_MOC_LITERAL(0, 0, 20), // "structureBscansGraph"
QT_MOC_LITERAL(1, 21, 25), // "updateBscansTransGlWidget"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 38), // "std::map<std::string,st_bscan..."
QT_MOC_LITERAL(4, 87, 17), // "bscansInfoDataMap"
QT_MOC_LITERAL(5, 105, 17), // "st_bscansWorkData"
QT_MOC_LITERAL(6, 123, 14), // "bscansWorkData"
QT_MOC_LITERAL(7, 138, 32), // "std::vector<std::pair<int,int> >"
QT_MOC_LITERAL(8, 171, 39), // "vectorBscansTransLineSectionF..."
QT_MOC_LITERAL(9, 211, 14), // "highLowOnBscan"
QT_MOC_LITERAL(10, 226, 28), // "closeCorrectBscansSurfWidget"
QT_MOC_LITERAL(11, 255, 27), // "showCorrectBscansSurfWidget"
QT_MOC_LITERAL(12, 283, 7), // "QColor&"
QT_MOC_LITERAL(13, 291, 16), // "colorBscansSurf1"
QT_MOC_LITERAL(14, 308, 16), // "colorBscansSurf2"
QT_MOC_LITERAL(15, 325, 11), // "fixLatitude"
QT_MOC_LITERAL(16, 337, 12), // "fixLongitude"
QT_MOC_LITERAL(17, 350, 30), // "signalClearBscansTransGlWidget"
QT_MOC_LITERAL(18, 381, 21), // "sendOutBscansSurfInfo"
QT_MOC_LITERAL(19, 403, 17), // "outBscansSurfInfo"
QT_MOC_LITERAL(20, 421, 19), // "signalFixFillBscans"
QT_MOC_LITERAL(21, 441, 20), // "resetCutPointsBscans"
QT_MOC_LITERAL(22, 462, 20), // "slotCloseCut3dAction"
QT_MOC_LITERAL(23, 483, 12), // "cut3dVisible"
QT_MOC_LITERAL(24, 496, 23), // "receiveColorsBscansSurf"
QT_MOC_LITERAL(25, 520, 21), // "handleElementSelected"
QT_MOC_LITERAL(26, 542, 29), // "QAbstract3DGraph::ElementType"
QT_MOC_LITERAL(27, 572, 4) // "type"

    },
    "structureBscansGraph\0updateBscansTransGlWidget\0"
    "\0std::map<std::string,st_bscanInfoData>\0"
    "bscansInfoDataMap\0st_bscansWorkData\0"
    "bscansWorkData\0std::vector<std::pair<int,int> >\0"
    "vectorBscansTransLineSectionFinalPoints\0"
    "highLowOnBscan\0closeCorrectBscansSurfWidget\0"
    "showCorrectBscansSurfWidget\0QColor&\0"
    "colorBscansSurf1\0colorBscansSurf2\0"
    "fixLatitude\0fixLongitude\0"
    "signalClearBscansTransGlWidget\0"
    "sendOutBscansSurfInfo\0outBscansSurfInfo\0"
    "signalFixFillBscans\0resetCutPointsBscans\0"
    "slotCloseCut3dAction\0cut3dVisible\0"
    "receiveColorsBscansSurf\0handleElementSelected\0"
    "QAbstract3DGraph::ElementType\0type"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_structureBscansGraph[] = {

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
      10,    0,   68,    2, 0x06 /* Public */,
      11,    4,   69,    2, 0x06 /* Public */,
      17,    0,   78,    2, 0x06 /* Public */,
      18,    1,   79,    2, 0x06 /* Public */,
      20,    1,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      22,    1,   85,    2, 0x0a /* Public */,
      24,    2,   88,    2, 0x0a /* Public */,
      25,    1,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 7, QMetaType::Bool,    4,    6,    8,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 12, QMetaType::Double, QMetaType::Double,   13,   14,   15,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::Bool,   21,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   23,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 12,   13,   14,
    QMetaType::Void, 0x80000000 | 26,   27,

       0        // eod
};

void structureBscansGraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        structureBscansGraph *_t = static_cast<structureBscansGraph *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateBscansTransGlWidget((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2])),(*reinterpret_cast< const std::vector<std::pair<int,int> >(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->closeCorrectBscansSurfWidget(); break;
        case 2: _t->showCorrectBscansSurfWidget((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 3: _t->signalClearBscansTransGlWidget(); break;
        case 4: _t->sendOutBscansSurfInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->signalFixFillBscans((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->slotCloseCut3dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->receiveColorsBscansSurf((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 8: _t->handleElementSelected((*reinterpret_cast< QAbstract3DGraph::ElementType(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (structureBscansGraph::*_t)(const std::map<std::string,st_bscanInfoData> & , const st_bscansWorkData & , const std::vector<std::pair<int,int>> & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&structureBscansGraph::updateBscansTransGlWidget)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (structureBscansGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&structureBscansGraph::closeCorrectBscansSurfWidget)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (structureBscansGraph::*_t)(QColor & , QColor & , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&structureBscansGraph::showCorrectBscansSurfWidget)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (structureBscansGraph::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&structureBscansGraph::signalClearBscansTransGlWidget)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (structureBscansGraph::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&structureBscansGraph::sendOutBscansSurfInfo)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (structureBscansGraph::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&structureBscansGraph::signalFixFillBscans)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject structureBscansGraph::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_structureBscansGraph.data,
      qt_meta_data_structureBscansGraph,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *structureBscansGraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *structureBscansGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_structureBscansGraph.stringdata0))
        return static_cast<void*>(const_cast< structureBscansGraph*>(this));
    return QObject::qt_metacast(_clname);
}

int structureBscansGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void structureBscansGraph::updateBscansTransGlWidget(const std::map<std::string,st_bscanInfoData> & _t1, const st_bscansWorkData & _t2, const std::vector<std::pair<int,int>> & _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void structureBscansGraph::closeCorrectBscansSurfWidget()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void structureBscansGraph::showCorrectBscansSurfWidget(QColor & _t1, QColor & _t2, double _t3, double _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void structureBscansGraph::signalClearBscansTransGlWidget()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void structureBscansGraph::sendOutBscansSurfInfo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void structureBscansGraph::signalFixFillBscans(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
