#ifndef bscansTransTRANSGLWIDGET_H
#define bscansTransTRANSGLWIDGET_H

#include <QPainter>
#include <QMouseEvent>
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_1_1>

#include "bscansData.h"

class bscansTransGlWidget : public QGLWidget, public QOpenGLFunctions_1_1
{
    Q_OBJECT

public:
    bscansTransGlWidget();
    ~bscansTransGlWidget();

    void setupColorsBscansTransData(const QColor& colorUp, const QColor& colorLow);

    void setInfoDataMap(int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int materialId);
    void setShowLogBscansTrans(bool showLogBscansTrans);
    void representBscansTransGlWidget(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData);

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

    void setupTypeScaling(int typeScaling);
    void setupBscansTransPixels();

    bool m_showLogBscansTrans {false};
    bool m_showFullBscans{true};
    std::string m_kindInit {"init bscansTrans"};
    int m_materialId{0};
    int m_filterId{0};

    int m_lowPixelLevel{0};
    int m_highPixelLevel{0};

    std::vector<std::tuple<int, int, int>> m_vectorTuplesColors;

public slots:
    void receiveBscansTransGlWidgetPoints(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
        const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
    );
    void slotClearBscansTransGlWidget();

    void slotChangeBscansTransPixelsFnX(int bscansTransPixelsFnX);
    void slotChangeBscansTransPixelsFnY(int bscansTransPixelsFnY);

signals:
    void signalSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData);
    void signalSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData);

protected:
    void initializeGL() override;
    void paintGL() override;
    void paintEvent(QPaintEvent*);
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event);

private:
    std::vector<std::vector<double> > m_vectorBscansTransResultImpulses;
    std::vector<std::pair<int, int>> m_vectorBscansTransLineSectionFinalPoints;

    double m_time_step_ns;
    double m_maxImpulse {0.0};
    double m_minImpulse {0.0};

    GLubyte* m_pixels     {nullptr};
    GLubyte* m_bscansTransBWPalettes {nullptr};

    int m_width {0};
    int m_height {0};

    int m_pixelsInX {0};
    int m_pixelsInY {0};

    int m_pixelsFnX {0};
    int m_pixelsFnY {0};

    int m_pixelsWidth {0};
    int m_pixelsHeight {0};

    int m_pixelsCentX {0};
    int m_pixelsCentY {0};

    int m_inX;
    int m_inY;

    int m_fnX;
    int m_fnY;

    int m_pressX {0};
    int m_pressY {0};

    int m_releaseX {0};
    int m_releaseY {0};

    int m_bscansTransOpenGlHeight {0};

    bool m_wheelActive    {false};
    bool m_allScaling {true};
    bool m_hScaling   {false};
    bool m_vScaling   {false};

    int m_quantImpulsesOfPacket{0};
    int m_maxQuantImpulsesOfPacket{0};
    bool m_highLowOnBscan{true};

    double m_leftGlLatitude  {0.0};
    double m_rightGlLatitude {0.0};
    double m_lowGlLongitude  {0.0};
    double m_upGlLongitude   {0.0};

    double m_deltaGlLatitude  {0.0};
    double m_deltaGlLongitude {0.0};

    void outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
    void fixBscansTrans();
};

#endif // bscansTransTRANSGLWIDGET_H
