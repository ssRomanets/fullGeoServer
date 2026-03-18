#include "bscanGlWidget.h"

#include <QMouseEvent>
#include <iostream>
#include "transformation.h"

#define NV 10
#define NH 10

bscanGlWidget::bscanGlWidget()
{
    m_mapSelectLogPoints.clear();
    m_mapSelectLog10BscanPairXY.clear();
    m_mapLoadLog10BscanPairXY.clear();

    QColor colorUp  = QColor(0, 255,   0);
    QColor colorLow = QColor(0,   0, 255);
    setupColorsData(colorUp, colorLow);
}

bscanGlWidget::~bscanGlWidget()
{
    if (m_pixels   != nullptr) delete [] m_pixels;
    if (m_palette != nullptr) delete []  m_palette;
}

void bscanGlWidget::setupColorsData(const QColor& colorUp, const QColor& colorLow)
{
    m_vectorTuplesColors = defVectorTuplesColors(colorUp, colorLow);
    m_lowPixelLevel  = 0;
    m_highPixelLevel = m_vectorTuplesColors.size()-1;
}

void bscanGlWidget::setShowLogBscan(bool showLogBscan)
{
    m_showLogBscan = showLogBscan;
    if (m_showLogBscan) m_kindInit = " log";
    else m_kindInit = " init";

    m_loadBscanAutoSelectionDataFile = "";
    m_mapLoadLog10BscanPairXY.clear();
}

void bscanGlWidget::setLogBscanMouseSelection(bool logBscanMouseSelect)
{
    m_logBscanMouseSelect = logBscanMouseSelect;
}

void bscanGlWidget::setShowInitBscanSelection(bool showInitBscanSelection)
{
    m_showInitBscanSelect = showInitBscanSelection;
    m_loadBscanAutoSelectionDataFile = "";
    m_mapLoadLog10BscanPairXY.clear();
}

void bscanGlWidget::loadBscanAutoSelectionData(QString loadBscanAutoSelectionDataFile, std::string bscanName)
{
    m_loadBscanAutoSelectionDataFile = loadBscanAutoSelectionDataFile;
    loadBscanAutoSelection(m_loadBscanAutoSelectionDataFile, m_mapLoadLog10BscanPairXY[bscanName]);
    if (m_showLogBscan) emit signalFixPixels(m_bscanName);
}

void bscanGlWidget::resetBscanSelection(bool clearSelectBscanPoints, std::string bscanName)
{
    settingsBscanSelection(false, clearSelectBscanPoints, bscanName);
}

void bscanGlWidget::setupProperties(const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int absPixelsInX, int absPixelsFnX)
{
    if (
        m_bscanName != bscanName                        ||
        m_width != bscanInfoData.vectorBscanData.size() ||
        m_height != bscanInfoData.quantImpulsesOfPacket ||
        absPixelsInX != m_absPixelsInX                  ||
        absPixelsFnX != m_absPixelsFnX
    )
    {
        m_trackNumber = 0;

        m_appearingBscan = true;
        m_wheelActive = false;

        m_width  = bscanInfoData.vectorBscanData.size();
        m_height = bscanInfoData.quantImpulsesOfPacket;

        m_time_step_ns = bscanInfoData.vectorBscanData[0].time_step_ns;

        if (m_bscanName == bscanName)
        {
            m_absPixelsInX = (absPixelsInX == 0) ? 0 : absPixelsInX;
            m_absPixelsFnX = (absPixelsFnX == 0) ? ((m_width < limitBscanWidth) ? m_width-1 : limitBscanWidth-1):absPixelsFnX;
        }
        else
        {
            m_absPixelsInX = 0;
            m_absPixelsFnX = (m_width < limitBscanWidth) ? m_width-1 : limitBscanWidth-1;
            m_bscanName = bscanName;
        }

        m_pixelsInX    = m_absPixelsInX;
        m_pixelsInY    = 0;

        m_pixelsFnX    = m_absPixelsFnX;
        m_pixelsFnY    = m_height-1;

        m_pixelsWidth  = m_absPixelsFnX-m_absPixelsInX+1;
        m_pixelsHeight = m_height;

        m_pixelsCentX  = (m_pixelsInX + m_pixelsFnX)/2;
        m_pixelsCentY  = (m_pixelsInY + m_pixelsFnY)/2;

        emit signalSetupScrollHData(0, 0, 0);
        emit signalSetupScrollVData(0, 0, 0);
    }

    if (m_showLogBscan == false)
    {
        m_maxBscan = bscanInfoData.vectorBscanData[0].vectorMaxImpulses[m_filterId];
        m_minBscan = bscanInfoData.vectorBscanData[0].vectorMinImpulses[m_filterId];
        for (int i=m_absPixelsInX; i<=m_absPixelsFnX; i++)
        {
            if (bscanInfoData.vectorBscanData[i].vectorMaxImpulses[m_filterId] > m_maxBscan) m_maxBscan = bscanInfoData.vectorBscanData[i].vectorMaxImpulses[m_filterId];
            if (bscanInfoData.vectorBscanData[i].vectorMinImpulses[m_filterId] < m_maxBscan) m_minBscan = bscanInfoData.vectorBscanData[i].vectorMinImpulses[m_filterId];
        }
    }

    emit signalFixPixels(m_bscanName);
}

void bscanGlWidget::slotSetupPixels(const st_bscanInfoData& bscanInfoData)
{
    m_leftLatitude  = -1.0;
    m_rightLatitude = -1.0;
    m_lowLongitude  = -1.0;
    m_upLongitude   = -1.0;

    for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)  devMainLatLongParameters(bscanInfoData, i, m_leftLatitude, m_rightLatitude, m_lowLongitude, m_upLongitude);

    if (m_pixels != nullptr) delete [] m_pixels;
    m_pixels = new GLubyte [4*m_pixelsWidth*m_pixelsHeight];

    for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
    {
        for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
        {
            m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+0] = 0;
            m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+1] = 0;
            m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+2] = 0;
            m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 0;
        }
    }

    if (m_palette != nullptr) delete [] m_palette;
    m_palette = new GLubyte [4*(m_vectorTuplesColors.size())];
    for (int i = 0; i <= m_vectorTuplesColors.size()-1; i++)
    {
        m_palette[4*i+0] = std::get<0>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
        m_palette[4*i+1] = std::get<1>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
        m_palette[4*i+2] = std::get<2>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
        m_palette[4*i+3] = 255;
    }

    int levelColor   = 0;

    std::vector<int> maskVector;
    maskVector.resize(m_pixelsWidth*m_pixelsHeight);
    for (int i = 0; i <= m_pixelsWidth*m_pixelsHeight-1; i++) maskVector[i] = 0;

    if (m_showLogBscan == false)
    {
        for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
        {
            for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
            {
                levelColor = (int)((m_vectorTuplesColors.size()-1)*(((bscanInfoData.vectorBscanData[i].vectorsDoubleData[m_filterId])[j] - m_minBscan)/(m_maxBscan - m_minBscan)));
                if ((levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel))
                    maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] = 1;
            }
        }

        m_minSmallBscan = 0.0;
        m_maxSmallBscan = 0.0;
        for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
        {
            for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
            {
                if (maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] == 1)
                {
                    if (i == m_pixelsInX && j == m_pixelsInY)
                        m_minSmallBscan = (bscanInfoData.vectorBscanData[i].vectorsDoubleData[m_filterId])[j];
                    else if (m_minSmallBscan > (bscanInfoData.vectorBscanData[i].vectorsDoubleData[m_filterId])[j])
                        m_minSmallBscan = (bscanInfoData.vectorBscanData[i].vectorsDoubleData[m_filterId])[j];

                    if (i == m_pixelsInX && j == m_pixelsInY)
                        m_maxSmallBscan = (bscanInfoData.vectorBscanData[i].vectorsDoubleData[m_filterId])[j];
                    else if (m_maxSmallBscan < (bscanInfoData.vectorBscanData[i].vectorsDoubleData[m_filterId])[j])
                        m_maxSmallBscan = (bscanInfoData.vectorBscanData[i].vectorsDoubleData[m_filterId])[j];
                }
            }
        }

        if ((fabs(m_minSmallBscan) > 0.0) || (fabs(m_maxSmallBscan) > 0.0))
        {
            for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
            {
                for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
                {
                    if (maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] == 1)
                    {
                        levelColor = (int)((m_vectorTuplesColors.size()-1)*
                        (((bscanInfoData.vectorBscanData[i].vectorsDoubleData[m_filterId])[j] - m_minSmallBscan)/(m_maxSmallBscan - m_minSmallBscan)));

                        m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+0] =
                                std::get<0>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                        m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+1] =
                                std::get<1>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                        m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+2] =
                                std::get<2>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                        m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 255;
                    }
                }
            }
        }
    }
    else
    {
        double bscanLog10 = 0.0;
        for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
        {
            for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
            {
                bscanLog10 = defbscanLog10(bscanInfoData,m_filterId,i,j);

                levelColor = (int)((m_vectorTuplesColors.size()-1)*((fabs(bscanLog10 - bscanInfoData.vectorMinLog10BscanData[m_filterId]))/
                (bscanInfoData.vectorMaxLog10BscanData[m_filterId] - bscanInfoData.vectorMinLog10BscanData[m_filterId])));

                if ((levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel))
                    maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] = 1;
            }
        }

        m_minSmallLog10Bscan = 0.0;
        m_maxSmallLog10Bscan = 0.0;
        for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
        {
            for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
            {
                if (maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] == 1)
                {
                    if (i == m_pixelsInX && j == m_pixelsInY)
                        m_minSmallLog10Bscan = defbscanLog10(bscanInfoData,m_filterId,i,j);
                    else if (m_minSmallLog10Bscan > defbscanLog10(bscanInfoData,m_filterId,i,j))
                        m_minSmallLog10Bscan = defbscanLog10(bscanInfoData,m_filterId,i,j);

                    if (i == 0 && j == 0)
                        m_maxSmallLog10Bscan = defbscanLog10(bscanInfoData,m_filterId,i,j);
                    else if (m_maxSmallLog10Bscan < defbscanLog10(bscanInfoData,m_filterId,i,j))
                        m_maxSmallLog10Bscan = defbscanLog10(bscanInfoData,m_filterId,i,j);
                }
            }
        }

        if ((fabs(m_minSmallLog10Bscan) > 0.0) || (fabs(m_maxSmallLog10Bscan) > 0.0))
        {
            for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
            {
                for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
                {
                    if (maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] == 1)
                    {
                        bscanLog10 = defbscanLog10(bscanInfoData,m_filterId,i,j);
                        levelColor = (int)((m_vectorTuplesColors.size()-1)*(fabs(bscanLog10 - m_minSmallLog10Bscan)/(m_maxSmallLog10Bscan - m_minSmallLog10Bscan)));

                        m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+0] =
                                std::get<0>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                        m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+1] =
                                std::get<1>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                        m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+2] =
                                std::get<2>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                        m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 255;
                    }
                }
            }
        }
    }

    if (m_showLogBscan)
    {
        if (m_logBscanMouseSelect)
        {
            if (m_mapSelectLogPoints[m_bscanName].size() > 0)
            {
                for (int count = 0; count < m_mapSelectLogPoints[m_bscanName].size()/4; count++)
                {
                    addLineToPixelsData(
                        m_mapSelectLogPoints[m_bscanName][4*count],   m_mapSelectLogPoints[m_bscanName][4*count+1],
                        m_mapSelectLogPoints[m_bscanName][4*count+2], m_mapSelectLogPoints[m_bscanName][4*count+3],
                        m_mapSelectLog10BscanPairXY[m_bscanName]
                    );
                }
            }
            outputVectorBscanSelectArea(m_pixelsWidth, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, m_mapSelectLog10BscanPairXY[m_bscanName], m_pixels);
            emit signalCreateMapBscanTypeBscanSelectionInfo(1, m_height, m_bscanName, m_mapSelectLog10BscanPairXY[m_bscanName]);
        }
        else if (m_logBscanAutoSelect)
        {
            outputMapBscanSelectArea(m_pixelsWidth, m_pixelsHeight, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, bscanInfoData.vectorMapAutoLog10BscanPairXY[m_filterId], m_pixels);
        }

        if (m_loadBscanAutoSelectionDataFile != "" && m_mapLoadLog10BscanPairXY[m_bscanName].size() > 0)
        {
            outputMapBscanSelectArea(m_pixelsWidth, m_pixelsHeight, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, m_mapLoadLog10BscanPairXY[m_bscanName], m_pixels);
        }
    }

    if (m_showInitBscanSelect)
    {
        if    (m_mapSelectLog10BscanPairXY[m_bscanName].size() > 0)
        {
            outputVectorBscanSelectArea(m_pixelsWidth, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, m_mapSelectLog10BscanPairXY[m_bscanName], m_pixels);
            emit signalCreateMapBscanTypeBscanSelectionInfo(1, m_height, m_bscanName, m_mapSelectLog10BscanPairXY[m_bscanName]);
        }
        else if (bscanInfoData.vectorMapAutoLog10BscanPairXY[m_filterId].size() > 0)
        {
            outputMapBscanSelectArea(m_pixelsWidth, m_pixelsHeight, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, bscanInfoData.vectorMapAutoLog10BscanPairXY[m_filterId], m_pixels);
        }
    }

    if (m_logBscanAutoSelect && bscanInfoData.vectorsBscanLog10PairXY[m_filterId].size() > 0)
        emit sendVectorBscanPairXY(m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, m_filterId, bscanInfoData);
    else if (m_loadBscanAutoSelectionDataFile != "" && m_mapLoadLog10BscanPairXY.size() > 0)
        emit sendLoadMapBscanPairXY(m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, bscanInfoData, m_mapLoadLog10BscanPairXY[m_bscanName]);
    else
        emit signalHideTableBscanLogAutoData();

    for (int i=m_pixelsInX; i <= m_pixelsFnX; i++)
    {
        for (int j=m_pixelsInY; j <= m_pixelsFnY; j++)
        {
            if (m_trackNumber > m_pixelsInX && m_trackNumber < m_pixelsFnX && i == m_trackNumber)
            {
                m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+0] = 255;
                m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+1] =   0;
                m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+2] =   0;
                m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 255;
            }
        }
    }

    update();
}

void bscanGlWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void bscanGlWidget::resizeGL(int width, int height)
{
    m_inX = (0.1/2.0)*width;
    m_inY = (0.25/2.0)*height;

    m_fnX = (1.6/2.0)*width;
    m_fnY = (1.75/2.0)*height;

    m_bscanOpenGlHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void bscanGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_pixels != nullptr && (m_pixelsWidth > 0 && m_pixelsHeight > 0))
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)m_pixelsWidth, (GLsizei)m_pixelsHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
        outputTexture(-0.9, -0.75, 0.6, -0.75, 0.6,  0.75, -0.9, 0.75);

        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)1, (GLsizei)m_vectorTuplesColors.size()-1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_palette);
        outputTexture(0.75, -0.75, 0.8, -0.75,  0.8,  0.75, 0.75, 0.75);

        glDisable(GL_TEXTURE_2D);

        //разметочные линии b-скана
        glBegin(GL_LINES);

        glVertex3d(-0.925,  -0.75, 1.0);
        glColor3d ( 1.0,   1.0, 1.0);
        glVertex3d( 0.625,  -0.75, 1.0);
        glColor3d ( 1.0,    1.0, 1.0);

        //c левой стороны
        for (int i = 0; i < NV; i++)
        {
            glVertex3d(-0.925,  -0.75 + 1.5*i/(NV-1), 1.0);
            glColor3d(  1.0,               1.0, 1.0);
            glVertex3d(-0.9, -0.75 + 1.5*i/(NV-1), 1.0);
            glColor3d ( 1.0,               1.0, 1.0);
        }

        glVertex3d(-0.9,  -0.75, 1.0);
        glColor3d ( 1.0,   1.0, 1.0);
        glVertex3d( -0.9,  0.75, 1.0);
        glColor3d ( 1.0,    1.0, 1.0);

        //cнизу
        for (int i = 0; i < NH; i++)
        {
            glVertex3d(-0.9 + 1.5*i/(NH-1),  -0.775, 1.0);
            glColor3d(  1.0,               1.0, 1.0);
            glVertex3d(-0.9 + 1.5*i/(NH-1), -0.75, 1.0);
            glColor3d ( 1.0,               1.0, 1.0);
        }

        glVertex3d(0.6,  -0.75, 1.0);
        glColor3d (1.0,   1.0, 1.0);
        glVertex3d(0.6,  0.75, 1.0);
        glColor3d (1.0,    1.0, 1.0);

        //c правой стороны
        for (int i = 0; i < NV; i++)
        {
            glVertex3d(0.6, 0.75 - (1.5)*i/(NV-1), 1.0);
            glColor3d (1.0,  1.0,               1.0);
            glVertex3d(0.625,  0.75 - (1.5)*i/(NV-1), 1.0);
            glColor3d (1.0,  1.0,               1.0);
        }
        glEnd();

        //разметочные линии палитры
        glBegin(GL_LINES);

        glVertex3d(0.75, -0.75, 1.0);
        glColor3d (1.0,   1.0, 1.0);
        glVertex3d(0.8, -0.75, 1.0);
        glColor3d (1.0,    1.0, 1.0);

        glVertex3d(0.8, -0.75, 1.0);
        glColor3d (1.0,   1.0, 1.0);
        glVertex3d(0.8,  0.75, 1.0);
        glColor3d (1.0,    1.0, 1.0);

        for (int i = 0; i < NV; i++)
        {
            glVertex3d(0.8, 0.75 - (1.5)*i/(NV-1), 1.0);
            glColor3d (1.0,  1.0,               1.0);
            glVertex3d(0.825,  0.75 - (1.5)*i/(NV-1), 1.0);
            glColor3d (1.0,  1.0,               1.0);
        }

        glVertex3d(0.8,  0.75, 1.0);
        glColor3d (1.0,   1.0, 1.0);
        glVertex3d(0.75, 0.75, 1.0);
        glColor3d (1.0,    1.0, 1.0);

        glVertex3d(0.75, 0.75, 1.0);
        glColor3d (1.0,   1.0, 1.0);
        glVertex3d(0.75, -0.75, 1.0);
        glColor3d (1.0,    1.0, 1.0);

        glEnd();
    }
}

void bscanGlWidget::paintEvent(QPaintEvent* )
{
    makeCurrent();
    QPainter p(this);
    p.begin(this);
    p.restore();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    paintGL();

    if (m_pixels != nullptr && (m_pixelsWidth > 0 && m_pixelsHeight > 0))
    {
        //Заголовок
        p.setPen(QPen(Qt::white));
        p.drawText(300, 40, QString::fromStdString(m_bscanName+m_kindInit));

        p.drawText(35, 100, QString::fromStdString("ref"));

        //по оси Y слева
        for (int i = 0; i < NV-1; i++ )  p.drawText(10, 110+71*i, QString::number(m_pixelsInY + i*(m_pixelsFnY-m_pixelsInY)/(NV-1)));
        p.drawText(10,  110+71*(NV-1), QString::number((m_pixelsFnY)));

        //по оси X
        p.setPen(QPen(Qt::white));
        p.drawText(30,  780, QString::number( m_leftLatitude, 'f', 6));
        p.drawText(30,  795, QString::number( m_lowLongitude, 'f', 6));

        p.drawText(107,  780, QString::number( m_leftLatitude + 1.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 6 ));
        p.drawText(107,  795, QString::number( m_lowLongitude + 1.0*(m_upLongitude   - m_lowLongitude)/(NH-1), 'f', 6 ));

        p.drawText(184,  780, QString::number( m_leftLatitude + 2.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 6 ));
        p.drawText(184,  795, QString::number( m_lowLongitude + 2.0*(m_upLongitude   - m_lowLongitude)/(NH-1), 'f', 6 ));

        p.drawText(262,  780, QString::number( m_leftLatitude + 3.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 6 ));
        p.drawText(262,  795, QString::number( m_lowLongitude + 3.0*(m_upLongitude   - m_lowLongitude)/(NH-1), 'f', 6 ));

        p.drawText(345,  780, QString::number( m_leftLatitude + 4.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 6 ));
        p.drawText(345,  795, QString::number( m_lowLongitude + 4.0*(m_upLongitude   - m_lowLongitude)/(NH-1), 'f', 6 ));

        p.drawText(422,  780, QString::number( m_leftLatitude + 5.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 6 ));
        p.drawText(422,  795, QString::number( m_lowLongitude + 5.0*(m_upLongitude   - m_lowLongitude)/(NH-1), 'f', 6 ));

        p.drawText(504,  780, QString::number( m_leftLatitude + 6.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 6 ));
        p.drawText(504,  795, QString::number( m_lowLongitude + 6.0*(m_upLongitude   - m_lowLongitude)/(NH-1), 'f', 6 ));

        p.drawText(582,  780, QString::number( m_leftLatitude + 7.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 6 ));
        p.drawText(582,  795, QString::number( m_lowLongitude + 7.0*(m_upLongitude   - m_lowLongitude)/(NH-1), 'f', 6 ));

        p.drawText(664,  780, QString::number( m_leftLatitude + 8.0*(m_rightLatitude - m_leftLatitude)/(NH-1),  'f', 6 ));
        p.drawText(664,  795, QString::number( m_lowLongitude + 8.0*(m_upLongitude   - m_lowLongitude)/(NH-1),  'f', 6 ));

        p.drawText(739,  780, QString::number( m_leftLatitude + 9.0*(m_rightLatitude - m_leftLatitude)/(NH-1),  'f', 6 ));
        p.drawText(739,  795, QString::number( m_lowLongitude + 9.0*(m_upLongitude   - m_lowLongitude)/(NH-1),  'f', 6 ));

        p.drawText(812,  780, QString::fromStdString("lat"));
        p.drawText(812,  795, QString::fromStdString("long"));

        //по оси Y справа
        p.drawText(730, 100, QString::fromStdString("ns"));

        p.drawText(750, 100, QString::number(m_pixelsInY*m_time_step_ns, 'f', 2));
        for (int i = 1; i < NV-1; i++ ) p.drawText(750, 110+71*i,  QString::number(m_pixelsInY*m_time_step_ns + i*(m_pixelsFnY-m_pixelsInY)*m_time_step_ns/(NV-1), 'f', 2));
        p.drawText(750, 110+71*(NV-1), QString::number(m_pixelsInY*m_time_step_ns + (m_pixelsFnY-m_pixelsInY)*m_time_step_ns, 'f', 2));

        //по палитре
        if      (m_showLogBscan == false) p.drawText(780, 100, QString::fromStdString("imp"));
        else if (m_showLogBscan == true)  p.drawText(780, 100, QString::fromStdString("log imp"));

        if (m_showLogBscan == false)
        {
            p.drawText(835, 105, QString::number(m_maxSmallBscan, 'f', 6));
            for (int i = 1; i < NV-1; i++ ) p.drawText(835, 105+71*i,  QString::number((m_maxSmallBscan + i*(m_minSmallBscan-m_maxSmallBscan)/(double)(NV-1)), 'f', 6));
            p.drawText(835, 105+71*(NV-1), QString::number(m_minSmallBscan, 'f', 6));
        }
        else if (m_showLogBscan == true)
        {
            p.drawText(835, 105, QString::number(m_maxSmallLog10Bscan, 'f', 2));
            for (int i = 1; i < NV-1; i++ ) p.drawText(835, 105+71*i,  QString::number( (m_maxSmallLog10Bscan + i*(m_minSmallLog10Bscan-m_maxSmallLog10Bscan)/(double)(NV-1)) , 'f', 2));
            p.drawText(835, 105+71*(NV-1), QString::number( m_minSmallLog10Bscan, 'f', 2));
        }
    }
    p.end();
}

void bscanGlWidget::slotSendOutBscanInfo(const st_bscanInfoData& bscanInfoData)
{
    QString outBscanInfo = "";
    double depthAtImpTrackBscan = (m_pressY == 0) ?
    bscanInfoData.vectorBscanData[m_pressX].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][m_pressY] :
    (
        bscanInfoData.vectorBscanData[m_pressX].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][m_pressY] -
        bscanInfoData.vectorBscanData[m_pressX].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId][m_pressY-1]
    );

    outBscanInfo =
    QString::fromStdString("impId ") + QString::number(m_pressX) +
    QString::fromStdString(" refId ") + QString::number(m_pressY) +
    QString::fromStdString(" delay ns ") + QString::number(m_pressY*m_time_step_ns) +
    QString::fromStdString(" depth in ref ") + QString::number(depthAtImpTrackBscan) +
    QString::fromStdString(" m.");
    emit sendOutBscanInfo(outBscanInfo);
}

void bscanGlWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_appearingBscan == true)
    {
        m_pressX = (int)((event->x()-m_inX)*                    ((double)m_pixelsWidth/(double) (m_fnX-m_inX)));
        m_pressY = (int)((m_bscanOpenGlHeight - event->y() - m_inY)*((double)m_pixelsHeight/(double)(m_fnY-m_inY)));
        if (m_wheelActive == false)  {   emit signalFixBscanInfo(m_bscanName); }
    }
}

void bscanGlWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_appearingBscan == true) if (m_wheelActive == false) if ((m_showLogBscan == true) && (m_logBscanMouseSelect == true))  m_mouseMoveEventAct = true;
}

void bscanGlWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_appearingBscan == true)
    {
        m_releaseX = (int)((event->x()-m_inX)*                    ((double)m_pixelsWidth /(double)(m_fnX-m_inX)));
        m_releaseY = (int)((m_bscanOpenGlHeight - event->y()-m_inY)*((double)m_pixelsHeight/(double)(m_fnY-m_inY)));

        if (m_wheelActive == false)
        {
            if (m_mouseMoveEventAct == true)
            {
                if ((m_showLogBscan == true) && (m_logBscanMouseSelect == true))
                {
                    if ((m_pressX   >= 0 && m_pressX   <= m_pixelsWidth  -1) &&
                        (m_pressY   >= 0 && m_pressY   <= m_pixelsHeight -1) &&
                        (m_releaseX >= 0 && m_releaseX <= m_pixelsWidth  -1) &&
                        (m_releaseY >= 0 && m_releaseY <= m_pixelsHeight -1) )
                    {
                        m_mapSelectLogPoints[m_bscanName].push_back(m_pressX);
                        m_mapSelectLogPoints[m_bscanName].push_back(m_pressY);

                        m_mapSelectLogPoints[m_bscanName].push_back(m_releaseX);
                        m_mapSelectLogPoints[m_bscanName].push_back(m_releaseY);
                        emit signalFixPixels(m_bscanName);
                    }
                }
                m_mouseMoveEventAct = false;
            }
        }
        else
        {
            moveOpengl(
                m_pressX, m_pressY, m_releaseX, m_releaseY,
                m_absPixelsInX, m_absPixelsFnX, m_height, m_pixelsWidth, m_pixelsHeight,
                m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, m_pixelsCentX,  m_pixelsCentY
            );

            emit signalSetupScrollHData(m_pixelsWidth-1,  m_pixelsFnX, m_absPixelsFnX);
            emit signalSetupScrollVData(m_pixelsHeight-1, m_pixelsFnY, m_height-1);

            fixBscan();
        }
    }
}

void bscanGlWidget::wheelEvent(QWheelEvent* event)
{
    if (m_appearingBscan == true)
    {
        double eventDelta = event->delta();

        wheelOpengl(
             eventDelta,  m_allScaling,   m_hScaling,  m_vScaling, m_absPixelsInX, m_absPixelsFnX, m_height,
             m_pixelsWidth, m_pixelsHeight, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY,
             m_pixelsCentX, m_pixelsCentY
        );

        emit signalSetupScrollHData(m_pixelsWidth-1,  m_pixelsFnX, m_absPixelsFnX);
        emit signalSetupScrollVData(m_pixelsHeight-1, m_pixelsFnY, m_height-1);

        if ((m_pixelsWidth == m_absPixelsFnX-m_absPixelsInX+1) && (m_pixelsHeight == m_height)) m_wheelActive = false;
        else                                                                                    m_wheelActive = true;

        fixBscan();
    }
}

void bscanGlWidget::slotChangePixelsFnX(int pixelsFnX)
{
    if (pixelsFnX != 0)
    {
        m_pixelsFnX = pixelsFnX;
        m_pixelsInX = m_pixelsFnX - m_pixelsWidth+1;
        m_pixelsCentX = (m_pixelsFnX + m_pixelsInX)/2;

        fixBscan();
    }
}

void bscanGlWidget::slotChangePixelsFnY(int pixelsFnY)
{
    if (pixelsFnY != 0)
    {
        m_pixelsFnY = pixelsFnY;
        m_pixelsInY = m_pixelsFnY - m_pixelsHeight+1;
        m_pixelsCentY = (m_pixelsFnY + m_pixelsInY)/2;
        fixBscan();
    }
}

void bscanGlWidget::fixBscan()
{
    emit signalFixPixels(m_bscanName);
    emit signalFixDataBscanWidgets(m_bscanName);
}

void bscanGlWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_lowPixelLevel = lowPixelLevel;
    if (m_pixels != nullptr){ emit signalFixPixels(m_bscanName); }
}

void bscanGlWidget::setHighPixelLevel(int highPixelLevel)
{
    m_highPixelLevel = highPixelLevel;
    if (m_pixels != nullptr){ emit signalFixPixels(m_bscanName); }
}

void bscanGlWidget::remarkTrack(const int& trackNumber)
{
    m_trackNumber = trackNumber;
    if (m_pixels != nullptr) { emit signalFixPixels(m_bscanName); }
}

void bscanGlWidget::settingsBscanSelection(bool logBscanAutoSelect, bool clearSelectBscanPoints, std::string bscanName)
{
    m_logBscanAutoSelect = logBscanAutoSelect;
    if (clearSelectBscanPoints) {  clearSelectedData(bscanName); }
    m_loadBscanAutoSelectionDataFile = "";
}

void bscanGlWidget::clearSelectedData(std::string bscanName)
{
    if (m_mapSelectLog10BscanPairXY[bscanName].size() > 0) m_mapSelectLog10BscanPairXY[bscanName].resize(0);
    if (m_mapSelectLogPoints     [bscanName].size() > 0)   m_mapSelectLogPoints     [bscanName].clear();
    if (m_mapLoadLog10BscanPairXY  [bscanName].size() > 0) m_mapLoadLog10BscanPairXY  [bscanName].clear();
}

void bscanGlWidget::setupTypeScaling(int typeScaling)
{
    switch (typeScaling)
    {
        case (0) :
        {
            m_allScaling = true;
            m_hScaling   = false;
            m_vScaling   = false;
            break;
        }
        case (1) :
        {
            m_allScaling = false;
            m_hScaling   = true;
            m_vScaling   = false;
            break;
        }
        case (2) :
        {
            m_allScaling = false;
            m_hScaling   = false;
            m_vScaling   = true;
            break;
        }
    }
}

void bscanGlWidget::outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLE_STRIP);

    glTexCoord2f(0.0f, 0.0f);
    glVertex4f(X1, Y1, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex4f(X2, Y2, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex4f(X3, Y3, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex4f(X4, Y4, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex4f(X1, Y1, 1.0f, 1.0f);
    glEnd();
}
