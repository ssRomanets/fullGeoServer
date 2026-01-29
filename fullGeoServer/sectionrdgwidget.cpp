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

void sectionRdgWidget::outputSectionChart(const QString& trzFileCurrent)
{
   m_contentWidget->createDefaultChartView(m_chart);
   m_chart->setTitle(m_titleSection+" "+trzFileCurrent);
}

void sectionRdgWidget::createRdgSection(const st_rdgInfoData& rdgInfoData)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        m_rdgLineSeries->clear();
        for (int i = m_rdgPixelsInY; i <= m_rdgPixelsFnY; i++)
        {
            if (m_showLogRdg == false)
            {
                if (m_normActCheckBox->checkState() == 0)
                    m_rdgLineSeries->append((rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDoubleData[m_filterId])[i]*(nanokoef*spc/epsdData(m_materialId)), i);
                else
                {
                    m_rdgLineSeries->append(
                        ((rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDoubleData[m_filterId])[i] - rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMinImpulses[m_filterId])/
                         (rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMaxImpulses[m_filterId]     - rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMinImpulses[m_filterId]), i
                    );
                }
            }
            else
            {
                double rdgLog10 = 0.0;
                if (
                    (rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDoubleData[m_filterId])[i] >
                    rdgInfoData.contrastLog10RdgKoeff*(rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMaxImpulses[m_filterId]-rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMinImpulses[m_filterId])
                )
                    rdgLog10 = log10(
                        1 + (rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorsDoubleData[m_filterId])[i]-
                        rdgInfoData.contrastLog10RdgKoeff*(rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMaxImpulses[m_filterId]-rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMinImpulses[m_filterId])
                    );
                else rdgLog10 = 0.0;

                if (m_normActCheckBox->checkState() == 0)  m_rdgLineSeries->append(rdgLog10, i);
                else   m_rdgLineSeries->append(fabs(rdgLog10 - rdgInfoData.vectorMinLog10RdgData[m_filterId])/(rdgInfoData.vectorMaxLog10RdgData[m_filterId] - rdgInfoData.vectorMinLog10RdgData[m_filterId]),i);

            }
        }

        if (m_rdgLineSeries->points().size() > 0)
        {
            if (m_showLogRdg == false)
            {
                if (m_normActCheckBox->checkState() == 0) m_axisX->setTitleText("Яркость сканирования по каждому треку mm");
                else                                      m_axisX->setTitleText("Нормированная яркость сканирования по каждому треку");
            }
            else
            {
                if (m_normActCheckBox->checkState() == 0) m_axisX->setTitleText("Логарифмическая яркость сканирования по каждому треку");
                else                                      m_axisX->setTitleText("Нормированная логарифмическая яркость сканирования по каждому треку");
            }
            m_axisY->setTitleText("импульс радарограммы");

            if (m_chart->series().size()==0)
            {
                m_chart->addSeries(m_rdgLineSeries);
                m_chart->setAxisX(m_axisX, m_rdgLineSeries);
                m_chart->setAxisY(m_axisY, m_rdgLineSeries);
            }
            m_axisY->setReverse(true);
            m_axisY->setRange(m_rdgPixelsInY, m_rdgPixelsFnY);

            if (m_normActCheckBox->checkState() == 0)
            {
                if (m_showLogRdg == false)
                {
                    m_axisX->setRange(
                        rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMinImpulses[m_filterId]*(nanokoef*spc/epsdData(m_materialId)),
                        rdgInfoData.vectorRdgData[m_trackRdgNumber].vectorMaxImpulses[m_filterId]*(nanokoef*spc/epsdData(m_materialId))
                    );
                }
                else m_axisX->setRange( rdgInfoData.vectorMinLog10RdgData[m_filterId], rdgInfoData.vectorMaxLog10RdgData[m_filterId]);
            }
            else     m_axisX->setRange(0, 1);
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
    const std::string& rdgName, const st_rdgInfoData& rdgInfoData, int trackRdgNumber,
    int rdgPixelsInY, int rdgPixelsFnY, int pageRdgDataIndex, int materialId
)
{
    m_rdgName = rdgName;
    m_materialId = materialId;
    if (m_normActCheckBox->isEnabled() == false)  m_normActCheckBox->setEnabled(true);
    if (pageRdgDataIndex == 0)  outputLineRdgSection(rdgInfoData, trackRdgNumber, rdgPixelsInY, rdgPixelsFnY);
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
