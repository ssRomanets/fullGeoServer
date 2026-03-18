#include "tableAscanDataWidget.h"
#include "transformation.h"

#include <iostream>

tableAscanDataWidget::tableAscanDataWidget(QWidget* parent): QWidget(parent)
{
    m_tableAscanDataView = new QTableView(parent);
    m_ascanDataModel     = new ascanDataTableModel();

    m_verLayout = new QVBoxLayout;
    m_verLayout->addWidget(m_tableAscanDataView);
    setLayout(m_verLayout);
}

tableAscanDataWidget::~tableAscanDataWidget(){}

void tableAscanDataWidget::outputAscanDataTable(const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY, int pixelsFnY)
{
    showAscanDataTable(bscanInfoData, trackNumber, pixelsInY, pixelsFnY);
}

void tableAscanDataWidget::showAscanDataTable(const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY,  int pixelsFnY)
{
    if (bscanInfoData.vectorBscanData.size() > 0)
    {
        emit fixTrackIndex(trackNumber);
        showAscanDataMainModel(bscanInfoData, trackNumber, pixelsInY, pixelsFnY);
    }
}

void tableAscanDataWidget::showAscanDataMainModel(const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY, int pixelsFnY)
{
    if (bscanInfoData.vectorBscanData.size() > 0)
    {
        int lastRowCount = 0;
        if (m_ascanDataModel->m_data.size() > 0) lastRowCount = m_ascanDataModel->m_data.size();
        if ( lastRowCount > 0) m_ascanDataModel->m_data.clear();

        m_ascanDataModel->m_showLogBscan = m_showLog;
        m_ascanDataModel->m_columnCount = 6;
        m_ascanDataModel->m_rowCount = pixelsFnY - pixelsInY + 1;

        for (int i = pixelsInY; i <= pixelsFnY; i++) {
            QVector<QString> dataVec;
            dataVec.resize(m_ascanDataModel->m_columnCount);
            dataVec[0] = QString::number(trackNumber);
            dataVec[1] = QString::number(i);
            dataVec[2] = QString::number(i*bscanInfoData.vectorBscanData[trackNumber].time_step_ns);

            if (m_showLog == false) dataVec[3] = QString::number((bscanInfoData.vectorBscanData[trackNumber].vectorsDoubleData[m_filterId])[i]);
            else
            {
                double bscanLog10 = 0.0;
                if (bscanInfoData.vectorBscanData[trackNumber].vectorsDoubleData[m_filterId][i] > 0.0)
                    bscanLog10 =      log10(1 + (bscanInfoData.vectorBscanData[trackNumber].vectorsDoubleData[m_filterId])[i]);
                else
                    bscanLog10 = -1.0*log10(1 + fabs((bscanInfoData.vectorBscanData[trackNumber].vectorsDoubleData[m_filterId])[i]));

                dataVec[3] = QString::number(bscanLog10);
            }

            dataVec[4] = QString::number(bscanInfoData.vectorBscanData[trackNumber].latitude_degree);
            dataVec[5] = QString::number(bscanInfoData.vectorBscanData[trackNumber].longitude_degree);
            m_ascanDataModel->m_data.append(dataVec);
        }

        if (m_tableAscanDataView->isHidden())           m_tableAscanDataView->show();
        else if (lastRowCount > 0)
        {
            m_tableAscanDataView->hide();
            m_tableAscanDataView->show();
        }

        m_tableAscanDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_tableAscanDataView->verticalHeader  ()->setSectionResizeMode(QHeaderView::Stretch);

        m_tableAscanDataView->scroll(0, 1);
        m_tableAscanDataView->scroll(0,-1);
        m_tableAscanDataView->setModel(m_ascanDataModel);
    }
}

void tableAscanDataWidget::removeAscanDataTable() {m_tableAscanDataView->hide(); }
void tableAscanDataWidget::setShowLog(bool showLog) { m_showLog = showLog;}
