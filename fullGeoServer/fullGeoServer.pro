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
    rdg2dWidget.cpp \
    contentWidget.cpp \
    rdgGlWidget.cpp \
    sectionRdgWidget.cpp \
    sectionChartView.cpp \
    rdgs2dWidget.cpp \
    rdgsSurfGlWidget.cpp \
    sectionRdgsSurfWidget.cpp \
    rdgs3dWidget.cpp \
    dataRdgWidget.cpp \
    tableRdgTrackDataWidget.cpp \
    tableRdgLogAutoDataWidget.cpp \
    rdgTrackDataTableModel.cpp \
    rdgSelectedDataTableModel.cpp \
    accomplishmentThread.cpp \
    rdgsTransGlWidget.cpp \
    structureRdgsGraph.cpp \
    structureRdgsWidget.cpp \
    correctRdgsSurfDialog.cpp \
    colorsPaletteDialog.cpp

HEADERS += \
        mainwindow.h \
    accomplishment.h \
    rdg2dWidget.h \
    rdgGlWidget.h \
    rdgsData.h \
    contentWidget.h \
    sectionChartView.h \
    sectionRdgWidget.h \
    rdgs2dWidget.h \
    rdgsSurfGlWidget.h \
    sectionRdgsSurfWidget.h \
    rdgs3dWidget.h \
    transformation.h \
    dataRdgWidget.h \
    tableRdgTrackDataWidget.h \
    tableRdgLogAutoDataWidget.h \
    csvfile.h \
    rdgTrackDataTableModel.h \
    rdgSelectedDataTableModel.h \
    accomplishmentThread.h \
    rdgsTransGlWidget.h \
    structureRdgsGraph.h \
    structureRdgsWidget.h \
    correctRdgsSurfDialog.h \
    colorsPaletteDialog.h

RESOURCES += \
    item.qrc
