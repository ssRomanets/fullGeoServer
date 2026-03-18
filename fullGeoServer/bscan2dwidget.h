#ifndef bscan2DWIDGET_H
#define bscan2DWIDGET_H

#include <QSlider>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <map>
#include <QPointer>

#include <accomplishment.h>
#include "ascanDataWidget.h"

class bscanGlWidget;

class bscan2dWidget : public QWidget
{
    Q_OBJECT

public:
    bscan2dWidget(QWidget* parent = nullptr);
    ~bscan2dWidget();

    void setupTrzBscans(const QStringList& filesbscanNamesTrz, const std::vector<int> trzNumAntennasVector, std::vector<std::pair<std::string, std::string>>& BscansNamesVectorPairs);
    void setupCsvBscans(const QStringList& filesbscanNamesCsv, std::vector<std::pair<std::string, std::string>>& bscansNamesVectorPairs);
    void setupHdf5Bscans(const QStringList& infoBscansHdf5Names, std::vector<std::pair<std::string, std::string>>& bscansNamesVectorPairs);

    void outputNewImage(const std::string& fileName, int materialId, int filterId, int selectionId);
    void setShowLogBscan(bool showLogBscan);
    void showNewBscan(const std::string& bscanName);

    void setLogBscanMouseSelection(bool logBscanMouseSelection);
    void settingsBscanSelection(bool logBscanAutoSelection, bool clearSelectBscanPoints, std::string bscanName);

    void setShowInitBscanSelection(bool showInitBscanSelection);
    void resetBscanSelection(bool clearSelectBscanPoints, std::string bscanName);

    void saveBscanAutoSelectionData(QString saveBscanSelectionDataFile);
    void loadBscanAutoSelectionData(QString loadBscanSelectionDataFile, std::string bscanName);

    void setupTypeScaling(int typeScaling);
    void saveBscan();

    void setupColorsBscanAllProps(const QColor& colorUp, const QColor& colorLow);

    void changeBscanRightDataAtShift();
    void changeBscanImage(int absBscanPixelsInX, int absBscanPixelsFnX);

public slots:
    void slotActivateAccompThread(std::vector<std::pair<std::string, std::string>> BscansNamesVectorPairs, BscanFileFormat BscanFileFormat);

    void receiveOutBscanInfo(QString outBscanInfo);

    void setLowPixelLevel (int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);
    void setContrastLog10Level(int contrastBscanLevel);

    void slotClearBscansInfoDataMap();
    void slotEraseBscansInfoDataMap(const std::string& bscanName);
    void slotFixPixels(const std::string& bscanName);
    void slotFixBscanInfo(const std::string& bscanName);
    void slotFixBscanTrackElements(const std::string& bscanName);
    void slotFixBscanSection(const std::string& bscanName);
    void slotFixPageData(const std::string& bscanName);

    void slotCreateMapBscanTypeBscanSelectionInfo(int typeBscanSelection,  int bscanHeight, std::string bscanName, const std::vector<QPair<int, int> >& vectorBscanPairXY);

    void slotFixDataBscanWidgets(std::string bscanName);
    void accomplishmentThreadTerminate();

    void slotSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData);
    void slotSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData);

signals:
    void signalActivateAccompThread(std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs, BscanFileFormat bscanFileFormat);

    void sendOutBscanInfo(QString outBscanInfo);
    void signalSetupPixels(const st_bscanInfoData& bscanInfoData);
    void signalSendOutBscanInfo(const st_bscanInfoData& bscanInfoData);
//    void signalActivateBscan(int bscanId);

public:
    QPointer<Accomplishment>   m_accomplishment;
    QPointer<bscanGlWidget>    m_bscanGlWidget;
    QPointer<QScrollBar>       m_scrollHBar;
    QPointer<QScrollBar>       m_scrollVBar;
    QPointer<ascanDataWidget>  m_ascanDataWidget;

private:
    void bscanInput(const std::string& bscanName, int absPixelsInX, int absPixelsFnX);
    void setEnabledOpenGlControls(boolean enabled);
    void setupInitPixelSlidersData();
    void setupOpenglControls();

private:
    QPointer<QVBoxLayout> m_leftVLayout;
    QPointer<QHBoxLayout> m_leftHLayout;
    QPointer<QVBoxLayout> m_leftAllVLayout;

    QPointer<QSlider> m_contrastLog10Slider;
    QPointer<QLabel>  m_contrastLog10Label;

    QPointer<QSlider> m_lowBrightPixelSlider;
    QPointer<QLabel>  m_lowBrightLabel;

    QPointer<QSlider> m_highBrightPixelSlider;
    QPointer<QLabel>  m_highBrightLabel;

    QPointer<QVBoxLayout> m_rightLayout;
    QPointer<QHBoxLayout> m_leftLowLayout;
    QPointer<QHBoxLayout> m_mainLayout;
};

#endif // bscan2DWIDGET_H
