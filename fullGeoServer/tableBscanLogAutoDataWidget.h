#ifndef TABLEBscanLOGAUTODATAWIDGET_H
#define TABLEBscanLOGAUTODATAWIDGET_H

#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>

#include "bscansData.h"
#include "bscanSelectedDataTableModel.h"

class tableBscanLogAutoDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tableBscanLogAutoDataWidget(QWidget* parent = nullptr);
    ~tableBscanLogAutoDataWidget();

    void outputLogAutoDataTable  (int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData);
    void showLogAutoDataMainModel(int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData);
    void showLogAutoDataTable    (int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData);
    void removeLogAutoDataTable();
    void saveAutoSelectionData(QString saveAutoSelectionDataFile);

    std::map<int, std::vector<QPair<int, int>> > m_mapPairXY;

public slots:
    void receiveVectorPairXY (int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, int filterId, const st_bscanInfoData& bscanInfoData);
    void receiveLoadMapPairXY(
        int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData, std::map<int, std::vector<QPair<int, int>> > mapPairXY
    );

private:
    QPointer<bscanSelectedDataTableModel> m_logAutoDataModel;
    QPointer<QTableView>  m_tableLogAutoDataView;
    QPointer<QVBoxLayout> m_verLayout;
    double m_time_step_ns {0.0};
};

#endif // TABLEBscanLOGAUTODATAWIDGET_H
