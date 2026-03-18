#ifndef aSCANDEEPDATAWIDGET_H
#define aSCANDEEPDATAWIDGET_H

#include <QLabel>
#include <QTabWidget>
#include <QWidget>
#include <QPointer>

#include "ascanDeepWidget.h"
#include "tableAscanDeepDataWidget.h"

class ascanDeepDataWidget :  public QWidget
{
    Q_OBJECT

public:
    ascanDeepDataWidget(QWidget* parent = nullptr);
    ~ascanDeepDataWidget();

    void setBscanName(std::string bscanName);

    QPointer<QTabWidget> m_tabWidget;
    QPointer<QLabel>     m_numTrackLabel;
    QPointer<QSlider>    m_trackSlider;

    QPointer<QHBoxLayout> m_horLayout;

    QPointer<ascanDeepWidget>             m_ascanDeepWidget;
    QPointer<tableAscanDeepDataWidget>    m_tableAscanDeepDataWidget;

    int m_pageIndex{0};
    std::string m_bscanName{""};
    int m_trackNumber{0};

    void trackElementsVisible(bool visible);   
    void setupTrackSliderData(int pixelsInX, int pixelsFnX);
    void activateWidgets(const st_bscanInfoData& bscanInfoData, int pixelsInY, int pixelsFnY);

signals:
    void signalFixPageData(const std::string& bscanName);

public slots:
    void setPageIndex(int pageIndex);
    void setTrackBscan(int trackNumber);
    void outputPageData(int pixelsInY, int pixelsFnY, const st_bscanInfoData& bscanInfoData);

public:
    int m_materialId  ;
    int m_filterId    ;
    int m_selectionId ;

};

#endif // BSCANDEEPDATAWIDGET_H
