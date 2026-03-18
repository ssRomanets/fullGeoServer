#ifndef TABLERASCANDATAWIDGET_H
#define TABLEASCANDATAWIDGET_H

#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>

#include "bscansData.h"
#include "ascanDataTableModel.h"

class tableAscanDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit tableAscanDataWidget(QWidget* parent = nullptr);
    ~tableAscanDataWidget();

    void outputAscanDataTable(const st_bscanInfoData& bscanInfoData, int trackNumber,  int pixelsInY,  int pixelsFnY);

    void showAscanDataTable  (const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY,  int pixelsFnY);
    void removeAscanDataTable();

    void setShowLog(bool showLog);

public:
    int  m_filterId{0};

signals:
    void fixTrackIndex(const int& trackNumber);

private:
    void showAscanDataMainModel(const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY,  int pixelsFnY);

private:
    QPointer<ascanDataTableModel> m_ascanDataModel;

    QPointer<QTableView>  m_tableAscanDataView;
    QPointer<QVBoxLayout> m_verLayout;

    bool m_showLog{false};
};

#endif // TABLEASCANDATAWIDGET_H
