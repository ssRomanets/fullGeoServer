#ifndef SECTIONDEEPRDGWIDGET_H
#define SECTIONDEEPRDGWIDGET_H

#include "contentwidget.h"
#include "rdgsData.h"

#include <QChart>
#include <QLineSeries>
#include <QObject>
#include <QValueAxis>a
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>

class sectionDeepRdgWidget: public QWidget
{
    Q_OBJECT
    
public:
    explicit sectionDeepRdgWidget(QWidget* parent = nullptr);
    ~sectionDeepRdgWidget();    

    void setTitle          (const QString& titleSection   );
    void outputSectionChart(const QString& rdgsFileCurrent);

    void removeDeepRdgSection();
    void outputLineDeepRdgSection(
        const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY, int rdgPixelsFnY,
        int filterId, int materialId, int selectionId
    );
    void createDeepRdgSection(const st_rdgInfoData& rdgInfoData);
    
signals:
    void fixTrackIndex(const int& trackRdgNumber);

private:
    QPointer<QChart> m_chart;
    QPointer<QLineSeries> m_deepRdgLineSeries;

    QString m_titleSection;
    QPointer<contentWidget> m_contentWidget;

    QPointer<QVBoxLayout> m_verLayout;    
    
    QPointer<QValueAxis> m_axisX;
    QPointer<QValueAxis> m_axisY;

    int m_filterId    {0};
    int m_materialId  {0};
    int m_selectionId {0};

    int m_trackRdgNumber;
    int m_rdgPixelsInY;
    int m_rdgPixelsFnY;
};

#endif // SECTIONDEEPRDGWIDGET_H



