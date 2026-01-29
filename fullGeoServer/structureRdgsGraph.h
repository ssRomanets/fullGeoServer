#ifndef STRUCTURERDGSGRAPH_H
#define STRUCTURERDGSGRAPH_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QAbstract3DGraph>
#include <QPointer>
#include <QImage>

#include "rdgsData.h"

using namespace QtDataVisualization;

class structureRdgsGraph : public QObject
{
    Q_OBJECT

public:
    explicit structureRdgsGraph(Q3DSurface* surface);
    ~structureRdgsGraph();

    void fillRdgs(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        int quantImpulsesOfPacket, int filterId, int selectionId,  bool highLowOnRdg, bool showFullRdgs, bool resetCutPointsRdgs,
        int materialId, int absRdgsStructInX, int absRdgsStructFnX
    );
    void fillRdgsExecute (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData, bool resetCutPointsRdgs);
    void fillAllRdgsProxy(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData);

    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);

    bool m_cut3dVisible {true};
    QPointer<Q3DSurface> m_graph;

signals:
    void updateRdgsTransGlWidget(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        const std::vector<std::pair<int, int>>& vectorRdgsTransLineSectionFinalPoints, bool highLowOnRdg
    );
    void closeCorrectRdgsSurfWidget();
    void showCorrectRdgsSurfWidget( QColor& colorRdgsSurf1, QColor& colorRdgsSurf2, double fixLatitude, double fixLongitude);
    void signalClearRdgsTransGlWidget();
    void sendOutRdgsSurfInfo(QString outRdgsSurfInfo);
    void signalFixFillRdgs(bool resetCutPointsRdgs);

public slots:
    void slotCloseCut3dAction(bool cut3dVisible);
    void receiveColorsRdgsSurf(QColor& colorRdgsSurf1, QColor& colorRdgsSurf2);

private:
    void activateRdgsModel();
    void activateRdgsReliefModel();
    void activateRdgsSectionLineModel();
    void graphDataClear();
    void sendRdgsSurfInfo();
    void currentClear3dProxy();

private:
    double m_leftLatitude {0.0};
    double m_rightLatitude {0.0};
    double m_lowLongitude {0.0};
    double m_upLongitude {0.0};

    double m_deltaLatitude  {0.0};
    double m_deltaLongitude {0.0};

    int m_rdgsSurfWidth  {0};
    int m_rdgsSurfHeight {0};

    double m_maxDeep {0.0};

    double m_maxRelief {0.0};
    double m_minRelief {0.0};

    int m_filterId    {0};
    int m_selectionId {0};

    int m_absRdgsStructInX {0};
    int m_absRdgsStructFnX {0};

    QPointer<QSurfaceDataProxy> m_rdgsProxy  {new QSurfaceDataProxy()};
    QPointer<QSurface3DSeries>  m_rdgsSeries {new QSurface3DSeries(m_rdgsProxy)};

    QPointer<QSurfaceDataProxy> m_rdgsReliefProxy  {new QSurfaceDataProxy()};
    QPointer<QSurface3DSeries>  m_rdgsReliefSeries {new QSurface3DSeries(m_rdgsReliefProxy)};

    QPointer<QSurfaceDataProxy> m_rdgsLineSectionProxy  {new QSurfaceDataProxy()};
    QPointer<QSurface3DSeries>  m_rdgsLineSectionSeries {new QSurface3DSeries(m_rdgsLineSectionProxy)};

    QSurfaceDataArray* m_dataRdgsArray{nullptr};
    QSurfaceDataArray* m_dataRdgsReliefArray{nullptr};
    QSurfaceDataArray* m_dataRdgsLineSectionArray{nullptr};

    std::vector<QVector3D> m_vectorPositions;

    QImage m_colorMarker;

    int m_curX{-1};
    int m_curY{-1};

    int m_inCutPosX {0};
    int m_inCutPosY {0};

    int m_fnCutPosX {0};
    int m_fnCutPosY {0};

    bool m_inPosFix {false};
    bool m_fnPosFix {false};

    int m_quantImpulsesOfPacket{0};

    std::vector<std::pair<int, int>> m_vectorRdgsTransLineSectionPoints;
    std::vector<uchar> m_imageReliefVectorData;

    QColor m_colorRdgsSurf1;
    QColor m_colorRdgsSurf2;

    bool m_highLowOnRdg             {true};
    bool m_showFullRdgs             {true};
    bool m_chImageReliefVectorData {false};
    int m_materialId{0};

    std::vector<QSurfaceDataRow*> m_newRdgsRowVector;
    std::vector<QSurfaceDataRow*> m_newRdgsReliefRowVector;
    std::vector<QSurfaceDataRow*> m_newRdgsLineSectionRowVector;

private slots:
    void handleElementSelected(QAbstract3DGraph::ElementType type);

private:
    void addMarker(int x, int y, int vecPosId);
    void outputCutInterfaceMarkers();
};

#endif // STRUCTURERDGSGRAPH_H
