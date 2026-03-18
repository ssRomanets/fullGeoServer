#-------------------------------------------------
#
# Project created by QtCreator 2025-11-04T13:22:49
#
#-------------------------------------------------

QT +=  core gui widgets charts datavisualization opengl

LIBS += -L"C:\Program Files\HDF_Group\HDF5\1.12.0\lib"
LIBS += -llibhdf5
LIBS += -llibhdf5_cpp
LIBS += -llibhdf5_hl
LIBS += -llibhdf5_hl_cpp
LIBS += -llibhdf5_tools
LIBS += -llibszip
LIBS += -llibzlib

INCLUDEPATH += "C:\Program Files\HDF_Group\HDF5\1.12.0\include"
DEPENDPATH  += "C:\Program Files\HDF_Group\HDF5\1.12.0\include"

TARGET = fullGeoServer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    accomplishment.cpp \
    bscan2dWidget.cpp \
    contentWidget.cpp \
    bscanGlWidget.cpp \
    ascanWidget.cpp \
    sectionChartView.cpp \
    bscans2dWidget.cpp \
    bscansSurfGlWidget.cpp \
    sectionBscansSurfWidget.cpp \
    bscans3dWidget.cpp \
    ascanDataWidget.cpp \
    tableBscanLogAutoDataWidget.cpp \
    ascanDataTableModel.cpp \
    bscanSelectedDataTableModel.cpp \
    accomplishmentThread.cpp \
    bscansTransGlWidget.cpp \
    structureBscansGraph.cpp \
    structureBscansWidget.cpp \
    correctBscansSurfDialog.cpp \
    colorsPaletteDialog.cpp \
    ascanDeepWidget.cpp \
    ascanDeepDataTableModel.cpp \
    tableAscanDeepDataWidget.cpp \
    tableAscanDataWidget.cpp \
    bscanDeep2dWidget.cpp \
    bscanDeepGlWidget.cpp \
    ascanDeepDataWidget.cpp

HEADERS += \
        mainwindow.h \
    accomplishment.h \
    bscan2dWidget.h \
    bscanGlWidget.h \
    contentWidget.h \
    sectionChartView.h \
    ascanWidget.h \
    bscans2dWidget.h \
    bscansSurfGlWidget.h \
    sectionBscansSurfWidget.h \
    bscans3dWidget.h \
    transformation.h \
    ascanDataWidget.h \
    tableBscanLogAutoDataWidget.h \
    csvfile.h \
    ascanDataTableModel.h \
    accomplishmentThread.h \
    bscansTransGlWidget.h \
    structureBscansGraph.h \
    structureBscansWidget.h \
    correctBscansSurfDialog.h \
    colorsPaletteDialog.h \
    ascanDeepWidget.h \
    ascanDeepDataTableModel.h \
    tableAscanDeepDataWidget.h \
    tableAscanDataWidget.h \
    bscanSelectedDataTableModel.h \
    bscansData.h \
    bscanDeep2dWidget.h \
    bscanDeepGlWidget.h \
    ascanDeepDataWidget.h

RESOURCES += \
    item.qrc
