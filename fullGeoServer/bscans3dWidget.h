#ifndef bscanS3DWIDGET_H
#define bscanS3DWIDGET_H

#include <QGridLayout>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPointer>

#include "bscansData.h"

class structureBscansWidget;
class bscansTransGlWidget;

class bscans3dWidget  : public QWidget
{
    Q_OBJECT

public:
    bscans3dWidget(QWidget* parent = nullptr);
    ~bscans3dWidget();

    QPointer<structureBscansWidget>  m_structBscansWidget;
    QPointer<bscansTransGlWidget>    m_bscansTransGlWidget;
    QPointer<QScrollBar>             m_scrollHBar;
    QPointer<QScrollBar>             m_scrollVBar;

    QPointer<QGridLayout> m_mainLayout;
    QPointer<QVBoxLayout> m_leftLayout;

    QPointer<QVBoxLayout> m_rightVLayout;
    QPointer<QHBoxLayout> m_rightHLayout;
    QPointer<QVBoxLayout> m_rightAllVLayout;

    QPointer<QSlider> m_lowBrightPixelSlider;
    QPointer<QLabel>  m_lowBrightLabel;

    QPointer<QSlider> m_highBrightPixelSlider;
    QPointer<QLabel>  m_highBrightLabel;

    QPointer<QHBoxLayout> m_rightLowLayout;

    void setupColorsBscansTransAllProps(const QColor& colorUp, const QColor& colorLow);

    void showBscansImage(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan, bool showFullBscans,
        bool resetCutPointsBscans, int absBscansStructInX, int absBscansStructFnX, int materialId
    );

    void changeBscansImage(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        int quantImpulsesOfPacket,  int filterId, int selectionId, bool highLowOnBscan, bool showFullBscans, bool resetCutPointsBscans, int materialId,
        int absBscansGraphInX, int absBscansGraphFnX
    );

    void setShowLogBscansTrans(bool showLogBscansTrans);
    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);
    void representBscansTransGlWidget(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData);
    void setupTypeScaling(int typeScaling);

    void saveBscansStruct();

    int  m_materialId{0};

signals:
    void sendOutBscansSurfInfo(QString outBscansSurfInfo);
    void signalCloseCut3dAction(bool cut3dVisible);
    void sendDeleteBscanName(const std::string& deleteBscanName);
    void sendProgressData(int progressPos, int progressWidth);

    void signalFixFillBscans(bool resetCutPointsBscans);
    void signalFixDeleteBscanName(double fixLatitude, double fixLongitude);

    void signalDefDeleteBscanName (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, double fixLatitude, double fixLongitude);
    void signalFillBscansExecute(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData, bool resetCutPointsBscans);

    void updateBscansTransGlWidget(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
    );

public slots:
    void slotCloseCut3dAction(bool cut3dVisible);
    void receiveDeleteBscanName(const std::string& deleteBscanName);

    void slotFixDeleteBscanName(double fixLatitude, double fixLongitude);
    void slotFixFillBscans(bool resetCutPointsBscans);

    void slotDefDeleteBscanName (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, double fixLatitude, double fixLongitude);
    void slotFillBscansExecute(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData, bool resetCutPointsBscans);

    void receiveBscansTransGlWidgetPoints(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
    );

    void setLowPixelLevel (int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

    void slotSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData);
    void slotSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData);

private:
    void outputBscansData();
    void setupInitPixelSlidersData();
};

#endif // bscanS3DWIDGET_H
