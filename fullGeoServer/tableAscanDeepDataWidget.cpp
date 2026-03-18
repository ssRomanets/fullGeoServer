#include "tableAscanDeepDataWidget.h"
#include "transformation.h"

tableAscanDeepDataWidget::tableAscanDeepDataWidget(QWidget *parent):QWidget(parent)
{
    m_tableAscanDeepDataView = new QTableView(parent);
    m_ascanDeepDataModel = new ascanDeepDataTableModel();

    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_tableAscanDeepDataView);
    setLayout(m_verLayout);
}

tableAscanDeepDataWidget::~tableAscanDeepDataWidget(){}

void tableAscanDeepDataWidget::outputAscanDeepDataTable(
    const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY, int pixelsFnY, int filterId, int materialId, int selectionId
)
{
    showAscanDeepDataTable(bscanInfoData, trackNumber, pixelsInY,  pixelsFnY, filterId, materialId, selectionId);
}

void tableAscanDeepDataWidget::showAscanDeepDataTable(
    const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY,  int pixelsFnY, int filterId, int materialId, int selectionId
)
{
    if (bscanInfoData.vectorBscanData.size() > 0)
    {
        emit fixTrackIndex(trackNumber);
        showAscanDeepDataMainModel(bscanInfoData, trackNumber, pixelsInY, pixelsFnY, filterId, materialId, selectionId);
    }
}

void tableAscanDeepDataWidget::showAscanDeepDataMainModel(
    const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY,  int pixelsFnY, int filterId, int materialId, int selectionId
)
{
    if (bscanInfoData.vectorBscanData.size() > 0)
    {
        int lastRowCount = 0;
        if (m_ascanDeepDataModel->m_data.size() > 0) lastRowCount = m_ascanDeepDataModel->m_data.size();
        if ( lastRowCount > 0) m_ascanDeepDataModel->m_data.clear();

        m_ascanDeepDataModel->m_columnCount = 6;
        m_ascanDeepDataModel->m_rowCount = pixelsFnY - pixelsInY + 1;

        for (int i = pixelsInY; i <= pixelsFnY; i++) {
            QVector<QString> dataVec;
            dataVec.resize(m_ascanDeepDataModel->m_columnCount);
            dataVec[0] = QString::number(trackNumber);
            dataVec[1] = QString::number(i);
            dataVec[2] = QString::number(i*bscanInfoData.vectorBscanData[trackNumber].time_step_ns);
            dataVec[3] = QString::number(bscanInfoData.vectorBscanData[trackNumber].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][i]);
            dataVec[4] = QString::number(bscanInfoData.vectorBscanData[trackNumber].latitude_degree);
            dataVec[5] = QString::number(bscanInfoData.vectorBscanData[trackNumber].longitude_degree);
            m_ascanDeepDataModel->m_data.append(dataVec);
        }

        if (m_tableAscanDeepDataView->isHidden())           m_tableAscanDeepDataView->show();
        else if (lastRowCount > 0)
        {
            m_tableAscanDeepDataView->hide();
            m_tableAscanDeepDataView->show();
        }

        m_tableAscanDeepDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_tableAscanDeepDataView->verticalHeader  ()->setSectionResizeMode(QHeaderView::Stretch);

        m_tableAscanDeepDataView->scroll(0, 1);
        m_tableAscanDeepDataView->scroll(0,-1);
        m_tableAscanDeepDataView->setModel(m_ascanDeepDataModel);
    }
}

void tableAscanDeepDataWidget::removeAscanDeepDataTable()
{
    m_tableAscanDeepDataView->hide();
}
