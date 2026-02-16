#ifndef TABLEDEEPRDGTRACKDATAWIDGET_H
#define TABLEDEEPRDGTRACKDATAWIDGET_H

#include <QPointer>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>

#include "rdgsData.h"
#include "deepRdgTrackDataTableModel.h"

class tableDeepRdgTrackDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tableDeepRdgTrackDataWidget(QWidget* parent = nullptr);
    ~tableDeepRdgTrackDataWidget();

    void outputDeepRdgTrackDataTable(
        const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY,
        int filterId, int materialId, int selectionId
    );
    void showDeepRdgTrackDataTable  (
        const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY,
        int filterId, int materialId, int selectionId
    );

    void removeDeepRdgTrackDataTable();

signals:
    void fixTrackIndex(const int& trackRdgNumber);

private:
    void showDeepRdgTrackDataMainModel(
        const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY,  int rdgPixelsFnY,
        int filterId, int materialId, int selectionId
    );

private:
    QPointer<deepRdgTrackDataTableModel> m_deepRdgTrackDataModel;
    QPointer<QTableView> m_tableDeepRdgTrackDataView;
    QPointer<QVBoxLayout> m_verLayout;
};

#endif // TABLEDEEPRDGTRACKDATAWIDGET_H
