#include "tableDeepRdgTrackDataWidget.h"
#include "transformation.h"

tableDeepRdgTrackDataWidget::tableDeepRdgTrackDataWidget(QWidget *parent):QWidget(parent)
{
    m_tableDeepRdgTrackDataView = new QTableView(parent);
    m_deepRdgTrackDataModel = new deepRdgTrackDataTableModel();

    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_tableDeepRdgTrackDataView);
    setLayout(m_verLayout);
}

tableDeepRdgTrackDataWidget::~tableDeepRdgTrackDataWidget(){}

void tableDeepRdgTrackDataWidget::outputDeepRdgTrackDataTable(
    const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY, int rdgPixelsFnY,
    int filterId, int materialId, int selectionId
)
{
    showDeepRdgTrackDataTable(rdgInfoData, trackRdgNumber, rdgPixelsInY,  rdgPixelsFnY, filterId, materialId, selectionId);
}

void tableDeepRdgTrackDataWidget::showDeepRdgTrackDataTable(
    const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY,
    int filterId, int materialId, int selectionId
)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        emit fixTrackIndex(trackRdgNumber);
        showDeepRdgTrackDataMainModel(rdgInfoData, trackRdgNumber, rdgPixelsInY, rdgPixelsFnY, filterId, materialId, selectionId);
    }
}

void tableDeepRdgTrackDataWidget::showDeepRdgTrackDataMainModel(
    const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY,
    int filterId, int materialId, int selectionId
)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        int lastRowCount = 0;
        if (m_deepRdgTrackDataModel->m_data.size() > 0) lastRowCount = m_deepRdgTrackDataModel->m_data.size();
        if ( lastRowCount > 0) m_deepRdgTrackDataModel->m_data.clear();

        m_deepRdgTrackDataModel->m_columnCount = 5;
        m_deepRdgTrackDataModel->m_rowCount = rdgPixelsFnY - rdgPixelsInY + 1;

        for (int i = rdgPixelsInY; i <= rdgPixelsFnY; i++) {
            QVector<QString> dataVec;
            dataVec.resize(m_deepRdgTrackDataModel->m_columnCount);
            dataVec[0] = QString::number(trackRdgNumber);
            dataVec[1] = QString::number(i*rdgInfoData.vectorRdgData[trackRdgNumber].time_step_ns);
            dataVec[2] = QString::number(rdgInfoData.vectorRdgData[trackRdgNumber].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][i]);
            dataVec[3] = QString::number(rdgInfoData.vectorRdgData[trackRdgNumber].latitude_degree);
            dataVec[4] = QString::number(rdgInfoData.vectorRdgData[trackRdgNumber].longitude_degree);
            m_deepRdgTrackDataModel->m_data.append(dataVec);
        }

        if (m_tableDeepRdgTrackDataView->isHidden())           m_tableDeepRdgTrackDataView->show();
        else if (lastRowCount > 0)
        {
            m_tableDeepRdgTrackDataView->hide();
            m_tableDeepRdgTrackDataView->show();
        }

        m_tableDeepRdgTrackDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_tableDeepRdgTrackDataView->verticalHeader  ()->setSectionResizeMode(QHeaderView::Stretch);

        m_tableDeepRdgTrackDataView->scroll(0, 1);
        m_tableDeepRdgTrackDataView->scroll(0,-1);
        m_tableDeepRdgTrackDataView->setModel(m_deepRdgTrackDataModel);
    }
}

void tableDeepRdgTrackDataWidget::removeDeepRdgTrackDataTable()
{
    m_tableDeepRdgTrackDataView->hide();
}
