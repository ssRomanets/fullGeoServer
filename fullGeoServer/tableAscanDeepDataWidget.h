#ifndef TABLEAscanDeepDATAWIDGET_H
#define TABLEAscanDeepDATAWIDGET_H

#include <QPointer>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>

#include "bscansData.h"
#include "ascanDeepDataTableModel.h"

class tableAscanDeepDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tableAscanDeepDataWidget(QWidget* parent = nullptr);
    ~tableAscanDeepDataWidget();

    void outputAscanDeepDataTable(
        const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY,  int pixelsFnY,
        int filterId, int materialId, int selectionId
    );
    void showAscanDeepDataTable  (
        const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY,  int pixelsFnY,
        int filterId, int materialId, int selectionId
    );

    void removeAscanDeepDataTable();

signals:
    void fixTrackIndex(const int& trackNumber);

private:
    void showAscanDeepDataMainModel(
        const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY,  int pixelsFnY,
        int filterId, int materialId, int selectionId
    );

private:
    QPointer<ascanDeepDataTableModel> m_ascanDeepDataModel;
    QPointer<QTableView> m_tableAscanDeepDataView;
    QPointer<QVBoxLayout> m_verLayout;
};

#endif // TABLEDEEPBscanTRACKDATAWIDGET_H
