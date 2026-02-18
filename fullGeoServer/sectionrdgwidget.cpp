#include "sectionRdgWidget.h"
#include "transformation.h"

#include <iostream>

sectionRdgWidget::sectionRdgWidget(QWidget* parent): QWidget(parent)
{
    m_chart = new QChart;
    m_rdgLineSeries = new QLineSeries();

    m_contentWidget = new contentWidget(parent);

    m_normActCheckBox  = new QCheckBox;
    m_normActCheckBox->setText(tr("Включить/Выключить нормировку глубин по rdg"));
    m_normActCheckBox->setMaximumHeight(30);

    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_contentWidget);
    m_verLayout->addWidget(m_normActCheckBox);
    setLayout(m_verLayout);

    connect(m_normActCheckBox, &QCheckBox::stateChanged, this,  &sectionRdgWidget::incExcRdgNorm);

    m_normActCheckBox->setEnabled(false);

    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;
}

sectionRdgWidget::~sectionRdgWidget()
{
}

void sectionRdgWidget::setTitle(const QString& titleSection)
{
    m_titleSection = titleSection;
}

void sectionRdgWidget::outputSectionChart(const QString& fileCurrent)
{
   m_contentWidget->createDefaultChartView(m_chart);
   m_chart->setTitle(m_titleSection+" "+fileCurrent);
}

void sectionRdgWidget::createRdgSection(const st_rdgInfoData& rdgInfoData)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        m_rdgLineSeries->clear();

        double maxRdg = rdgInfoData.vectorRdgData[0].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][0];
        double minRdg = rdgInfoData.vectorRdgData[0].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][0];

        for (int i = m_rdgPixelsInY; i <= m_rdgPixelsFnY; i++)  defMaxRdgMinRdg(rdgInfoData, m_materialId, m_filterId, m_trackRdgNumber, i, maxRdg , minRdg);


        for (int i = m_rdgPixelsInY; i <= m_rdgPixelsFnY; i++)
        {
            if (m_showLogRdg == false)
            {
                if (i == 0)
                {
                    if (m_normActCheckBox->checkState() == 0)
                        m_rdgLineSeries->append(i, rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][i]);
                    else
                    {
                        m_rdgLineSeries->append(i,
                            (rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][i] - minRdg)/
                            (maxRdg - minRdg)
                        );
                    }
                }
                else
                {
                    if (m_normActCheckBox->checkState() == 0)
                        m_rdgLineSeries->append(i,
                            rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][i] -
                            rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][i-1]
                        );
                    else
                    {
                        m_rdgLineSeries->append(i, (
                            rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][i] -
                            rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][i-1] - minRdg
                        )/(maxRdg - minRdg));
                    }\
                }
            }
            else
            {               
                double rdgLog10 = defRdgLog10(rdgInfoData,  m_filterId, m_trackRdgNumber,  i);
                if (m_normActCheckBox->checkState() == 0)  m_rdgLineSeries->append(i, rdgLog10);
                else   m_rdgLineSeries->append(i, fabs(rdgLog10 - rdgInfoData.vectorMinLog10RdgData[m_filterId])/(rdgInfoData.vectorMaxLog10RdgData[m_filterId] - rdgInfoData.vectorMinLog10RdgData[m_filterId]));
            }
        }

        if (m_rdgLineSeries->points().size() > 0)
        {
            if (m_showLogRdg == false)
            {
                if (m_normActCheckBox->checkState() == 0) m_axisY->setTitleText("Глубина сканирования по каждому импульсу m");
                else                                      m_axisY->setTitleText("Нормированная глубина сканирования по каждому импульсу");
            }
            else
            {
                if (m_normActCheckBox->checkState() == 0) m_axisY->setTitleText("Логарифмическая яркость сканирования по каждому треку");
                else                                      m_axisY->setTitleText("Нормированная логарифмическая яркость сканирования по каждому треку");
            }
            m_axisX->setTitleText("импульс радарограммы");

            if (m_chart->series().size()==0)
            {
                m_chart->addSeries(m_rdgLineSeries);
                m_chart->setAxisX(m_axisX, m_rdgLineSeries);
                m_chart->setAxisY(m_axisY, m_rdgLineSeries);
            }
            //m_axisY->setReverse(true);
            m_axisX->setRange(m_rdgPixelsInY, m_rdgPixelsFnY);

            if (m_normActCheckBox->checkState() == 0)
            {
                if (m_showLogRdg == false) m_axisY->setRange(minRdg, maxRdg);
                else                       m_axisY->setRange(rdgInfoData.vectorMinLog10RdgData[m_filterId], rdgInfoData.vectorMaxLog10RdgData[m_filterId]);
            }
            else     m_axisY->setRange(0, 1);
            m_chart->legend()->hide();
        }
    }
}

void sectionRdgWidget::removeRdgSection()
{
    m_chart->removeAxis(m_chart->axisX());
    m_chart->removeAxis(m_chart->axisY());
    m_chart->setTitle("");
    m_chart->removeSeries(m_rdgLineSeries);
    m_rdgLineSeries->clear();
    m_normActCheckBox->setEnabled(false);
}

void sectionRdgWidget::outputRdgSection(
    const std::string& rdgName, const st_rdgInfoData& rdgInfoData, int trackRdgNumber,  int rdgPixelsInY, int rdgPixelsFnY
)
{
    m_rdgName = rdgName;
    if (m_normActCheckBox->isEnabled() == false)  m_normActCheckBox->setEnabled(true);
    outputLineRdgSection(rdgInfoData, trackRdgNumber, rdgPixelsInY, rdgPixelsFnY);
}

void sectionRdgWidget::outputLineRdgSection(const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY, int rdgPixelsFnY)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        m_trackRdgNumber = trackRdgNumber;
        m_rdgPixelsInY   = rdgPixelsInY;
        m_rdgPixelsFnY   = rdgPixelsFnY;

        createRdgSection(rdgInfoData);
        emit fixTrackIndex(trackRdgNumber);
    }
}

void sectionRdgWidget::incExcRdgNorm()
{
    emit signalFixRdgSection(m_rdgName);
}

void sectionRdgWidget::setShowLogRdg(bool showLogRdg)
{
    m_showLogRdg = showLogRdg;
}
