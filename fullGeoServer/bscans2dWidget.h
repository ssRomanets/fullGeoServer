#ifndef bscanS2DWIDGET_H
#define bscanS2DWIDGET_H

#include <QHBoxLayout>
#include <QScrollBar>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>

#include "bscansData.h"
#include "bscansSurfGlWidget.h"
#include "sectionBscansSurfWidget.h"

class bscans2dWidget : public QWidget
{
    Q_OBJECT

public:
    bscans2dWidget(QWidget* parent = nullptr);
    ~bscans2dWidget();

    void showBscansSurf(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData,
        int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan, bool showFullBscans,  bool resetSectPoints2dBscans,
        int materialId, int absBscansStructInX, int absBscansStructFnX
    );

    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);

    void saveBscansSurf();
    void setupColorsBscansSurfAllProps(const QColor& colorUp, const QColor& colorLow);

    void changeBscansSurf(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData,
        int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan, bool showFullBscans, bool resetSectPoints2dBscans,
        int materialId, int absBscansStructInX, int absBscansStructFnX
    );

    void setupTypeScaling(int typeScaling);

public slots:
    void slotFixBscansSurfSection();
    void slotCloseSection2dAction(bool section2dVisible);
    void receiveOutBscansSurfInfo(QString outBscansSurfInfo);
    void slotFixBscansSurfPixels();
    void slotSetupBscansSurfPixels  (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData);
    void slotOutputBscansSurfSection(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData);

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

    void slotSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData);
    void slotSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData);

signals:
    void signalCloseSection2dAction(bool section2dVisible);
    void sendProgressData(int progressPos, int progressWidth);
    void sendOutBscansSurfInfo(QString outBscansSurfInfo);
    void signalFixBscansSurfPixels();
    void signalFixBscansSurfSection();

private:
    void setupInitPixelSlidersData();
    void output2dBscansData();

private:
    QPointer<bscansSurfGlWidget>       m_bscansSurfGlWidget;
    QPointer<QScrollBar>               m_scrollHBar;
    QPointer<QScrollBar>               m_scrollVBar;
    QPointer<sectionBscansSurfWidget>  m_sectionBscansSurfWidget;

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

#endif // bscanS2DWIDGET_H
