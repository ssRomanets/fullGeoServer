#include "ascanDeepWidget.h"
#include "transformation.h"

#include "iostream"

ascanDeepWidget::ascanDeepWidget(QWidget *parent): QWidget(parent)
{
    m_chart = new QChart;
    m_deepLineSeries = new QLineSeries();
    
    m_contentWidget = new contentWidget(parent);
    
    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_contentWidget);
    setLayout(m_verLayout);
    
    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;
}

ascanDeepWidget::~ascanDeepWidget(){}

void ascanDeepWidget::setTitle(const QString &titleSection)
{
    m_titleSection = titleSection;
}

void ascanDeepWidget::outputSectionChart(const QString& fileCurrent)
{
    m_contentWidget->createDefaultChartView(m_chart);
    m_chart->setTitle(m_titleSection+" "+fileCurrent);
}

void ascanDeepWidget::createDeepSection(const st_bscanInfoData& bscanInfoData)
{
    if (bscanInfoData.vectorBscanData.size() > 0)
    {
        m_deepLineSeries->clear();

        double minDeepData =
        bscanInfoData.vectorBscanData[m_trackNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId+m_selectionId][m_pixelsInY];
        double maxDeepData =
        bscanInfoData.vectorBscanData[m_trackNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId+m_selectionId][m_pixelsFnY];

        for (int i = m_pixelsInY; i <= m_pixelsFnY; i++)
            m_deepLineSeries->append(bscanInfoData.vectorBscanData[m_trackNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId+m_selectionId][i], i);

        if (m_deepLineSeries->points().size() > 0)
        {
            m_axisY->setTitleText("Импульсы");
            m_axisX->setTitleText("Глубина сканирования в метрах");

            if (m_chart->series().size()==0)
            {
                m_chart->addSeries(m_deepLineSeries);
                m_chart->setAxisX(m_axisX, m_deepLineSeries);
                m_chart->setAxisY(m_axisY, m_deepLineSeries);
            }
            m_axisY->setReverse(true);
            m_axisY->setRange(m_pixelsInY, m_pixelsFnY);

            m_axisX->setRange(minDeepData, maxDeepData);
            m_chart->legend()->hide();
        }
    }
}

void ascanDeepWidget::outputLineDeepSection(const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY, int pixelsFnY, int filterId, int materialId, int selectionId)
{
    if (bscanInfoData.vectorBscanData.size() > 0)
    {
        m_trackNumber = trackNumber;
        m_pixelsInY   = pixelsInY;
        m_pixelsFnY   = pixelsFnY;

        m_filterId    = filterId;
        m_materialId  = materialId;
        m_selectionId = selectionId;

        createDeepSection(bscanInfoData);
        emit fixTrackIndex(trackNumber);
    }
}

void ascanDeepWidget::removeDeepSection()
{
    m_chart->removeAxis(m_chart->axisX());
    m_chart->removeAxis(m_chart->axisY());
    m_chart->setTitle("");
    m_chart->removeSeries(m_deepLineSeries);
    m_deepLineSeries->clear();
}
