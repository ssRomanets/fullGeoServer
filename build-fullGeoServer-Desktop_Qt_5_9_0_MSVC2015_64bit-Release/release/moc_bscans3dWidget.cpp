/****************************************************************************
** Meta object code from reading C++ file 'bscans3dWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/bscans3dWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bscans3dWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_bscans3dWidget_t {
    QByteArrayData data[46];
    char stringdata0[880];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_bscans3dWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_bscans3dWidget_t qt_meta_stringdata_bscans3dWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "bscans3dWidget"
QT_MOC_LITERAL(1, 15, 21), // "sendOutBscansSurfInfo"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 17), // "outBscansSurfInfo"
QT_MOC_LITERAL(4, 56, 22), // "signalCloseCut3dAction"
QT_MOC_LITERAL(5, 79, 12), // "cut3dVisible"
QT_MOC_LITERAL(6, 92, 19), // "sendDeleteBscanName"
QT_MOC_LITERAL(7, 112, 11), // "std::string"
QT_MOC_LITERAL(8, 124, 15), // "deleteBscanName"
QT_MOC_LITERAL(9, 140, 16), // "sendProgressData"
QT_MOC_LITERAL(10, 157, 11), // "progressPos"
QT_MOC_LITERAL(11, 169, 13), // "progressWidth"
QT_MOC_LITERAL(12, 183, 19), // "signalFixFillBscans"
QT_MOC_LITERAL(13, 203, 20), // "resetCutPointsBscans"
QT_MOC_LITERAL(14, 224, 24), // "signalFixDeleteBscanName"
QT_MOC_LITERAL(15, 249, 11), // "fixLatitude"
QT_MOC_LITERAL(16, 261, 12), // "fixLongitude"
QT_MOC_LITERAL(17, 274, 24), // "signalDefDeleteBscanName"
QT_MOC_LITERAL(18, 299, 38), // "std::map<std::string,st_bscan..."
QT_MOC_LITERAL(19, 338, 17), // "bscansInfoDataMap"
QT_MOC_LITERAL(20, 356, 23), // "signalFillBscansExecute"
QT_MOC_LITERAL(21, 380, 17), // "st_bscansWorkData"
QT_MOC_LITERAL(22, 398, 14), // "bscansWorkData"
QT_MOC_LITERAL(23, 413, 25), // "updateBscansTransGlWidget"
QT_MOC_LITERAL(24, 439, 32), // "std::vector<std::pair<int,int> >"
QT_MOC_LITERAL(25, 472, 39), // "vectorBscansTransLineSectionF..."
QT_MOC_LITERAL(26, 512, 14), // "highLowOnBscan"
QT_MOC_LITERAL(27, 527, 20), // "slotCloseCut3dAction"
QT_MOC_LITERAL(28, 548, 22), // "receiveDeleteBscanName"
QT_MOC_LITERAL(29, 571, 22), // "slotFixDeleteBscanName"
QT_MOC_LITERAL(30, 594, 17), // "slotFixFillBscans"
QT_MOC_LITERAL(31, 612, 22), // "slotDefDeleteBscanName"
QT_MOC_LITERAL(32, 635, 21), // "slotFillBscansExecute"
QT_MOC_LITERAL(33, 657, 32), // "receiveBscansTransGlWidgetPoints"
QT_MOC_LITERAL(34, 690, 16), // "setLowPixelLevel"
QT_MOC_LITERAL(35, 707, 13), // "lowPixelLevel"
QT_MOC_LITERAL(36, 721, 17), // "setHighPixelLevel"
QT_MOC_LITERAL(37, 739, 14), // "highPixelLevel"
QT_MOC_LITERAL(38, 754, 20), // "slotSetupScrollHData"
QT_MOC_LITERAL(39, 775, 14), // "minScrollHData"
QT_MOC_LITERAL(40, 790, 11), // "scrollHData"
QT_MOC_LITERAL(41, 802, 14), // "maxScrollHData"
QT_MOC_LITERAL(42, 817, 20), // "slotSetupScrollVData"
QT_MOC_LITERAL(43, 838, 14), // "minScrollVData"
QT_MOC_LITERAL(44, 853, 11), // "scrollVData"
QT_MOC_LITERAL(45, 865, 14) // "maxScrollVData"

    },
    "bscans3dWidget\0sendOutBscansSurfInfo\0"
    "\0outBscansSurfInfo\0signalCloseCut3dAction\0"
    "cut3dVisible\0sendDeleteBscanName\0"
    "std::string\0deleteBscanName\0"
    "sendProgressData\0progressPos\0progressWidth\0"
    "signalFixFillBscans\0resetCutPointsBscans\0"
    "signalFixDeleteBscanName\0fixLatitude\0"
    "fixLongitude\0signalDefDeleteBscanName\0"
    "std::map<std::string,st_bscanInfoData>\0"
    "bscansInfoDataMap\0signalFillBscansExecute\0"
    "st_bscansWorkData\0bscansWorkData\0"
    "updateBscansTransGlWidget\0"
    "std::vector<std::pair<int,int> >\0"
    "vectorBscansTransLineSectionFinalPoints\0"
    "highLowOnBscan\0slotCloseCut3dAction\0"
    "receiveDeleteBscanName\0slotFixDeleteBscanName\0"
    "slotFixFillBscans\0slotDefDeleteBscanName\0"
    "slotFillBscansExecute\0"
    "receiveBscansTransGlWidgetPoints\0"
    "setLowPixelLevel\0lowPixelLevel\0"
    "setHighPixelLevel\0highPixelLevel\0"
    "slotSetupScrollHData\0minScrollHData\0"
    "scrollHData\0maxScrollHData\0"
    "slotSetupScrollVData\0minScrollVData\0"
    "scrollVData\0maxScrollVData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_bscans3dWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x06 /* Public */,
       4,    1,  117,    2, 0x06 /* Public */,
       6,    1,  120,    2, 0x06 /* Public */,
       9,    2,  123,    2, 0x06 /* Public */,
      12,    1,  128,    2, 0x06 /* Public */,
      14,    2,  131,    2, 0x06 /* Public */,
      17,    3,  136,    2, 0x06 /* Public */,
      20,    3,  143,    2, 0x06 /* Public */,
      23,    4,  150,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      27,    1,  159,    2, 0x0a /* Public */,
      28,    1,  162,    2, 0x0a /* Public */,
      29,    2,  165,    2, 0x0a /* Public */,
      30,    1,  170,    2, 0x0a /* Public */,
      31,    3,  173,    2, 0x0a /* Public */,
      32,    3,  180,    2, 0x0a /* Public */,
      33,    4,  187,    2, 0x0a /* Public */,
      34,    1,  196,    2, 0x0a /* Public */,
      36,    1,  199,    2, 0x0a /* Public */,
      38,    3,  202,    2, 0x0a /* Public */,
      42,    3,  209,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   15,   16,
    QMetaType::Void, 0x80000000 | 18, QMetaType::Double, QMetaType::Double,   19,   15,   16,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 21, QMetaType::Bool,   19,   22,   13,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 21, 0x80000000 | 24, QMetaType::Bool,   19,   22,   25,   26,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   15,   16,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, 0x80000000 | 18, QMetaType::Double, QMetaType::Double,   19,   15,   16,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 21, QMetaType::Bool,   19,   22,   13,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 21, 0x80000000 | 24, QMetaType::Bool,   19,   22,   25,   26,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   39,   40,   41,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   43,   44,   45,

       0        // eod
};

void bscans3dWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        bscans3dWidget *_t = static_cast<bscans3dWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendOutBscansSurfInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signalCloseCut3dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->sendDeleteBscanName((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 3: _t->sendProgressData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->signalFixFillBscans((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->signalFixDeleteBscanName((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: _t->signalDefDeleteBscanName((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 7: _t->signalFillBscansExecute((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 8: _t->updateBscansTransGlWidget((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2])),(*reinterpret_cast< const std::vector<std::pair<int,int> >(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 9: _t->slotCloseCut3dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->receiveDeleteBscanName((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 11: _t->slotFixDeleteBscanName((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 12: _t->slotFixFillBscans((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->slotDefDeleteBscanName((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 14: _t->slotFillBscansExecute((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 15: _t->receiveBscansTransGlWidgetPoints((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2])),(*reinterpret_cast< const std::vector<std::pair<int,int> >(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 16: _t->setLowPixelLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->setHighPixelLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->slotSetupScrollHData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 19: _t->slotSetupScrollVData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (bscans3dWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::sendOutBscansSurfInfo)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (bscans3dWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::signalCloseCut3dAction)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (bscans3dWidget::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::sendDeleteBscanName)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (bscans3dWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::sendProgressData)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (bscans3dWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::signalFixFillBscans)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (bscans3dWidget::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::signalFixDeleteBscanName)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (bscans3dWidget::*_t)(const std::map<std::string,st_bscanInfoData> & , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::signalDefDeleteBscanName)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (bscans3dWidget::*_t)(const std::map<std::string,st_bscanInfoData> & , const st_bscansWorkData & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::signalFillBscansExecute)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (bscans3dWidget::*_t)(const std::map<std::string,st_bscanInfoData> & , const st_bscansWorkData & , const std::vector<std::pair<int,int>> & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bscans3dWidget::updateBscansTransGlWidget)) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject bscans3dWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_bscans3dWidget.data,
      qt_meta_data_bscans3dWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *bscans3dWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *bscans3dWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_bscans3dWidget.stringdata0))
        return static_cast<void*>(const_cast< bscans3dWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int bscans3dWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void bscans3dWidget::sendOutBscansSurfInfo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void bscans3dWidget::signalCloseCut3dAction(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void bscans3dWidget::sendDeleteBscanName(const std::string & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void bscans3dWidget::sendProgressData(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void bscans3dWidget::signalFixFillBscans(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void bscans3dWidget::signalFixDeleteBscanName(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void bscans3dWidget::signalDefDeleteBscanName(const std::map<std::string,st_bscanInfoData> & _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void bscans3dWidget::signalFillBscansExecute(const std::map<std::string,st_bscanInfoData> & _t1, const st_bscansWorkData & _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void bscans3dWidget::updateBscansTransGlWidget(const std::map<std::string,st_bscanInfoData> & _t1, const st_bscansWorkData & _t2, const std::vector<std::pair<int,int>> & _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
