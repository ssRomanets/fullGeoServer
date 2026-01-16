#include "structureRdgsGraph.h"
#include "transformation.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/QCustom3DItem>
#include "csvfile.h"
#include <iostream>
#include <math.h>

structureRdgsGraph::structureRdgsGraph(Q3DSurface* surface): m_graph(surface)
{
    m_vectorPositions.resize(2);

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);
    m_graph->setShadowQuality(Q3DSurface::ShadowQualityNone);

    connect(m_graph, &Q3DSurface::selectedElementChanged, this, &structureRdgsGraph::handleElementSelected);

    m_colorMarker = QImage(2, 2, QImage::Format_RGB32);
    m_colorMarker.fill(Qt::red);

    m_colorRdgsSurf1 = QColor(0,0,0);
    m_colorRdgsSurf2 = QColor(255,255,255);

    m_colorMarker = QImage(2, 2, QImage::Format_RGB32);
    m_colorMarker.fill(Qt::red);
}

structureRdgsGraph::~structureRdgsGraph()
{
    if (m_dataRdgsArray != nullptr)
        if (m_dataRdgsArray->size() > 0)
            for(int i = 0; i < m_dataRdgsArray->size(); i++)
                m_dataRdgsArray->at(i)->clear();

    if (m_dataRdgsReliefArray != nullptr)
        if (m_dataRdgsReliefArray->size() > 0)
            for(int i = 0; i < m_dataRdgsReliefArray->size(); i++)
                m_dataRdgsReliefArray->at(i)->clear();

    if (m_dataRdgsLineSectionArray != nullptr)
        if (m_dataRdgsLineSectionArray->size() > 0)
            for(int i = 0; i < m_dataRdgsLineSectionArray->size(); i++)
                m_dataRdgsLineSectionArray->at(i)->clear();
}

void structureRdgsGraph::fillRdgs(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId,  bool highLowOnRdg, bool showFullRdgs, bool resetCutPointsRdgs,
    int materialId, int absRdgsStructInX, int absRdgsStructFnX
)
{
    m_quantImpulsesOfPacket = quantImpulsesOfPacket;
    m_highLowOnRdg = highLowOnRdg;

    if (m_showFullRdgs != showFullRdgs) resetCutPointsRdgs = true;
    m_showFullRdgs = showFullRdgs;

    m_filterId = filterId;
    m_selectionId = selectionId;
    m_materialId = materialId;

    m_leftLatitude  = rdgsWorkData.leftLatitude;
    m_rightLatitude = rdgsWorkData.rightLatitude;
    m_lowLongitude  = rdgsWorkData.lowLongitude;
    m_upLongitude   = rdgsWorkData.upLongitude;

    m_deltaLatitude  = rdgsWorkData.deltaLatitude;
    m_deltaLongitude = rdgsWorkData.deltaLongitude;

    if (m_showFullRdgs == true) m_rdgsSurfWidth  = rdgsWorkData.rdgsSurfWidth;  else m_rdgsSurfWidth  = rdgsWorkData.rdgsSurfMinWidth;
    m_rdgsSurfHeight = rdgsWorkData.rdgsSurfHeight;


    m_absRdgsStructInX = absRdgsStructInX;
    m_absRdgsStructFnX = (absRdgsStructFnX >= m_rdgsSurfWidth-1) ? m_rdgsSurfWidth-1 : absRdgsStructFnX;

    m_maxRelief      = rdgsWorkData.maxRelief;
    m_minRelief      = rdgsWorkData.minRelief;

    defMaxSumImpulses(rdgsInfoDataMap, m_filterId, m_selectionId, m_maxSumImpulses, m_absRdgsStructInX, m_absRdgsStructFnX);
    fillRdgsExecute(rdgsInfoDataMap, rdgsWorkData,  resetCutPointsRdgs);
}

void structureRdgsGraph::fillRdgsExecute(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData, bool resetCutPointsRdgs)
{
    if (m_newRdgsRowVector.size() != m_rdgsSurfHeight)              m_newRdgsRowVector.resize(m_rdgsSurfHeight);
    if (m_newRdgsReliefRowVector.size() != m_rdgsSurfHeight)        m_newRdgsReliefRowVector.resize(m_rdgsSurfHeight);
    if (m_newRdgsLineSectionRowVector.size() != m_rdgsSurfHeight)   m_newRdgsLineSectionRowVector.resize(m_rdgsSurfHeight);

    if (m_graph->seriesList().size() > 0)
    {
        m_inPosFix = false;
        m_fnPosFix = false;

        if (m_rdgsProxy->rowCount()            > 0)
        {
            m_rdgsProxy           ->removeRows(0, m_rdgsProxy->rowCount()-1           );
            m_graph->removeSeries(m_rdgsSeries);
        }
        if (m_rdgsReliefProxy->rowCount()      > 0)
        {
            m_rdgsReliefProxy     ->removeRows(0, m_rdgsReliefProxy->rowCount()-1     );
            m_graph->removeSeries(m_rdgsReliefSeries);
        }
        if (m_rdgsLineSectionProxy->rowCount() > 0)
        {
            m_rdgsLineSectionProxy->removeRows(0, m_rdgsLineSectionProxy->rowCount()-1);
            m_graph->removeSeries(m_rdgsLineSectionSeries);
        }
    }

    if (resetCutPointsRdgs == true)
    {
        m_inCutPosX = m_absRdgsStructInX;
        m_inCutPosY = 1;
        m_fnCutPosX = m_absRdgsStructFnX;
        m_fnCutPosY = m_rdgsSurfHeight - 2;

        sendRdgsSurfInfo();
        m_imageReliefVectorData.resize((m_absRdgsStructFnX- m_absRdgsStructInX + 1) * m_rdgsSurfHeight);
        m_chImageReliefVectorData = false;
    }

    if (fabs(rdgsWorkData.maxRelief - m_minRelief) > deltaParam && m_chImageReliefVectorData == false)
    {
        int maskColor = 0;
        for (int i = m_absRdgsStructInX; i <= m_absRdgsStructFnX; i++)
        {
            for (int j = 0; j < m_rdgsSurfHeight; j++)
            {
                maskColor = (int)(255*( (rdgsWorkData.vectorRdgsFnRelief[i][j] -  m_minRelief)/(m_maxRelief - m_minRelief) ));
                m_imageReliefVectorData[(m_absRdgsStructFnX- m_absRdgsStructInX + 1)*j + i-m_absRdgsStructInX] = maskColor;
            }
        }
        m_chImageReliefVectorData = true;
    }

    defVectorRdgsTransLineSectionPoints(m_inCutPosX, m_inCutPosY, m_fnCutPosX, m_fnCutPosY, m_vectorRdgsTransLineSectionPoints);

    if (m_graph->customItems().size() > 0) m_graph->removeCustomItems();

    m_graph->axisX()->setLabelFormat("%.3f");
    m_graph->axisY()->setLabelFormat("%.3f");
    m_graph->axisZ()->setLabelFormat("%.3f");

    m_graph->axisX()->setRange(m_leftLatitude +m_deltaLatitude*m_absRdgsStructInX, m_leftLatitude + m_deltaLatitude*m_absRdgsStructFnX);
    m_graph->axisY()->setRange(m_minRelief-m_maxSumImpulses*(nanokoef*spc/epsdData(m_materialId)), rdgsWorkData.maxRelief);
    m_graph->axisZ()->setRange(m_lowLongitude, m_lowLongitude + m_deltaLongitude*(m_rdgsSurfHeight-1));

    outputCutInterfaceMarkers();
    fillAllRdgsProxy(rdgsInfoDataMap, rdgsWorkData);
    fillAllRdgsProxy(rdgsInfoDataMap, rdgsWorkData);
}

void structureRdgsGraph::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    saveBoundPointsLinePoints(saveBoundPointsLinePointsFile, m_inCutPosX, m_inCutPosY, m_fnCutPosX, m_fnCutPosY);
}

void structureRdgsGraph::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    loadBoundPointsLinePoints(loadBoundPointsLinePointsFile, m_inCutPosX, m_inCutPosY, m_fnCutPosX, m_fnCutPosY);
    emit signalFixFillRdgs(false);
}

void structureRdgsGraph::fillAllRdgsProxy(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData)
{
    if (m_dataRdgsArray != nullptr)
        if (m_dataRdgsArray->size() > 0)
            for(int i = 0; i < m_dataRdgsArray->size(); i++)
                m_dataRdgsArray->at(i)->clear();
    m_dataRdgsArray = new QSurfaceDataArray;
    m_dataRdgsArray->reserve(m_rdgsSurfHeight);

    if (m_dataRdgsReliefArray != nullptr)
        if (m_dataRdgsReliefArray->size() > 0)
            for(int i = 0; i < m_dataRdgsReliefArray->size(); i++)
                m_dataRdgsReliefArray->at(i)->clear();
    m_dataRdgsReliefArray = new QSurfaceDataArray;
    m_dataRdgsReliefArray->reserve(m_rdgsSurfHeight);

    if (m_dataRdgsLineSectionArray != nullptr)
        if (m_dataRdgsLineSectionArray->size() > 0)
            for(int i = 0; i < m_dataRdgsLineSectionArray->size(); i++)
                m_dataRdgsLineSectionArray->at(i)->clear();
    m_dataRdgsLineSectionArray = new QSurfaceDataArray;
    m_dataRdgsLineSectionArray->reserve(m_rdgsSurfHeight);

    if (fabs(m_maxRelief - m_minRelief) > deltaParam)
    {
        for (int i = 0; i < m_rdgsSurfHeight; i++)
        {
            m_newRdgsReliefRowVector[i] = new QSurfaceDataRow(m_absRdgsStructFnX-m_absRdgsStructInX+1);
            for (int j = m_absRdgsStructInX; j <= m_absRdgsStructFnX; j++)
            {
                (*m_newRdgsReliefRowVector[i])[j-m_absRdgsStructInX].setPosition(QVector3D(
                    m_leftLatitude+j*m_deltaLatitude,  rdgsWorkData.vectorRdgsFnRelief[j][i],  m_lowLongitude+i*m_deltaLongitude
                ));
            }
            *m_dataRdgsReliefArray << m_newRdgsReliefRowVector[i];
        }
        m_rdgsReliefProxy->resetArray(m_dataRdgsReliefArray);
        activateRdgsReliefModel();
    }

    std::string nameRdg;
    int kRdg           ;

    std::pair<int, int> rdgPoint;
    std::vector<std::pair<int, int>>::iterator pointIt;
    std::vector<std::pair<int, int>> vectorRdgsTransLineSectionFinalPoints;

    if  (m_cut3dVisible == true)
    {
        for (int i = 0; i < m_rdgsSurfHeight; i++)
        {
            m_newRdgsRowVector[i]            = new QSurfaceDataRow(m_absRdgsStructFnX-m_absRdgsStructInX+1);
            m_newRdgsLineSectionRowVector[i] = new QSurfaceDataRow(m_absRdgsStructFnX-m_absRdgsStructInX+1);

            for (int j = m_absRdgsStructInX; j <= m_absRdgsStructFnX; j++)
            {

                pointIt = std::find_if(
                    m_vectorRdgsTransLineSectionPoints.begin(), m_vectorRdgsTransLineSectionPoints.end(),
                    [&](const std::pair<int, int>& pair) { return (pair.first == j && pair.second == i); }
                );

                nameRdg = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*i+j]);
                kRdg    = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*i+j]);

                double depthScanOnTrackRdg = 0.0;
                if (m_highLowOnRdg)
                {
                    if (nameRdg != "")
                    {
                        depthScanOnTrackRdg = defDepthScanUpLowRdg(rdgsInfoDataMap, nameRdg, kRdg, m_quantImpulsesOfPacket, m_filterId, m_selectionId)*(nanokoef*spc/epsdData(m_materialId));
                    }
                }
                else
                {
                    if (nameRdg != "")
                    {
                        depthScanOnTrackRdg = defDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg, kRdg, m_quantImpulsesOfPacket, m_filterId, m_selectionId)*(nanokoef*spc/epsdData(m_materialId));
                    }
                }

                if (pointIt != m_vectorRdgsTransLineSectionPoints.end())
                {
                    (*m_newRdgsRowVector[i])[j-m_absRdgsStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        rdgsWorkData.vectorRdgsFnRelief[j][i] - m_maxSumImpulses*(nanokoef*spc/epsdData(m_materialId)),
                        m_lowLongitude+i*m_deltaLongitude
                    ));
                    (*m_newRdgsLineSectionRowVector[i])[j-m_absRdgsStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        rdgsWorkData.vectorRdgsFnRelief[j][i],
                        m_lowLongitude+i*m_deltaLongitude
                    ));

                    rdgPoint.first = j;
                    rdgPoint.second = i;
                    vectorRdgsTransLineSectionFinalPoints.push_back(rdgPoint);
                } else {
                    if (m_highLowOnRdg)
                    {
                        (*m_newRdgsRowVector[i])[j-m_absRdgsStructInX].setPosition(QVector3D(
                            m_leftLatitude+j*m_deltaLatitude,
                            rdgsWorkData.vectorRdgsFnRelief[j][i] - depthScanOnTrackRdg,
                            m_lowLongitude+i*m_deltaLongitude
                        ));
                    }
                    else
                    {
                        double fullDepthScanOnTrackRdg = 0.0;
                        if (nameRdg == "") fullDepthScanOnTrackRdg = m_maxSumImpulses*(nanokoef*spc/epsdData(m_materialId));
                        else               fullDepthScanOnTrackRdg = defFullDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg, kRdg, m_filterId, m_selectionId)*(nanokoef*spc/epsdData(m_materialId));

                        (*m_newRdgsRowVector[i])[j-m_absRdgsStructInX].setPosition(QVector3D(
                            m_leftLatitude+j*m_deltaLatitude,
                            rdgsWorkData.vectorRdgsFnRelief[j][i] - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg),
                            m_lowLongitude+i*m_deltaLongitude
                        ));
                    }

                    (*m_newRdgsLineSectionRowVector[i])[j-m_absRdgsStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        rdgsWorkData.vectorRdgsFnRelief[j][i] - m_maxSumImpulses*(nanokoef*spc/epsdData(m_materialId)),
                        m_lowLongitude+i*m_deltaLongitude
                    ));
                }
            }

            *m_dataRdgsArray << m_newRdgsRowVector[i];
            *m_dataRdgsLineSectionArray << m_newRdgsLineSectionRowVector[i];
        }
        m_rdgsProxy->resetArray(m_dataRdgsArray);
        m_rdgsLineSectionProxy->resetArray(m_dataRdgsLineSectionArray);

        activateRdgsModel();
        activateRdgsSectionLineModel();

        emit updateRdgsTransGlWidget(rdgsInfoDataMap, rdgsWorkData, vectorRdgsTransLineSectionFinalPoints, m_highLowOnRdg);
    }
    else
    {
        for (int i = 0; i < m_rdgsSurfHeight; i++)
        {
            m_newRdgsRowVector[i]            = new QSurfaceDataRow(m_absRdgsStructFnX-m_absRdgsStructInX+1);

            for (int j = m_absRdgsStructInX; j <= m_absRdgsStructFnX; j++)
            {
                nameRdg         = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*i+j]);
                kRdg            = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*i+j]);

                double depthScanOnTrackRdg = 0.0;
                if (m_highLowOnRdg)
                {
                    if (nameRdg != "")
                    {
                        depthScanOnTrackRdg = defDepthScanUpLowRdg(rdgsInfoDataMap, nameRdg, kRdg, m_quantImpulsesOfPacket, m_filterId, m_selectionId)*(nanokoef*spc/epsdData(m_materialId));
                    }
                }
                else
                {
                    if (nameRdg != "")
                    {
                        depthScanOnTrackRdg = defDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg, kRdg, m_quantImpulsesOfPacket, m_filterId, m_selectionId)*(nanokoef*spc/epsdData(m_materialId));
                    }
                }


                if (m_highLowOnRdg)
                {
                    (*m_newRdgsRowVector[i])[j-m_absRdgsStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        rdgsWorkData.vectorRdgsFnRelief[j][i] - depthScanOnTrackRdg,
                        m_lowLongitude+i*m_deltaLongitude
                    ));
                }
                else
                {
                    double fullDepthScanOnTrackRdg = 0.0;
                    if (nameRdg == "")
                        fullDepthScanOnTrackRdg = m_maxSumImpulses*(nanokoef*spc/epsdData(m_materialId));
                    else
                        fullDepthScanOnTrackRdg =
                        defFullDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg, kRdg, m_filterId, m_selectionId)*(nanokoef*spc/epsdData(m_materialId));

                    (*m_newRdgsRowVector[i])[j-m_absRdgsStructInX].setPosition(QVector3D(
                        m_leftLatitude+j*m_deltaLatitude,
                        rdgsWorkData.vectorRdgsFnRelief[j][i] - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg),
                        m_lowLongitude+i*m_deltaLongitude
                    ));
                }
            }
            *m_dataRdgsArray << m_newRdgsRowVector[i];
        }
        m_rdgsProxy->resetArray(m_dataRdgsArray);

        activateRdgsModel();
        emit signalClearRdgsTransGlWidget();
    }
}

void structureRdgsGraph::activateRdgsReliefModel()
{
    m_rdgsReliefSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_rdgsReliefSeries->setFlatShadingEnabled(true);

    QImage::Format format = QImage::Format_Grayscale8;
    QImage rawImage(m_imageReliefVectorData.data(), m_absRdgsStructFnX-m_absRdgsStructInX+1, m_rdgsSurfHeight, format);
    m_rdgsReliefSeries->setTexture(rawImage);
    m_rdgsReliefSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    m_graph->addSeries(m_rdgsReliefSeries);
}

void structureRdgsGraph::activateRdgsModel()
{
    m_rdgsSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_rdgsSeries->setFlatShadingEnabled(true);

    QLinearGradient gr;
    gr.setColorAt(1.0, m_colorRdgsSurf1);
    gr.setColorAt(0.0, m_colorRdgsSurf2);
    m_rdgsSeries->setBaseGradient(gr);
    m_rdgsSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    m_graph->addSeries(m_rdgsSeries);
}

void structureRdgsGraph::activateRdgsSectionLineModel()
{
    m_rdgsLineSectionSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_rdgsLineSectionSeries->setFlatShadingEnabled(true);

    QLinearGradient gr;
    gr.setColorAt(1.0, QColor(255,0,0));
    gr.setColorAt(0.0, QColor(255,0,0));
    m_rdgsLineSectionSeries->setBaseGradient(gr);
    m_rdgsLineSectionSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    m_graph->addSeries(m_rdgsLineSectionSeries);
}

void structureRdgsGraph::handleElementSelected(QAbstract3DGraph::ElementType type)
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
                        m_inCutPosX = m_absRdgsStructInX+point.y();
                        m_inCutPosY = point.x();
                        addMarker(m_inCutPosX, m_inCutPosY, 0);
                    }
                    else if (m_fnPosFix == true)
                    {
                        m_fnCutPosX = m_absRdgsStructInX+point.y();
                        m_fnCutPosY = point.x();
                        addMarker(m_fnCutPosX, m_fnCutPosY, 1);

                    }
                    sendRdgsSurfInfo();

                    defVectorRdgsTransLineSectionPoints(m_inCutPosX, m_inCutPosY, m_fnCutPosX, m_fnCutPosY, m_vectorRdgsTransLineSectionPoints);
                    currentClear3dProxy();
                    emit signalFixFillRdgs(false);

                    m_inPosFix = false;
                    m_fnPosFix = false;
                }
            } else {
                double fixLatitude  = m_leftLatitude + (m_absRdgsStructInX+point.y())*m_deltaLatitude;
                double fixLongitude = m_lowLongitude + point.x()*m_deltaLongitude;

                emit showCorrectRdgsSurfWidget(m_colorRdgsSurf1, m_colorRdgsSurf2, fixLatitude, fixLongitude);
            }
        }
    }
}

void structureRdgsGraph::receiveColorsRdgsSurf(QColor& colorSurfRdgs1, QColor& colorSurfRdgs2)
{
    m_colorRdgsSurf1 = colorSurfRdgs1;
    m_colorRdgsSurf2 = colorSurfRdgs2;
    currentClear3dProxy();
    emit signalFixFillRdgs(false);
}

void structureRdgsGraph::slotCloseCut3dAction(bool cut3dVisible)
{
    if (m_cut3dVisible == true)
    {
        if (m_graph->customItems().size() > 0) m_graph->removeCustomItems();
    }
    else
    {
        outputCutInterfaceMarkers();
        emit closeCorrectRdgsSurfWidget();
    }
    currentClear3dProxy();
    m_cut3dVisible = cut3dVisible;
    emit signalFixFillRdgs(false);
}

void structureRdgsGraph::addMarker(int x, int y, int vecPosId)
{
    QVector3D position = QVector3D(
        m_leftLatitude + x*m_deltaLatitude,
        (m_minRelief-0.45f * m_maxSumImpulses*(nanokoef*spc/epsdData(m_materialId))),
        m_lowLongitude + y*m_deltaLongitude
    );
    QCustom3DItem *item = new QCustom3DItem(":/pipe.obj", position, QVector3D(0.005f, 1.2f, 0.005f), QQuaternion(), m_colorMarker);
    m_graph->addCustomItem(item);
    m_vectorPositions[vecPosId] = position;
}

void structureRdgsGraph::outputCutInterfaceMarkers()
{
    if (m_cut3dVisible == true)
    {
        addMarker(m_inCutPosX, m_inCutPosY, 0);
        addMarker(m_fnCutPosX, m_fnCutPosY, 1);
    }
}

void structureRdgsGraph::sendRdgsSurfInfo()
{
    QString outRdgsSurfInfo =
    QString::fromStdString("in cut Latitude ")   + QString::number(m_leftLatitude + m_inCutPosX*m_deltaLatitude) +
    QString::fromStdString(" in cut Longitude ") + QString::number(m_lowLongitude + m_inCutPosY*m_deltaLongitude) +
    QString::fromStdString(" fn cut Latitude ")  + QString::number(m_leftLatitude + m_fnCutPosX*m_deltaLatitude) +
    QString::fromStdString(" fn cut Longitude ") + QString::number(m_lowLongitude + m_fnCutPosY*m_deltaLongitude) +
    QString::fromStdString(".");
    emit sendOutRdgsSurfInfo(outRdgsSurfInfo);
}

void structureRdgsGraph::currentClear3dProxy()
{
    if (m_cut3dVisible == true)
    {
        m_rdgsLineSectionProxy->removeRows(0, m_rdgsSurfHeight-1);
        m_graph->removeSeries(m_rdgsLineSectionSeries);
    }

    m_rdgsProxy->removeRows(0, m_rdgsSurfHeight-1);
    m_graph->removeSeries(m_rdgsSeries);
    if (fabs(m_maxRelief - m_minRelief) > deltaParam)
    {
        m_rdgsReliefProxy     ->removeRows(0, m_rdgsSurfHeight     );
        m_graph->removeSeries(m_rdgsReliefSeries);
    }
}
