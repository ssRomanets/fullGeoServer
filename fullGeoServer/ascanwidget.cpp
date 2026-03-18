#include "ascanWidget.h"
#include "transformation.h"

#include <iostream>

ascanWidget::ascanWidget(QWidget* parent): QWidget(parent)
{
    m_chart = new QChart;
    m_lineSeries = new QLineSeries();

    m_contentWidget = new contentWidget(parent);

    m_normActCheckBox  = new QCheckBox;
    m_normActCheckBox->setText(tr("Включить/Выключить нормировку"));
    m_normActCheckBox->setMaximumHeight(30);

    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_contentWidget);
    m_verLayout->addWidget(m_normActCheckBox);
    setLayout(m_verLayout);

    connect(m_normActCheckBox, &QCheckBox::stateChanged, this,  &ascanWidget::incExcNorm);
    m_normActCheckBox->setEnabled(false);

    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;
}

ascanWidget::~ascanWidget(){}

void ascanWidget::setTitle(const QString& titleSection){    m_titleSection = titleSection;}

void ascanWidget::outputSectionChart(const QString& fileCurrent)
{
   m_contentWidget->createDefaultChartView(m_chart);
   m_chart->setTitle(m_titleSection+" "+fileCurrent);
}

void ascanWidget::createSection(const st_bscanInfoData& bscanInfoData)
{
    if (bscanInfoData.vectorBscanData.size() > 0)
    {
        m_lineSeries->clear();

        double minSecData = 0.0;
        double maxSecData = 0.0;

        if (m_showLog == false)
        {
            minSecData = (bscanInfoData.vectorBscanData[m_trackNumber].vectorsDoubleData[m_filterId])[0];
            maxSecData = (bscanInfoData.vectorBscanData[m_trackNumber].vectorsDoubleData[m_filterId])[0];
            for (int i = m_pixelsInY; i <= m_pixelsFnY; i++)
            {
                if (minSecData > (bscanInfoData.vectorBscanData[m_trackNumber].vectorsDoubleData[m_filterId])[i])
                    minSecData = (bscanInfoData.vectorBscanData[m_trackNumber].vectorsDoubleData[m_filterId])[i];

                if (maxSecData < (bscanInfoData.vectorBscanData[m_trackNumber].vectorsDoubleData[m_filterId])[i])
                    maxSecData = (bscanInfoData.vectorBscanData[m_trackNumber].vectorsDoubleData[m_filterId])[i];
            }
        }
        else
        {
            minSecData = defbscanLog10(bscanInfoData,  m_filterId, m_trackNumber, 0);
            maxSecData = defbscanLog10(bscanInfoData,  m_filterId, m_trackNumber, 0);
            for (int i = m_pixelsInY; i <= m_pixelsFnY; i++)
            {
                if (minSecData > defbscanLog10(bscanInfoData,  m_filterId, m_trackNumber,  i))
                    minSecData = defbscanLog10(bscanInfoData,  m_filterId, m_trackNumber,  i);

                if (maxSecData < defbscanLog10(bscanInfoData,  m_filterId, m_trackNumber,  i))
                    maxSecData = defbscanLog10(bscanInfoData,  m_filterId, m_trackNumber,  i);
            }
        }

        for (int i = m_pixelsInY; i <= m_pixelsFnY; i++)
        {
            if (m_showLog == false)
            {
                if (m_normActCheckBox->checkState() == 0)
                    m_lineSeries->append((bscanInfoData.vectorBscanData[m_trackNumber].vectorsDoubleData[m_filterId])[i], i);
                else
                {
                    m_lineSeries->append( ((bscanInfoData.vectorBscanData[m_trackNumber].vectorsDoubleData[m_filterId])[i] - minSecData)/  (maxSecData - minSecData), i);
                }
            }
            else
            {               
                double bscanLog10 = defbscanLog10(bscanInfoData,  m_filterId, m_trackNumber,  i);
                if (m_normActCheckBox->checkState() == 0)  m_lineSeries->append(bscanLog10, i);
                else   m_lineSeries->append(fabs(bscanLog10 - minSecData)/(maxSecData - minSecData),i);
            }
        }

        if (m_lineSeries->points().size() > 0)
        {
            if (m_showLog == false)
            {
                if (m_normActCheckBox->checkState() == 0) m_axisX->setTitleText("Яркость сканирования по каждому треку");
                else                                      m_axisX->setTitleText("Нормированная яркость сканирования по каждому треку");
            }
            else
            {
                if (m_normActCheckBox->checkState() == 0) m_axisX->setTitleText("Логарифмическая яркость сканирования по каждому треку");
                else                                      m_axisX->setTitleText("Нормированная логарифмическая яркость сканирования по каждому треку");
            }
            m_axisY->setTitleText("отчеты трека радарограммы");

            if (m_chart->series().size()==0)
            {
                m_chart->addSeries(m_lineSeries);
                m_chart->setAxisX(m_axisX, m_lineSeries);
                m_chart->setAxisY(m_axisY, m_lineSeries);
            }
            m_axisY->setReverse(true);
            m_axisY->setRange(m_pixelsInY, m_pixelsFnY);

            if (m_normActCheckBox->checkState() == 0) m_axisX->setRange(minSecData,maxSecData);
            else                                      m_axisX->setRange(0, 1);
            m_chart->legend()->hide();
        }
    }
}

void ascanWidget::removeSection()
{
    m_chart->removeAxis(m_chart->axisX());
    m_chart->removeAxis(m_chart->axisY());
    m_chart->setTitle("");
    m_chart->removeSeries(m_lineSeries);
    m_lineSeries->clear();
    m_normActCheckBox->setEnabled(false);
}

void ascanWidget::outputSection( const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int trackNumber,  int pixelsInY, int pixelsFnY)
{
    m_bscanName = bscanName;
    if (m_normActCheckBox->isEnabled() == false)  m_normActCheckBox->setEnabled(true);
    outputLineSection(bscanInfoData, trackNumber, pixelsInY, pixelsFnY);
}

void ascanWidget::outputLineSection(const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY, int pixelsFnY)
{
    if (bscanInfoData.vectorBscanData.size() > 0)
    {
        m_trackNumber = trackNumber;
        m_pixelsInY   = pixelsInY;
        m_pixelsFnY   = pixelsFnY;

        createSection(bscanInfoData);
        emit fixTrackIndex(trackNumber);
    }
}

void ascanWidget::incExcNorm(){   emit signalFixSection(m_bscanName);}

void ascanWidget::setShowLog(bool showLog){ m_showLog = showLog;}
