#ifndef RDG2DWIDGET_H
#define RDG2DWIDGET_H

#include <QSlider>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <map>
#include <QPointer>

#include <accomplishment.h>
#include "dataRdgWidget.h"

class rdgGlWidget;

class rdg2dWidget : public QWidget
{
    Q_OBJECT

public:
    rdg2dWidget(QWidget* parent = nullptr);
    ~rdg2dWidget();

    void setupTrzRdgs(const QStringList& filesRdgNamesTrz, const std::vector<int> trzNumAntennasVector, std::vector<std::pair<std::string, std::string>>& rdgsNamesVectorPairs);    
    void setupCsvRdgs(const QStringList& filesRdgNamesCsv, std::vector<std::pair<std::string, std::string>>& rdgsNamesVectorPairs);
    void setupHdf5Rdgs(const QStringList& infoRdgsHdf5Names, std::vector<std::pair<std::string, std::string>>& rdgsNamesVectorPairs);

    void outputNewImage(const std::string& fileName,int materialId, int filterId);
    void setShowLogRdg(bool showLogRdg);
    void showNewRdg(const std::string& rdgName);

    void setLogRdgMouseSelection(bool logRdgMouseSelection);
    void settingsRdgSelection(bool logRdgAutoSelection, bool clearSelectRdgPoints, std::string rdgName);

    void setShowInitRdgSelection(bool showInitRdgSelection);
    void resetRdgSelection(bool clearSelectRdgPoints, std::string rdgName);

    void saveRdgAutoSelectionData(QString saveRdgSelectionDataFile);
    void loadRdgAutoSelectionData(QString loadRdgSelectionDataFile, std::string rdgName);

    void setupRdgScaling(int typeRdgScaling);
    void saveRdg();

    void setupColorsRdgAllProps(const QColor& colorUp, const QColor& colorLow);

    void changeRdgRightDataAtShift();
    void changeRdgImage(int absRdgPixelsInX, int absRdgPixelsFnX);

public slots:
    void slotActivateAccompThread(std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, RdgFileFormat rdgFileFormat);

    void receiveOutRdgInfo(QString outRdgInfo);

    void setLowPixelLevel (int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);
    void setContrastLog10Level(int contrastRdgLevel);

    void slotClearRdgsInfoDataMap();
    void slotEraseRdgsInfoDataMap(const std::string& rdgName);
    void slotFixRdgPixels(const std::string& rdgName);
    void slotFixRdgInfo(const std::string& rdgName);
    void slotFixRdgTrackElements(const std::string& rdgName);
    void slotFixRdgSection(const std::string& rdgName);
    void slotFixPageRdgData(const std::string& rdgName);

    void slotCreateMapRdgTypeRdgSelectionInfo(int typeRdgSelection,  int rdgHeight, std::string rdgName, const std::vector<QPair<int, int> >& vectorRdgPairXY);

    void slotFixDataRdgWidgets(std::string rdgName);
    void accomplishmentThreadTerminate();

    void slotSetupScrollHRdgData(int minScrollHRdgData, int scrollHRdgData, int maxScrollHRdgData);
    void slotSetupScrollVRdgData(int minScrollVRdgData, int scrollVRdgData, int maxScrollVRdgData);

signals:
    void signalActivateAccompThread(std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, RdgFileFormat rdgFileFormat);

    void sendOutRdgInfo(QString outRdgInfo);
    void signalSetupRdgPixels(const st_rdgInfoData& rdgInfoData);
    void signalSendOutRdgInfo(const st_rdgInfoData& rdgInfoData);
    void signalActivateRdg(int rdgId);

public:
    QPointer<Accomplishment> m_accomplishment;
    QPointer<rdgGlWidget>    m_rdgGlWidget;
    QPointer<QScrollBar>     m_scrollHRdgBar;
    QPointer<QScrollBar>     m_scrollVRdgBar;
    QPointer<dataRdgWidget>  m_dataRdgWidget;

private:
    void rdgInput(const std::string& rdgName, int absRdgPixelsInX, int absRdgPixelsFnX);
    void setEnabledOpenGlControls(boolean enabled);
    void setupInitPixelSlidersData();
    void setupRdgOpenglControls();

private:
    QPointer<QSlider> m_contrastLog10Slider;
    QPointer<QLabel>  m_contrastLog10Label;

    QPointer<QSlider> m_lowBrightPixelSlider;
    QPointer<QLabel>  m_lowBrightLabel;

    QPointer<QSlider> m_highBrightPixelSlider;
    QPointer<QLabel>  m_highBrightLabel;

    QPointer<QVBoxLayout> m_leftVLayout;
    QPointer<QHBoxLayout> m_leftHLayout;
    QPointer<QVBoxLayout> m_leftAllVLayout;

    QPointer<QVBoxLayout> m_rightLayout;

    QPointer<QHBoxLayout> m_leftLowLayout;
    QPointer<QHBoxLayout> m_mainLayout;
};

#endif // RDG2DWIDGET_H
