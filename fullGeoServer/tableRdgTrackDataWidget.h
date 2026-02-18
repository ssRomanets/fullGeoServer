#ifndef TABLERDGTRACKDATAWIDGET_H
#define TABLERDGTRACKDATAWIDGET_H

#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>

#include "rdgsData.h"
#include "rdgTrackDataTableModel.h"

class tableRdgTrackDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tableRdgTrackDataWidget(QWidget* parent = nullptr);
    ~tableRdgTrackDataWidget();

    void outputRdgTrackDataTable(const st_rdgInfoData& rdgInfoData, int trackRdgNumber,  int rdgPixelsInY,  int rdgPixelsFnY);

    void showRdgTrackDataTable  (const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY);
    void removeRdgTrackDataTable();

    void setShowLogRdg(bool showLogRdg);

public:
    int  m_filterId{0};
    int  m_materialId{0};

signals:
    void fixTrackIndex(const int& trackRdgNumber);

private:
    void showRdgTrackDataMainModel(const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY);

private:
    QPointer<rdgTrackDataTableModel> m_rdgTrackDataModel;

    QPointer<QTableView>  m_tableRdgTrackDataView;
    QPointer<QVBoxLayout> m_verLayout;

    bool m_showLogRdg{false};
};

#endif // TABLERDGTRACKDATAWIDGET_H
