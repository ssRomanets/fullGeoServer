#ifndef bscanDeepGLWIDGET_H
#define bscanDeepGLWIDGET_H

#include <QPainter>
#include <QMouseEvent>
#include <QPen>

#include <QtOpenGL>
#include <QOpenGLFunctions_1_1>
#include "bscansData.h"

class bscanDeepGlWidget : public QGLWidget, public QOpenGLFunctions_1_1
{
    Q_OBJECT

public:
    bscanDeepGlWidget();
    ~bscanDeepGlWidget();

    QPainter m_painter;

    int m_materialId     {0};
    int m_filterId       {0};
    int m_selectionId    {0};

    std::string m_bscanName {""};

    int m_lowPixelLevel;
    int m_highPixelLevel;

    GLubyte* m_pixels   {nullptr};
    GLubyte* m_palette {nullptr};

    int m_absPixelsInX {0};
    int m_absPixelsFnX {0};

    int m_pixelsFnX {0};
    int m_pixelsFnY {0};

    int m_pixelsInX {0};
    int m_pixelsInY {0};

    int m_width {0};
    int m_height{0};

    int m_pixelsWidth {0};
    int m_pixelsHeight {0};

    double m_maxDeep {0.0};
    double m_minDeep {0.0};

    double m_minSmallDeep {0.0};
    double m_maxSmallDeep {0.0};

    bool m_appearingBscan {false};

    std::vector<std::tuple<int, int, int>> m_vectorTuplesColors;

    void setupColorsData(const QColor& colorUp, const QColor& colorLow);

    void setupTypeScaling(int typeScaling);

    void setupProperties(
        const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int absPixelsInX, int absPixelsFnX
    );

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

signals:
    void signalFixPixels();
    void signalFixDataBscanDeepWidgets();
    void signalSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHdata);
    void signalSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVdata);

public slots:
    void slotSetupPixels(const st_bscanInfoData& bscanInfoData);
    void remarkTrack(const int& trackBscanNumber);

    void slotChangePixelsFnX(int bscanPixelsFnX);
    void slotChangePixelsFnY(int bscanPixelsFnY);

protected:
    void initializeGL() override;
    void paintEvent(QPaintEvent*);
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event);

private:
    double m_time_step_ns;

    double m_leftLatitude {-1.0};
    double m_rightLatitude {-1.0};
    double m_lowLongitude {-1.0};
    double m_upLongitude {-1.0};

    int m_trackNumber{0};

    st_bscanInfoData m_bscanInfoData;
    std::map<std::string, st_bscanInfoData> m_bscansInfoDataMap;

    int m_inX;
    int m_inY;

    int m_fnX;
    int m_fnY;

    int m_pressX {0};
    int m_pressY {0};

    int m_releaseX {0};
    int m_releaseY {0};

    int m_pixelsCentX {0};
    int m_pixelsCentY {0};

    bool m_wheelActive {false};
    bool m_allScaling {true};
    bool m_hScaling   {false};
    bool m_vScaling   {false};

    QString m_loadAutoSelectionDataFile;

    int m_openGlHeight;

private:
    void outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
    void fixBscanDeep();
};

#endif // bscanGLWIDGET_H

