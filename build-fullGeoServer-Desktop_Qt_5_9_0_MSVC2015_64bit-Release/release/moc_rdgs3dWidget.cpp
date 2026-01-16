/****************************************************************************
** Meta object code from reading C++ file 'rdgs3dWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/rdgs3dWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rdgs3dWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_rdgs3dWidget_t {
    QByteArrayData data[46];
    char stringdata0[906];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_rdgs3dWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_rdgs3dWidget_t qt_meta_stringdata_rdgs3dWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "rdgs3dWidget"
QT_MOC_LITERAL(1, 13, 19), // "sendOutRdgsSurfInfo"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 15), // "outRdgsSurfInfo"
QT_MOC_LITERAL(4, 50, 22), // "signalCloseCut3dAction"
QT_MOC_LITERAL(5, 73, 12), // "cut3dVisible"
QT_MOC_LITERAL(6, 86, 17), // "sendDeleteRdgName"
QT_MOC_LITERAL(7, 104, 11), // "std::string"
QT_MOC_LITERAL(8, 116, 13), // "deleteRdgName"
QT_MOC_LITERAL(9, 130, 16), // "sendProgressData"
QT_MOC_LITERAL(10, 147, 11), // "progressPos"
QT_MOC_LITERAL(11, 159, 13), // "progressWidth"
QT_MOC_LITERAL(12, 173, 17), // "signalFixFillRdgs"
QT_MOC_LITERAL(13, 191, 18), // "resetCutPointsRdgs"
QT_MOC_LITERAL(14, 210, 22), // "signalFixDeleteRdgName"
QT_MOC_LITERAL(15, 233, 11), // "fixLatitude"
QT_MOC_LITERAL(16, 245, 12), // "fixLongitude"
QT_MOC_LITERAL(17, 258, 22), // "signalDefDeleteRdgName"
QT_MOC_LITERAL(18, 281, 36), // "std::map<std::string,st_rdgIn..."
QT_MOC_LITERAL(19, 318, 15), // "rdgsInfoDataMap"
QT_MOC_LITERAL(20, 334, 21), // "signalFillRdgsExecute"
QT_MOC_LITERAL(21, 356, 15), // "st_rdgsWorkData"
QT_MOC_LITERAL(22, 372, 12), // "rdgsWorkData"
QT_MOC_LITERAL(23, 385, 23), // "updateRdgsTransGlWidget"
QT_MOC_LITERAL(24, 409, 32), // "std::vector<std::pair<int,int> >"
QT_MOC_LITERAL(25, 442, 37), // "vectorRdgsTransLineSectionFin..."
QT_MOC_LITERAL(26, 480, 12), // "highLowOnRdg"
QT_MOC_LITERAL(27, 493, 20), // "slotCloseCut3dAction"
QT_MOC_LITERAL(28, 514, 20), // "receiveDeleteRdgName"
QT_MOC_LITERAL(29, 535, 20), // "slotFixDeleteRdgName"
QT_MOC_LITERAL(30, 556, 15), // "slotFixFillRdgs"
QT_MOC_LITERAL(31, 572, 20), // "slotDefDeleteRdgName"
QT_MOC_LITERAL(32, 593, 19), // "slotFillRdgsExecute"
QT_MOC_LITERAL(33, 613, 30), // "receiveRdgsTransGlWidgetPoints"
QT_MOC_LITERAL(34, 644, 16), // "setLowPixelLevel"
QT_MOC_LITERAL(35, 661, 13), // "lowPixelLevel"
QT_MOC_LITERAL(36, 675, 17), // "setHighPixelLevel"
QT_MOC_LITERAL(37, 693, 14), // "highPixelLevel"
QT_MOC_LITERAL(38, 708, 29), // "slotSetupScrollHRdgsTransData"
QT_MOC_LITERAL(39, 738, 23), // "minScrollHRdgsTransData"
QT_MOC_LITERAL(40, 762, 20), // "scrollHRdgsTransData"
QT_MOC_LITERAL(41, 783, 23), // "maxScrollHRdgsTransData"
QT_MOC_LITERAL(42, 807, 29), // "slotSetupScrollVRdgsTransData"
QT_MOC_LITERAL(43, 837, 23), // "minScrollVRdgsTransData"
QT_MOC_LITERAL(44, 861, 20), // "scrollVRdgsTransData"
QT_MOC_LITERAL(45, 882, 23) // "maxScrollVRdgsTransData"

    },
    "rdgs3dWidget\0sendOutRdgsSurfInfo\0\0"
    "outRdgsSurfInfo\0signalCloseCut3dAction\0"
    "cut3dVisible\0sendDeleteRdgName\0"
    "std::string\0deleteRdgName\0sendProgressData\0"
    "progressPos\0progressWidth\0signalFixFillRdgs\0"
    "resetCutPointsRdgs\0signalFixDeleteRdgName\0"
    "fixLatitude\0fixLongitude\0"
    "signalDefDeleteRdgName\0"
    "std::map<std::string,st_rdgInfoData>\0"
    "rdgsInfoDataMap\0signalFillRdgsExecute\0"
    "st_rdgsWorkData\0rdgsWorkData\0"
    "updateRdgsTransGlWidget\0"
    "std::vector<std::pair<int,int> >\0"
    "vectorRdgsTransLineSectionFinalPoints\0"
    "highLowOnRdg\0slotCloseCut3dAction\0"
    "receiveDeleteRdgName\0slotFixDeleteRdgName\0"
    "slotFixFillRdgs\0slotDefDeleteRdgName\0"
    "slotFillRdgsExecute\0receiveRdgsTransGlWidgetPoints\0"
    "setLowPixelLevel\0lowPixelLevel\0"
    "setHighPixelLevel\0highPixelLevel\0"
    "slotSetupScrollHRdgsTransData\0"
    "minScrollHRdgsTransData\0scrollHRdgsTransData\0"
    "maxScrollHRdgsTransData\0"
    "slotSetupScrollVRdgsTransData\0"
    "minScrollVRdgsTransData\0scrollVRdgsTransData\0"
    "maxScrollVRdgsTransData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_rdgs3dWidget[] = {

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

void rdgs3dWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        rdgs3dWidget *_t = static_cast<rdgs3dWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendOutRdgsSurfInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signalCloseCut3dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->sendDeleteRdgName((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 3: _t->sendProgressData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->signalFixFillRdgs((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->signalFixDeleteRdgName((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: _t->signalDefDeleteRdgName((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 7: _t->signalFillRdgsExecute((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_rdgsWorkData(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 8: _t->updateRdgsTransGlWidget((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_rdgsWorkData(*)>(_a[2])),(*reinterpret_cast< const std::vector<std::pair<int,int> >(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 9: _t->slotCloseCut3dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->receiveDeleteRdgName((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 11: _t->slotFixDeleteRdgName((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 12: _t->slotFixFillRdgs((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->slotDefDeleteRdgName((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 14: _t->slotFillRdgsExecute((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_rdgsWorkData(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 15: _t->receiveRdgsTransGlWidgetPoints((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_rdgsWorkData(*)>(_a[2])),(*reinterpret_cast< const std::vector<std::pair<int,int> >(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 16: _t->setLowPixelLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->setHighPixelLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->slotSetupScrollHRdgsTransData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 19: _t->slotSetupScrollVRdgsTransData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (rdgs3dWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::sendOutRdgsSurfInfo)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (rdgs3dWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::signalCloseCut3dAction)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (rdgs3dWidget::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::sendDeleteRdgName)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (rdgs3dWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::sendProgressData)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (rdgs3dWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::signalFixFillRdgs)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (rdgs3dWidget::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::signalFixDeleteRdgName)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (rdgs3dWidget::*_t)(const std::map<std::string,st_rdgInfoData> & , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::signalDefDeleteRdgName)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (rdgs3dWidget::*_t)(const std::map<std::string,st_rdgInfoData> & , const st_rdgsWorkData & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::signalFillRdgsExecute)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (rdgs3dWidget::*_t)(const std::map<std::string,st_rdgInfoData> & , const st_rdgsWorkData & , const std::vector<std::pair<int,int>> & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rdgs3dWidget::updateRdgsTransGlWidget)) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject rdgs3dWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_rdgs3dWidget.data,
      qt_meta_data_rdgs3dWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *rdgs3dWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *rdgs3dWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_rdgs3dWidget.stringdata0))
        return static_cast<void*>(const_cast< rdgs3dWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int rdgs3dWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void rdgs3dWidget::sendOutRdgsSurfInfo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void rdgs3dWidget::signalCloseCut3dAction(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void rdgs3dWidget::sendDeleteRdgName(const std::string & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void rdgs3dWidget::sendProgressData(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void rdgs3dWidget::signalFixFillRdgs(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void rdgs3dWidget::signalFixDeleteRdgName(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void rdgs3dWidget::signalDefDeleteRdgName(const std::map<std::string,st_rdgInfoData> & _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void rdgs3dWidget::signalFillRdgsExecute(const std::map<std::string,st_rdgInfoData> & _t1, const st_rdgsWorkData & _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void rdgs3dWidget::updateRdgsTransGlWidget(const std::map<std::string,st_rdgInfoData> & _t1, const st_rdgsWorkData & _t2, const std::vector<std::pair<int,int>> & _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
