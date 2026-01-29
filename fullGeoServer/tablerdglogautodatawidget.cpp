#include "tableRdgLogAutoDataWidget.h"
#include "transformation.h"

#include <iostream>

tableRdgLogAutoDataWidget::tableRdgLogAutoDataWidget(QWidget* parent): QWidget(parent)
{
   m_tableRdgLogAutoDataView = new QTableView(parent);
   m_rdgLogAutoDataModel = new rdgSelectedDataTableModel();

   m_verLayout = new QVBoxLayout;
   m_verLayout->addWidget(m_tableRdgLogAutoDataView);
   setLayout(m_verLayout);

   m_tableRdgLogAutoDataView->hide();
}

tableRdgLogAutoDataWidget::~tableRdgLogAutoDataWidget(){}

void tableRdgLogAutoDataWidget::outputRdgLogAutoDataTable(int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, const st_rdgInfoData& rdgInfoData)
{
   showRdgLogAutoDataTable(rdgPixelsInX, rdgPixelsInY, rdgPixelsFnX,  rdgPixelsFnY, rdgInfoData);
}

void tableRdgLogAutoDataWidget::showRdgLogAutoDataTable(int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY,const st_rdgInfoData& rdgInfoData)
{
   if (rdgInfoData.vectorRdgData.size() > 0)
       showRdgLogAutoDataMainModel(rdgPixelsInX, rdgPixelsInY, rdgPixelsFnX,  rdgPixelsFnY, rdgInfoData);
}

void tableRdgLogAutoDataWidget::showRdgLogAutoDataMainModel(int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, const st_rdgInfoData& rdgInfoData)
{
   if (m_mapRdgPairXY.size() > 0)
   {
       int lastRowCount = 0;
       if (m_rdgLogAutoDataModel->m_data.size() > 0) lastRowCount = m_rdgLogAutoDataModel->m_data.size();
       if ( lastRowCount > 0) m_rdgLogAutoDataModel->m_data.clear();

       m_rdgLogAutoDataModel->m_columnCount = 6;

       m_time_step_ns = rdgInfoData.vectorRdgData[0].time_step_ns;

       for (auto itMap = m_mapRdgPairXY.begin(); itMap != m_mapRdgPairXY.end(); itMap++)
       {
           for (int i = 0; i <= itMap->second.size()-1; i++)
           {
               if ((itMap->second[i].first  >= rdgPixelsInX && itMap->second[i].first <= rdgPixelsFnX) &&
                   (itMap->second[i].second >= rdgPixelsInY && itMap->second[i].second <= rdgPixelsFnY))
               {
                   QVector<QString> dataVec;
                   dataVec.resize(m_rdgLogAutoDataModel->m_columnCount);
                   dataVec[0] = QString::number(itMap->first);
                   dataVec[1] = QString::number(itMap->second[i].first);
                   dataVec[2] = QString::number(itMap->second[i].second);
                   dataVec[3] = QString::number(itMap->second[i].second*rdgInfoData.vectorRdgData[itMap->second[i].first].time_step_ns);
                   dataVec[4] = QString::number(rdgInfoData.vectorRdgData[itMap->second[i].first].latitude_degree);
                   dataVec[5] = QString::number(rdgInfoData.vectorRdgData[itMap->second[i].first].longitude_degree);
                   m_rdgLogAutoDataModel->m_data.append(dataVec);
               }
           }
       }

       m_rdgLogAutoDataModel->m_rowCount = m_rdgLogAutoDataModel->m_data.size();

       if (m_tableRdgLogAutoDataView->isHidden())           m_tableRdgLogAutoDataView->show();
       else if (lastRowCount > 0)
       {
           m_tableRdgLogAutoDataView->hide();
           m_tableRdgLogAutoDataView->show();
       }

       m_tableRdgLogAutoDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
       m_tableRdgLogAutoDataView->verticalHeader  ()->setSectionResizeMode(QHeaderView::Stretch);

       m_tableRdgLogAutoDataView->scroll(0, 1);
       m_tableRdgLogAutoDataView->scroll(0,-1);
       m_tableRdgLogAutoDataView->setModel(m_rdgLogAutoDataModel);
   }
}

void tableRdgLogAutoDataWidget::removeRdgLogAutoDataTable()
{
   m_mapRdgPairXY.clear();
   m_tableRdgLogAutoDataView->hide();
   m_rdgLogAutoDataModel->m_data.clear();
}

void tableRdgLogAutoDataWidget::receiveVectorRdgPairXY(int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, int filterId, const st_rdgInfoData& rdgInfoData)
{
   m_mapRdgPairXY = rdgInfoData.vectorMapAutoLog10RdgPairXY[filterId];
   showRdgLogAutoDataMainModel(rdgPixelsInX, rdgPixelsInY, rdgPixelsFnX, rdgPixelsFnY, rdgInfoData);
}

void tableRdgLogAutoDataWidget::receiveLoadMapRdgPairXY(int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, const st_rdgInfoData& rdgInfoData, std::map<int, std::vector<QPair<int, int>> > mapRdgPairXY)
{
   m_mapRdgPairXY = mapRdgPairXY;
   showRdgLogAutoDataMainModel(rdgPixelsInX, rdgPixelsInY, rdgPixelsFnX, rdgPixelsFnY, rdgInfoData);
}

void tableRdgLogAutoDataWidget::saveRdgAutoSelectionData(QString saveRdgAutoSelectionDataFile)
{
   if (m_mapRdgPairXY.size() > 0) saveRdgAutoSelection(m_mapRdgPairXY, m_time_step_ns, saveRdgAutoSelectionDataFile);
}
