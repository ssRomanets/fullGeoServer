#ifndef TABLERDGLOGAUTODATAWIDGET_H
#define TABLERDGLOGAUTODATAWIDGET_H

#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>

#include "rdgsData.h"
#include "rdgSelectedDataTableModel.h"

class tableRdgLogAutoDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tableRdgLogAutoDataWidget(QWidget* parent = nullptr);
    ~tableRdgLogAutoDataWidget();

    void outputRdgLogAutoDataTable  (int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, const st_rdgInfoData& rdgInfoData);
    void showRdgLogAutoDataMainModel(int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, const st_rdgInfoData& rdgInfoData);
    void showRdgLogAutoDataTable    (int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, const st_rdgInfoData& rdgInfoData);
    void removeRdgLogAutoDataTable();
    void saveRdgAutoSelectionData(QString saveRdgAutoSelectionDataFile);

    std::map<int, std::vector<QPair<int, int>> > m_mapRdgPairXY;

public slots:
    void receiveVectorRdgPairXY (int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, int filterId, const st_rdgInfoData& rdgInfoData);
    void receiveLoadMapRdgPairXY(
        int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY,
        const st_rdgInfoData& rdgInfoData, std::map<int, std::vector<QPair<int, int>> > mapRdgPairXY
    );

private:
    QPointer<rdgSelectedDataTableModel> m_rdgLogAutoDataModel;
    QPointer<QTableView>  m_tableRdgLogAutoDataView;
    QPointer<QVBoxLayout> m_verLayout;
    double m_time_step_ns {0.0};
};

#endif // TABLERDGLOGAUTODATAWIDGET_H
