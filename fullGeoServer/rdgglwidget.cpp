#include "rdgGlWidget.h"

#include <QMouseEvent>
#include <iostream>
#include "transformation.h"

#define NV 10
#define NH 10

rdgGlWidget::rdgGlWidget()
{
    m_mapSelectLogPoints.clear();
    m_mapSelectLog10RdgPairXY.clear();
    m_mapLoadLog10RdgPairXY.clear();

    QColor colorUp  = QColor(255, 255, 255);
    QColor colorLow = QColor(0,   0,   0);
    setupColorsRdgData(colorUp, colorLow);
}

rdgGlWidget::~rdgGlWidget()
{
    if (m_rdgPixels   != nullptr) delete [] m_rdgPixels;
    if (m_rdgBWPalettes != nullptr) delete [] m_rdgBWPalettes;
}

void rdgGlWidget::setupColorsRdgData(const QColor& colorUp, const QColor& colorLow)
{
    m_vectorTuplesColorsRdg = defVectorTuplesColors(colorUp, colorLow);
    m_lowPixelLevel  = 0;
    m_highPixelLevel = m_vectorTuplesColorsRdg.size()-1;
}

void rdgGlWidget::setAppearingRdg(bool appearingRdg)
{
    m_appearingRdg = appearingRdg;
}

void rdgGlWidget::setShowLogRdg(bool showLogRdg)
{
    m_showLogRdg = showLogRdg;
    if (m_showLogRdg) m_kindInit = " log";
    else m_kindInit = " init";

    m_loadRdgAutoSelectionDataFile = "";
    m_mapLoadLog10RdgPairXY.clear();
}

void rdgGlWidget::setLogRdgMouseSelection(bool logRdgMouseSelect)
{
    m_logRdgMouseSelect = logRdgMouseSelect;
}

void rdgGlWidget::setShowInitRdgSelection(bool showInitRdgSelection)
{
    m_showInitRdgSelect = showInitRdgSelection;
    m_loadRdgAutoSelectionDataFile = "";
    m_mapLoadLog10RdgPairXY.clear();
}

void rdgGlWidget::loadRdgAutoSelectionData(QString loadRdgAutoSelectionDataFile, std::string rdgName)
{
    m_loadRdgAutoSelectionDataFile = loadRdgAutoSelectionDataFile;
    loadRdgAutoSelection(m_loadRdgAutoSelectionDataFile, m_mapLoadLog10RdgPairXY[rdgName]);
    if (m_showLogRdg) emit signalFixRdgPixels(m_rdgName);
}

void rdgGlWidget::resetRdgSelection(bool clearSelectRdgPoints, std::string rdgName)
{
    settingsRdgSelection(false, clearSelectRdgPoints, rdgName);
}

void rdgGlWidget::setupRdgProperties(const std::string& rdgName, const st_rdgInfoData& rdgInfoData, int materialId, int absRdgPixelsInX, int absRdgPixelsFnX)
{
    if (m_rdgName != rdgName || m_rdgWidth != rdgInfoData.vectorRdgData.size() || m_rdgHeight != rdgInfoData.quantImpulsesOfPacket)
    {
        m_rdgName = rdgName;
        m_trackRdgNumber = 0;

        m_appearingRdg = true;
        m_wheelActive = false;

        m_rdgWidth = rdgInfoData.vectorRdgData.size();
        m_rdgHeight = rdgInfoData.quantImpulsesOfPacket;

        m_time_step_ns = rdgInfoData.vectorRdgData[0].time_step_ns;
    }

    if (m_showLogRdg == false)
    {
        m_maxRdg = rdgInfoData.vectorRdgData[0].vectorMaxImpulses[m_filterId];
        m_minRdg = rdgInfoData.vectorRdgData[0].vectorMinImpulses[m_filterId];
        for (int i=1; i<m_rdgWidth; i++)
        {
            if (rdgInfoData.vectorRdgData[i].vectorMaxImpulses[m_filterId] > m_maxRdg) m_maxRdg = rdgInfoData.vectorRdgData[i].vectorMaxImpulses[m_filterId];
            if (rdgInfoData.vectorRdgData[i].vectorMinImpulses[m_filterId] < m_maxRdg) m_minRdg = rdgInfoData.vectorRdgData[i].vectorMinImpulses[m_filterId];
        }
    }

    m_materialId = materialId;

    m_absRdgPixelsInX = (absRdgPixelsInX == 0) ? 0 : absRdgPixelsInX;
    m_absRdgPixelsFnX = (absRdgPixelsFnX == 0) ? ((m_rdgWidth < limitRdgWidth) ? m_rdgWidth-1 : limitRdgWidth-1):absRdgPixelsFnX;

    m_rdgPixelsInX    = m_absRdgPixelsInX;
    m_rdgPixelsInY    = 0;

    m_rdgPixelsFnX    = m_absRdgPixelsFnX;
    m_rdgPixelsFnY    = m_rdgHeight-1;

    m_rdgPixelsWidth  = m_absRdgPixelsFnX-m_absRdgPixelsInX+1;
    m_rdgPixelsHeight = m_rdgHeight;

    m_rdgPixelsCentX  = (m_rdgPixelsInX + m_rdgPixelsFnX)/2;
    m_rdgPixelsCentY  = (m_rdgPixelsInY + m_rdgPixelsFnY)/2;

    emit signalFixRdgPixels(m_rdgName);
}

void rdgGlWidget::slotSetupRdgPixels(const st_rdgInfoData& rdgInfoData)
{
    m_leftLatitude  = -1.0;
    m_rightLatitude = -1.0;
    m_lowLongitude  = -1.0;
    m_upLongitude   = -1.0;

    for (int i = m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)  devMainLatLongParameters(rdgInfoData, i, m_leftLatitude, m_rightLatitude, m_lowLongitude, m_upLongitude);

    if (m_rdgPixels != nullptr) delete [] m_rdgPixels;
    m_rdgPixels = new GLubyte [4*m_rdgPixelsWidth*m_rdgPixelsHeight];

    for (int i = m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)
    {
        for (int j = m_rdgPixelsInY; j <= m_rdgPixelsFnY; j++)
        {
            m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+0] = 0;
            m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+1] = 0;
            m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+2] = 0;
            m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+3] = 0;
        }
    }

    if (m_rdgBWPalettes != nullptr) delete [] m_rdgBWPalettes;
    m_rdgBWPalettes = new GLubyte [4*(m_vectorTuplesColorsRdg.size())];
    for (int i = 0; i <= m_vectorTuplesColorsRdg.size()-1; i++)
    {
        m_rdgBWPalettes[4*i+0] = std::get<0>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-i]);
        m_rdgBWPalettes[4*i+1] = std::get<1>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-i]);
        m_rdgBWPalettes[4*i+2] = std::get<2>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-i]);
        m_rdgBWPalettes[4*i+3] = 255;
    }

    int levelColor   = 0;

    std::vector<int> maskRdgVector;
    maskRdgVector.resize(m_rdgPixelsWidth*m_rdgPixelsHeight);
    for (int i = 0; i <= m_rdgPixelsWidth*m_rdgPixelsHeight-1; i++) maskRdgVector[i] = 0;

    if (m_showLogRdg == false)
    {
        for (int i = m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)
        {
            for (int j = m_rdgPixelsInY; j <= m_rdgPixelsFnY; j++)
            {
                levelColor = (int)((m_vectorTuplesColorsRdg.size()-1)*(((rdgInfoData.vectorRdgData[i].vectorsDoubleData[m_filterId])[j] - m_minRdg)/(m_maxRdg - m_minRdg)));
                if ((levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel))
                    maskRdgVector[m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+(i-m_rdgPixelsInX)] = 1;
            }
        }

        m_minSmallRdg = 0.0;
        m_maxSmallRdg = 0.0;
        for (int i = m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)
        {
            for (int j = m_rdgPixelsInY; j <= m_rdgPixelsFnY; j++)
            {
                if (maskRdgVector[m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+(i-m_rdgPixelsInX)] == 1)
                {
                    if (i == m_rdgPixelsInX && j == m_rdgPixelsInY)
                        m_minSmallRdg = (rdgInfoData.vectorRdgData[i].vectorsDoubleData[m_filterId])[j];
                    else if (m_minSmallRdg > (rdgInfoData.vectorRdgData[i].vectorsDoubleData[m_filterId])[j])
                        m_minSmallRdg = (rdgInfoData.vectorRdgData[i].vectorsDoubleData[m_filterId])[j];

                    if (i == m_rdgPixelsInX && j == m_rdgPixelsInY)
                        m_maxSmallRdg = (rdgInfoData.vectorRdgData[i].vectorsDoubleData[m_filterId])[j];
                    else if (m_maxSmallRdg < (rdgInfoData.vectorRdgData[i].vectorsDoubleData[m_filterId])[j])
                        m_maxSmallRdg = (rdgInfoData.vectorRdgData[i].vectorsDoubleData[m_filterId])[j];
                }
            }
        }

        if ((fabs(m_minSmallRdg) > 0.0) || (fabs(m_maxSmallRdg) > 0.0))
        {
            for (int i = m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)
            {
                for (int j = m_rdgPixelsInY; j <= m_rdgPixelsFnY; j++)
                {
                    if (maskRdgVector[m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+(i-m_rdgPixelsInX)] == 1)
                    {
                        levelColor = (int)((m_vectorTuplesColorsRdg.size()-1)*(((rdgInfoData.vectorRdgData[i].vectorsDoubleData[m_filterId])[j] - m_minSmallRdg)/(m_maxSmallRdg - m_minSmallRdg)));

                        m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+0] = std::get<0>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-levelColor]);
                        m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+1] = std::get<1>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-levelColor]);
                        m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+2] = std::get<2>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-levelColor]);
                        m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+3] = 255;
                    }
                }
            }
        }
    }
    else
    {
        double rdgLog10 = 0.0;
        for (int i = m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)
        {
            for (int j = m_rdgPixelsInY; j <= m_rdgPixelsFnY; j++)
            {
                rdgLog10 = defRdgLog10(rdgInfoData,m_filterId,i,j);
                levelColor = (int)((m_vectorTuplesColorsRdg.size()-1)*((fabs(rdgLog10 - rdgInfoData.vectorMinLog10RdgData[m_filterId]))/(rdgInfoData.vectorMaxLog10RdgData[m_filterId] - rdgInfoData.vectorMinLog10RdgData[m_filterId])));
                if ((levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel))   maskRdgVector[m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+(i-m_rdgPixelsInX)] = 1;
            }
        }

        m_minSmallLog10Rdg = 0.0;
        m_maxSmallLog10Rdg = 0.0;
        for (int i = m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)
        {
            for (int j = m_rdgPixelsInY; j <= m_rdgPixelsFnY; j++)
            {
                if (maskRdgVector[m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+(i-m_rdgPixelsInX)] == 1)
                {
                    if (i == m_rdgPixelsInX && j == m_rdgPixelsInY)             m_minSmallLog10Rdg = defRdgLog10(rdgInfoData,m_filterId,i,j);
                    else if (m_minSmallLog10Rdg > defRdgLog10(rdgInfoData,m_filterId,i,j)) m_minSmallLog10Rdg = defRdgLog10(rdgInfoData,m_filterId,i,j);

                    if (i == 0 && j == 0)                                       m_maxSmallLog10Rdg = defRdgLog10(rdgInfoData,m_filterId,i,j);
                    else if (m_maxSmallLog10Rdg < defRdgLog10(rdgInfoData,m_filterId,i,j)) m_maxSmallLog10Rdg = defRdgLog10(rdgInfoData,m_filterId,i,j);
                }
            }
        }

        if ((fabs(m_minSmallLog10Rdg) > 0.0) || (fabs(m_maxSmallLog10Rdg) > 0.0))
        {
            for (int i = m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)
            {
                for (int j = m_rdgPixelsInY; j <= m_rdgPixelsFnY; j++)
                {
                    if (maskRdgVector[m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+(i-m_rdgPixelsInX)] == 1)
                    {
                        rdgLog10 = defRdgLog10(rdgInfoData,m_filterId,i,j);
                        levelColor = (int)((m_vectorTuplesColorsRdg.size()-1)*(fabs(rdgLog10 - m_minSmallLog10Rdg)/(m_maxSmallLog10Rdg - m_minSmallLog10Rdg)));

                        m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+0] = std::get<0>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-levelColor]);
                        m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+1] = std::get<1>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-levelColor]);
                        m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+2] = std::get<2>(m_vectorTuplesColorsRdg[m_vectorTuplesColorsRdg.size()-1-levelColor]);
                        m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+3] = 255;
                    }
                }
            }
        }
    }

    if (m_showLogRdg)
    {
        if (m_logRdgMouseSelect)
        {
            if (m_mapSelectLogPoints[m_rdgName].size() > 0)
            {
                for (int count = 0; count < m_mapSelectLogPoints[m_rdgName].size()/4; count++)
                {
                    addLineToPixelsData(
                        m_mapSelectLogPoints[m_rdgName][4*count],   m_mapSelectLogPoints[m_rdgName][4*count+1],
                        m_mapSelectLogPoints[m_rdgName][4*count+2], m_mapSelectLogPoints[m_rdgName][4*count+3],
                        m_mapSelectLog10RdgPairXY[m_rdgName]
                    );
                }
            }
            outputVectorRdgSelectArea(m_rdgPixelsWidth, m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY, m_mapSelectLog10RdgPairXY[m_rdgName], m_rdgPixels);
            emit signalCreateMapRdgTypeRdgSelectionInfo(1, m_rdgHeight, m_rdgName, m_mapSelectLog10RdgPairXY[m_rdgName]);
        }
        else if (m_logRdgAutoSelect)
        {
            outputMapRdgSelectArea(m_rdgPixelsWidth, m_rdgPixelsHeight, m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY, rdgInfoData.vectorMapAutoLog10RdgPairXY[m_filterId], m_rdgPixels);
        }

        if (m_loadRdgAutoSelectionDataFile != "" && m_mapLoadLog10RdgPairXY[m_rdgName].size() > 0)
        {
            outputMapRdgSelectArea(m_rdgPixelsWidth, m_rdgPixelsHeight, m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY, m_mapLoadLog10RdgPairXY[m_rdgName], m_rdgPixels);
        }
    }

    if (m_showInitRdgSelect)
    {
        if    (m_mapSelectLog10RdgPairXY[m_rdgName].size() > 0)
        {
            outputVectorRdgSelectArea(m_rdgPixelsWidth, m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY, m_mapSelectLog10RdgPairXY[m_rdgName], m_rdgPixels);
            emit signalCreateMapRdgTypeRdgSelectionInfo(1, m_rdgHeight, m_rdgName, m_mapSelectLog10RdgPairXY[m_rdgName]);
        }
        else if (rdgInfoData.vectorMapAutoLog10RdgPairXY[m_filterId].size() > 0)
        {
            outputMapRdgSelectArea(m_rdgPixelsWidth, m_rdgPixelsHeight, m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY, rdgInfoData.vectorMapAutoLog10RdgPairXY[m_filterId], m_rdgPixels);
        }
    }

    if (m_logRdgAutoSelect && rdgInfoData.vectorsRdgLog10PairXY[m_filterId].size() > 0)  emit sendVectorRdgPairXY(m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY, m_filterId, rdgInfoData);
    else if (m_loadRdgAutoSelectionDataFile != "" && m_mapLoadLog10RdgPairXY.size() > 0) emit sendLoadMapRdgPairXY(m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY, rdgInfoData, m_mapLoadLog10RdgPairXY[m_rdgName]);
    else                                                                                 emit signalHideTableRdgLogAutoData();

    for (int i=m_rdgPixelsInX; i <= m_rdgPixelsFnX; i++)
    {
        for (int j=m_rdgPixelsInY; j <= m_rdgPixelsFnY; j++)
        {
            if (m_trackRdgNumber > m_rdgPixelsInX && m_trackRdgNumber < m_rdgPixelsFnX && i == m_trackRdgNumber)
            {
                m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+0] = 255;
                m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+1] =   0;
                m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+2] =   0;
                m_rdgPixels[4*m_rdgPixelsWidth*(m_rdgPixelsHeight-1-(j-m_rdgPixelsInY))+4*(i-m_rdgPixelsInX)+3] = 255;
            }
        }
    }

    update();
}

void rdgGlWidget::initializeGL()
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

void rdgGlWidget::resizeGL(int width, int height)
{
    m_inX = (0.1/2.0)*width;
    m_inY = (0.25/2.0)*height;

    m_fnX = (1.6/2.0)*width;
    m_fnY = (1.75/2.0)*height;

    m_rdgOpenGlHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void rdgGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_rdgPixels != nullptr && (m_rdgPixelsWidth > 0 && m_rdgPixelsHeight > 0))
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)m_rdgPixelsWidth, (GLsizei)m_rdgPixelsHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_rdgPixels);
        outputTexture(-0.9, -0.75, 0.6, -0.75, 0.6,  0.75, -0.9, 0.75);

        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)1, (GLsizei)m_vectorTuplesColorsRdg.size()-1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_rdgBWPalettes);
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

void rdgGlWidget::paintEvent(QPaintEvent* )
{
    makeCurrent();
    QPainter p(this);
    p.begin(this);
    p.restore();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    paintGL();

    if (m_rdgPixels != nullptr && (m_rdgPixelsWidth > 0 && m_rdgPixelsHeight > 0))
    {
        //Заголовок
        p.setPen(QPen(Qt::white));
        p.drawText(300, 40, QString::fromStdString(m_rdgName+m_kindInit));

        p.drawText(35, 100, QString::fromStdString("ref"));

        //по оси Y слева
        for (int i = 0; i < NV-1; i++ )  p.drawText(10, 110+71*i, QString::number(m_rdgPixelsInY + i*(m_rdgPixelsFnY-m_rdgPixelsInY)/(NV-1)));
        p.drawText(10,  110+71*(NV-1), QString::number((m_rdgPixelsFnY)));

        //по оси X
        p.setPen(QPen(Qt::white));
        p.drawText(30,  780, QString::number( m_leftLatitude, 'f', 3));
        p.drawText(30,  795, QString::number( m_lowLongitude, 'f', 3));

        p.drawText(107,  780, QString::number( m_leftLatitude + 1.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 3 ));
        p.drawText(107,  795, QString::number( m_lowLongitude + 1.0*(m_upLongitude - m_lowLongitude)/(NH-1),   'f', 3 ));

        p.drawText(184,  780, QString::number( m_leftLatitude + 2.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 3 ));
        p.drawText(184,  795, QString::number( m_lowLongitude + 2.0*(m_upLongitude - m_lowLongitude)/(NH-1),   'f', 3 ));

        p.drawText(262,  780, QString::number( m_leftLatitude + 3.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 3 ));
        p.drawText(262,  795, QString::number( m_lowLongitude + 3.0*(m_upLongitude - m_lowLongitude)/(NH-1),   'f', 3 ));

        p.drawText(345,  780, QString::number( m_leftLatitude + 4.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 3 ));
        p.drawText(345,  795, QString::number( m_lowLongitude + 4.0*(m_upLongitude - m_lowLongitude)/(NH-1),   'f', 3 ));

        p.drawText(422,  780, QString::number( m_leftLatitude + 5.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 3 ));
        p.drawText(422,  795, QString::number( m_lowLongitude + 5.0*(m_upLongitude - m_lowLongitude)/(NH-1),   'f', 3 ));

        p.drawText(504,  780, QString::number( m_leftLatitude + 6.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 3 ));
        p.drawText(504,  795, QString::number( m_lowLongitude + 6.0*(m_upLongitude - m_lowLongitude)/(NH-1) ,  'f', 3 ));

        p.drawText(582,  780, QString::number( m_leftLatitude + 7.0*(m_rightLatitude - m_leftLatitude)/(NH-1), 'f', 3 ));
        p.drawText(582,  795, QString::number( m_lowLongitude + 7.0*(m_upLongitude - m_lowLongitude)/(NH-1),   'f', 3 ));

        p.drawText(664,  780, QString::number( m_leftLatitude + 8.0*(m_rightLatitude - m_leftLatitude)/(NH-1),  'f', 3 ));
        p.drawText(664,  795, QString::number( m_lowLongitude + 8.0*(m_upLongitude - m_lowLongitude)/(NH-1) ,   'f', 3));

        p.drawText(739,  780, QString::number( m_leftLatitude + 9.0*(m_rightLatitude - m_leftLatitude)/(NH-1),  'f', 3 ));
        p.drawText(739,  795, QString::number( m_lowLongitude + 9.0*(m_upLongitude - m_lowLongitude)/(NH-1),    'f', 3 ));

        p.drawText(782,  780, QString::fromStdString("lat"));
        p.drawText(782,  795, QString::fromStdString("long"));

        //по оси Y справа
        p.drawText(730, 100, QString::fromStdString("ns"));

        p.drawText(750, 100, QString::number(m_rdgPixelsInY*m_time_step_ns, 'f', 2));
        for (int i = 1; i < NV-1; i++ ) p.drawText(750, 110+71*i,  QString::number(m_rdgPixelsInY*m_time_step_ns + i*(m_rdgPixelsFnY-m_rdgPixelsInY)*m_time_step_ns/(NV-1), 'f', 2));
        p.drawText(750, 110+71*(NV-1), QString::number(m_rdgPixelsInY*m_time_step_ns + (m_rdgPixelsFnY-m_rdgPixelsInY)*m_time_step_ns, 'f', 2));

        //по палитре
        if      (m_showLogRdg == false) p.drawText(800, 100, QString::fromStdString("mm"));
        else if (m_showLogRdg == true)  p.drawText(800, 100, QString::fromStdString(""));

        if (m_showLogRdg == false)
        {
            p.drawText(835, 105, QString::number( (nanokoef*spc/epsdData(m_materialId))*m_maxSmallRdg, 'f', 2));
            for (int i = 1; i < NV-1; i++ ) p.drawText(835, 105+71*i,  QString::number( (nanokoef*spc/epsdData(m_materialId))*(m_maxSmallRdg + i*(m_minSmallRdg-m_maxSmallRdg)/(double)(NV-1)) , 'f', 2));
            p.drawText(835, 105+71*(NV-1), QString::number( (nanokoef*spc/epsdData(m_materialId))*m_minSmallRdg, 'f', 2));
        }
        else if (m_showLogRdg == true)
        {
            p.drawText(835, 105, QString::number(m_maxSmallLog10Rdg, 'f', 2));
            for (int i = 1; i < NV-1; i++ ) p.drawText(835, 105+71*i,  QString::number( (m_maxSmallLog10Rdg + i*(m_minSmallLog10Rdg-m_maxSmallLog10Rdg)/(double)(NV-1)) , 'f', 2));
            p.drawText(835, 105+71*(NV-1), QString::number( m_minSmallLog10Rdg, 'f', 2));
        }
    }
    p.end();
}

void rdgGlWidget::slotSendOutRdgInfo(const st_rdgInfoData& rdgInfoData)
{
    QString outRdgInfo = "";
    if (m_showLogRdg == false)
    {
        outRdgInfo =
        QString::fromStdString("impId ") + QString::number(m_pressX) +
        QString::fromStdString(" refId ") + QString::number(m_pressY) +
        QString::fromStdString(" delay ns ") + QString::number(m_pressY*m_time_step_ns) +
        QString::fromStdString(" depth in ref ") +
        QString::number((rdgInfoData.vectorRdgData[m_pressX].vectorsDoubleData[m_filterId])[m_pressY]*(nanokoef*spc/epsdData(m_materialId))) +
        QString::fromStdString(" mm.");
    }
    else
    {
        outRdgInfo =
        QString::fromStdString("impId ") + QString::number(m_pressX) +
        QString::fromStdString(" refId ") + QString::number(m_pressY) +
        QString::fromStdString(" delay ns ") + QString::number(m_pressY*m_time_step_ns) +
        QString::fromStdString(" log depth ") +
        QString::number(log10(1 + (rdgInfoData.vectorRdgData[m_pressX].vectorsDoubleData[m_filterId])[m_pressY]));
    }
    emit sendOutRdgInfo(outRdgInfo);
}

void rdgGlWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_appearingRdg == true)
    {
        m_pressX = (int)((event->x()-m_inX)*                    ((double)m_rdgPixelsWidth/(double) (m_fnX-m_inX)));
        m_pressY = (int)((m_rdgOpenGlHeight - event->y() - m_inY)*((double)m_rdgPixelsHeight/(double)(m_fnY-m_inY)));
        if (m_wheelActive == false)  {   emit signalFixRdgInfo(m_rdgName); }
    }
}

void rdgGlWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_appearingRdg == true) if (m_wheelActive == false) if ((m_showLogRdg == true) && (m_logRdgMouseSelect == true))  m_mouseMoveEventAct = true;
}

void rdgGlWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_appearingRdg == true)
    {
        m_releaseX = (int)((event->x()-m_inX)*                    ((double)m_rdgPixelsWidth /(double)(m_fnX-m_inX)));
        m_releaseY = (int)((m_rdgOpenGlHeight - event->y()-m_inY)*((double)m_rdgPixelsHeight/(double)(m_fnY-m_inY)));

        if (m_wheelActive == false)
        {
            if (m_mouseMoveEventAct == true)
            {
                if ((m_showLogRdg == true) && (m_logRdgMouseSelect == true))
                {
                    if ((m_pressX   >= 0 && m_pressX   <= m_rdgPixelsWidth  -1) &&
                        (m_pressY   >= 0 && m_pressY   <= m_rdgPixelsHeight -1) &&
                        (m_releaseX >= 0 && m_releaseX <= m_rdgPixelsWidth  -1) &&
                        (m_releaseY >= 0 && m_releaseY <= m_rdgPixelsHeight -1) )
                    {
                        m_mapSelectLogPoints[m_rdgName].push_back(m_pressX);
                        m_mapSelectLogPoints[m_rdgName].push_back(m_pressY);

                        m_mapSelectLogPoints[m_rdgName].push_back(m_releaseX);
                        m_mapSelectLogPoints[m_rdgName].push_back(m_releaseY);
                        emit signalFixRdgPixels(m_rdgName);
                    }
                }
                m_mouseMoveEventAct = false;
            }
        }
        else
        {
            moveOpengl(
                m_pressX, m_pressY, m_releaseX, m_releaseY,
                m_absRdgPixelsInX, m_absRdgPixelsFnX, m_rdgHeight, m_rdgPixelsWidth, m_rdgPixelsHeight,
                m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY, m_rdgPixelsCentX,  m_rdgPixelsCentY
            );

            emit signalSetupScrollHRdgData(m_rdgPixelsWidth-1,  m_rdgPixelsFnX, m_absRdgPixelsFnX);
            emit signalSetupScrollVRdgData(m_rdgPixelsHeight-1, m_rdgPixelsFnY, m_rdgHeight-1);

            fixRdg();
        }
    }
}

void rdgGlWidget::wheelEvent(QWheelEvent* event)
{
    if (m_appearingRdg == true)
    {
        double eventDelta = event->delta();

        wheelOpengl(
             eventDelta,  m_rdgAllScaling,   m_rdgHScaling,  m_rdgVScaling, m_absRdgPixelsInX, m_absRdgPixelsFnX, m_rdgHeight,
             m_rdgPixelsWidth, m_rdgPixelsHeight, m_rdgPixelsInX, m_rdgPixelsInY, m_rdgPixelsFnX, m_rdgPixelsFnY,
             m_rdgPixelsCentX, m_rdgPixelsCentY
        );

        emit signalSetupScrollHRdgData(m_rdgPixelsWidth-1,  m_rdgPixelsFnX, m_absRdgPixelsFnX);
        emit signalSetupScrollVRdgData(m_rdgPixelsHeight-1, m_rdgPixelsFnY, m_rdgHeight-1);

        if ((m_rdgPixelsWidth == m_absRdgPixelsFnX-m_absRdgPixelsInX+1) && (m_rdgPixelsHeight == m_rdgHeight)) m_wheelActive = false;
        else                                                                                                   m_wheelActive = true;

        fixRdg();
    }
}

void rdgGlWidget::slotChangeRdgPixelsFnX(int rdgPixelsFnX)
{
    if (rdgPixelsFnX != 0)
    {
        m_rdgPixelsFnX = rdgPixelsFnX;
        m_rdgPixelsInX = m_rdgPixelsFnX - m_rdgPixelsWidth+1;
        m_rdgPixelsCentX = (m_rdgPixelsFnX + m_rdgPixelsInX)/2;

        fixRdg();
    }
}

void rdgGlWidget::slotChangeRdgPixelsFnY(int rdgPixelsFnY)
{
    if (rdgPixelsFnY != 0)
    {
        m_rdgPixelsFnY = rdgPixelsFnY;
        m_rdgPixelsInY = m_rdgPixelsFnY - m_rdgPixelsHeight+1;
        m_rdgPixelsCentY = (m_rdgPixelsFnY + m_rdgPixelsInY)/2;
        fixRdg();
    }
}

void rdgGlWidget::fixRdg()
{
    emit signalFixRdgPixels(m_rdgName);
    emit signalFixDataRdgWidgets(m_rdgName, m_materialId);
}

void rdgGlWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_lowPixelLevel = lowPixelLevel;
    if (m_rdgPixels != nullptr){ emit signalFixRdgPixels(m_rdgName); }
}

void rdgGlWidget::setHighPixelLevel(int highPixelLevel)
{
    m_highPixelLevel = highPixelLevel;
    if (m_rdgPixels != nullptr){ emit signalFixRdgPixels(m_rdgName); }
}

void rdgGlWidget::remarkTrackRdg(const int& trackRdgNumber)
{
    m_trackRdgNumber = trackRdgNumber;
    if (m_rdgPixels != nullptr) { emit signalFixRdgPixels(m_rdgName); }
}

void rdgGlWidget::settingsRdgSelection(bool logRdgAutoSelect, bool clearSelectRdgPoints, std::string rdgName)
{
    m_logRdgAutoSelect = logRdgAutoSelect;
    if (clearSelectRdgPoints) {  clearSelectedData(rdgName); }
    m_loadRdgAutoSelectionDataFile = "";
}

void rdgGlWidget::clearSelectedData(std::string rdgName)
{
    if (m_mapSelectLog10RdgPairXY[rdgName].size() > 0) m_mapSelectLog10RdgPairXY[rdgName].resize(0);
    if (m_mapSelectLogPoints     [rdgName].size() > 0) m_mapSelectLogPoints     [rdgName].clear();
    if (m_mapLoadLog10RdgPairXY  [rdgName].size() > 0) m_mapLoadLog10RdgPairXY  [rdgName].clear();
}

void rdgGlWidget::setupRdgScaling(int typeRdgScaling)
{
    switch (typeRdgScaling)
    {
        case (0) :
        {
            m_rdgAllScaling = true;
            m_rdgHScaling   = false;
            m_rdgVScaling   = false;
            break;
        }
        case (1) :
        {
            m_rdgAllScaling = false;
            m_rdgHScaling   = true;
            m_rdgVScaling   = false;
            break;
        }
        case (2) :
        {
            m_rdgAllScaling = false;
            m_rdgHScaling   = false;
            m_rdgVScaling   = true;
            break;
        }
    }
}

void rdgGlWidget::outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)
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
