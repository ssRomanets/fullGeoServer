#ifndef bscanSSURFGLWIDGET_H
#define bscanSSURFGLWIDGET_H

#include <QDateTime>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_1_1>

#include "bscansData.h"

class bscansSurfGlWidget : public QGLWidget, public QOpenGLFunctions_1_1
{
    Q_OBJECT

public:
    bscansSurfGlWidget();
    ~bscansSurfGlWidget();

    int m_lowPixelLevel;
    int m_highPixelLevel;

    bool m_section2dVisible {true};
    bool m_highLowOnBscan     {true};
    bool m_showFullBscans     {true};

    int m_absBscansStructInX {0};
    int m_absBscansStructFnX {0};

    std::vector<std::tuple<int, int, int>> m_vectorTuplesColors;

    void setupColorsData(const QColor& colorUp, const QColor& colorLow);

    void setupProperties(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan,  bool showFullBscans, bool resetSectPoints2dBscans, int materialId,
        int absBscansStructInX, int absBscansStructFnX
    );

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);
    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);
    void setupBscansSurfPixels(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData);
    void setupTypeScaling(int typeBscansSurfScaling);

public slots:
    void slotCloseSection2dAction(bool section2dVisible);

    void slotChangeBscansSurfPixelsFnX(int bscansSurfPixelsFnX);
    void slotChangeBscansSurfPixelsFnY(int bscansSurfPixelsFnY);

protected:
    void initializeGL() override;
    void paintEvent(QPaintEvent*);
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event);

signals:
    void updateBscansSurfSections( int inPosX, int inPosY, int fnPosX, int fnPosY);
    void hideBscansSurfSections();
    void sendOutBscansSurfInfo(QString outBscansSurfInfo);
    void signalFixBscansSurfPixels();

    void signalSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData);
    void signalSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData);

private:
    bool m_wheelActive    {false};
    bool m_mouseMoveEventAct{false};

    GLubyte* m_pixels {nullptr};
    GLubyte* m_bscansSurfBWPalettes    {nullptr};
    std::vector<QPair<int, int> > m_vectorPairXY;

    bool m_allScaling {true};
    bool m_hScaling   {false};
    bool m_vScaling   {false};

    int m_pixelsInX {0};
    int m_pixelsInY {0};

    int m_pixelsFnX {0};
    int m_pixelsFnY {0};

    int m_pixelsWidth {0};
    int m_pixelsHeight {0};

    int m_pixelsCentX {0};
    int m_pixelsCentY {0};

    int m_inX {0};
    int m_inY {0};

    int m_fnX {0};
    int m_fnY {0};

    int m_pressX {0};
    int m_pressY {0};

    int m_releaseX {0};
    int m_releaseY {0};

    int m_inSectPointX {0};
    int m_inSectPointY {0};

    int m_fnSectPointX {0};
    int m_fnSectPointY {0};
    int m_quantImpulsesOfPacket{};
    int m_materialId{0};

    int m_width  {0};
    int m_height {0};

    double m_leftLatitude {0.0};
    double m_rightLatitude {0.0};
    double m_lowLongitude {0.0};
    double m_upLongitude {0.0};

    double m_deltaLatitude  {0.0};
    double m_deltaLongitude {0.0};

    double m_maxDeep {0.0};

    int m_filterId{0};
    int m_selectionId {0};

    void includeMaskColorToBscansSurfPixels(int maskColor, int i, int j);
    void outputInitBscansSurfSection(bool resetSectPoints2dBscans);
    void generateTexture(GLubyte* texImPixels, int bscansSurfWidth, int bscansSurfHeight);

    void outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);

    int m_bscansSurfOpenGlHeight;
};

#endif // bscanSSURFGLWIDGET_H
