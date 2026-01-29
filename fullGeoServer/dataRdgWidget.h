#ifndef DATARDGWIDGET_H
#define DATARDGWIDGET_H

#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPointer>
#include <QHBoxLayout>

#include "sectionRdgWidget.h"
#include "tableRdgTrackDataWidget.h"
#include "tableRdgLogAutoDataWidget.h"

class dataRdgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dataRdgWidget(QWidget* parent = nullptr);
    ~dataRdgWidget();

    void saveRdgAutoSelectionData(QString saveRdgAutoSelectionDataFile);
    void setRdgName(std::string rdgName);
    void outputPageRdgData(int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, const st_rdgInfoData& rdgInfoData);

    void setupTrackRdgSliderData(int rdgPixelsInX, int rdgPixelsFnX);
    void activateWidgets(
        std::string rdgName, const st_rdgInfoData& rdgInfoData, int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY
    );

    QPointer<QTabWidget> m_tabWidget;
    QPointer<QLabel>     m_numTrackRdgLabel;
    QPointer<QSlider>    m_trackRdgSlider;

    QPointer<QHBoxLayout> m_horLayout;

    QPointer<sectionRdgWidget>          m_sectionRdgWidget;
    QPointer<tableRdgTrackDataWidget>   m_tableRdgTrackDataWidget;
    QPointer<tableRdgLogAutoDataWidget> m_tableRdgLogAutoDataWidget;

    int m_pageIndex{0};
    int m_trackRdgNumber{0};
    std::string m_rdgName{""};

    void trackElementsVisible(bool visible);
    void setupTrackElements(const st_rdgInfoData& rdgInfoData);

signals:
    void sendVectorRdgPairXY (int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, int filterId, const st_rdgInfoData& rdgInfoData);
    void sendLoadMapRdgPairXY(
        int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY,
        const st_rdgInfoData& rdgInfoData,const std::map<int, std::vector<QPair<int, int>> >& mapRdgPairXY
    );
    void signalFixRdgTrackElements(const std::string& rdgName);
    void signalFixPageRdgData(const std::string& rdgName);

public slots:
    void setPageIndex(int pageIndex);
    void setTrackRdg(int trackRdgNumber);

    void receiveVectorRdgPairXY (int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, int filterId,  const st_rdgInfoData& rdgInfoData);
    void receiveLoadMapRdgPairXY(
        int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY,
        const st_rdgInfoData& rdgInfoData, const std::map<int, std::vector<QPair<int, int>> >& mapRdgPairXY
    );

    void slotHideTableRdgLogAutoData();
};
#endif // DATARDGWIDGET_H
