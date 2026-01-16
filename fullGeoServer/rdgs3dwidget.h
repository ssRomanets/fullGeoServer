#ifndef RDGS3DWIDGET_H
#define RDGS3DWIDGET_H

#include <QGridLayout>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPointer>

#include "rdgsData.h"

class structureRdgsWidget;
class rdgsTransGlWidget;

class rdgs3dWidget  : public QWidget
{
    Q_OBJECT

public:
    rdgs3dWidget(QWidget* parent = nullptr);
    ~rdgs3dWidget();

    QPointer<structureRdgsWidget>  m_structRdgsWidget;
    QPointer<rdgsTransGlWidget>    m_rdgsTransGlWidget;
    QPointer<QScrollBar>           m_scrollHRdgsTransBar;
    QPointer<QScrollBar>           m_scrollVRdgsTransBar;

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

    void setupColorsRdgsTransAllProps(const QColor& colorUp, const QColor& colorLow);

    void showRdgsImage(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg, bool showFullRdgs,
        bool resetCutPointsRdgs, int absRdgsStructInX, int absRdgsStructFnX, int materialId
    );

    void changeRdgsImage(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        int quantImpulsesOfPacket,  int filterId, int selectionId, bool highLowOnRdg, bool showFullRdgs, bool resetCutPointsRdgs, int materialId,
        int absRdgsGraphInX, int absRdgsGraphFnX
    );

    void setShowLogRdgsTrans(bool showLogRdgsTrans);
    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);
    void representRdgsTransGlWidget(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData);
    void setupRdgsTransScaling(int typeRdgsTransScaling);

    void saveRdgsStruct();

    int  m_materialId{0};

signals:
    void sendOutRdgsSurfInfo(QString outRdgsSurfInfo);
    void signalCloseCut3dAction(bool cut3dVisible);
    void sendDeleteRdgName(const std::string& deleteRdgName);
    void sendProgressData(int progressPos, int progressWidth);

    void signalFixFillRdgs(bool resetCutPointsRdgs);
    void signalFixDeleteRdgName(double fixLatitude, double fixLongitude);

    void signalDefDeleteRdgName (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, double fixLatitude, double fixLongitude);
    void signalFillRdgsExecute(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData, bool resetCutPointsRdgs);

    void updateRdgsTransGlWidget(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        const std::vector<std::pair<int, int>>& vectorRdgsTransLineSectionFinalPoints, bool highLowOnRdg
    );

public slots:
    void slotCloseCut3dAction(bool cut3dVisible);
    void receiveDeleteRdgName(const std::string& deleteRdgName);

    void slotFixDeleteRdgName(double fixLatitude, double fixLongitude);
    void slotFixFillRdgs(bool resetCutPointsRdgs);

    void slotDefDeleteRdgName (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, double fixLatitude, double fixLongitude);
    void slotFillRdgsExecute(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData, bool resetCutPointsRdgs);

    void receiveRdgsTransGlWidgetPoints(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        const std::vector<std::pair<int, int>>& vectorRdgsTransLineSectionFinalPoints, bool highLowOnRdg
    );

    void setLowPixelLevel (int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

    void slotSetupScrollHRdgsTransData(int minScrollHRdgsTransData, int scrollHRdgsTransData, int maxScrollHRdgsTransData);
    void slotSetupScrollVRdgsTransData(int minScrollVRdgsTransData, int scrollVRdgsTransData, int maxScrollVRdgsTransData);

private:
    void outputRdgsData();
    void  setupInitPixelSlidersData();
};

#endif // RDGS3DWIDGET_H
