#ifndef STRUCTUREBSCANSWIDGET_H
#define STRUCTUREBSCANSWIDGET_H

#include <QMenu>
#include <QScreen>
#include <QWidget>
#include <QVBoxLayout>
#include <QPointer>
#include <QtDataVisualization/Q3DSurface>
#include "structureBscansGraph.h"
#include "correctBscansSurfDialog.h"
#include "bscansData.h"

using namespace QtDataVisualization;

class structureBscansWidget : public QWidget
{
    Q_OBJECT

public:
    structureBscansWidget(QWidget* parent = nullptr);
    ~structureBscansWidget();

    void showBscansSurf(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        int quantImpulsesOfPacket,  int filterId, int selectionId,  bool highLowOnBscan, bool showFullBscans, bool resetCutPointsBscans,
        int materialId, int absBscansStructInX, int absBscansStructFnX
    );
    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);

    QPointer<Q3DSurface>          m_graph {new Q3DSurface()};

public slots:
    void receiveBscansTransGlWidgetPoints(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
    );
    void slotClearBscansTransGlWidget();
    void receiveOutBscansSurfInfo(QString outBscansSurfInfo);
    void slotCloseCut3dAction(bool cut3dVisible);

    void showCorrectBscansSurfWidget(QColor& colorBscansSurf1, QColor& colorBscansSurf2, double fixLatitude, double fixLongitude);
    void closeCorrectBscansSurfWidget();
    void receiveDeleteBscanName(const std::string& deleteBscanName);

    void slotDefDeleteBscanName (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, double fixLatitude, double fixLongitude);
    void slotFixFillBscans(bool resetCutPointsBscans);
    void slotFillBscansExecute(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData, bool resetCutPointsBscans);

signals:
    void updateBscansTransGlWidget(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
    );
    void signalClearBscansTransGlWidget();
    void sendOutBscansSurfInfo(QString outBscansSurfInfo);
    void signalCloseCut3dAction(bool cut3dVisible);
    void sendDeleteBscanName(const std::string& deleteBscanName);
    void signalFixFillBscans(bool resetCutPointsBscans);
    void signalFixDeleteBscanName(double fixLatitude, double fixLongitude);

private:
    QPointer<structureBscansGraph> m_modifier { new structureBscansGraph(m_graph)};
    QPointer<QVBoxLayout> m_mainLayout;
    QPointer<correctBscansSurfDialog> m_correctBscansSurfDialog;
    QPointer<QLabel>         m_out3dCutBscansDataLabel;
};

#endif STRUCTUREBSCANSWIDGET_H
