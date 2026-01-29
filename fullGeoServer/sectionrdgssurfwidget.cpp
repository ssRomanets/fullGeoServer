#include "sectionRdgsSurfWidget.h"
#include "rdgsData.h"

#include <QChart>
#include <QLineSeries>
#include <iostream>
#include <math.h>

#include "transformation.h"

sectionRdgsSurfWidget::sectionRdgsSurfWidget(QWidget* parent): QWidget(parent)
{
    m_chart = new QChart;
    m_rdgsSurfLineSeries = new QLineSeries();

    m_contentWidget = new contentWidget(parent);

    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_contentWidget);
    setLayout(m_verLayout);
}

sectionRdgsSurfWidget::~sectionRdgsSurfWidget(){}

void sectionRdgsSurfWidget::outputSectionsChart()
{
    m_contentWidget->createDefaultChartView(m_chart);
    m_chart->setTitle("Сечение по отсканированной поверхности");
}

void sectionRdgsSurfWidget::setRdgsSurfSectionMap(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, int quantImpulsesOfPacket, int filterId, int selectionId,
    bool highLowOnRdg, int materialId, int absRdgsStructInX, int absRdgsStructFnX
)
{
    m_materialId = materialId;
    m_highLowOnRdg  = highLowOnRdg;
    m_quantImpulsesOfPacket = quantImpulsesOfPacket;
    m_filterId = filterId;
    m_selectionId = selectionId;
    defMaxDeep(rdgsInfoDataMap, materialId, filterId, selectionId, m_maxDeep, absRdgsStructInX, absRdgsStructFnX);
}

void sectionRdgsSurfWidget::outputRdgsSurfSection(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData)
{
    if (rdgsInfoDataMap.size() > 0)
    {
        m_chart->removeSeries(m_rdgsSurfLineSeries);
        m_chart->removeAxis(m_chart->axisX());
        m_chart->removeAxis(m_chart->axisY());
        m_rdgsSurfLineSeries->clear();

        int length = (int)(sqrt( (m_fnPosX-m_inPosX)*(m_fnPosX-m_inPosX) + (m_fnPosY-m_inPosY)*(m_fnPosY-m_inPosY) ) );

        double x{0.0};
        double y{0.0}, depth{0.0};

        int x1{0}, x2{0}, x3{0}, x4{0};
        int y1{0}, y2{0}, y3{0}, y4{0};

        std::vector<double> vectorSectRdgsCoordXY;
        vectorSectRdgsCoordXY.resize(0);

        for (int count = 0; count < length; count++)
        {
            if (m_inPosY != m_fnPosY)
            {
                x = m_inPosX + count*(m_fnPosX - m_inPosX)/(length-1);
                y = m_inPosY + count*(m_fnPosY - m_inPosY)/(length-1);

                x1 = (int)x;
                y1 = (int)y;

                x2 = x1;
                y2 = y1+1;

                x3 = x1+1;
                y3 = y1+1;

                x4 = x1+1;
                y4 = y1;


                if ((x1 <= rdgsWorkData.rdgsSurfWidth-1  && x2 <= rdgsWorkData.rdgsSurfWidth-1  &&
                     x3 <= rdgsWorkData.rdgsSurfWidth-1  && x4 <= rdgsWorkData.rdgsSurfWidth-1) &&
                    (y1 <= rdgsWorkData.rdgsSurfHeight-1 && y2 <= rdgsWorkData.rdgsSurfHeight-1 &&
                     y3 <= rdgsWorkData.rdgsSurfHeight-1 && y4 <= rdgsWorkData.rdgsSurfHeight-1))
                {
                    std::string nameRdg1 = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y1+x1]);
                    int kRdg1            = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y1+x1]);
                    std::string nameRdg2 = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y2+x2]);
                    int kRdg2            = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y2+x2]);
                    std::string nameRdg3 = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y3+x3]);
                    int kRdg3            = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y3+x3]);
                    std::string nameRdg4 = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y4+x4]);
                    int kRdg4            = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y4+x4]);

                    double depth1        = 0.0;
                    double depth2        = 0.0;
                    double depth3        = 0.0;
                    double depth4        = 0.0;

                    if (m_highLowOnRdg)
                    {
                        if (nameRdg1 != "") depth1 = defDepthScanUpLowRdg(rdgsInfoDataMap, nameRdg1, kRdg1, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg2 != "") depth2 = defDepthScanUpLowRdg(rdgsInfoDataMap, nameRdg2, kRdg2, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg3 != "") depth3 = defDepthScanUpLowRdg(rdgsInfoDataMap, nameRdg3, kRdg3, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg4 != "") depth4 = defDepthScanUpLowRdg(rdgsInfoDataMap, nameRdg4, kRdg4, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);

                        depth =(
                            fabs((x3-x)*(y3-y))*(rdgsWorkData.vectorRdgsFnRelief[x1][y1] - depth1) +
                            fabs((x1-x)*(y1-y))*(rdgsWorkData.vectorRdgsFnRelief[x3][y3] - depth3) +
                            fabs((x2-x)*(y2-y))*(rdgsWorkData.vectorRdgsFnRelief[x4][y4] - depth4) +
                            fabs((x4-x)*(y4-y))*(rdgsWorkData.vectorRdgsFnRelief[x2][y2] - depth2)
                        );
                    }
                    else
                    {
                        if (nameRdg1 != "") depth1 = defDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg1, kRdg1, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg2 != "") depth2 = defDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg2, kRdg2, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg3 != "") depth3 = defDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg3, kRdg3, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg4 != "") depth4 = defDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg4, kRdg4, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);

                        double fullDepth1 = m_maxDeep;
                        double fullDepth2 = m_maxDeep;
                        double fullDepth3 = m_maxDeep;
                        double fullDepth4 = m_maxDeep;

                        if (nameRdg1 != "") fullDepth1 = defFullDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg1, kRdg1, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg2 != "") fullDepth2 = defFullDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg2, kRdg2, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg3 != "") fullDepth3 = defFullDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg3, kRdg3, m_materialId, m_filterId, m_selectionId);
                        if (nameRdg4 != "") fullDepth4 = defFullDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg4, kRdg4, m_materialId, m_filterId, m_selectionId);

                        depth =(
                            fabs((x3-x)*(y3-y))*(rdgsWorkData.vectorRdgsFnRelief[x1][y1] - (fullDepth1 - depth1)) +
                            fabs((x1-x)*(y1-y))*(rdgsWorkData.vectorRdgsFnRelief[x3][y3] - (fullDepth3 - depth3)) +
                            fabs((x2-x)*(y2-y))*(rdgsWorkData.vectorRdgsFnRelief[x4][y4] - (fullDepth4 - depth4)) +
                            fabs((x4-x)*(y4-y))*(rdgsWorkData.vectorRdgsFnRelief[x2][y2] - (fullDepth2 - depth2))
                        );
                    }

                    vectorSectRdgsCoordXY.push_back(count);
                    vectorSectRdgsCoordXY.push_back(depth);
                }
            }
            else
            {
                x = m_inPosX + count*(m_fnPosX - m_inPosX)/(length-1);
                y = m_inPosY;
                x1 = (int)x;
                y1 = (int)y;
                std::string nameRdg1 = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y1+x1]);
                int kRdg1            = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*y1+x1]);
                double depth1  = 0.0;
                if (m_highLowOnRdg)
                {
                    if (nameRdg1 != "") depth1 = defDepthScanUpLowRdg(rdgsInfoDataMap, nameRdg1, kRdg1, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                    depth = rdgsWorkData.vectorRdgsFnRelief[x1][y1] - depth1;
                }
                else
                {
                    if (nameRdg1 != "") depth1     = defDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg1, kRdg1, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                    double fullDepth1 = m_maxDeep;
                    if (nameRdg1 != "") fullDepth1 = defFullDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg1, kRdg1, m_materialId, m_filterId, m_selectionId);
                    depth = rdgsWorkData.vectorRdgsFnRelief[x1][y1] - (fullDepth1 - depth1);
                }
                vectorSectRdgsCoordXY.push_back(count);
                vectorSectRdgsCoordXY.push_back(depth);
            }
        }

        for (int count = 0; count < vectorSectRdgsCoordXY.size()/2; count++)
        {
            if (count >= 2)
            {
                if (fabs(vectorSectRdgsCoordXY[2*count+1]-vectorSectRdgsCoordXY[2*(count-1)+1]) < deltaParam)
                {
                    vectorSectRdgsCoordXY[2*(count-1)+1] = 0.5*(vectorSectRdgsCoordXY[2*count+1] + vectorSectRdgsCoordXY[2*(count-2)+1]);
                }
            }
        }

        if (m_highLowOnRdg)
        {
            if (vectorSectRdgsCoordXY.size()/2 >= 5)
            {
                int min1Count = 0;
                int min2Count = 0;

                for (int count1 = 1; count1 < vectorSectRdgsCoordXY.size()/2 - 1 ; count1++)
                {
                    if (min1Count != 0 && min2Count == 0)
                    {
                        if ((vectorSectRdgsCoordXY[2*count1+1] <= vectorSectRdgsCoordXY[2*(count1-1)+1]) &&
                            (vectorSectRdgsCoordXY[2*count1+1] <= vectorSectRdgsCoordXY[2*(count1+1)+1]))
                        {
                            min2Count = count1;
                        }
                    }

                    if (min1Count == 0)
                    {
                        if ((vectorSectRdgsCoordXY[2*count1+1] <= vectorSectRdgsCoordXY[2*(count1-1)+1]) &&
                            (vectorSectRdgsCoordXY[2*count1+1] <= vectorSectRdgsCoordXY[2*(count1+1)+1]))
                        {
                            min1Count = count1;
                        }
                    }

                    if (min1Count != 0 && min2Count != 0)
                    {
                        if (min2Count > min1Count+1)
                        {
                            for (int count2 = min1Count + 1; count2 <= min2Count - 1; count2++)
                            {
                                vectorSectRdgsCoordXY[2*count2+1] = vectorSectRdgsCoordXY[2*min1Count+1] +
                                (vectorSectRdgsCoordXY[2*count2] - vectorSectRdgsCoordXY[2*min1Count])*
                                (vectorSectRdgsCoordXY[2*min2Count+1] - vectorSectRdgsCoordXY[2*min1Count+1])/
                                (vectorSectRdgsCoordXY[2*min2Count] - vectorSectRdgsCoordXY[2*min1Count]);
                            }
                        }
                        min1Count = min2Count;
                        min2Count = 0;
                    }
                }
            }
        }
        else
        {
            if (vectorSectRdgsCoordXY.size()/2 >= 5)
            {
                int max1Count = 0;
                int max2Count = 0;

                for (int count1 = 1; count1 < vectorSectRdgsCoordXY.size()/2 - 1 ; count1++)
                {
                    if (max1Count != 0 && max2Count == 0)
                    {
                        if ((vectorSectRdgsCoordXY[2*count1+1] >= vectorSectRdgsCoordXY[2*(count1-1)+1]) &&
                            (vectorSectRdgsCoordXY[2*count1+1] >= vectorSectRdgsCoordXY[2*(count1+1)+1]))
                        {
                            max2Count = count1;
                        }
                    }

                    if (max1Count == 0)
                    {
                        if ((vectorSectRdgsCoordXY[2*count1+1] >= vectorSectRdgsCoordXY[2*(count1-1)+1]) &&
                            (vectorSectRdgsCoordXY[2*count1+1] >= vectorSectRdgsCoordXY[2*(count1+1)+1]))
                        {
                            max1Count = count1;
                        }
                    }

                    if (max1Count != 0 && max2Count != 0)
                    {
                        if (max2Count > max1Count+1)
                        {
                            for (int count2 = max1Count + 1; count2 <= max2Count - 1; count2++)
                            {
                                vectorSectRdgsCoordXY[2*count2+1] = vectorSectRdgsCoordXY[2*max1Count+1] +
                                (vectorSectRdgsCoordXY[2*count2] - vectorSectRdgsCoordXY[2*max1Count])*
                                (vectorSectRdgsCoordXY[2*max2Count+1] - vectorSectRdgsCoordXY[2*max1Count+1])/
                                (vectorSectRdgsCoordXY[2*max2Count] - vectorSectRdgsCoordXY[2*max1Count]);
                            }
                        }
                        max1Count = max2Count;
                        max2Count = 0;
                    }
                }
            }
        }

        for (int count = 0; count < vectorSectRdgsCoordXY.size()/2; count++)
        {
            m_rdgsSurfLineSeries->append(vectorSectRdgsCoordXY[2*count], vectorSectRdgsCoordXY[2*count+1]);
        }

        QValueAxis *axisX = new QValueAxis;
        QValueAxis *axisY = new QValueAxis;

        axisY->setTitleText("Глубина сканирования mm");
        axisX->setTitleText("номер точки линии");

        m_chart->addSeries(m_rdgsSurfLineSeries);
        m_chart->setAxisX(axisX, m_rdgsSurfLineSeries);
        m_chart->setAxisY(axisY, m_rdgsSurfLineSeries);
        m_chart->legend()->hide();
    }
}

void sectionRdgsSurfWidget::incExcRdgsSurfNorm()
{
     emit signalFixRdgsSurfSection();
}

void sectionRdgsSurfWidget::updateRdgsSurfSections( int inPosX, int inPosY, int fnPosX, int fnPosY)
{
    m_inPosX = inPosX;
    m_inPosY = inPosY;

    m_fnPosX = fnPosX;
    m_fnPosY = fnPosY;
    emit signalFixRdgsSurfSection();
}

void sectionRdgsSurfWidget::hideRdgsSurfSections()
{
    m_chart->removeAxis(m_chart->axisX());
    m_chart->removeAxis(m_chart->axisY());
    m_chart->setTitle("");
    m_chart->removeSeries(m_rdgsSurfLineSeries);
    m_rdgsSurfLineSeries->clear();
}
