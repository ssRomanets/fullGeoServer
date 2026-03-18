#include "tableBscanLogAutoDataWidget.h"
#include "transformation.h"

#include <iostream>

tableBscanLogAutoDataWidget::tableBscanLogAutoDataWidget(QWidget* parent): QWidget(parent)
{
   m_tableLogAutoDataView = new QTableView(parent);
   m_logAutoDataModel = new bscanSelectedDataTableModel();

   m_verLayout = new QVBoxLayout;
   m_verLayout->addWidget(m_tableLogAutoDataView);
   setLayout(m_verLayout);

   m_tableLogAutoDataView->hide();
}

tableBscanLogAutoDataWidget::~tableBscanLogAutoDataWidget(){}

void tableBscanLogAutoDataWidget::outputLogAutoDataTable(int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData)
{
   showLogAutoDataTable(pixelsInX, pixelsInY, pixelsFnX,  pixelsFnY, bscanInfoData);
}

void tableBscanLogAutoDataWidget::showLogAutoDataTable(int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY,const st_bscanInfoData& bscanInfoData)
{
   if (bscanInfoData.vectorBscanData.size() > 0)
       showLogAutoDataMainModel(pixelsInX, pixelsInY, pixelsFnX,  pixelsFnY, bscanInfoData);
}

void tableBscanLogAutoDataWidget::showLogAutoDataMainModel(int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData)
{
   if (m_mapPairXY.size() > 0)
   {
       int lastRowCount = 0;
       if (m_logAutoDataModel->m_data.size() > 0) lastRowCount = m_logAutoDataModel->m_data.size();
       if ( lastRowCount > 0) m_logAutoDataModel->m_data.clear();

       m_logAutoDataModel->m_columnCount = 6;

       m_time_step_ns = bscanInfoData.vectorBscanData[0].time_step_ns;

       for (auto itMap = m_mapPairXY.begin(); itMap != m_mapPairXY.end(); itMap++)
       {
           for (int i = 0; i <= itMap->second.size()-1; i++)
           {
               if ((itMap->second[i].first  >= pixelsInX && itMap->second[i].first  <= pixelsFnX) &&
                   (itMap->second[i].second >= pixelsInY && itMap->second[i].second <= pixelsFnY))
               {
                   QVector<QString> dataVec;
                   dataVec.resize(m_logAutoDataModel->m_columnCount);
                   dataVec[0] = QString::number(itMap->first);
                   dataVec[1] = QString::number(itMap->second[i].first);
                   dataVec[2] = QString::number(itMap->second[i].second);
                   dataVec[3] = QString::number(itMap->second[i].second*bscanInfoData.vectorBscanData[itMap->second[i].first].time_step_ns);
                   dataVec[4] = QString::number(bscanInfoData.vectorBscanData[itMap->second[i].first].latitude_degree);
                   dataVec[5] = QString::number(bscanInfoData.vectorBscanData[itMap->second[i].first].longitude_degree);
                   m_logAutoDataModel->m_data.append(dataVec);
               }
           }
       }

       m_logAutoDataModel->m_rowCount = m_logAutoDataModel->m_data.size();

       if (m_tableLogAutoDataView->isHidden())           m_tableLogAutoDataView->show();
       else if (lastRowCount > 0)
       {
           m_tableLogAutoDataView->hide();
           m_tableLogAutoDataView->show();
       }

       m_tableLogAutoDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
       m_tableLogAutoDataView->verticalHeader  ()->setSectionResizeMode(QHeaderView::Stretch);

       m_tableLogAutoDataView->scroll(0, 1);
       m_tableLogAutoDataView->scroll(0,-1);
       m_tableLogAutoDataView->setModel(m_logAutoDataModel);
   }
}

void tableBscanLogAutoDataWidget::removeLogAutoDataTable()
{
   m_mapPairXY.clear();
   m_tableLogAutoDataView->hide();
   m_logAutoDataModel->m_data.clear();
}

void tableBscanLogAutoDataWidget::receiveVectorPairXY(int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, int filterId, const st_bscanInfoData& bscanInfoData)
{
   m_mapPairXY = bscanInfoData.vectorMapAutoLog10BscanPairXY[filterId];
   showLogAutoDataMainModel(pixelsInX, pixelsInY, pixelsFnX, pixelsFnY, bscanInfoData);
}

void tableBscanLogAutoDataWidget::receiveLoadMapPairXY(int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData, std::map<int, std::vector<QPair<int, int>> > mapPairXY)
{
   m_mapPairXY = mapPairXY;
   showLogAutoDataMainModel(pixelsInX, pixelsInY, pixelsFnX, pixelsFnY, bscanInfoData);
}

void tableBscanLogAutoDataWidget::saveAutoSelectionData(QString saveAutoSelectionDataFile)
{
   if (m_mapPairXY.size() > 0) saveBscanAutoSelection(m_mapPairXY, m_time_step_ns, saveAutoSelectionDataFile);
}
