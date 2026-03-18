#include "structureBscansGraph.h"
#include "transformation.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/QCustom3DItem>
#include "csvfile.h"
#include <iostream>
#include <math.h>

structureBscansGraph::structureBscansGraph(Q3DSurface* surface): m_graph(surface)
{
    m_vectorPositions.resize(2);

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->setShadowQuality(Q3DSurface::ShadowQualityNone);

    connect(m_graph, &Q3DSurface::selectedElementChanged, this, &structureBscansGraph::handleElementSelected);

    m_colorMarker = QImage(2, 2, QImage::Format_RGB32);
    m_colorMarker.fill(Qt::red);

    m_colorBscansSurf1 = QColor(0,0,0);
    m_colorBscansSurf2 = QColor(255,255,255);

    m_colorMarker = QImage(2, 2, QImage::Format_RGB32);
    m_colorMarker.fill(Qt::red);
}

structureBscansGraph::~structureBscansGraph()
{
    if (m_dataBscansArray != nullptr)       if (m_dataBscansArray->size() > 0)
        for(int i = 0; i < m_dataBscansArray->size(); i++)        m_dataBscansArray->at(i)->clear();

    if (m_dataBscansReliefArray != nullptr) if (m_dataBscansReliefArray->size() > 0)
        for(int i = 0; i < m_dataBscansReliefArray->size(); i++)  m_dataBscansReliefArray->at(i)->clear();

    if (m_dataBscansLineSectionArray != nullptr) if (m_dataBscansLineSectionArray->size() > 0)
        for(int i = 0; i < m_dataBscansLineSectionArray->size(); i++)  m_dataBscansLineSectionArray->at(i)->clear();
}

void structureBscansGraph::fillBscans(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId,  bool highLowOnBscan, bool showFullBscans, bool resetCutPointsBscans,
    int materialId, int absBscansStructInX, int absBscansStructFnX
)
{
    m_quantImpulsesOfPacket = quantImpulsesOfPacket;
    m_highLowOnBscan = highLowOnBscan;

    if (m_showFullBscans != showFullBscans) resetCutPointsBscans = true;
    m_showFullBscans = showFullBscans;

    m_materialId = materialId;
    m_filterId = filterId;
    m_selectionId = selectionId;

    m_leftLatitude  = bscansWorkData.leftLatitude;
    m_rightLatitude = bscansWorkData.rightLatitude;
    m_lowLongitude  = bscansWorkData.lowLongitude;
    m_upLongitude   = bscansWorkData.upLongitude;

    m_deltaLatitude  = bscansWorkData.deltaLatitude;
    m_deltaLongitude = bscansWorkData.deltaLongitude;

    if (m_showFullBscans == true) m_bscansSurfWidth  = bscansWorkData.bscansSurfWidth;
    else                         m_bscansSurfWidth   = bscansWorkData.bscansSurfMinWidth;

    m_bscansSurfHeight = bscansWorkData.bscansSurfHeight;

    m_absBscansStructInX = absBscansStructInX;
    m_absBscansStructFnX = (absBscansStructFnX >= m_bscansSurfWidth-1) ? m_bscansSurfWidth-1 : absBscansStructFnX;

    m_maxRelief      = bscansWorkData.maxRelief;
    m_minRelief      = bscansWorkData.minRelief;

    defMaxDeep(bscansInfoDataMap, m_materialId, m_filterId, m_selectionId, m_maxDeep, m_absBscansStructInX, m_absBscansStructFnX);
    fillBscansExecute(bscansInfoDataMap, bscansWorkData,  resetCutPointsBscans);
}

void structureBscansGraph::fillBscansExecute(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData, bool resetCutPointsBscans
)
{
    if (m_newBscansRowVector.size() != m_bscansSurfHeight)              m_newBscansRowVector.resize(m_bscansSurfHeight);
    if (m_newBscansReliefRowVector.size() != m_bscansSurfHeight)        m_newBscansReliefRowVector.resize(m_bscansSurfHeight);
    if (m_newBscansLineSectionRowVector.size() != m_bscansSurfHeight)   m_newBscansLineSectionRowVector.resize(m_bscansSurfHeight);

    if (m_graph->seriesList().size() > 0)
    {
        m_inPosFix = false;
        m_fnPosFix = false;

        if (m_bscansProxy->rowCount()            > 0)
        {
            m_bscansProxy           ->removeRows(0, m_bscansProxy->rowCount()-1           );
            m_graph->removeSeries(m_bscansSeries);
        }
        if (m_bscansReliefProxy->rowCount()      > 0)
        {
            m_bscansReliefProxy     ->removeRows(0, m_bscansReliefProxy->rowCount()-1     );
            m_graph->removeSeries(m_bscansReliefSeries);
        }
        if (m_bscansLineSectionProxy->rowCount() > 0)
        {
            m_bscansLineSectionProxy->removeRows(0, m_bscansLineSectionProxy->rowCount()-1);
            m_graph->removeSeries(m_bscansLineSectionSeries);
        }
    }

    if (resetCutPointsBscans == true)
    {
        m_inCutPosX = m_absBscansStructInX;
        m_inCutPosY = 1;
        m_fnCutPosX = m_absBscansStructFnX;
        m_fnCutPosY = m_bscansSurfHeight - 2;

        sendBscansSurfInfo();
        m_imageReliefVectorData.resize((m_absBscansStructFnX- m_absBscansStructInX + 1) * m_bscansSurfHeight);
        m_chImageReliefVectorData = false;
    }

    if (fabs(m_maxRelief - m_minRelief) > deltaParam && m_chImageReliefVectorData == false)
    {
        int maskColor = 0;
        for (int i = m_absBscansStructInX; i <= m_absBscansStructFnX; i++)
        {
            for (int j = 0; j < m_bscansSurfHeight; j++)
            {
                maskColor = (int)(255*( (bscansWorkData.vectorBscansFnRelief[i][j] -  m_minRelief)/(m_maxRelief - m_minRelief) ));
                m_imageReliefVectorData[(m_absBscansStructFnX- m_absBscansStructInX + 1)*j + i-m_absBscansStructInX] = maskColor;
            }
        }
        m_chImageReliefVectorData = true;
    }

    defVectorBscansTransLineSectionPoints(m_inCutPosX, m_inCutPosY, m_fnCutPosX, m_fnCutPosY, m_vectorBscansTransLineSectionPoints);

    if (m_graph->customItems().size() > 0) m_graph->removeCustomItems();

    m_graph->axisX()->setLabelFormat("%.3f");
    m_graph->axisY()->setLabelFormat("%.3f");
    m_graph->axisZ()->setLabelFormat("%.3f");

    m_graph->axisX()->setRange(m_leftLatitude +m_deltaLatitude*m_absBscansStructInX, m_leftLatitude + m_deltaLatitude*m_absBscansStructFnX);
    m_graph->axisY()->setRange(m_minRelief-m_maxDeep, bscansWorkData.maxRelief);
    m_graph->axisZ()->setRange(m_lowLongitude, m_lowLongitude + m_deltaLongitude*(m_bscansSurfHeight-1));

    outputCutInterfaceMarkers();
    fillAllBscansProxy(bscansInfoDataMap, bscansWorkData);
    fillAllBscansProxy(bscansInfoDataMap, bscansWorkData);
}

void structureBscansGraph::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    saveBoundPointsLinePoints(saveBoundPointsLinePointsFile, m_inCutPosX, m_inCutPosY, m_fnCutPosX, m_fnCutPosY);
}

void structureBscansGraph::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    loadBoundPointsLinePoints(loadBoundPointsLinePointsFile, m_inCutPosX, m_inCutPosY, m_fnCutPosX, m_fnCutPosY);
    emit signalFixFillBscans(false);
}

void structureBscansGraph::fillAllBscansProxy(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData
)
{
    if (m_dataBscansArray != nullptr)
        if (m_dataBscansArray->size() > 0)
            for(int i = 0; i < m_dataBscansArray->size(); i++)
                m_dataBscansArray->at(i)->clear();
    m_dataBscansArray = new QSurfaceDataArray;
    m_dataBscansArray->reserve(m_bscansSurfHeight);

    if (m_dataBscansReliefArray != nullptr)
        if (m_dataBscansReliefArray->size() > 0)
            for(int i = 0; i < m_dataBscansReliefArray->size(); i++)
                m_dataBscansReliefArray->at(i)->clear();
    m_dataBscansReliefArray = new QSurfaceDataArray;
    m_dataBscansReliefArray->reserve(m_bscansSurfHeight);

    if (m_dataBscansLineSectionArray != nullptr)
        if (m_dataBscansLineSectionArray->size() > 0)
            for(int i = 0; i < m_dataBscansLineSectionArray->size(); i++)
                m_dataBscansLineSectionArray->at(i)->clear();
    m_dataBscansLineSectionArray = new QSurfaceDataArray;
    m_dataBscansLineSectionArray->reserve(m_bscansSurfHeight);

    if (fabs(m_maxRelief - m_minRelief) > deltaParam)
    {
        for (int i = 0; i < m_bscansSurfHeight; i++)
        {
            m_newBscansReliefRowVector[i] = new QSurfaceDataRow(m_absBscansStructFnX-m_absBscansStructInX+1);
            for (int j = m_absBscansStructInX; j <= m_absBscansStructFnX; j++)
            {
                (*m_newBscansReliefRowVector[i])[j-m_absBscansStructInX].setPosition(QVector3D(
                    m_leftLatitude+j*m_deltaLatitude,  bscansWorkData.vectorBscansFnRelief[j][i],  m_lowLongitude+i*m_deltaLongitude
                ));
            }
            *m_dataBscansReliefArray << m_newBscansReliefRowVector[i];
        }
        m_bscansReliefProxy->resetArray(m_dataBscansReliefArray);
        activateBscansReliefModel();
    }

    std::string nameBscan;
    int kBscan           ;

    std::pair<int, int> bscanPoint;
    std::vector<std::pair<int, int>>::iterator pointIt;
    std::vector<std::pair<int, int>> vectorBscansTransLineSectionFinalPoints;

    if  (m_cut3dVisible == true)
    {
        for (int i = 0; i < m_bscansSurfHeight; i++)
        {
            m_newBscansRowVector[i]            = new QSurfaceDataRow(m_absBscansStructFnX-m_absBscansStructInX+1);
            m_newBscansLineSectionRowVector[i] = new QSurfaceDataRow(m_absBscansStructFnX-m_absBscansStructInX+1);

            for (int j = m_absBscansStructInX; j <= m_absBscansStructFnX; j++)
            {

                pointIt = std::find_if(
                    m_vectorBscansTransLineSectionPoints.begin(), m_vectorBscansTransLineSectionPoints.end(),
                    [&](const std::pair<int, int>& pair) { return (pair.first == j && pair.second == i); }
                );

                nameBscan = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*i+j]);
                kBscan    = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*i+j]);

                double depthScanOnTrackBscan = 0.0;
                if (m_highLowOnBscan)
                {
                    if (nameBscan != "")
                    {
                        depthScanOnTrackBscan = defDepthScanUpLowBscan(bscansInfoDataMap, nameBscan, kBscan, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                    }
                }
                else
                {
                    if (nameBscan != "")
                    {
                        depthScanOnTrackBscan = defDepthScanLowUpBscan(bscansInfoDataMap, nameBscan, kBscan, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                    }
                }

                if (pointIt != m_vectorBscansTransLineSectionPoints.end())
                {
                    (*m_newBscansRowVector[i])[j-m_absBscansStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        bscansWorkData.vectorBscansFnRelief[j][i] - m_maxDeep,
                        m_lowLongitude+i*m_deltaLongitude
                    ));
                    (*m_newBscansLineSectionRowVector[i])[j-m_absBscansStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        bscansWorkData.vectorBscansFnRelief[j][i],
                        m_lowLongitude+i*m_deltaLongitude
                    ));

                    bscanPoint.first = j;
                    bscanPoint.second = i;
                    vectorBscansTransLineSectionFinalPoints.push_back(bscanPoint);
                } else {
                    if (m_highLowOnBscan)
                    {
                        (*m_newBscansRowVector[i])[j-m_absBscansStructInX].setPosition(QVector3D(
                            m_leftLatitude+j*m_deltaLatitude,
                            bscansWorkData.vectorBscansFnRelief[j][i] - depthScanOnTrackBscan,
                            m_lowLongitude+i*m_deltaLongitude
                        ));
                    }
                    else
                    {
                        double fullDepthScanOnTrackBscan = 0.0;
                        if (nameBscan == "") fullDepthScanOnTrackBscan = m_maxDeep;
                        else                 fullDepthScanOnTrackBscan = defFullDepthScanLowUpBscan(bscansInfoDataMap, nameBscan, kBscan, m_materialId, m_filterId, m_selectionId);

                        (*m_newBscansRowVector[i])[j-m_absBscansStructInX].setPosition(QVector3D(
                            m_leftLatitude+j*m_deltaLatitude,
                            bscansWorkData.vectorBscansFnRelief[j][i] - (fullDepthScanOnTrackBscan - depthScanOnTrackBscan),
                            m_lowLongitude+i*m_deltaLongitude
                        ));
                    }

                    (*m_newBscansLineSectionRowVector[i])[j-m_absBscansStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        bscansWorkData.vectorBscansFnRelief[j][i] - m_maxDeep,
                        m_lowLongitude+i*m_deltaLongitude
                    ));
                }
            }

            *m_dataBscansArray << m_newBscansRowVector[i];
            *m_dataBscansLineSectionArray << m_newBscansLineSectionRowVector[i];
        }
        m_bscansProxy->resetArray(m_dataBscansArray);
        m_bscansLineSectionProxy->resetArray(m_dataBscansLineSectionArray);

        activateBscansModel();
        activateBscansSectionLineModel();

        emit updateBscansTransGlWidget(bscansInfoDataMap, bscansWorkData, vectorBscansTransLineSectionFinalPoints, m_highLowOnBscan);
    }
    else
    {
        for (int i = 0; i < m_bscansSurfHeight; i++)
        {
            m_newBscansRowVector[i]            = new QSurfaceDataRow(m_absBscansStructFnX-m_absBscansStructInX+1);

            for (int j = m_absBscansStructInX; j <= m_absBscansStructFnX; j++)
            {
                nameBscan         = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*i+j]);
                kBscan            = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*i+j]);

                double depthScanOnTrackBscan = 0.0;
                if (m_highLowOnBscan)
                {
                    if (nameBscan != "")
                    {
                        depthScanOnTrackBscan = defDepthScanUpLowBscan(bscansInfoDataMap, nameBscan, kBscan, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                    }
                }
                else
                {
                    if (nameBscan != "")
                    {
                        depthScanOnTrackBscan = defDepthScanLowUpBscan(bscansInfoDataMap, nameBscan, kBscan, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                    }
                }


                if (m_highLowOnBscan)
                {
                    (*m_newBscansRowVector[i])[j-m_absBscansStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        bscansWorkData.vectorBscansFnRelief[j][i] - depthScanOnTrackBscan,
                        m_lowLongitude+i*m_deltaLongitude
                    ));
                }
                else
                {
                    double fullDepthScanOnTrackBscan = 0.0;
                    if (nameBscan == "")
                        fullDepthScanOnTrackBscan = m_maxDeep;
                    else
                        fullDepthScanOnTrackBscan =
                        defFullDepthScanLowUpBscan(bscansInfoDataMap, nameBscan, kBscan, m_materialId, m_filterId, m_selectionId);

                    (*m_newBscansRowVector[i])[j-m_absBscansStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        bscansWorkData.vectorBscansFnRelief[j][i] - (fullDepthScanOnTrackBscan - depthScanOnTrackBscan),
                        m_lowLongitude+i*m_deltaLongitude
                    ));
                }
            }
            *m_dataBscansArray << m_newBscansRowVector[i];
        }
        m_bscansProxy->resetArray(m_dataBscansArray);

        activateBscansModel();
        emit signalClearBscansTransGlWidget();
    }
}

void structureBscansGraph::activateBscansReliefModel()
{
    m_bscansReliefSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_bscansReliefSeries->setFlatShadingEnabled(true);

    QImage::Format format = QImage::Format_Grayscale8;
    QImage rawImage(m_imageReliefVectorData.data(), m_absBscansStructFnX-m_absBscansStructInX+1, m_bscansSurfHeight, format);
    m_bscansReliefSeries->setTexture(rawImage);
    m_bscansReliefSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    m_graph->addSeries(m_bscansReliefSeries);
}

void structureBscansGraph::activateBscansModel()
{
    m_bscansSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_bscansSeries->setFlatShadingEnabled(true);

    QLinearGradient gr;
    gr.setColorAt(1.0, m_colorBscansSurf1);
    gr.setColorAt(0.0, m_colorBscansSurf2);
    m_bscansSeries->setBaseGradient(gr);
    m_bscansSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    m_graph->addSeries(m_bscansSeries);
}

void structureBscansGraph::activateBscansSectionLineModel()
{
    m_bscansLineSectionSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_bscansLineSectionSeries->setFlatShadingEnabled(true);

    QLinearGradient gr;
    gr.setColorAt(1.0, QColor(255,0,0));
    gr.setColorAt(0.0, QColor(255,0,0));
    m_bscansLineSectionSeries->setBaseGradient(gr);
    m_bscansLineSectionSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    m_graph->addSeries(m_bscansLineSectionSeries);
}

void structureBscansGraph::handleElementSelected(QAbstract3DGraph::ElementType type)
{
    if (type == QAbstract3DGraph::ElementSeries)
    {
        QSurface3DSeries* series = m_graph->selectedSeries();
        if (series)
        {
            QPoint point = series->selectedPoint();
            if (m_cut3dVisible == true)
            {
                if ((m_inPosFix == false) && (m_fnPosFix == false))
                {
                    if ( (m_inCutPosX - point.y())*(m_inCutPosX - point.y()) + (m_inCutPosY - point.x())*(m_inCutPosY - point.x()) <
                         (m_fnCutPosX - point.y())*(m_fnCutPosX - point.y()) + (m_fnCutPosY - point.x())*(m_fnCutPosY - point.x()))
                    {
                        m_inPosFix = true;
                        m_graph->removeCustomItemAt(m_vectorPositions[0]);
                    }
                    else
                    {
                        m_fnPosFix = true;
                        m_graph->removeCustomItemAt(m_vectorPositions[1]);
                    }
                }
                else
                {
                    if (m_inPosFix == true)
                    {
                        m_inCutPosX = m_absBscansStructInX+point.y();
                        m_inCutPosY = point.x();
                        addMarker(m_inCutPosX, m_inCutPosY, 0);
                    }
                    else if (m_fnPosFix == true)
                    {
                        m_fnCutPosX = m_absBscansStructInX+point.y();
                        m_fnCutPosY = point.x();
                        addMarker(m_fnCutPosX, m_fnCutPosY, 1);

                    }
                    sendBscansSurfInfo();

                    defVectorBscansTransLineSectionPoints(m_inCutPosX, m_inCutPosY, m_fnCutPosX, m_fnCutPosY, m_vectorBscansTransLineSectionPoints);
                    currentClear3dProxy();
                    emit signalFixFillBscans(false);

                    m_inPosFix = false;
                    m_fnPosFix = false;
                }
            } else {
                double fixLatitude  = m_leftLatitude + (m_absBscansStructInX+point.y())*m_deltaLatitude;
                double fixLongitude = m_lowLongitude + point.x()*m_deltaLongitude;

                emit showCorrectBscansSurfWidget(m_colorBscansSurf1, m_colorBscansSurf2, fixLatitude, fixLongitude);
            }
        }
    }
}

void structureBscansGraph::receiveColorsBscansSurf(QColor& colorSurfBscans1, QColor& colorSurfBscans2)
{
    m_colorBscansSurf1 = colorSurfBscans1;
    m_colorBscansSurf2 = colorSurfBscans2;
    currentClear3dProxy();
    emit signalFixFillBscans(false);
}

void structureBscansGraph::slotCloseCut3dAction(bool cut3dVisible)
{
    if (m_cut3dVisible == true)
    {
        if (m_graph->customItems().size() > 0) m_graph->removeCustomItems();
    }
    else
    {
        outputCutInterfaceMarkers();
        emit closeCorrectBscansSurfWidget();
    }
    currentClear3dProxy();
    m_cut3dVisible = cut3dVisible;
    emit signalFixFillBscans(false);
}

void structureBscansGraph::addMarker(int x, int y, int vecPosId)
{
    QVector3D position = QVector3D(
        m_leftLatitude + x*m_deltaLatitude,
        (m_minRelief-0.45f * m_maxDeep),
        m_lowLongitude + y*m_deltaLongitude
    );
    QCustom3DItem *item = new QCustom3DItem(":/pipe.obj", position, QVector3D(0.005f, 1.2f, 0.005f), QQuaternion(), m_colorMarker);
    m_graph->addCustomItem(item);
    m_vectorPositions[vecPosId] = position;
}

void structureBscansGraph::outputCutInterfaceMarkers()
{
    if (m_cut3dVisible == true)
    {
        addMarker(m_inCutPosX, m_inCutPosY, 0);
        addMarker(m_fnCutPosX, m_fnCutPosY, 1);
    }
}

void structureBscansGraph::sendBscansSurfInfo()
{
    QString outBscansSurfInfo =
    QString::fromStdString("in cut Latitude ")   + QString::number(m_leftLatitude + m_inCutPosX*m_deltaLatitude) +
    QString::fromStdString(" in cut Longitude ") + QString::number(m_lowLongitude + m_inCutPosY*m_deltaLongitude) +
    QString::fromStdString(" fn cut Latitude ")  + QString::number(m_leftLatitude + m_fnCutPosX*m_deltaLatitude) +
    QString::fromStdString(" fn cut Longitude ") + QString::number(m_lowLongitude + m_fnCutPosY*m_deltaLongitude) +
    QString::fromStdString(".");
    emit sendOutBscansSurfInfo(outBscansSurfInfo);
}

void structureBscansGraph::currentClear3dProxy()
{
    if (m_cut3dVisible == true)
    {
        m_bscansLineSectionProxy->removeRows(0, m_bscansSurfHeight-1);
        m_graph->removeSeries(m_bscansLineSectionSeries);
    }

    m_bscansProxy->removeRows(0, m_bscansSurfHeight-1);
    m_graph->removeSeries(m_bscansSeries);
    if (fabs(m_maxRelief - m_minRelief) > deltaParam)
    {
        m_bscansReliefProxy     ->removeRows(0, m_bscansSurfHeight     );
        m_graph->removeSeries(m_bscansReliefSeries);
    }
}
