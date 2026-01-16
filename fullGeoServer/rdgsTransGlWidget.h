#ifndef rdgsTransTRANSGLWIDGET_H
#define rdgsTransTRANSGLWIDGET_H

#include <QPainter>
#include <QMouseEvent>
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_1_1>

#include "rdgsData.h"

class rdgsTransGlWidget : public QGLWidget, public QOpenGLFunctions_1_1
{
    Q_OBJECT

public:
    rdgsTransGlWidget();
    ~rdgsTransGlWidget();

    void setupColorsRdgsTransData(const QColor& colorUp, const QColor& colorLow);

    void setInfoDataMap(int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int materialId);
    void setShowLogRdgsTrans(bool showLogRdgsTrans);
    void representRdgsTransGlWidget(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData);

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

    void setupRdsgTransScaling(int typeRdgsTransScaling);
    void setupRdgsTransPixels();

    bool m_showLogRdgsTrans {false};
    bool m_showRdg{true};
    std::string m_kindInit {"init rdgsTrans"};
    int m_materialId{0};
    int m_filterId{0};

    int m_lowPixelLevel{0};
    int m_highPixelLevel{0};

    std::vector<std::tuple<int, int, int>> m_vectorTuplesColorsRdgsTrans;

public slots:
    void receiveRdgsTransGlWidgetPoints(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
        const std::vector<std::pair<int, int>>& vectorrdgsTransLineSectionFinalPoints, bool highLowOnRdg
    );
    void slotClearRdgsTransGlWidget();

    void slotChangeRdgsTransPixelsFnX(int rdgsTransPixelsFnX);
    void slotChangeRdgsTransPixelsFnY(int rdgsTransPixelsFnY);

signals:
    void signalSetupScrollHRdgsTransData(int minScrollHRdgsTransData, int scrollHRdgsTransData, int maxScrollHRdgsTransData);
    void signalSetupScrollVRdgsTransData(int minScrollVRdgsTransData, int scrollVRdgsTransData, int maxScrollVRdgsTransData);

protected:
    void initializeGL() override;
    void paintGL() override;
    void paintEvent(QPaintEvent*);
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event);

private:
    std::vector<std::vector<double> > m_vectorRdgsTransResultImpulses;
    std::vector<std::pair<int, int>> m_vectorRdgsTransLineSectionFinalPoints;

    double m_time_step_ns;
    double m_maxImpulse {0.0};
    double m_minImpulse {0.0};

    GLubyte* m_rdgsTransPixels     {nullptr};
    GLubyte* m_rdgsTransBWPalettes {nullptr};
    int m_rdgsTransWidth {0};
    int m_rdgsTransHeight {0};

    int m_rdgsTransPixelsInX {0};
    int m_rdgsTransPixelsInY {0};

    int m_rdgsTransPixelsFnX {0};
    int m_rdgsTransPixelsFnY {0};

    int m_rdgsTransPixelsWidth {0};
    int m_rdgsTransPixelsHeight {0};

    int m_rdgsTransPixelsCentX {0};
    int m_rdgsTransPixelsCentY {0};

    int m_inX;
    int m_inY;

    int m_fnX;
    int m_fnY;

    int m_pressX {0};
    int m_pressY {0};

    int m_releaseX {0};
    int m_releaseY {0};

    int m_rdgsTransOpenGlHeight {0};

    bool m_wheelActive    {false};
    bool m_rdgsTransAllScaling {true};
    bool m_rdgsTransHScaling   {false};
    bool m_rdgsTransVScaling   {false};

    int m_quantImpulsesOfPacket{0};
    int m_maxQuantImpulsesOfPacket{0};
    bool m_highLowOnRdg{true};

    double m_leftGlLatitude  {0.0};
    double m_rightGlLatitude {0.0};
    double m_lowGlLongitude  {0.0};
    double m_upGlLongitude   {0.0};

    double m_deltaGlLatitude  {0.0};
    double m_deltaGlLongitude {0.0};

    void outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
    void fixRdgsTrans();
};

#endif // rdgsTransTRANSGLWIDGET_H
