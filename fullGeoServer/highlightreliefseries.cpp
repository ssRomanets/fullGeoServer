
#include "highlightreliefseries.h"

using namespace QtDataVisualization;

HighlightReliefSeries::HighlightReliefSeries()
{
    setDrawMode(QSurface3DSeries::DrawSurface);
    setFlatShadingEnabled(true);
    setVisible(false);
}

HighlightReliefSeries::~HighlightReliefSeries()
{

}

void HighlightReliefSeries::setReliefSeries(QSurface3DSeries* reliefSeries, int width, int height)
{
    m_reliefSeries = reliefSeries;
    m_width = width;
    m_height = height;

    handlePositionChange();
    handleGradientChange();
}

void HighlightReliefSeries::handlePositionChange()
{
    int startX = 0;
    int endX   = m_width - 1;

    int startZ = 0;
    int endZ   = m_height - 1;

    QSurfaceDataProxy* srcProxy = m_reliefSeries->dataProxy();
    const QSurfaceDataArray &srcArray = *srcProxy->array();

    QSurfaceDataArray* dataArray = new QSurfaceDataArray;
    dataArray->reserve(endZ - startZ);
    for (int i = startZ; i < endZ; i++)
    {
        QSurfaceDataRow* newRow = new QSurfaceDataRow(endX - startX);
        QSurfaceDataRow* srcRow = srcArray.at(i);
        for (int j = startX, p = 0; j < endX; j++, p++) {
            QVector3D pos = srcRow->at(j).position();
            (*newRow)[p].setPosition(QVector3D(pos.x(), pos.y(), pos.z()));
        }
        *dataArray << newRow;
    }

    dataProxy()->resetArray(dataArray);
    setVisible(true);
}

void HighlightReliefSeries::handleGradientChange()
{
    QLinearGradient gr;
    gr.setColorAt(0.0f, Qt::black);
    gr.setColorAt(0.098, Qt::darkGreen);
    gr.setColorAt(0.196, Qt::green);
    gr.setColorAt(0.294, Qt::yellow);
    gr.setColorAt(0.392, Qt::red);
    gr.setColorAt(0.49, Qt::darkRed);

    setBaseGradient(gr);
    setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}
