#ifndef ascanDEEPWIDGET_H
#define ascanDEEPWIDGET_H

#include "contentwidget.h"
#include "bscansData.h"

#include <QChart>
#include <QLineSeries>
#include <QObject>
#include <QValueAxis>a
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>

class ascanDeepWidget: public QWidget
{
    Q_OBJECT
    
public:
    explicit ascanDeepWidget(QWidget* parent = nullptr);
    ~ascanDeepWidget();

    void setTitle          (const QString& titleSection   );
    void outputSectionChart(const QString& bscansFileCurrent);

    void removeDeepSection();
    void outputLineDeepSection( const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY, int pixelsFnY, int filterId, int materialId, int selectionId);

    void createDeepSection(const st_bscanInfoData& bscanInfoData);
    
signals:
    void fixTrackIndex(const int& trackNumber);

private:
    QPointer<QChart> m_chart;
    QPointer<QLineSeries> m_deepLineSeries;

    QString m_titleSection;
    QPointer<contentWidget> m_contentWidget;

    QPointer<QVBoxLayout> m_verLayout;    
    
    QPointer<QValueAxis> m_axisX;
    QPointer<QValueAxis> m_axisY;

    int m_filterId    {0};
    int m_materialId  {0};
    int m_selectionId {0};

    int m_trackNumber;
    int m_pixelsInY;
    int m_pixelsFnY;
};

#endif // SECTIONBscanDEEPWIDGET_H



