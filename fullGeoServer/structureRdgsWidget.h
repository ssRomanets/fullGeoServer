#ifndef STRUCTURERDGSWIDGET_H
#define STRUCTURERDGSWIDGET_H

#include <QMenu>
#include <QScreen>
#include <QWidget>
#include <QVBoxLayout>
#include <QPointer>
#include <QtDataVisualization/Q3DSurface>
#include "structureRdgsGraph.h"
#include "correctRdgsSurfDialog.h"
#include "rdgsData.h"

using namespace QtDataVisualization;

class structureRdgsWidget : public QWidget
{
    Q_OBJECT

public:
    structureRdgsWidget(QWidget* parent = nullptr);
    ~structureRdgsWidget();

    void showRdgsSurf(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        int quantImpulsesOfPacket,  int filterId, int selectionId,  bool highLowOnRdg, bool showFullRdgs, bool resetCutPointsRdgs,
        int materialId, int absRdgsStructInX, int absRdgsStructFnX
    );
    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);

    QPointer<Q3DSurface>          m_graph {new Q3DSurface()};

public slots:
    void receiveRdgsTransGlWidgetPoints(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        const std::vector<std::pair<int, int>>& vectorRdgsTransLineSectionFinalPoints, bool highLowOnRdg
    );
    void slotClearRdgsTransGlWidget();
    void receiveOutRdgsSurfInfo(QString outRdgsSurfInfo);
    void slotCloseCut3dAction(bool cut3dVisible);

    void showCorrectRdgsSurfWidget(QColor& colorRdgsSurf1, QColor& colorRdgsSurf2, double fixLatitude, double fixLongitude);
    void closeCorrectRdgsSurfWidget();
    void receiveDeleteRdgName(const std::string& deleteRdgName);

    void slotDefDeleteRdgName (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, double fixLatitude, double fixLongitude);
    void slotFixFillRdgs(bool resetCutPointsRdgs);
    void slotFillRdgsExecute(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData, bool resetCutPointsRdgs);

signals:
    void updateRdgsTransGlWidget(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        const std::vector<std::pair<int, int>>& vectorRdgsTransLineSectionFinalPoints, bool highLowOnRdg
    );
    void signalClearRdgsTransGlWidget();
    void sendOutRdgsSurfInfo(QString outRdgsSurfInfo);
    void signalCloseCut3dAction(bool cut3dVisible);
    void sendDeleteRdgName(const std::string& deleteRdgName);
    void signalFixFillRdgs(bool resetCutPointsRdgs);
    void signalFixDeleteRdgName(double fixLatitude, double fixLongitude);

private:
    QPointer<structureRdgsGraph> m_modifier { new structureRdgsGraph(m_graph)};
    QPointer<QVBoxLayout> m_mainLayout;
    QPointer<correctRdgsSurfDialog> m_correctRdgsSurfDialog;
    QPointer<QLabel>         m_out3dCutRdgsDataLabel;
};

#endif // STRUCTURERDGSWIDGET_H
