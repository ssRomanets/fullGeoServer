#ifndef STRUCTUREBscanSGRAPH_H
#define STRUCTUREBscanSGRAPH_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QAbstract3DGraph>
#include <QPointer>
#include <QImage>

#include "bscansData.h"

using namespace QtDataVisualization;

class structureBscansGraph : public QObject
{
    Q_OBJECT

public:
    explicit structureBscansGraph(Q3DSurface* surface);
    ~structureBscansGraph();

    void fillBscans(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        int quantImpulsesOfPacket, int filterId, int selectionId,  bool highLowOnBscan, bool showFullBscans, bool resetCutPointsBscans,
        int materialId, int absBscansStructInX, int absBscansStructFnX
    );
    void fillBscansExecute (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData, bool resetCutPointsBscans);
    void fillAllBscansProxy(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData);

    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);

    bool m_cut3dVisible {true};
    QPointer<Q3DSurface> m_graph;

signals:
    void updateBscansTransGlWidget(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
    );
    void closeCorrectBscansSurfWidget();
    void showCorrectBscansSurfWidget( QColor& colorBscansSurf1, QColor& colorBscansSurf2, double fixLatitude, double fixLongitude);
    void signalClearBscansTransGlWidget();
    void sendOutBscansSurfInfo(QString outBscansSurfInfo);
    void signalFixFillBscans(bool resetCutPointsBscans);

public slots:
    void slotCloseCut3dAction(bool cut3dVisible);
    void receiveColorsBscansSurf(QColor& colorBscansSurf1, QColor& colorBscansSurf2);

private:
    void activateBscansModel();
    void activateBscansReliefModel();
    void activateBscansSectionLineModel();
    void graphDataClear();
    void sendBscansSurfInfo();
    void currentClear3dProxy();

private:
    double m_leftLatitude {0.0};
    double m_rightLatitude {0.0};
    double m_lowLongitude {0.0};
    double m_upLongitude {0.0};

    double m_deltaLatitude  {0.0};
    double m_deltaLongitude {0.0};

    int m_bscansSurfWidth  {0};
    int m_bscansSurfHeight {0};

    double m_maxDeep {0.0};

    double m_maxRelief {0.0};
    double m_minRelief {0.0};

    int m_filterId    {0};
    int m_selectionId {0};

    int m_absBscansStructInX {0};
    int m_absBscansStructFnX {0};

    QPointer<QSurfaceDataProxy> m_bscansProxy  {new QSurfaceDataProxy()};
    QPointer<QSurface3DSeries>  m_bscansSeries {new QSurface3DSeries(m_bscansProxy)};

    QPointer<QSurfaceDataProxy> m_bscansReliefProxy  {new QSurfaceDataProxy()};
    QPointer<QSurface3DSeries>  m_bscansReliefSeries {new QSurface3DSeries(m_bscansReliefProxy)};

    QPointer<QSurfaceDataProxy> m_bscansLineSectionProxy  {new QSurfaceDataProxy()};
    QPointer<QSurface3DSeries>  m_bscansLineSectionSeries {new QSurface3DSeries(m_bscansLineSectionProxy)};

    QSurfaceDataArray* m_dataBscansArray{nullptr};
    QSurfaceDataArray* m_dataBscansReliefArray{nullptr};
    QSurfaceDataArray* m_dataBscansLineSectionArray{nullptr};

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

    std::vector<std::pair<int, int>> m_vectorBscansTransLineSectionPoints;
    std::vector<uchar> m_imageReliefVectorData;

    QColor m_colorBscansSurf1;
    QColor m_colorBscansSurf2;

    bool m_highLowOnBscan             {true};
    bool m_showFullBscans             {true};
    bool m_chImageReliefVectorData {false};
    int m_materialId{0};

    std::vector<QSurfaceDataRow*> m_newBscansRowVector;
    std::vector<QSurfaceDataRow*> m_newBscansReliefRowVector;
    std::vector<QSurfaceDataRow*> m_newBscansLineSectionRowVector;

private slots:
    void handleElementSelected(QAbstract3DGraph::ElementType type);

private:
    void addMarker(int x, int y, int vecPosId);
    void outputCutInterfaceMarkers();
};

#endif // STRUCTUREbscanSGRAPH_H
