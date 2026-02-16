#include "sectionDeepRdgWidget.h"
#include "transformation.h"

#include "iostream"

sectionDeepRdgWidget::sectionDeepRdgWidget(QWidget *parent): QWidget(parent)
{
    m_chart = new QChart;
    m_deepRdgLineSeries = new QLineSeries();
    
    m_contentWidget = new contentWidget(parent);
    
    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_contentWidget);
    setLayout(m_verLayout);
    
    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;
}

sectionDeepRdgWidget::~sectionDeepRdgWidget(){}

void sectionDeepRdgWidget::setTitle(const QString &titleSection)
{
    m_titleSection = titleSection;
}

void sectionDeepRdgWidget::outputSectionChart(const QString& fileCurrent)
{
    m_contentWidget->createDefaultChartView(m_chart);
    m_chart->setTitle(m_titleSection+" "+fileCurrent);
}

void sectionDeepRdgWidget::createDeepRdgSection(const st_rdgInfoData& rdgInfoData)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        removeDeepRdgSection();
        for (int i = m_rdgPixelsInY; i <= m_rdgPixelsFnY; i++)
        {
            m_deepRdgLineSeries->append(
                i,
                -1.0*rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId+m_selectionId][i]
            );
        }

        QValueAxis *axisX = new QValueAxis;
        QValueAxis *axisY = new QValueAxis;
        axisY->setTitleText("Глубина сканирования в метрах");
        axisX->setTitleText("Импульсы");

        m_chart->addSeries(m_deepRdgLineSeries);
        m_chart->setAxisX(axisX, m_deepRdgLineSeries);
        m_chart->setAxisY(axisY, m_deepRdgLineSeries);
        m_chart->legend()->hide();
    }
}

void sectionDeepRdgWidget::outputLineDeepRdgSection(
    const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY, int rdgPixelsFnY,
    int filterId, int materialId, int selectionId
)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        m_trackRdgNumber = trackRdgNumber;
        m_rdgPixelsInY   = rdgPixelsInY;
        m_rdgPixelsFnY   = rdgPixelsFnY;

        m_filterId    = filterId;
        m_materialId  = materialId;
        m_selectionId = selectionId;

        createDeepRdgSection(rdgInfoData);
        emit fixTrackIndex(trackRdgNumber);
    }
}

void sectionDeepRdgWidget::removeDeepRdgSection()
{
    m_chart->removeAxis(m_chart->axisX());
    m_chart->removeAxis(m_chart->axisY());
    m_chart->setTitle("");
    m_chart->removeSeries(m_deepRdgLineSeries);
    m_deepRdgLineSeries->clear();
}
