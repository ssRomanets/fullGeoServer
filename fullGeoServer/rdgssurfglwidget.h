#ifndef RDGSSURFGLWIDGET_H
#define RDGSSURFGLWIDGET_H

#include <QDateTime>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_1_1>

#include "rdgsData.h"

class rdgsSurfGlWidget : public QGLWidget, public QOpenGLFunctions_1_1
{
    Q_OBJECT

public:
    rdgsSurfGlWidget();
    ~rdgsSurfGlWidget();

    int m_lowPixelLevel;
    int m_highPixelLevel;

    bool m_section2dVisible {true};
    bool m_highLowOnRdg     {true};
    bool m_showFullRdgs     {true};

    int m_absRdgsStructInX {0};
    int m_absRdgsStructFnX {0};

    std::vector<std::tuple<int, int, int>> m_vectorTuplesColorsRdgsSurf;

    void setupColorsRdgsSurfData(const QColor& colorUp, const QColor& colorLow);

    void setupRdgsSurfProperties(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg,  bool showFullRdgs, bool resetSectPoints2dRdgs, int materialId,
        int absRdgsStructInX, int absRdgsStructFnX
    );

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);
    void saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile);
    void loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile);
    void setupRdgsSurfPixels(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData);
    void setupRdgsSurfScaling(int typeRdgsSurfScaling);

public slots:
    void slotCloseSection2dAction(bool section2dVisible);

    void slotChangeRdgsSurfPixelsFnX(int rdgsSurfPixelsFnX);
    void slotChangeRdgsSurfPixelsFnY(int rdgsSurfPixelsFnY);

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
    void updateRdgsSurfSections( int inPosX, int inPosY, int fnPosX, int fnPosY);
    void hideRdgsSurfSections();
    void sendOutRdgsSurfInfo(QString outRdgsSurfInfo);
    void signalFixRdgsSurfPixels();

    void signalSetupScrollHRdgsSurfData(int minScrollHRdgsSurfData, int scrollHRdgsSurfData, int maxScrollHRdgsSurfData);
    void signalSetupScrollVRdgsSurfData(int minScrollVRdgsSurfData, int scrollVRdgsSurfData, int maxScrollVRdgsSurfData);

private:
    bool m_wheelActive    {false};
    bool m_mouseMoveEventAct{false};

    GLubyte* m_rdgsSurfPixels {nullptr};
    GLubyte* m_rdgsSurfBWPalettes    {nullptr};
    std::vector<QPair<int, int> > m_vectorPairXY;

    bool m_rdgsSurfAllScaling {true};
    bool m_rdgsSurfHScaling   {false};
    bool m_rdgsSurfVScaling   {false};

    int m_rdgsSurfPixelsInX {0};
    int m_rdgsSurfPixelsInY {0};

    int m_rdgsSurfPixelsFnX {0};
    int m_rdgsSurfPixelsFnY {0};

    int m_rdgsSurfPixelsWidth {0};
    int m_rdgsSurfPixelsHeight {0};

    int m_rdgsSurfPixelsCentX {0};
    int m_rdgsSurfPixelsCentY {0};

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

    int m_rdgsSurfWidth  {0};
    int m_rdgsSurfHeight {0};

    double m_leftLatitude {0.0};
    double m_rightLatitude {0.0};
    double m_lowLongitude {0.0};
    double m_upLongitude {0.0};

    double m_deltaLatitude  {0.0};
    double m_deltaLongitude {0.0};

    double m_maxSumImpulses {0.0};

    int m_filterId{0};
    int m_selectionId {0};

    void includeMaskColorToRdgsSurfPixels(int maskColor, int i, int j);
    void outputInitRdgsSurfSection(bool resetSectPoints2dRdgs);
    void generateTexture(GLubyte* texImPixels, int rdgsSurfWidth, int rdgsSurfHeight);

    void outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);

    int m_rdgsSurfOpenGlHeight;

    int m_minHeightResultRelief;
    int m_maxHeightResultRelief;
};

#endif // RDGSSURFGLWIDGET_H
