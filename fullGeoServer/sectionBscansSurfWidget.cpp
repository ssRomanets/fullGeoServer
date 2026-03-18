#include "sectionBscansSurfWidget.h"
#include "bscansData.h"

#include <QChart>
#include <QLineSeries>
#include <iostream>
#include <math.h>

#include "transformation.h"

sectionBscansSurfWidget::sectionBscansSurfWidget(QWidget* parent): QWidget(parent)
{
    m_chart = new QChart;
    m_bscansSurfLineSeries = new QLineSeries();

    m_contentWidget = new contentWidget(parent);

    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_contentWidget);
    setLayout(m_verLayout);
}

sectionBscansSurfWidget::~sectionBscansSurfWidget(){}

void sectionBscansSurfWidget::outputSectionsChart()
{
    m_contentWidget->createDefaultChartView(m_chart);
    m_chart->setTitle("Сечение по отсканированной поверхности");
}

void sectionBscansSurfWidget::setBscansSurfSectionMap(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, int quantImpulsesOfPacket, int filterId, int selectionId,
    bool highLowOnBscan, int materialId, int absBscansStructInX, int absBscansStructFnX
)
{
    m_materialId = materialId;
    m_highLowOnBscan  = highLowOnBscan;
    m_quantImpulsesOfPacket = quantImpulsesOfPacket;
    m_filterId = filterId;
    m_selectionId = selectionId;
    defMaxDeep(bscansInfoDataMap, materialId, filterId, selectionId, m_maxDeep, absBscansStructInX, absBscansStructFnX);
}

void sectionBscansSurfWidget::outputBscansSurfSection(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData
)
{
    if (bscansInfoDataMap.size() > 0)
    {
        m_chart->removeSeries(m_bscansSurfLineSeries);
        m_chart->removeAxis(m_chart->axisX());
        m_chart->removeAxis(m_chart->axisY());
        m_bscansSurfLineSeries->clear();

        int length = (int)(sqrt( (m_fnPosX-m_inPosX)*(m_fnPosX-m_inPosX) + (m_fnPosY-m_inPosY)*(m_fnPosY-m_inPosY) ) );

        double x{0.0};
        double y{0.0}, depth{0.0};

        int x1{0}, x2{0}, x3{0}, x4{0};
        int y1{0}, y2{0}, y3{0}, y4{0};

        std::vector<double> vectorSectBscansCoordXY;
        vectorSectBscansCoordXY.resize(0);

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


                if ((x1 <= bscansWorkData.bscansSurfWidth-1  && x2 <= bscansWorkData.bscansSurfWidth-1  &&
                     x3 <= bscansWorkData.bscansSurfWidth-1  && x4 <= bscansWorkData.bscansSurfWidth-1) &&
                    (y1 <= bscansWorkData.bscansSurfHeight-1 && y2 <= bscansWorkData.bscansSurfHeight-1 &&
                     y3 <= bscansWorkData.bscansSurfHeight-1 && y4 <= bscansWorkData.bscansSurfHeight-1))
                {
                    std::string nameBscan1 = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y1+x1]);
                    int kBscan1            = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y1+x1]);
                    std::string nameBscan2 = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y2+x2]);
                    int kBscan2            = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y2+x2]);
                    std::string nameBscan3 = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y3+x3]);
                    int kBscan3            = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y3+x3]);
                    std::string nameBscan4 = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y4+x4]);
                    int kBscan4            = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y4+x4]);

                    double depth1        = 0.0;
                    double depth2        = 0.0;
                    double depth3        = 0.0;
                    double depth4        = 0.0;

                    if (m_highLowOnBscan)
                    {
                        if (nameBscan1 != "") depth1 = defDepthScanUpLowBscan(bscansInfoDataMap, nameBscan1, kBscan1, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan2 != "") depth2 = defDepthScanUpLowBscan(bscansInfoDataMap, nameBscan2, kBscan2, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan3 != "") depth3 = defDepthScanUpLowBscan(bscansInfoDataMap, nameBscan3, kBscan3, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan4 != "") depth4 = defDepthScanUpLowBscan(bscansInfoDataMap, nameBscan4, kBscan4, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);

                        depth =(
                            fabs((x3-x)*(y3-y))*(bscansWorkData.vectorBscansFnRelief[x1][y1] - depth1) +
                            fabs((x1-x)*(y1-y))*(bscansWorkData.vectorBscansFnRelief[x3][y3] - depth3) +
                            fabs((x2-x)*(y2-y))*(bscansWorkData.vectorBscansFnRelief[x4][y4] - depth4) +
                            fabs((x4-x)*(y4-y))*(bscansWorkData.vectorBscansFnRelief[x2][y2] - depth2)
                        );
                    }
                    else
                    {
                        if (nameBscan1 != "") depth1 = defDepthScanLowUpBscan(bscansInfoDataMap, nameBscan1, kBscan1, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan2 != "") depth2 = defDepthScanLowUpBscan(bscansInfoDataMap, nameBscan2, kBscan2, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan3 != "") depth3 = defDepthScanLowUpBscan(bscansInfoDataMap, nameBscan3, kBscan3, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan4 != "") depth4 = defDepthScanLowUpBscan(bscansInfoDataMap, nameBscan4, kBscan4, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);

                        double fullDepth1 = m_maxDeep;
                        double fullDepth2 = m_maxDeep;
                        double fullDepth3 = m_maxDeep;
                        double fullDepth4 = m_maxDeep;

                        if (nameBscan1 != "") fullDepth1 = defFullDepthScanLowUpBscan(bscansInfoDataMap, nameBscan1, kBscan1, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan2 != "") fullDepth2 = defFullDepthScanLowUpBscan(bscansInfoDataMap, nameBscan2, kBscan2, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan3 != "") fullDepth3 = defFullDepthScanLowUpBscan(bscansInfoDataMap, nameBscan3, kBscan3, m_materialId, m_filterId, m_selectionId);
                        if (nameBscan4 != "") fullDepth4 = defFullDepthScanLowUpBscan(bscansInfoDataMap, nameBscan4, kBscan4, m_materialId, m_filterId, m_selectionId);

                        depth =(
                            fabs((x3-x)*(y3-y))*(bscansWorkData.vectorBscansFnRelief[x1][y1] - (fullDepth1 - depth1)) +
                            fabs((x1-x)*(y1-y))*(bscansWorkData.vectorBscansFnRelief[x3][y3] - (fullDepth3 - depth3)) +
                            fabs((x2-x)*(y2-y))*(bscansWorkData.vectorBscansFnRelief[x4][y4] - (fullDepth4 - depth4)) +
                            fabs((x4-x)*(y4-y))*(bscansWorkData.vectorBscansFnRelief[x2][y2] - (fullDepth2 - depth2))
                        );
                    }

                    vectorSectBscansCoordXY.push_back(count);
                    vectorSectBscansCoordXY.push_back(depth);
                }
            }
            else
            {
                x = m_inPosX + count*(m_fnPosX - m_inPosX)/(length-1);
                y = m_inPosY;
                x1 = (int)x;
                y1 = (int)y;
                std::string nameBscan1 = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y1+x1]);
                int kBscan1            = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*y1+x1]);
                double depth1  = 0.0;
                if (m_highLowOnBscan)
                {
                    if (nameBscan1 != "") depth1 = defDepthScanUpLowBscan(bscansInfoDataMap, nameBscan1, kBscan1, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                    depth = bscansWorkData.vectorBscansFnRelief[x1][y1] - depth1;
                }
                else
                {
                    if (nameBscan1 != "") depth1     = defDepthScanLowUpBscan(bscansInfoDataMap, nameBscan1, kBscan1, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                    double fullDepth1 = m_maxDeep;
                    if (nameBscan1 != "") fullDepth1 = defFullDepthScanLowUpBscan(bscansInfoDataMap, nameBscan1, kBscan1, m_materialId, m_filterId, m_selectionId);
                    depth = bscansWorkData.vectorBscansFnRelief[x1][y1] - (fullDepth1 - depth1);
                }
                vectorSectBscansCoordXY.push_back(count);
                vectorSectBscansCoordXY.push_back(depth);
            }
        }

        for (int count = 0; count < vectorSectBscansCoordXY.size()/2; count++)
        {
            if (count >= 2)
            {
                if (fabs(vectorSectBscansCoordXY[2*count+1]-vectorSectBscansCoordXY[2*(count-1)+1]) < deltaParam)
                {
                    vectorSectBscansCoordXY[2*(count-1)+1] = 0.5*(vectorSectBscansCoordXY[2*count+1] + vectorSectBscansCoordXY[2*(count-2)+1]);
                }
            }
        }

        if (m_highLowOnBscan)
        {
            if (vectorSectBscansCoordXY.size()/2 >= 5)
            {
                int min1Count = 0;
                int min2Count = 0;

                for (int count1 = 1; count1 < vectorSectBscansCoordXY.size()/2 - 1 ; count1++)
                {
                    if (min1Count != 0 && min2Count == 0)
                    {
                        if ((vectorSectBscansCoordXY[2*count1+1] <= vectorSectBscansCoordXY[2*(count1-1)+1]) &&
                            (vectorSectBscansCoordXY[2*count1+1] <= vectorSectBscansCoordXY[2*(count1+1)+1]))
                        {
                            min2Count = count1;
                        }
                    }

                    if (min1Count == 0)
                    {
                        if ((vectorSectBscansCoordXY[2*count1+1] <= vectorSectBscansCoordXY[2*(count1-1)+1]) &&
                            (vectorSectBscansCoordXY[2*count1+1] <= vectorSectBscansCoordXY[2*(count1+1)+1]))
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
                                vectorSectBscansCoordXY[2*count2+1] = vectorSectBscansCoordXY[2*min1Count+1] +
                                (vectorSectBscansCoordXY[2*count2] - vectorSectBscansCoordXY[2*min1Count])*
                                (vectorSectBscansCoordXY[2*min2Count+1] - vectorSectBscansCoordXY[2*min1Count+1])/
                                (vectorSectBscansCoordXY[2*min2Count] - vectorSectBscansCoordXY[2*min1Count]);
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
            if (vectorSectBscansCoordXY.size()/2 >= 5)
            {
                int max1Count = 0;
                int max2Count = 0;

                for (int count1 = 1; count1 < vectorSectBscansCoordXY.size()/2 - 1 ; count1++)
                {
                    if (max1Count != 0 && max2Count == 0)
                    {
                        if ((vectorSectBscansCoordXY[2*count1+1] >= vectorSectBscansCoordXY[2*(count1-1)+1]) &&
                            (vectorSectBscansCoordXY[2*count1+1] >= vectorSectBscansCoordXY[2*(count1+1)+1]))
                        {
                            max2Count = count1;
                        }
                    }

                    if (max1Count == 0)
                    {
                        if ((vectorSectBscansCoordXY[2*count1+1] >= vectorSectBscansCoordXY[2*(count1-1)+1]) &&
                            (vectorSectBscansCoordXY[2*count1+1] >= vectorSectBscansCoordXY[2*(count1+1)+1]))
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
                                vectorSectBscansCoordXY[2*count2+1] = vectorSectBscansCoordXY[2*max1Count+1] +
                                (vectorSectBscansCoordXY[2*count2] - vectorSectBscansCoordXY[2*max1Count])*
                                (vectorSectBscansCoordXY[2*max2Count+1] - vectorSectBscansCoordXY[2*max1Count+1])/
                                (vectorSectBscansCoordXY[2*max2Count] - vectorSectBscansCoordXY[2*max1Count]);
                            }
                        }
                        max1Count = max2Count;
                        max2Count = 0;
                    }
                }
            }
        }

        for (int count = 0; count < vectorSectBscansCoordXY.size()/2; count++)
        {
            m_bscansSurfLineSeries->append(vectorSectBscansCoordXY[2*count], vectorSectBscansCoordXY[2*count+1]);
        }

        QValueAxis *axisX = new QValueAxis;
        QValueAxis *axisY = new QValueAxis;

        axisY->setTitleText("Глубина сканирования mm");
        axisX->setTitleText("номер точки линии");

        m_chart->addSeries(m_bscansSurfLineSeries);
        m_chart->setAxisX(axisX, m_bscansSurfLineSeries);
        m_chart->setAxisY(axisY, m_bscansSurfLineSeries);
        m_chart->legend()->hide();
    }
}

void sectionBscansSurfWidget::incExcBscansSurfNorm()
{
     emit signalFixBscansSurfSection();
}

void sectionBscansSurfWidget::updateBscansSurfSections( int inPosX, int inPosY, int fnPosX, int fnPosY)
{
    m_inPosX = inPosX;
    m_inPosY = inPosY;

    m_fnPosX = fnPosX;
    m_fnPosY = fnPosY;
    emit signalFixBscansSurfSection();
}

void sectionBscansSurfWidget::hideBscansSurfSections()
{
    m_chart->removeAxis(m_chart->axisX());
    m_chart->removeAxis(m_chart->axisY());
    m_chart->setTitle("");
    m_chart->removeSeries(m_bscansSurfLineSeries);
    m_bscansSurfLineSeries->clear();
}
