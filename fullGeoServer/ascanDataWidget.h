#ifndef ascanDATAWIDGET_H
#define ascanDATAWIDGET_H

#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPointer>
#include <QHBoxLayout>

#include "ascanWidget.h"
#include "tableAscanDataWidget.h"
#include "tableBscanLogAutoDataWidget.h"


class ascanDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ascanDataWidget(QWidget* parent = nullptr);
    ~ascanDataWidget();

    void saveBscanAutoSelectionData(QString saveBscanAutoSelectionDataFile);
    void setbscanName(std::string bscanName);
    void outputPageData(int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData);

    void setupTrackSliderData(int pixelsInX, int pixelsFnX);
    void activateWidgets(
        std::string bscanName, const st_bscanInfoData& bscanInfoData, int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY
    );

    int m_filterId    {0};
    int m_materialId  {0};
    int m_selectionId {0};

    QPointer<QTabWidget> m_tabWidget;
    QPointer<QLabel>     m_numTrackLabel;
    QPointer<QSlider>    m_trackSlider;

    QPointer<QHBoxLayout> m_horLayout;

    QPointer<ascanWidget>                 m_ascanWidget;
    QPointer<tableAscanDataWidget>        m_tableAscanDataWidget;
    QPointer<tableBscanLogAutoDataWidget> m_tableBscanLogAutoDataWidget;

    int m_pageIndex{0};
    int m_trackNumber{0};
    std::string m_bscanName{""};

    void trackElementsVisible(bool visible);
    void setupTrackElements(const st_bscanInfoData& bscanInfoData);

signals:
    void sendVectorBscanPairXY (int bscanPixelsInX, int bscanPixelsInY, int bscanPixelsFnX, int bscanPixelsFnY, int filterId, const st_bscanInfoData& bscanInfoData);
    void sendLoadMapBscanPairXY(
        int bscanPixelsInX, int bscanPixelsInY, int bscanPixelsFnX, int bscanPixelsFnY,
        const st_bscanInfoData& bscanInfoData,const std::map<int, std::vector<QPair<int, int>> >& mapBscanPairXY
    );

    void signalFixPageData(const std::string& bscanName);

public slots:
    void setPageIndex(int pageIndex);
    void setTrackBscan(int trackBscanNumber);

    void receiveVectorBscanPairXY (int bscanPixelsInX, int bscanPixelsInY, int bscanPixelsFnX, int bscanPixelsFnY, int filterId,  const st_bscanInfoData& bscanInfoData);
    void receiveLoadMapBscanPairXY(
        int bscanPixelsInX, int bscanPixelsInY, int bscanPixelsFnX, int bscanPixelsFnY,
        const st_bscanInfoData& bscanInfoData, const std::map<int, std::vector<QPair<int, int>> >& mapBscanPairXY
    );

    void slotHideTableBscanLogAutoData();
};
#endif // DATABscanWIDGET_H
