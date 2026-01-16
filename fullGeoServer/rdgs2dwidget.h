#ifndef RDGS2DWIDGET_H
#define RDGS2DWIDGET_H

#include <QHBoxLayout>
#include <QScrollBar>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>

#include "rdgsData.h"
#include "rdgsSurfGlWidget.h"
#include "sectionRdgsSurfWidget.h"

class rdgs2dWidget : public QWidget
{
    Q_OBJECT

public:
    rdgs2dWidget(QWidget* parent = nullptr);
    ~rdgs2dWidget();

    void showRdgsSurf(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData,
        int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg, bool showFullRdgs,  bool resetSectPoints2dRdgs,
        int materialId, int absRdgsStructInX, int absRdgsStructFnX
    );

    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);

    void saveRdgsSurf();
    void setupColorsRdgsSurfAllProps(const QColor& colorUp, const QColor& colorLow);

    void changeRdgsSurf(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData,
        int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg, bool showFullRdgs, bool resetSectPoints2dRdgs,
        int materialId, int absRdgsStructInX, int absRdgsStructFnX
    );

public slots:
    void slotFixRdgsSurfSection();
    void slotCloseSection2dAction(bool section2dVisible);
    void receiveOutRdgsSurfInfo(QString outRdgsSurfInfo);
    void slotFixRdgsSurfPixels();
    void slotSetupRdgsSurfPixels  (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData);
    void slotOutputRdgsSurfSection(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData);

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

    void slotSetupScrollHRdgsSurfData(int minScrollHRdgsSurfData, int scrollHRdgsSurfData, int maxScrollHRdgsSurfData);
    void slotSetupScrollVRdgsSurfData(int minScrollVRdgsSurfData, int scrollVRdgsSurfData, int maxScrollVRdgsSurfData);

signals:
    void signalCloseSection2dAction(bool section2dVisible);
    void sendProgressData(int progressPos, int progressWidth);
    void sendOutRdgsSurfInfo(QString outRdgsSurfInfo);
    void signalFixRdgsSurfPixels();
    void signalFixRdgsSurfSection();

private:
    void setupInitPixelSlidersData();
    void output2dRdgsData();

private:
    QPointer<rdgsSurfGlWidget>       m_rdgsSurfGlWidget;
    QPointer<QScrollBar>             m_scrollHRdgsSurfBar;
    QPointer<QScrollBar>             m_scrollVRdgsSurfBar;
    QPointer<sectionRdgsSurfWidget>  m_sectionRdgsSurfWidget;

    QPointer<QGridLayout> m_mainLayout;

    QPointer<QVBoxLayout> m_leftVLayout;
    QPointer<QHBoxLayout> m_leftHLayout;
    QPointer<QVBoxLayout> m_leftAllVLayout;

    QPointer<QVBoxLayout> m_rightLayout;

    QPointer<QHBoxLayout> m_leftLowLayout;

    QPointer<QSlider> m_lowBrightPixelSlider;
    QPointer<QLabel>  m_lowBrightLabel;

    QPointer<QSlider> m_highBrightPixelSlider;
    QPointer<QLabel>  m_highBrightLabel;
};

#endif // RDGS2DWIDGET_H
