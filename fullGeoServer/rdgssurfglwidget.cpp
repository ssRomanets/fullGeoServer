#include "rdgsSurfGlWidget.h"
#include "iostream"
#include "math.h"
#include "transformation.h"
#include <QDateTime>

#define NV 10
#define NH 10

rdgsSurfGlWidget::rdgsSurfGlWidget()
{
    QColor colorUp  = QColor(255, 255, 255);
    QColor colorLow = QColor(0,   0,   0);
    setupColorsRdgsSurfData(colorUp, colorLow);
}

rdgsSurfGlWidget::~rdgsSurfGlWidget()
{
    if (m_rdgsSurfPixels != nullptr) delete [] m_rdgsSurfPixels;
    if (m_rdgsSurfBWPalettes != nullptr) delete [] m_rdgsSurfBWPalettes;
}

void rdgsSurfGlWidget::setupColorsRdgsSurfData(const QColor& colorUp, const QColor& colorLow)
{
    m_vectorTuplesColorsRdgsSurf = defVectorTuplesColors(colorUp, colorLow);
    m_lowPixelLevel  = 0;
    m_highPixelLevel = m_vectorTuplesColorsRdgsSurf.size()-1;
}

void rdgsSurfGlWidget::setupRdgsSurfProperties(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg,  bool showFullRdgs, bool resetSectPoints2dRdgs, int materialId,
    int absRdgsStructInX, int absRdgsStructFnX
)
{
    m_materialId = materialId;
    m_highLowOnRdg = highLowOnRdg;

    if (m_showFullRdgs != showFullRdgs) resetSectPoints2dRdgs = true;
    m_showFullRdgs = showFullRdgs;

    m_quantImpulsesOfPacket = quantImpulsesOfPacket;

    if (m_showFullRdgs == true) m_rdgsSurfWidth   = rdgsWorkData.rdgsSurfWidth; else m_rdgsSurfWidth   = rdgsWorkData.rdgsSurfMinWidth;

    m_rdgsSurfHeight  = rdgsWorkData.rdgsSurfHeight;

    m_leftLatitude    = rdgsWorkData.leftLatitude;
    m_rightLatitude   = rdgsWorkData.rightLatitude;
    m_lowLongitude    = rdgsWorkData.lowLongitude;
    m_upLongitude     = rdgsWorkData.upLongitude;

    m_deltaLatitude  = rdgsWorkData.deltaLatitude;
    m_deltaLongitude = rdgsWorkData.deltaLongitude;

    m_filterId    = filterId;
    m_selectionId = selectionId;

    m_wheelActive = false;

    m_absRdgsStructInX = absRdgsStructInX;
    m_absRdgsStructFnX = (absRdgsStructFnX >= m_rdgsSurfWidth-1) ? m_rdgsSurfWidth-1 : absRdgsStructFnX;

    defMaxSumImpulses(rdgsInfoDataMap, m_filterId, m_selectionId, m_maxSumImpulses, m_absRdgsStructInX, m_absRdgsStructFnX);

    m_rdgsSurfPixelsWidth  = m_absRdgsStructFnX-m_absRdgsStructInX+1;
    m_rdgsSurfPixelsHeight = m_rdgsSurfHeight;

    m_rdgsSurfPixelsInX = m_absRdgsStructInX;
    m_rdgsSurfPixelsInY = 0;

    m_rdgsSurfPixelsFnX = m_absRdgsStructFnX;
    m_rdgsSurfPixelsFnY = m_rdgsSurfPixelsHeight-1;

    m_rdgsSurfPixelsCentX = (m_rdgsSurfPixelsInX + m_rdgsSurfPixelsFnX)/2;
    m_rdgsSurfPixelsCentY = (m_rdgsSurfPixelsInY + m_rdgsSurfPixelsFnY)/2;

    outputInitRdgsSurfSection(resetSectPoints2dRdgs);
}

void rdgsSurfGlWidget::outputInitRdgsSurfSection(bool resetSectPoints2dRdgs)
{
    if (resetSectPoints2dRdgs == true)
    {
        m_inSectPointX = m_absRdgsStructInX;
        m_inSectPointY = 1;

        m_fnSectPointX = m_absRdgsStructFnX;
        m_fnSectPointY = m_rdgsSurfHeight-2;
    }
    if (m_section2dVisible == true) emit updateRdgsSurfSections( m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY);
    emit signalFixRdgsSurfPixels();
}

void rdgsSurfGlWidget::slotCloseSection2dAction(bool section2dVisible)
{
    m_section2dVisible = section2dVisible;
    if (m_section2dVisible == true) emit updateRdgsSurfSections( m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY);
    else emit hideRdgsSurfSections();
    emit signalFixRdgsSurfPixels();
}

void rdgsSurfGlWidget::setupRdgsSurfPixels(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData)
{
    if (m_rdgsSurfPixels != nullptr) delete [] m_rdgsSurfPixels;
    m_rdgsSurfPixels = new GLubyte [4*m_rdgsSurfPixelsWidth*m_rdgsSurfPixelsHeight];

    m_minHeightResultRelief = 0.0;
    m_maxHeightResultRelief = 0.0;

    for (int i = m_rdgsSurfPixelsInX; i <= m_rdgsSurfPixelsFnX; i++)
    {
        for (int j = m_rdgsSurfPixelsInY; j <= m_rdgsSurfPixelsFnY; j++)
        {
            int levelColor = 0;
            double depthScanOnTrackRdg = 0.0;
            std::string nameRdg = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*j+i]);
            int kRdg            = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*j+i]);

            if (m_highLowOnRdg)
            {
                if (nameRdg != "") depthScanOnTrackRdg = defDepthScanUpLowRdg(rdgsInfoDataMap, nameRdg, kRdg, m_quantImpulsesOfPacket, m_filterId, m_selectionId);
                levelColor = (int)((m_vectorTuplesColorsRdgsSurf.size()-1)*((
                    rdgsWorkData.vectorRdgsFnRelief[i][j] - rdgsWorkData.minRelief + (m_maxSumImpulses - depthScanOnTrackRdg)*(nanokoef*spc/epsdData(m_materialId))
                )/(rdgsWorkData.maxRelief - rdgsWorkData.minRelief + m_maxSumImpulses*(nanokoef*spc/epsdData(m_materialId)))));
                if (i == m_rdgsSurfPixelsInX && j == m_rdgsSurfPixelsInY)
                {
                    m_minHeightResultRelief = (-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j] - rdgsWorkData.minRelief + (m_maxSumImpulses - depthScanOnTrackRdg)*(nanokoef*spc/epsdData(m_materialId)));
                    m_maxHeightResultRelief = (-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j] - rdgsWorkData.minRelief + (m_maxSumImpulses - depthScanOnTrackRdg)*(nanokoef*spc/epsdData(m_materialId)));
                }
                else
                {
                    if ((-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j] - rdgsWorkData.minRelief + (m_maxSumImpulses - depthScanOnTrackRdg)*(nanokoef*spc/epsdData(m_materialId))) < m_minHeightResultRelief)
                        m_minHeightResultRelief = (-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j] - rdgsWorkData.minRelief + (m_maxSumImpulses - depthScanOnTrackRdg)*(nanokoef*spc/epsdData(m_materialId)));

                    if ((-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j] - rdgsWorkData.minRelief + (m_maxSumImpulses - depthScanOnTrackRdg)*(nanokoef*spc/epsdData(m_materialId))) > m_maxHeightResultRelief)
                        m_maxHeightResultRelief = (-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j] - rdgsWorkData.minRelief + (m_maxSumImpulses - depthScanOnTrackRdg)*(nanokoef*spc/epsdData(m_materialId)));
                }
            }
            else
            {
                if (nameRdg != "") depthScanOnTrackRdg = defDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg, kRdg, m_quantImpulsesOfPacket, m_filterId, m_selectionId);
                double fullDepthScanOnTrackRdg = 0.0;
                if (nameRdg == "") fullDepthScanOnTrackRdg = m_maxSumImpulses;
                else               fullDepthScanOnTrackRdg = defFullDepthScanLowUpRdg(rdgsInfoDataMap, nameRdg, kRdg, m_filterId, m_selectionId);

                levelColor = (int)((m_vectorTuplesColorsRdgsSurf.size()-1)*((
                    rdgsWorkData.vectorRdgsFnRelief[i][j]  - rdgsWorkData.minRelief +
                    (m_maxSumImpulses - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg))*(nanokoef*spc/epsdData(m_materialId))
                )/(rdgsWorkData.maxRelief - rdgsWorkData.minRelief + m_maxSumImpulses*(nanokoef*spc/epsdData(m_materialId)))));

                if (i == m_rdgsSurfPixelsInX && j == m_rdgsSurfPixelsInY)
                {
                    m_minHeightResultRelief =
                            (-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j]  - rdgsWorkData.minRelief +
                            (m_maxSumImpulses - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg))*(nanokoef*spc/epsdData(m_materialId)));
                    m_maxHeightResultRelief =
                            (-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j]  - rdgsWorkData.minRelief +
                            (m_maxSumImpulses - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg))*(nanokoef*spc/epsdData(m_materialId)));
                }
                else
                {
                    if ((-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j]  - rdgsWorkData.minRelief + (m_maxSumImpulses - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg))*(nanokoef*spc/epsdData(m_materialId))) < m_minHeightResultRelief)
                        m_minHeightResultRelief = (-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j]  - rdgsWorkData.minRelief + (m_maxSumImpulses - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg))*(nanokoef*spc/epsdData(m_materialId)));

                    if ((-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j]  - rdgsWorkData.minRelief + (m_maxSumImpulses - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg))*(nanokoef*spc/epsdData(m_materialId))) > m_maxHeightResultRelief)
                        m_maxHeightResultRelief = (-1.0)*(rdgsWorkData.vectorRdgsFnRelief[i][j]  - rdgsWorkData.minRelief + (m_maxSumImpulses - (fullDepthScanOnTrackRdg - depthScanOnTrackRdg))*(nanokoef*spc/epsdData(m_materialId)));
                }
            }

            if (fabs(rdgsWorkData.maxRelief - rdgsWorkData.minRelief) < deltaParam) includeMaskColorToRdgsSurfPixels(levelColor, i, j);
            else
            {
//                if ((fabs(depthScanOnTrackRdg) < deltaParam) && m_highLowOnRdg)
//                {
//                    int maskReliefColor = (int)(122*((rdgsWorkData.maxRelief - rdgsWorkData.vectorRdgsFnRelief[i][j])/(rdgsWorkData.maxRelief - rdgsWorkData.minRelief)));

//                    m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+0] = 123 + maskReliefColor;
//                    m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+1] = 123 + maskReliefColor;
//                    m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+2] = 123 + maskReliefColor;
//                    m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+3] = 255;
//                }
//                else includeMaskColorToRdgsSurfPixels(maskColor, i, j);
            }
        }
    }

    if (m_rdgsSurfBWPalettes != nullptr) delete [] m_rdgsSurfBWPalettes;
    m_rdgsSurfBWPalettes = new GLubyte [4*(m_vectorTuplesColorsRdgsSurf.size())];
    for (int i = 0; i <= m_vectorTuplesColorsRdgsSurf.size()-1; i++)
    {
        m_rdgsSurfBWPalettes[4*i+0] = std::get<0>(m_vectorTuplesColorsRdgsSurf[m_vectorTuplesColorsRdgsSurf.size()-1-i]);
        m_rdgsSurfBWPalettes[4*i+1] = std::get<1>(m_vectorTuplesColorsRdgsSurf[m_vectorTuplesColorsRdgsSurf.size()-1-i]);
        m_rdgsSurfBWPalettes[4*i+2] = std::get<2>(m_vectorTuplesColorsRdgsSurf[m_vectorTuplesColorsRdgsSurf.size()-1-i]);
        m_rdgsSurfBWPalettes[4*i+3] = 255;
    }

    if (m_wheelActive == false)
    {
        m_vectorPairXY.clear();
        if (m_section2dVisible == true)
        {
            addLineToPixelsData(m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY, m_vectorPairXY);
            QString outRdgsSurfInfo =
            QString::fromStdString("in sect Latitude ")   + QString::number(m_leftLatitude + m_inSectPointX*m_deltaLatitude) +
            QString::fromStdString(" in sect Longitude ") + QString::number(m_lowLongitude + m_inSectPointY*m_deltaLongitude) +
            QString::fromStdString(" fn sect Latitude ")  + QString::number(m_leftLatitude + m_fnSectPointX*m_deltaLatitude) +
            QString::fromStdString(" fn sect Longitude ") + QString::number(m_lowLongitude + m_fnSectPointY*m_deltaLongitude) +
            QString::fromStdString(".");
            emit sendOutRdgsSurfInfo(outRdgsSurfInfo);
        }
    }

    if ((m_vectorPairXY.size()> 0) && (m_section2dVisible == true))
    {
        outputVectorRdgsSurfSelectArea(
            m_rdgsSurfPixelsInX, m_rdgsSurfPixelsInY, m_rdgsSurfPixelsFnX, m_rdgsSurfPixelsFnY,
            m_rdgsSurfPixelsWidth,   m_vectorPairXY,  m_rdgsSurfPixels
        );
    }
    update();
}

void rdgsSurfGlWidget::includeMaskColorToRdgsSurfPixels(int levelColor, int i, int j)
{
    if (levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel)
    {
        m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+0] =
                std::get<0>(m_vectorTuplesColorsRdgsSurf[m_vectorTuplesColorsRdgsSurf.size()-1-levelColor]);
        m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+1] =
                std::get<1>(m_vectorTuplesColorsRdgsSurf[m_vectorTuplesColorsRdgsSurf.size()-1-levelColor]);
        m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+2] =
                std::get<2>(m_vectorTuplesColorsRdgsSurf[m_vectorTuplesColorsRdgsSurf.size()-1-levelColor]);

        m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+3] = 255;
    }
    else
    {
        m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+0] = 0;
        m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+1] = 0;
        m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+2] = 0;
        m_rdgsSurfPixels[4*m_rdgsSurfPixelsWidth*((j-m_rdgsSurfPixelsInY))+4*(i-m_rdgsSurfPixelsInX)+3] = 255;
    }
}

void rdgsSurfGlWidget::initializeGL()
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

void rdgsSurfGlWidget::resizeGL(int width, int height)
{
    m_inX = (0.1/2.0)*width;
    m_inY = (0.25/2.0)*height;

    m_fnX = (1.6/2.0)*width;
    m_fnY = (1.75/2.0)*height;

    m_rdgsSurfOpenGlHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void rdgsSurfGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if ( m_rdgsSurfPixels != nullptr  && (m_rdgsSurfPixelsWidth > 0 && m_rdgsSurfPixelsHeight > 0))
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)m_rdgsSurfPixelsWidth, (GLsizei)m_rdgsSurfPixelsHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_rdgsSurfPixels);
        outputTexture(-0.9, -0.75, 0.6, -0.75, 0.6,  0.75, -0.9, 0.75);

        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)1, (GLsizei)m_vectorTuplesColorsRdgsSurf.size()-1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_rdgsSurfBWPalettes);
        outputTexture(0.75, -0.75, 0.8, -0.75,  0.8,  0.75, 0.75, 0.75);
        glDisable(GL_TEXTURE_2D);

        //разметочные линии
        glBegin(GL_LINES);

        //c левой стороны
        for (int i = 0; i < NV; i++)
        {
            glVertex3d(-0.925,  -0.75 + 1.5*i/(NV-1), 1.0);
            glColor3d(  1.0,               1.0, 1.0);
            glVertex3d(-0.9, -0.75 + 1.5*i/(NV-1), 1.0);
            glColor3d ( 1.0,               1.0, 1.0);
        }

        //cнизу
        for (int i = 0; i < NH; i++)
        {
            glVertex3d(-0.9 + 1.5*i/(NH-1),  -0.775, 1.0);
            glColor3d(  1.0,               1.0, 1.0);
            glVertex3d(-0.9 + 1.5*i/(NH-1), -0.75, 1.0);
            glColor3d ( 1.0,               1.0, 1.0);
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

void rdgsSurfGlWidget::paintEvent(QPaintEvent*)
{
    makeCurrent();
    QPainter p(this);
    p.begin(this);
    p.restore();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    paintGL();

    if ( m_rdgsSurfPixels != nullptr  && (m_rdgsSurfPixelsWidth > 0 && m_rdgsSurfPixelsHeight > 0))
    {
        //Заголовок
        p.setPen(QPen(Qt::white));
        p.setFont(QFont());
        p.drawText(412, 40, QString("All radarograms"));

        //по оси Y слева
        for (int i = 0; i < NH; i++ )
        {
            p.drawText(10, 118+70*i, QString::number(m_lowLongitude + (m_rdgsSurfPixelsInY + (((double)(i))/((double)(NH-1))) * (m_rdgsSurfPixelsFnY - m_rdgsSurfPixelsInY))*m_deltaLongitude, 'f', 3));
        }

        p.drawText(35,  100, QString::fromStdString("long"));

        //по оси X
        p.setPen(QPen(Qt::white));
        p.drawText(50,  0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + m_rdgsSurfPixelsInX*m_deltaLatitude , 'f', 3));
        p.drawText(120, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 1.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(190, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 2.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(260, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 3.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(335, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 4.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(410, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 5.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(485, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 6.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(560, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 7.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(635, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 8.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(710, 0.90*m_rdgsSurfOpenGlHeight, QString::number(m_leftLatitude + (m_rdgsSurfPixelsInX + 9.0*(m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsInX)/(NH-1))*m_deltaLatitude, 'f', 3));
        p.drawText(725, 0.88*m_rdgsSurfOpenGlHeight, QString::fromStdString("lat"));

        //по палитре
        p.drawText(800, 100, QString::fromStdString("mm"));

        p.drawText(820, 100, QString::number( m_maxHeightResultRelief, 'f', 2));
        for (int i = 1; i < NV-1; i++ ) p.drawText(820, 100+71*i,  QString::number( (m_maxHeightResultRelief + i*(m_minHeightResultRelief-m_maxHeightResultRelief)/(double)(NV-1)) , 'f', 2));
        p.drawText(820, 100+71*(NV-1), QString::number( m_minHeightResultRelief, 'f', 2));
    }
    p.end();
}

void rdgsSurfGlWidget::mousePressEvent(QMouseEvent* event)
{
    m_pressX = m_absRdgsStructInX + (int)((event->x() - m_inX)*((double)m_rdgsSurfPixelsWidth /(double)(m_fnX-m_inX)));
    m_pressY = (int)((m_rdgsSurfOpenGlHeight - event->y() - m_inY)*((double)m_rdgsSurfPixelsHeight/(double)(m_fnY-m_inY)));
}

void rdgsSurfGlWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_wheelActive == false) if (m_section2dVisible == true) m_mouseMoveEventAct = true;
}

void rdgsSurfGlWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_releaseX = m_absRdgsStructInX + (int)((event->x()-m_inX)*((double)m_rdgsSurfPixelsWidth/(double) (m_fnX-m_inX)));
    m_releaseY = (int)((m_rdgsSurfOpenGlHeight-event->y()-m_inY)*((double)m_rdgsSurfPixelsHeight/(double)(m_fnY-m_inY)));

    if (m_wheelActive == false)
    {
        if (m_mouseMoveEventAct == true && m_section2dVisible == true)
        {
            if ( (m_pressX   >= m_absRdgsStructInX && m_pressX   <= m_absRdgsStructFnX)  &&
                 (m_pressY   >= 0                      && m_pressY   <= m_rdgsSurfHeight-1)      &&
                 (m_releaseX >= m_absRdgsStructInX && m_releaseX <= m_absRdgsStructFnX)  &&
                 (m_releaseY >= 0                      && m_releaseY <= m_rdgsSurfHeight-1) )
            {
                m_inSectPointX = m_pressX;
                m_fnSectPointX = m_releaseX;

                m_inSectPointY = m_pressY;
                m_fnSectPointY = m_releaseY;

                outputInitRdgsSurfSection(false);
            }
            m_mouseMoveEventAct = false;
        }
    }
    else
    {     
        moveOpengl(
            m_pressX, m_pressY, m_releaseX, m_releaseY,
            m_absRdgsStructInX, m_absRdgsStructFnX, m_rdgsSurfHeight, m_rdgsSurfPixelsWidth, m_rdgsSurfPixelsHeight,
            m_rdgsSurfPixelsInX, m_rdgsSurfPixelsInY, m_rdgsSurfPixelsFnX, m_rdgsSurfPixelsFnY, m_rdgsSurfPixelsCentX,  m_rdgsSurfPixelsCentY
        );

        emit signalSetupScrollHRdgsSurfData(m_rdgsSurfPixelsWidth-1,  m_rdgsSurfPixelsFnX, m_absRdgsStructFnX);
        emit signalSetupScrollVRdgsSurfData(m_rdgsSurfPixelsHeight-1, m_rdgsSurfPixelsFnY, m_rdgsSurfHeight-1);

        emit signalFixRdgsSurfPixels();
    }
}

void rdgsSurfGlWidget::wheelEvent(QWheelEvent* event)
{
    double eventDelta = event->delta();

    wheelOpengl(
         eventDelta,  m_rdgsSurfAllScaling,   m_rdgsSurfHScaling,  m_rdgsSurfVScaling, m_absRdgsStructInX, m_absRdgsStructFnX, m_rdgsSurfHeight,
         m_rdgsSurfPixelsWidth, m_rdgsSurfPixelsHeight, m_rdgsSurfPixelsInX, m_rdgsSurfPixelsInY, m_rdgsSurfPixelsFnX, m_rdgsSurfPixelsFnY,
         m_rdgsSurfPixelsCentX, m_rdgsSurfPixelsCentY
    );

    emit signalSetupScrollHRdgsSurfData(m_rdgsSurfPixelsWidth-1,  m_rdgsSurfPixelsFnX, m_absRdgsStructFnX);
    emit signalSetupScrollVRdgsSurfData(m_rdgsSurfPixelsHeight-1, m_rdgsSurfPixelsFnY, m_rdgsSurfHeight-1);

    if ((m_rdgsSurfPixelsWidth == m_absRdgsStructFnX-m_absRdgsStructInX+1) && (m_rdgsSurfPixelsHeight == m_rdgsSurfHeight)) m_wheelActive = false;
    else                                                                                                                            m_wheelActive = true;

    emit signalFixRdgsSurfPixels();
}

void rdgsSurfGlWidget::slotChangeRdgsSurfPixelsFnX(int rdgsSurfPixelsFnX)
{
    if (rdgsSurfPixelsFnX != 0)
    {
        m_rdgsSurfPixelsFnX = rdgsSurfPixelsFnX;
        m_rdgsSurfPixelsInX = m_rdgsSurfPixelsFnX - m_rdgsSurfPixelsWidth+1;
        m_rdgsSurfPixelsCentX = (m_rdgsSurfPixelsFnX + m_rdgsSurfPixelsInX)/2;
        emit signalFixRdgsSurfPixels();
    }
}

void rdgsSurfGlWidget::slotChangeRdgsSurfPixelsFnY(int rdgsSurfPixelsFnY)
{
    if (rdgsSurfPixelsFnY != 0)
    {
        m_rdgsSurfPixelsFnY = rdgsSurfPixelsFnY;
        m_rdgsSurfPixelsInY = m_rdgsSurfPixelsFnY - m_rdgsSurfPixelsHeight+1;
        m_rdgsSurfPixelsCentY = (m_rdgsSurfPixelsFnY + m_rdgsSurfPixelsInY)/2;
        emit signalFixRdgsSurfPixels();
    }
}


void rdgsSurfGlWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_lowPixelLevel = lowPixelLevel;
    emit signalFixRdgsSurfPixels();
}

void rdgsSurfGlWidget::setHighPixelLevel(int highPixelLevel)
{
    m_highPixelLevel = highPixelLevel;
    emit signalFixRdgsSurfPixels();
}

void rdgsSurfGlWidget::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    saveBoundPointsLinePoints(saveBoundPointsLinePointsFile, m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY);
}

void rdgsSurfGlWidget::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    loadBoundPointsLinePoints(loadBoundPointsLinePointsFile, m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY);
    outputInitRdgsSurfSection(false);
}

void rdgsSurfGlWidget::setupRdgsSurfScaling(int typeRdgsSurfScaling)
{
    switch (typeRdgsSurfScaling)
    {
        case (0) :
        {
            m_rdgsSurfAllScaling = true;
            m_rdgsSurfHScaling   = false;
            m_rdgsSurfVScaling   = false;
            break;
        }
        case (1) :
        {
            m_rdgsSurfAllScaling = false;
            m_rdgsSurfHScaling   = true;
            m_rdgsSurfVScaling   = false;
            break;
        }
        case (2) :
        {
            m_rdgsSurfAllScaling = false;
            m_rdgsSurfHScaling   = false;
            m_rdgsSurfVScaling   = true;
            break;
        }
    }
}

void rdgsSurfGlWidget::outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)
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
