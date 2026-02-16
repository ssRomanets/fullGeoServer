#ifndef RDGGLWIDGET_H
#define RDGGLWIDGET_H

#include <QPainter>
#include <QMouseEvent>
#include <QPen>

#include <QtOpenGL>
#include <QOpenGLFunctions_1_1>
#include "rdgsData.h"

class rdgGlWidget : public QGLWidget, public QOpenGLFunctions_1_1
{
    Q_OBJECT

public:
    rdgGlWidget();
    ~rdgGlWidget();

    std::string m_kindInit{" Init"};

    QPainter m_painter;

    int m_materialId     {0};
    int m_filterId       {0};
    std::string m_rdgName {""};

    int m_lowPixelLevel;
    int m_highPixelLevel;

    GLubyte* m_rdgPixels   {nullptr};
    GLubyte* m_rdgBWPalettes {nullptr};

    int m_absRdgPixelsInX {0};
    int m_absRdgPixelsFnX {0};

    int m_rdgPixelsFnX {0};
    int m_rdgPixelsFnY {0};

    int m_rdgPixelsInX {0};
    int m_rdgPixelsInY {0};

    int m_rdgWidth {0};
    int m_rdgHeight{0};

    int m_rdgPixelsWidth {0};
    int m_rdgPixelsHeight {0};

    bool m_appearingRdg {false};
    bool m_showLogRdg          {false};
    bool m_logRdgMouseSelect      {false};
    bool m_logRdgAutoSelect      {false};
    bool m_showInitRdgSelect {false};

    std::vector<std::tuple<int, int, int>> m_vectorTuplesColorsRdg;

    void setupColorsRdgData(const QColor& colorUp, const QColor& colorLow);

    void setAppearingRdg(bool appearRdg);
    void setShowLogRdg(bool showLogRdg);
    void setLogRdgMouseSelection(bool logRdgMouseSelect);
    void setupRdgScaling(int typeRdgScaling);

    void settingsRdgSelection(bool logRdgAutoSelect, bool clearSelectRdgPoints, std::string rdgName);
    void loadRdgAutoSelectionData(QString loadRdgAutoSelectionDataFile, std::string rdgName);

    void setShowInitRdgSelection(bool showInitRdgSelection);
    void resetRdgSelection(bool clearSelectRdgPoints, std::string rdgName);

    void setupRdgProperties(
        const std::string& rdgName, const st_rdgInfoData& rdgInfoData, int absRdgPixelsInX, int absRdgPixelsFnX
    );

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

    void clearSelectedData(std::string rdgName);

signals:
    void sendVectorRdgPairXY (int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, int filterId, const st_rdgInfoData& rdgInfoData);
    void sendLoadMapRdgPairXY(
        int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY,
        const st_rdgInfoData& rdgInfoData, std::map<int, std::vector<QPair<int, int>> >& mapRdgPairXY
    );
    void signalHideTableRdgLogAutoData();
    void sendOutRdgInfo(QString outRdgInfo);
    void signalFixRdgPixels(const std::string& rdgName);
    void signalFixRdgInfo(const std::string& rdgName);

    void signalCreateMapRdgTypeRdgSelectionInfo(
        int selectionId, int rdgHeight, std::string rdgName, const std::vector<QPair<int, int> >& vectorRdgPairXY
    );

    void signalFixDataRdgWidgets(std::string rdgName);

    void signalSetupScrollHRdgData(int minScrollHRdgData, int scrollHRdgData, int maxScrollHRdgdata);
    void signalSetupScrollVRdgData(int minScrollVRdgData, int scrollVRdgData, int maxScrollVRdgdata);

public slots:
    void slotSetupRdgPixels(const st_rdgInfoData& rdgInfoData);
    void slotSendOutRdgInfo(const st_rdgInfoData& rdgInfoData);
    void remarkTrackRdg(const int& trackRdgNumber);

    void slotChangeRdgPixelsFnX(int rdgPixelsFnX);
    void slotChangeRdgPixelsFnY(int rdgPixelsFnY);

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
    bool m_mouseMoveEventAct{false};
    double m_time_step_ns;

    double m_leftLatitude {-1.0};
    double m_rightLatitude {-1.0};
    double m_lowLongitude {-1.0};
    double m_upLongitude {-1.0};

    double m_maxRdg {0.0};
    double m_minRdg {0.0};

    int m_trackRdgNumber{0};

    st_rdgInfoData m_rdgInfoData;
    std::map<std::string, st_rdgInfoData> m_rdgsInfoDataMap;

    int m_inX;
    int m_inY;

    int m_fnX;
    int m_fnY;

    int m_pressX {0};
    int m_pressY {0};

    int m_releaseX {0};
    int m_releaseY {0};

    int m_rdgPixelsCentX {0};
    int m_rdgPixelsCentY {0};

    bool m_wheelActive {false};
    bool m_rdgAllScaling {true};
    bool m_rdgHScaling   {false};
    bool m_rdgVScaling   {false};

    QString m_loadRdgAutoSelectionDataFile;

    std::map<std::string, std::vector<double>>                           m_mapSelectLogPoints;
    std::map<std::string, std::vector<QPair<int, int> > >                m_mapSelectLog10RdgPairXY;
    std::map<std::string, std::map<int, std::vector<QPair<int, int>> > > m_mapLoadLog10RdgPairXY;

    int m_rdgOpenGlHeight;

    double m_minSmallRdg;
    double m_maxSmallRdg;

    double m_minSmallLog10Rdg;
    double m_maxSmallLog10Rdg;

private:
    void outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
    void fixRdg();
};

#endif // RDGGLWIDGET_H
