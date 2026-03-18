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
    QByteArrayData data[99];
    char stringdata0[1964];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 26), // "signalSetupBscanDeepPixels"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 16), // "st_bscanInfoData"
QT_MOC_LITERAL(4, 56, 13), // "bscanInfoData"
QT_MOC_LITERAL(5, 70, 34), // "signalSetupFixDataBscanDeepWi..."
QT_MOC_LITERAL(6, 105, 28), // "signalClearBscansInfoDataMap"
QT_MOC_LITERAL(7, 134, 28), // "signalEraseBscansInfoDataMap"
QT_MOC_LITERAL(8, 163, 11), // "std::string"
QT_MOC_LITERAL(9, 175, 8), // "fileName"
QT_MOC_LITERAL(10, 184, 20), // "closeSection2dAction"
QT_MOC_LITERAL(11, 205, 16), // "section2dVisible"
QT_MOC_LITERAL(12, 222, 16), // "closeCut3dAction"
QT_MOC_LITERAL(13, 239, 12), // "cut3dVisible"
QT_MOC_LITERAL(14, 252, 27), // "signalSetupBscansSurfPixels"
QT_MOC_LITERAL(15, 280, 38), // "std::map<std::string,st_bscan..."
QT_MOC_LITERAL(16, 319, 17), // "bscansInfoDataMap"
QT_MOC_LITERAL(17, 337, 17), // "st_bscansWorkData"
QT_MOC_LITERAL(18, 355, 14), // "bscansWorkData"
QT_MOC_LITERAL(19, 370, 29), // "signalOutputBscansSurfSection"
QT_MOC_LITERAL(20, 400, 24), // "signalDefDeleteBscanName"
QT_MOC_LITERAL(21, 425, 11), // "fixLatitude"
QT_MOC_LITERAL(22, 437, 12), // "fixLongitude"
QT_MOC_LITERAL(23, 450, 23), // "signalFillBscansExecute"
QT_MOC_LITERAL(24, 474, 20), // "resetCutPointsBscans"
QT_MOC_LITERAL(25, 495, 28), // "signalSetupBscanDeepPageData"
QT_MOC_LITERAL(26, 524, 14), // "bscansInfoData"
QT_MOC_LITERAL(27, 539, 29), // "signalFixDataBscanDeepWidgets"
QT_MOC_LITERAL(28, 569, 22), // "receiveDeleteBscanName"
QT_MOC_LITERAL(29, 592, 15), // "deleteBscanName"
QT_MOC_LITERAL(30, 608, 19), // "receiveProgressData"
QT_MOC_LITERAL(31, 628, 11), // "progressPos"
QT_MOC_LITERAL(32, 640, 11), // "progressMax"
QT_MOC_LITERAL(33, 652, 21), // "receiveBscansWorkData"
QT_MOC_LITERAL(34, 674, 18), // "receiveBscansZData"
QT_MOC_LITERAL(35, 693, 23), // "slotFixBscansSurfPixels"
QT_MOC_LITERAL(36, 717, 22), // "slotFixDeleteBscanName"
QT_MOC_LITERAL(37, 740, 17), // "slotFixFillBscans"
QT_MOC_LITERAL(38, 758, 24), // "slotFixBscansSurfSection"
QT_MOC_LITERAL(39, 783, 24), // "slotActivateAccompThread"
QT_MOC_LITERAL(40, 808, 48), // "std::vector<std::pair<std::st..."
QT_MOC_LITERAL(41, 857, 22), // "BscansNamesVectorPairs"
QT_MOC_LITERAL(42, 880, 15), // "BscanFileFormat"
QT_MOC_LITERAL(43, 896, 15), // "bscanFileFormat"
QT_MOC_LITERAL(44, 912, 20), // "receiveColorsPalette"
QT_MOC_LITERAL(45, 933, 7), // "QColor&"
QT_MOC_LITERAL(46, 941, 7), // "colorUp"
QT_MOC_LITERAL(47, 949, 8), // "colorLow"
QT_MOC_LITERAL(48, 958, 24), // "slotSetupBscanDeepPixels"
QT_MOC_LITERAL(49, 983, 27), // "slotFixDataBscanDeepWidgets"
QT_MOC_LITERAL(50, 1011, 7), // "openTrz"
QT_MOC_LITERAL(51, 1019, 7), // "openCsv"
QT_MOC_LITERAL(52, 1027, 8), // "openHdf5"
QT_MOC_LITERAL(53, 1036, 7), // "openKml"
QT_MOC_LITERAL(54, 1044, 14), // "openImageBscan"
QT_MOC_LITERAL(55, 1059, 15), // "defPaletteBscan"
QT_MOC_LITERAL(56, 1075, 19), // "defPaletteBscanDeep"
QT_MOC_LITERAL(57, 1095, 18), // "openImageDeepBscan"
QT_MOC_LITERAL(58, 1114, 17), // "open2dImageBscans"
QT_MOC_LITERAL(59, 1132, 20), // "defPaletteSurfBscans"
QT_MOC_LITERAL(60, 1153, 17), // "open3dImageBscans"
QT_MOC_LITERAL(61, 1171, 21), // "defPaletteTransBscans"
QT_MOC_LITERAL(62, 1193, 20), // "includeFilterOnBscan"
QT_MOC_LITERAL(63, 1214, 14), // "openRecentFile"
QT_MOC_LITERAL(64, 1229, 18), // "deleteSelectedFile"
QT_MOC_LITERAL(65, 1248, 14), // "deleteAllFiles"
QT_MOC_LITERAL(66, 1263, 4), // "save"
QT_MOC_LITERAL(67, 1268, 23), // "setCloseSection2dAction"
QT_MOC_LITERAL(68, 1292, 19), // "setCloseCut3dAction"
QT_MOC_LITERAL(69, 1312, 25), // "saveBoundPointsLineAction"
QT_MOC_LITERAL(70, 1338, 25), // "loadBoundPointsLineAction"
QT_MOC_LITERAL(71, 1364, 12), // "setInitBscan"
QT_MOC_LITERAL(72, 1377, 11), // "setLogBscan"
QT_MOC_LITERAL(73, 1389, 25), // "setLogBscanMouseSelection"
QT_MOC_LITERAL(74, 1415, 24), // "setLogBscanAutoSelection"
QT_MOC_LITERAL(75, 1440, 25), // "setShowInitBscanSelection"
QT_MOC_LITERAL(76, 1466, 23), // "slotResetBscanSelection"
QT_MOC_LITERAL(77, 1490, 26), // "saveBscanAutoSelectionData"
QT_MOC_LITERAL(78, 1517, 26), // "loadBscanAutoSelectionData"
QT_MOC_LITERAL(79, 1544, 21), // "changeHighLowOnBscans"
QT_MOC_LITERAL(80, 1566, 18), // "showFullBscansData"
QT_MOC_LITERAL(81, 1585, 17), // "materialIdChanged"
QT_MOC_LITERAL(82, 1603, 10), // "materialId"
QT_MOC_LITERAL(83, 1614, 19), // "useSelectionOnBscan"
QT_MOC_LITERAL(84, 1634, 19), // "setupScalingOnBscan"
QT_MOC_LITERAL(85, 1654, 23), // "setupScalingOnBscanDeep"
QT_MOC_LITERAL(86, 1678, 24), // "setupScalingOnBscansSurf"
QT_MOC_LITERAL(87, 1703, 25), // "setupScalingOnBscansTrans"
QT_MOC_LITERAL(88, 1729, 11), // "changeBscan"
QT_MOC_LITERAL(89, 1741, 7), // "BscanId"
QT_MOC_LITERAL(90, 1749, 15), // "leftChangeBscan"
QT_MOC_LITERAL(91, 1765, 16), // "rightChangeBscan"
QT_MOC_LITERAL(92, 1782, 29), // "scrollHBscansLengthBarPressed"
QT_MOC_LITERAL(93, 1812, 27), // "scrollHBscansLengthBarMoved"
QT_MOC_LITERAL(94, 1840, 25), // "scrollHBscansLengthBarPos"
QT_MOC_LITERAL(95, 1866, 30), // "scrollHBscansLengthBarReleased"
QT_MOC_LITERAL(96, 1897, 29), // "scrollHBscansLengthBarChanged"
QT_MOC_LITERAL(97, 1927, 26), // "slotSetupBscanDeepPageData"
QT_MOC_LITERAL(98, 1954, 9) // "bscanName"

    },
    "MainWindow\0signalSetupBscanDeepPixels\0"
    "\0st_bscanInfoData\0bscanInfoData\0"
    "signalSetupFixDataBscanDeepWidgets\0"
    "signalClearBscansInfoDataMap\0"
    "signalEraseBscansInfoDataMap\0std::string\0"
    "fileName\0closeSection2dAction\0"
    "section2dVisible\0closeCut3dAction\0"
    "cut3dVisible\0signalSetupBscansSurfPixels\0"
    "std::map<std::string,st_bscanInfoData>\0"
    "bscansInfoDataMap\0st_bscansWorkData\0"
    "bscansWorkData\0signalOutputBscansSurfSection\0"
    "signalDefDeleteBscanName\0fixLatitude\0"
    "fixLongitude\0signalFillBscansExecute\0"
    "resetCutPointsBscans\0signalSetupBscanDeepPageData\0"
    "bscansInfoData\0signalFixDataBscanDeepWidgets\0"
    "receiveDeleteBscanName\0deleteBscanName\0"
    "receiveProgressData\0progressPos\0"
    "progressMax\0receiveBscansWorkData\0"
    "receiveBscansZData\0slotFixBscansSurfPixels\0"
    "slotFixDeleteBscanName\0slotFixFillBscans\0"
    "slotFixBscansSurfSection\0"
    "slotActivateAccompThread\0"
    "std::vector<std::pair<std::string,std::string> >\0"
    "BscansNamesVectorPairs\0BscanFileFormat\0"
    "bscanFileFormat\0receiveColorsPalette\0"
    "QColor&\0colorUp\0colorLow\0"
    "slotSetupBscanDeepPixels\0"
    "slotFixDataBscanDeepWidgets\0openTrz\0"
    "openCsv\0openHdf5\0openKml\0openImageBscan\0"
    "defPaletteBscan\0defPaletteBscanDeep\0"
    "openImageDeepBscan\0open2dImageBscans\0"
    "defPaletteSurfBscans\0open3dImageBscans\0"
    "defPaletteTransBscans\0includeFilterOnBscan\0"
    "openRecentFile\0deleteSelectedFile\0"
    "deleteAllFiles\0save\0setCloseSection2dAction\0"
    "setCloseCut3dAction\0saveBoundPointsLineAction\0"
    "loadBoundPointsLineAction\0setInitBscan\0"
    "setLogBscan\0setLogBscanMouseSelection\0"
    "setLogBscanAutoSelection\0"
    "setShowInitBscanSelection\0"
    "slotResetBscanSelection\0"
    "saveBscanAutoSelectionData\0"
    "loadBscanAutoSelectionData\0"
    "changeHighLowOnBscans\0showFullBscansData\0"
    "materialIdChanged\0materialId\0"
    "useSelectionOnBscan\0setupScalingOnBscan\0"
    "setupScalingOnBscanDeep\0"
    "setupScalingOnBscansSurf\0"
    "setupScalingOnBscansTrans\0changeBscan\0"
    "BscanId\0leftChangeBscan\0rightChangeBscan\0"
    "scrollHBscansLengthBarPressed\0"
    "scrollHBscansLengthBarMoved\0"
    "scrollHBscansLengthBarPos\0"
    "scrollHBscansLengthBarReleased\0"
    "scrollHBscansLengthBarChanged\0"
    "slotSetupBscanDeepPageData\0bscanName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      69,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  359,    2, 0x06 /* Public */,
       5,    1,  362,    2, 0x06 /* Public */,
       6,    0,  365,    2, 0x06 /* Public */,
       7,    1,  366,    2, 0x06 /* Public */,
      10,    1,  369,    2, 0x06 /* Public */,
      12,    1,  372,    2, 0x06 /* Public */,
      14,    2,  375,    2, 0x06 /* Public */,
      19,    2,  380,    2, 0x06 /* Public */,
      20,    3,  385,    2, 0x06 /* Public */,
      23,    3,  392,    2, 0x06 /* Public */,
      25,    1,  399,    2, 0x06 /* Public */,
      27,    1,  402,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      28,    1,  405,    2, 0x0a /* Public */,
      30,    2,  408,    2, 0x0a /* Public */,
      33,    0,  413,    2, 0x0a /* Public */,
      34,    0,  414,    2, 0x0a /* Public */,
      35,    0,  415,    2, 0x0a /* Public */,
      36,    2,  416,    2, 0x0a /* Public */,
      37,    1,  421,    2, 0x0a /* Public */,
      38,    0,  424,    2, 0x0a /* Public */,
      39,    2,  425,    2, 0x0a /* Public */,
      44,    2,  430,    2, 0x0a /* Public */,
      48,    0,  435,    2, 0x0a /* Public */,
      49,    0,  436,    2, 0x0a /* Public */,
      50,    0,  437,    2, 0x08 /* Private */,
      51,    0,  438,    2, 0x08 /* Private */,
      52,    0,  439,    2, 0x08 /* Private */,
      53,    0,  440,    2, 0x08 /* Private */,
      54,    0,  441,    2, 0x08 /* Private */,
      55,    0,  442,    2, 0x08 /* Private */,
      56,    0,  443,    2, 0x08 /* Private */,
      57,    0,  444,    2, 0x08 /* Private */,
      58,    0,  445,    2, 0x08 /* Private */,
      59,    0,  446,    2, 0x08 /* Private */,
      60,    0,  447,    2, 0x08 /* Private */,
      61,    0,  448,    2, 0x08 /* Private */,
      62,    0,  449,    2, 0x08 /* Private */,
      63,    0,  450,    2, 0x08 /* Private */,
      64,    0,  451,    2, 0x08 /* Private */,
      65,    0,  452,    2, 0x08 /* Private */,
      66,    0,  453,    2, 0x08 /* Private */,
      67,    0,  454,    2, 0x08 /* Private */,
      68,    0,  455,    2, 0x08 /* Private */,
      69,    0,  456,    2, 0x08 /* Private */,
      70,    0,  457,    2, 0x08 /* Private */,
      71,    0,  458,    2, 0x08 /* Private */,
      72,    0,  459,    2, 0x08 /* Private */,
      73,    0,  460,    2, 0x08 /* Private */,
      74,    0,  461,    2, 0x08 /* Private */,
      75,    0,  462,    2, 0x08 /* Private */,
      76,    0,  463,    2, 0x08 /* Private */,
      77,    0,  464,    2, 0x08 /* Private */,
      78,    0,  465,    2, 0x08 /* Private */,
      79,    0,  466,    2, 0x08 /* Private */,
      80,    0,  467,    2, 0x08 /* Private */,
      81,    1,  468,    2, 0x08 /* Private */,
      83,    0,  471,    2, 0x08 /* Private */,
      84,    0,  472,    2, 0x08 /* Private */,
      85,    0,  473,    2, 0x08 /* Private */,
      86,    0,  474,    2, 0x08 /* Private */,
      87,    0,  475,    2, 0x08 /* Private */,
      88,    1,  476,    2, 0x08 /* Private */,
      90,    0,  479,    2, 0x08 /* Private */,
      91,    0,  480,    2, 0x08 /* Private */,
      92,    0,  481,    2, 0x08 /* Private */,
      93,    1,  482,    2, 0x08 /* Private */,
      95,    0,  485,    2, 0x08 /* Private */,
      96,    1,  486,    2, 0x08 /* Private */,
      97,    1,  489,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 17,   16,   18,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 17,   16,   18,
    QMetaType::Void, 0x80000000 | 15, QMetaType::Double, QMetaType::Double,   16,   21,   22,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 17, QMetaType::Bool,   16,   18,   24,
    QMetaType::Void, 0x80000000 | 3,   26,
    QMetaType::Void, 0x80000000 | 3,   26,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,   29,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   31,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   21,   22,
    QMetaType::Void, QMetaType::Bool,   24,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 40, 0x80000000 | 42,   41,   43,
    QMetaType::Void, 0x80000000 | 45, 0x80000000 | 45,   46,   47,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   82,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   89,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   94,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   94,
    QMetaType::Void, 0x80000000 | 8,   98,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalSetupBscanDeepPixels((*reinterpret_cast< const st_bscanInfoData(*)>(_a[1]))); break;
        case 1: _t->signalSetupFixDataBscanDeepWidgets((*reinterpret_cast< const st_bscanInfoData(*)>(_a[1]))); break;
        case 2: _t->signalClearBscansInfoDataMap(); break;
        case 3: _t->signalEraseBscansInfoDataMap((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 4: _t->closeSection2dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->closeCut3dAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->signalSetupBscansSurfPixels((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2]))); break;
        case 7: _t->signalOutputBscansSurfSection((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2]))); break;
        case 8: _t->signalDefDeleteBscanName((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 9: _t->signalFillBscansExecute((*reinterpret_cast< const std::map<std::string,st_bscanInfoData>(*)>(_a[1])),(*reinterpret_cast< const st_bscansWorkData(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 10: _t->signalSetupBscanDeepPageData((*reinterpret_cast< const st_bscanInfoData(*)>(_a[1]))); break;
        case 11: _t->signalFixDataBscanDeepWidgets((*reinterpret_cast< const st_bscanInfoData(*)>(_a[1]))); break;
        case 12: _t->receiveDeleteBscanName((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 13: _t->receiveProgressData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->receiveBscansWorkData(); break;
        case 15: _t->receiveBscansZData(); break;
        case 16: _t->slotFixBscansSurfPixels(); break;
        case 17: _t->slotFixDeleteBscanName((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 18: _t->slotFixFillBscans((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->slotFixBscansSurfSection(); break;
        case 20: _t->slotActivateAccompThread((*reinterpret_cast< std::vector<std::pair<std::string,std::string> >(*)>(_a[1])),(*reinterpret_cast< BscanFileFormat(*)>(_a[2]))); break;
        case 21: _t->receiveColorsPalette((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 22: _t->slotSetupBscanDeepPixels(); break;
        case 23: _t->slotFixDataBscanDeepWidgets(); break;
        case 24: _t->openTrz(); break;
        case 25: _t->openCsv(); break;
        case 26: _t->openHdf5(); break;
        case 27: _t->openKml(); break;
        case 28: _t->openImageBscan(); break;
        case 29: _t->defPaletteBscan(); break;
        case 30: _t->defPaletteBscanDeep(); break;
        case 31: _t->openImageDeepBscan(); break;
        case 32: _t->open2dImageBscans(); break;
        case 33: _t->defPaletteSurfBscans(); break;
        case 34: _t->open3dImageBscans(); break;
        case 35: _t->defPaletteTransBscans(); break;
        case 36: _t->includeFilterOnBscan(); break;
        case 37: _t->openRecentFile(); break;
        case 38: _t->deleteSelectedFile(); break;
        case 39: _t->deleteAllFiles(); break;
        case 40: _t->save(); break;
        case 41: _t->setCloseSection2dAction(); break;
        case 42: _t->setCloseCut3dAction(); break;
        case 43: _t->saveBoundPointsLineAction(); break;
        case 44: _t->loadBoundPointsLineAction(); break;
        case 45: _t->setInitBscan(); break;
        case 46: _t->setLogBscan(); break;
        case 47: _t->setLogBscanMouseSelection(); break;
        case 48: _t->setLogBscanAutoSelection(); break;
        case 49: _t->setShowInitBscanSelection(); break;
        case 50: _t->slotResetBscanSelection(); break;
        case 51: _t->saveBscanAutoSelectionData(); break;
        case 52: _t->loadBscanAutoSelectionData(); break;
        case 53: _t->changeHighLowOnBscans(); break;
        case 54: _t->showFullBscansData(); break;
        case 55: _t->materialIdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 56: _t->useSelectionOnBscan(); break;
        case 57: _t->setupScalingOnBscan(); break;
        case 58: _t->setupScalingOnBscanDeep(); break;
        case 59: _t->setupScalingOnBscansSurf(); break;
        case 60: _t->setupScalingOnBscansTrans(); break;
        case 61: _t->changeBscan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 62: _t->leftChangeBscan(); break;
        case 63: _t->rightChangeBscan(); break;
        case 64: _t->scrollHBscansLengthBarPressed(); break;
        case 65: _t->scrollHBscansLengthBarMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 66: _t->scrollHBscansLengthBarReleased(); break;
        case 67: _t->scrollHBscansLengthBarChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 68: _t->slotSetupBscanDeepPageData((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(const st_bscanInfoData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalSetupBscanDeepPixels)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const st_bscanInfoData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalSetupFixDataBscanDeepWidgets)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalClearBscansInfoDataMap)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalEraseBscansInfoDataMap)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::closeSection2dAction)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::closeCut3dAction)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::map<std::string,st_bscanInfoData> & , const st_bscansWorkData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalSetupBscansSurfPixels)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::map<std::string,st_bscanInfoData> & , const st_bscansWorkData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalOutputBscansSurfSection)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::map<std::string,st_bscanInfoData> & , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalDefDeleteBscanName)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const std::map<std::string,st_bscanInfoData> & , const st_bscansWorkData & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalFillBscansExecute)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const st_bscanInfoData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalSetupBscanDeepPageData)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(const st_bscanInfoData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::signalFixDataBscanDeepWidgets)) {
                *result = 11;
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
        if (_id < 69)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 69;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 69)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 69;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::signalSetupBscanDeepPixels(const st_bscanInfoData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::signalSetupFixDataBscanDeepWidgets(const st_bscanInfoData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::signalClearBscansInfoDataMap()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MainWindow::signalEraseBscansInfoDataMap(const std::string & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::closeSection2dAction(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::closeCut3dAction(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::signalSetupBscansSurfPixels(const std::map<std::string,st_bscanInfoData> & _t1, const st_bscansWorkData & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::signalOutputBscansSurfSection(const std::map<std::string,st_bscanInfoData> & _t1, const st_bscansWorkData & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MainWindow::signalDefDeleteBscanName(const std::map<std::string,st_bscanInfoData> & _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MainWindow::signalFillBscansExecute(const std::map<std::string,st_bscanInfoData> & _t1, const st_bscansWorkData & _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void MainWindow::signalSetupBscanDeepPageData(const st_bscanInfoData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void MainWindow::signalFixDataBscanDeepWidgets(const st_bscanInfoData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
