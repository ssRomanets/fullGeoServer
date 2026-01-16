/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fullGeoServer/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[85];
    char stringdata0[1540];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 26), // "signalClearRdgsInfoDataMap"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 26), // "signalEraseRdgsInfoDataMap"
QT_MOC_LITERAL(4, 66, 11), // "std::string"
QT_MOC_LITERAL(5, 78, 8), // "fileName"
QT_MOC_LITERAL(6, 87, 20), // "closeSection2dAction"
QT_MOC_LITERAL(7, 108, 16), // "section2dVisible"
QT_MOC_LITERAL(8, 125, 16), // "closeCut3dAction"
QT_MOC_LITERAL(9, 142, 12), // "cut3dVisible"
QT_MOC_LITERAL(10, 155, 25), // "signalSetupRdgsSurfPixels"
QT_MOC_LITERAL(11, 181, 36), // "std::map<std::string,st_rdgIn..."
QT_MOC_LITERAL(12, 218, 15), // "rdgsInfoDataMap"
QT_MOC_LITERAL(13, 234, 15), // "st_rdgsWorkData"
QT_MOC_LITERAL(14, 250, 12), // "rdgsWorkData"
QT_MOC_LITERAL(15, 263, 27), // "signalOutputRdgsSurfSection"
QT_MOC_LITERAL(16, 291, 22), // "signalDefDeleteRdgName"
QT_MOC_LITERAL(17, 314, 11), // "fixLatitude"
QT_MOC_LITERAL(18, 326, 12), // "fixLongitude"
QT_MOC_LITERAL(19, 339, 21), // "signalFillRdgsExecute"
QT_MOC_LITERAL(20, 361, 18), // "resetCutPointsRdgs"
QT_MOC_LITERAL(21, 380, 20), // "receiveDeleteRdgName"
QT_MOC_LITERAL(22, 401, 13), // "deleteRdgName"
QT_MOC_LITERAL(23, 415, 19), // "receiveProgressData"
QT_MOC_LITERAL(24, 435, 11), // "progressPos"
QT_MOC_LITERAL(25, 447, 11), // "progressMax"
QT_MOC_LITERAL(26, 459, 19), // "receiveRdgsWorkData"
QT_MOC_LITERAL(27, 479, 16), // "receiveRdgsZData"
QT_MOC_LITERAL(28, 496, 21), // "slotFixRdgsSurfPixels"
QT_MOC_LITERAL(29, 518, 20), // "slotFixDeleteRdgName"
QT_MOC_LITERAL(30, 539, 15), // "slotFixFillRdgs"
QT_MOC_LITERAL(31, 555, 22), // "slotFixRdgsSurfSection"
QT_MOC_LITERAL(32, 578, 24), // "slotActivateAccompThread"
QT_MOC_LITERAL(33, 603, 48), // "std::vector<std::pair<std::st..."
QT_MOC_LITERAL(34, 652, 20), // "rdgsNamesVectorPairs"
QT_MOC_LITERAL(35, 673, 13), // "RdgFileFormat"
QT_MOC_LITERAL(36, 687, 13), // "rdgFileFormat"
QT_MOC_LITERAL(37, 701, 20), // "receiveColorsPalette"
QT_MOC_LITERAL(38, 722, 7), // "QColor&"
QT_MOC_LITERAL(39, 730, 7), // "colorUp"
QT_MOC_LITERAL(40, 738, 8), // "colorLow"
QT_MOC_LITERAL(41, 747, 7), // "openTrz"
QT_MOC_LITERAL(42, 755, 7), // "openCsv"
QT_MOC_LITERAL(43, 763, 8), // "openHdf5"
QT_MOC_LITERAL(44, 772, 7), // "openKml"
QT_MOC_LITERAL(45, 780, 12), // "openImageRdg"
QT_MOC_LITERAL(46, 793, 13), // "defPaletteRdg"
QT_MOC_LITERAL(47, 807, 15), // "open2dImageRdgs"
QT_MOC_LITERAL(48, 823, 18), // "defPaletteSurfRdgs"
QT_MOC_LITERAL(49, 842, 15), // "open3dImageRdgs"
QT_MOC_LITERAL(50, 858, 19), // "defPaletteTransRdgs"
QT_MOC_LITERAL(51, 878, 18), // "includeFilterOnRdg"
QT_MOC_LITERAL(52, 897, 14), // "openRecentFile"
QT_MOC_LITERAL(53, 912, 18), // "deleteSelectedFile"
QT_MOC_LITERAL(54, 931, 14), // "deleteAllFiles"
QT_MOC_LITERAL(55, 946, 4), // "save"
QT_MOC_LITERAL(56, 951, 23), // "setCloseSection2dAction"
QT_MOC_LITERAL(57, 975, 19), // "setCloseCut3dAction"
QT_MOC_LITERAL(58, 995, 25), // "saveBoundPointsLineAction"
QT_MOC_LITERAL(59, 1021, 25), // "loadBoundPointsLineAction"
QT_MOC_LITERAL(60, 1047, 10), // "setInitRdg"
QT_MOC_LITERAL(61, 1058, 9), // "setLogRdg"
QT_MOC_LITERAL(62, 1068, 23), // "setLogRdgMouseSelection"
QT_MOC_LITERAL(63, 1092, 22), // "setLogRdgAutoSelection"
QT_MOC_LITERAL(64, 1115, 23), // "setShowInitRdgSelection"
QT_MOC_LITERAL(65, 1139, 21), // "slotResetRdgSelection"
QT_MOC_LITERAL(66, 1161, 24), // "saveRdgAutoSelectionData"
QT_MOC_LITERAL(67, 1186, 24), // "loadRdgAutoSelectionData"
QT_MOC_LITERAL(68, 1211, 19), // "changeHighLowOnRdgs"
QT_MOC_LITERAL(69, 1231, 16), // "showFullRdgsData"
QT_MOC_LITERAL(70, 1248, 17), // "materialIdChanged"
QT_MOC_LITERAL(71, 1266, 10), // "materialId"
QT_MOC_LITERAL(72, 1277, 17), // "useSelectionOnRdg"
QT_MOC_LITERAL(73, 1295, 17), // "setupScalingOnRdg"
QT_MOC_LITERAL(74, 1313, 22), // "setupScalingOnRdgsSurf"
QT_MOC_LITERAL(75, 1336, 23), // "setupScalingOnRdgsTrans"
QT_MOC_LITERAL(76, 1360, 9), // "changeRdg"
QT_MOC_LITERAL(77, 1370, 5), // "rdgId"
QT_MOC_LITERAL(78, 1376, 13), // "leftChangeRdg"
QT_MOC_LITERAL(79, 1390, 14), // "rightChangeRdg"
QT_MOC_LITERAL(80, 1405, 27), // "scrollHRdgsLengthBarPressed"
QT_MOC_LITERAL(81, 1433, 25), // "scrollHRdgsLengthBarMoved"
QT_MOC_LITERAL(82, 1459, 23), // "scrollHRdgsLengthBarPos"
QT_MOC_LITERAL(83, 1483, 28), // "scrollHRdgsLengthBarReleased"
QT_MOC_LITERAL(84, 1512, 27) // "scrollHRdgsLengthBarChanged"

    },
    "MainWindow\0signalClearRdgsInfoDataMap\0"
    "\0signalEraseRdgsInfoDataMap\0std::string\0"
    "fileName\0closeSection2dAction\0"
    "section2dVisible\0closeCut3dAction\0"
    "cut3dVisible\0signalSetupRdgsSurfPixels\0"
    "std::map<std::string,st_rdgInfoData>\0"
    "rdgsInfoDataMap\0st_rdgsWorkData\0"
    "rdgsWorkData\0signalOutputRdgsSurfSection\0"
    "signalDefDeleteRdgName\0fixLatitude\0"
    "fixLongitude\0signalFillRdgsExecute\0"
    "resetCutPointsRdgs\0receiveDeleteRdgName\0"
    "deleteRdgName\0receiveProgressData\0"
    "progressPos\0progressMax\0receiveRdgsWorkData\0"
    "receiveRdgsZData\0slotFixRdgsSurfPixels\0"
    "slotFixDeleteRdgName\0slotFixFillRdgs\0"
    "slotFixRdgsSurfSection\0slotActivateAccompThread\0"
    "std::vector<std::pair<std::string,std::string> >\0"
    "rdgsNamesVectorPairs\0RdgFileFormat\0"
    "rdgFileFormat\0receiveColorsPalette\0"
    "QColor&\0colorUp\0colorLow\0openTrz\0"
    "openCsv\0openHdf5\0openKml\0openImageRdg\0"
    "defPaletteRdg\0open2dImageRdgs\0"
    "defPaletteSurfRdgs\0open3dImageRdgs\0"
    "defPaletteTransRdgs\0includeFilterOnRdg\0"
    "openRecentFile\0deleteSelectedFile\0"
    "deleteAllFiles\0save\0setCloseSection2dAction\0"
    "setCloseCut3dAction\0saveBoundPointsLineAction\0"
    "loadBoundPointsLineAction\0setInitRdg\0"
    "setLogRdg\0setLogRdgMouseSelection\0"
    "setLogRdgAutoSelection\0setShowInitRdgSelection\0"
    "slotResetRdgSelection\0saveRdgAutoSelectionData\0"
    "loadRdgAutoSelectionData\0changeHighLowOnRdgs\0"
    "showFullRdgsData\0materialIdChanged\0"
    "materialId\0useSelectionOnRdg\0"
    "setupScalingOnRdg\0setupScalingOnRdgsSurf\0"
    "setupScalingOnRdgsTrans\0changeRdg\0"
    "rdgId\0leftChangeRdg\0rightChangeRdg\0"
    "scrollHRdgsLengthBarPressed\0"
    "scrollHRdgsLengthBarMoved\0"
    "scrollHRdgsLengthBarPos\0"
    "scrollHRdgsLengthBarReleased\0"
    "scrollHRdgsLengthBarChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      59,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  309,    2, 0x06 /* Public */,
       3,    1,  310,    2, 0x06 /* Public */,
       6,    1,  313,    2, 0x06 /* Public */,
       8,    1,  316,    2, 0x06 /* Public */,
      10,    2,  319,    2, 0x06 /* Public */,
      15,    2,  324,    2, 0x06 /* Public */,
      16,    3,  329,    2, 0x06 /* Public */,
      19,    3,  336,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      21,    1,  343,    2, 0x0a /* Public */,
      23,    2,  346,    2, 0x0a /* Public */,
      26,    0,  351,    2, 0x0a /* Public */,
      27,    0,  352,    2, 0x0a /* Public */,
      28,    0,  353,    2, 0x0a /* Public */,
      29,    2,  354,    2, 0x0a /* Public */,
      30,    1,  359,    2, 0x0a /* Public */,
      31,    0,  362,    2, 0x0a /* Public */,
      32,    2,  363,    2, 0x0a /* Public */,
      37,    2,  368,    2, 0x0a /* Public */,
      41,    0,  373,    2, 0x08 /* Private */,
      42,    0,  374,    2, 0x08 /* Private */,
      43,    0,  375,    2, 0x08 /* Private */,
      44,    0,  376,    2, 0x08 /* Private */,
      45,    0,  377,    2, 0x08 /* Private */,
      46,    0,  378,    2, 0x08 /* Private */,
      47,    0,  379,    2, 0x08 /* Private */,
      48,    0,  380,    2, 0x08 /* Private */,
      49,    0,  381,    2, 0x08 /* Private */,
      50,    0,  382,    2, 0x08 /* Private */,
      51,    0,  383,    2, 0x08 /* Private */,
      52,    0,  384,    2, 0x08 /* Private */,
      53,    0,  385,    2, 0x08 /* Private */,
      54,    0,  386,    2, 0x08 /* Private */,
      55,    0,  387,    2, 0x08 /* Private */,
      56,    0,  388,    2, 0x08 /* Private */,
      57,    0,  389,    2, 0x08 /* Private */,
      58,    0,  390,    2, 0x08 /* Private */,
      59,    0,  391,    2, 0x08 /* Private */,
      60,    0,  392,    2, 0x08 /* Private */,
      61,    0,  393,    2, 0x08 /* Private */,
      62,    0,  394,    2, 0x08 /* Private */,
      63,    0,  395,    2, 0x08 /* Private */,
      64,    0,  396,    2, 0x08 /* Private */,
      65,    0,  397,    2, 0x08 /* Private */,
      66,    0,  398,    2, 0x08 /* Private */,
      67,    0,  399,    2, 0x08 /* Private */,
      68,    0,  400,    2, 0x08 /* Private */,
      69,    0,  401,    2, 0x08 /* Private */,
      70,    1,  402,    2, 0x08 /* Private */,
      72,    0,  405,    2, 0x08 /* Private */,
      73,    0,  406,    2, 0x08 /* Private */,
      74,    0,  407,    2, 0x08 /* Private */,
      75,    0,  408,    2, 0x08 /* Private */,
      76,    1,  409,    2, 0x08 /* Private */,
      78,    0,  412,    2, 0x08 /* Private */,
      79,    0,  413,    2, 0x08 /* Private */,
      80,    0,  414,    2, 0x08 /* Private */,
      81,    1,  415,    2, 0x08 /* Private */,
      83,    0,  418,    2, 0x08 /* Private */,
      84,    1,  419,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 13,   12,   14,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 13,   12,   14,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Double, QMetaType::Double,   12,   17,   18,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 13, QMetaType::Bool,   12,   14,   20,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   24,   25,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   17,   18,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 33, 0x80000000 | 35,   34,   36,
    QMetaType::Void, 0x80000000 | 38, 0x80000000 | 38,   39,   40,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   71,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   77,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   82,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   82,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalClearRdgsInfoDataMap(); break;
        case 1: _t->signalEraseRdgsInfoDataMap((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 2: _t->closeSection2dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->closeCut3dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->signalSetupRdgsSurfPixels((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_rdgsWorkData(*)>(_a[2]))); break;
        case 5: _t->signalOutputRdgsSurfSection((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_rdgsWorkData(*)>(_a[2]))); break;
        case 6: _t->signalDefDeleteRdgName((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 7: _t->signalFillRdgsExecute((*reinterpret_cast< const std::map<std::string,st_rdgInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_rdgsWorkData(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 8: _t->receiveDeleteRdgName((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 9: _t->receiveProgressData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->receiveRdgsWorkData(); break;
        case 11: _t->receiveRdgsZData(); break;
        case 12: _t->slotFixRdgsSurfPixels(); break;
        case 13: _t->slotFixDeleteRdgName((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 14: _t->slotFixFillRdgs((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->slotFixRdgsSurfSection(); break;
        case 16: _t->slotActivateAccompThread((*reinterpret_cast< std::vector<std::pair<std::string,std::string> >(*)>(_a[1])),(*reinterpret_cast< RdgFileFormat(*)>(_a[2]))); break;
        case 17: _t->receiveColorsPalette((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 18: _t->openTrz(); break;
        case 19: _t->openCsv(); break;
        case 20: _t->openHdf5(); break;
        case 21: _t->openKml(); break;
        case 22: _t->openImageRdg(); break;
        case 23: _t->defPaletteRdg(); break;
        case 24: _t->open2dImageRdgs(); break;
        case 25: _t->defPaletteSurfRdgs(); break;
        case 26: _t->open3dImageRdgs(); break;
        case 27: _t->defPaletteTransRdgs(); break;
        case 28: _t->includeFilterOnRdg(); break;
        case 29: _t->openRecentFile(); break;
        case 30: _t->deleteSelectedFile(); break;
        case 31: _t->deleteAllFiles(); break;
        case 32: _t->save(); break;
        case 33: _t->setCloseSection2dAction(); break;
        case 34: _t->setCloseCut3dAction(); break;
        case 35: _t->saveBoundPointsLineAction(); break;
        case 36: _t->loadBoundPointsLineAction(); break;
        case 37: _t->setInitRdg(); break;
        case 38: _t->setLogRdg(); break;
        case 39: _t->setLogRdgMouseSelection(); break;
        case 40: _t->setLogRdgAutoSelection(); break;
        case 41: _t->setShowInitRdgSelection(); break;
        case 42: _t->slotResetRdgSelection(); break;
        case 43: _t->saveRdgAutoSelectionData(); break;
        case 44: _t->loadRdgAutoSelectionData(); break;
        case 45: _t->changeHighLowOnRdgs(); break;
        case 46: _t->showFullRdgsData(); break;
        case 47: _t->materialIdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 48: _t->useSelectionOnRdg(); break;
        case 49: _t->setupScalingOnRdg(); break;
        case 50: _t->setupScalingOnRdgsSurf(); break;
        case 51: _t->setupScalingOnRdgsTrans(); break;
        case 52: _t->changeRdg((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->leftChangeRdg(); break;
        case 54: _t->rightChangeRdg(); break;
        case 55: _t->scrollHRdgsLengthBarPressed(); break;
        case 56: _t->scrollHRdgsLengthBarMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 57: _t->scrollHRdgsLengthBarReleased(); break;
        case 58: _t->scrollHRdgsLengthBarChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalClearRdgsInfoDataMap)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalEraseRdgsInfoDataMap)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::closeSection2dAction)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::closeCut3dAction)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::map<std::string,st_rdgInfoData> & , const st_rdgsWorkData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalSetupRdgsSurfPixels)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::map<std::string,st_rdgInfoData> & , const st_rdgsWorkData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalOutputRdgsSurfSection)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::map<std::string,st_rdgInfoData> & , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalDefDeleteRdgName)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::map<std::string,st_rdgInfoData> & , const st_rdgsWorkData & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalFillRdgsExecute)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 59)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 59;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 59)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 59;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::signalClearRdgsInfoDataMap()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainWindow::signalEraseRdgsInfoDataMap(const std::string & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::closeSection2dAction(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::closeCut3dAction(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::signalSetupRdgsSurfPixels(const std::map<std::string,st_rdgInfoData> & _t1, const st_rdgsWorkData & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::signalOutputRdgsSurfSection(const std::map<std::string,st_rdgInfoData> & _t1, const st_rdgsWorkData & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::signalDefDeleteRdgName(const std::map<std::string,st_rdgInfoData> & _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::signalFillRdgsExecute(const std::map<std::string,st_rdgInfoData> & _t1, const st_rdgsWorkData & _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
