#include "tableRdgTrackDataWidget.h"
#include "transformation.h"

#include <iostream>

tableRdgTrackDataWidget::tableRdgTrackDataWidget(QWidget* parent): QWidget(parent)
{
    m_tableRdgTrackDataView = new QTableView(parent);
    m_rdgTrackDataModel = new rdgTrackDataTableModel();

    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_tableRdgTrackDataView);
    setLayout(m_verLayout);
}

tableRdgTrackDataWidget::~tableRdgTrackDataWidget(){}

void tableRdgTrackDataWidget::outputRdgTrackDataTable(const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY, int rdgPixelsFnY)
{
    showRdgTrackDataTable(rdgInfoData, trackRdgNumber, rdgPixelsInY,  rdgPixelsFnY);
}

void tableRdgTrackDataWidget::showRdgTrackDataTable(const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        emit fixTrackIndex(trackRdgNumber);
        showRdgTrackDataMainModel(rdgInfoData, trackRdgNumber, rdgPixelsInY, rdgPixelsFnY);
    }
}

void tableRdgTrackDataWidget::showRdgTrackDataMainModel(const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY)
{
    if (rdgInfoData.vectorRdgData.size() > 0)
    {
        int lastRowCount = 0;
        if (m_rdgTrackDataModel->m_data.size() > 0) lastRowCount = m_rdgTrackDataModel->m_data.size();
        if ( lastRowCount > 0) m_rdgTrackDataModel->m_data.clear();

        m_rdgTrackDataModel->m_showLogRdg = m_showLogRdg;
        m_rdgTrackDataModel->m_columnCount = 5;
        m_rdgTrackDataModel->m_rowCount = rdgPixelsFnY - rdgPixelsInY + 1;

        for (int i = rdgPixelsInY; i <= rdgPixelsFnY; i++) {
            QVector<QString> dataVec;
            dataVec.resize(m_rdgTrackDataModel->m_columnCount);
            dataVec[0] = QString::number(trackRdgNumber);
            dataVec[1] = QString::number(i*rdgInfoData.vectorRdgData[trackRdgNumber].time_step_ns);

            if (m_showLogRdg == false) dataVec[2] = QString::number(rdgMetricKoeff*(rdgInfoData.vectorRdgData[trackRdgNumber].vectorsDoubleData[m_filterId])[i]);
            else
            {
                double rdgLog10 = 0.0;
                if (rdgInfoData.vectorRdgData[trackRdgNumber].vectorsDoubleData[m_filterId][i] > 0.0)
                    rdgLog10 =      log10(1 +     (rdgInfoData.vectorRdgData[trackRdgNumber].vectorsDoubleData[m_filterId])[i]);
                else
                    rdgLog10 = -1.0*log10(1 + fabs((rdgInfoData.vectorRdgData[trackRdgNumber].vectorsDoubleData[m_filterId])[i]));

                dataVec[2] = QString::number(rdgLog10);
            }

            dataVec[3] = QString::number(rdgInfoData.vectorRdgData[trackRdgNumber].latitude_degree);
            dataVec[4] = QString::number(rdgInfoData.vectorRdgData[trackRdgNumber].longitude_degree);
            m_rdgTrackDataModel->m_data.append(dataVec);
        }

        if (m_tableRdgTrackDataView->isHidden())           m_tableRdgTrackDataView->show();
        else if (lastRowCount > 0)
        {
            m_tableRdgTrackDataView->hide();
            m_tableRdgTrackDataView->show();
        }

        m_tableRdgTrackDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_tableRdgTrackDataView->verticalHeader  ()->setSectionResizeMode(QHeaderView::Stretch);

        m_tableRdgTrackDataView->scroll(0, 1);
        m_tableRdgTrackDataView->scroll(0,-1);
        m_tableRdgTrackDataView->setModel(m_rdgTrackDataModel);
    }
}

void tableRdgTrackDataWidget::removeRdgTrackDataTable()
{
    m_tableRdgTrackDataView->hide();
}

void tableRdgTrackDataWidget::setShowLogRdg(bool showLogRdg)
{
    m_showLogRdg = showLogRdg;
}
