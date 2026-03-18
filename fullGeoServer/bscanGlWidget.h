#ifndef bscanGLWIDGET_H
#define bscanGLWIDGET_H

#include <QPainter>
#include <QMouseEvent>
#include <QPen>

#include <QtOpenGL>
#include <QOpenGLFunctions_1_1>
#include "bscansData.h"

class bscanGlWidget : public QGLWidget, public QOpenGLFunctions_1_1
{
    Q_OBJECT

public:
    bscanGlWidget();
    ~bscanGlWidget();

    std::string m_kindInit{" Init"};

    QPainter m_painter;

    int m_materialId     {0};
    int m_filterId       {0};
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

    bool m_appearingBscan {false};
    bool m_showLogBscan          {false};
    bool m_logBscanMouseSelect      {false};
    bool m_logBscanAutoSelect      {false};
    bool m_showInitBscanSelect {false};

    std::vector<std::tuple<int, int, int>> m_vectorTuplesColors;

    void setupColorsData(const QColor& colorUp, const QColor& colorLow);

    void setAppearingBscan(bool appearBscan);
    void setShowLogBscan(bool showLogBscan);
    void setLogBscanMouseSelection(bool logBscanMouseSelect);
    void setupTypeScaling(int typeScaling);

    void settingsBscanSelection(bool logBscanAutoSelect, bool clearSelectBscanPoints, std::string bscanName);
    void loadBscanAutoSelectionData(QString loadBscanAutoSelectionDataFile, std::string bscanName);

    void setShowInitBscanSelection(bool showInitBscanSelection);
    void resetBscanSelection(bool clearSelectBscanPoints, std::string bscanName);

    void setupProperties(
        const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int absPixelsInX, int absPixelsFnX
    );

    void setLowPixelLevel(int lowPixelLevel);
    void setHighPixelLevel(int highPixelLevel);

    void clearSelectedData(std::string bscanName);

signals:
    void sendVectorBscanPairXY (int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, int filterId, const st_bscanInfoData& bscanInfoData);
    void sendLoadMapBscanPairXY(
        int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY,
        const st_bscanInfoData& bscanInfoData, std::map<int, std::vector<QPair<int, int>> >& mapBscanPairXY
    );
    void signalHideTableBscanLogAutoData();
    void sendOutBscanInfo(QString outBscanInfo);
    void signalFixPixels(const std::string& bscanName);
    void signalFixBscanInfo(const std::string& bscanName);

    void signalCreateMapBscanTypeBscanSelectionInfo(
        int selectionId, int bscanHeight, std::string bscanName, const std::vector<QPair<int, int> >& vectorBscanPairXY
    );

    void signalFixDataBscanWidgets(std::string bscanName);

    void signalSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData);
    void signalSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData);

public slots:
    void slotSetupPixels(const st_bscanInfoData& bscanInfoData);
    void slotSendOutBscanInfo(const st_bscanInfoData& bscanInfoData);
    void remarkTrack(const int& trackNumber);

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
    bool m_mouseMoveEventAct{false};
    double m_time_step_ns;

    double m_leftLatitude {-1.0};
    double m_rightLatitude {-1.0};
    double m_lowLongitude {-1.0};
    double m_upLongitude {-1.0};

    double m_maxBscan {0.0};
    double m_minBscan {0.0};

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

    QString m_loadBscanAutoSelectionDataFile;

    std::map<std::string, std::vector<double>>                           m_mapSelectLogPoints;
    std::map<std::string, std::vector<QPair<int, int> > >                m_mapSelectLog10BscanPairXY;
    std::map<std::string, std::map<int, std::vector<QPair<int, int>> > > m_mapLoadLog10BscanPairXY;

    int m_bscanOpenGlHeight;

    double m_minSmallBscan;
    double m_maxSmallBscan;

    double m_minSmallLog10Bscan;
    double m_maxSmallLog10Bscan;

private:
    void outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
    void fixBscan();
};

#endif // bscanGLWIDGET_H
