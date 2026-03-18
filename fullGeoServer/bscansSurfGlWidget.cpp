#include "bscansSurfGlWidget.h"
#include "iostream"
#include "math.h"
#include "transformation.h"
#include <QDateTime>

#define NV 10
#define NH 10

bscansSurfGlWidget::bscansSurfGlWidget()
{
    QColor colorUp  = QColor(0, 255, 0  );
    QColor colorLow = QColor(0,   0, 255);
    setupColorsData(colorUp, colorLow);
}

bscansSurfGlWidget::~bscansSurfGlWidget()
{
    if (m_pixels != nullptr)     delete [] m_pixels;
    if (m_bscansSurfBWPalettes != nullptr) delete [] m_bscansSurfBWPalettes;
}

void bscansSurfGlWidget::setupColorsData(const QColor& colorUp, const QColor& colorLow)
{
    m_vectorTuplesColors = defVectorTuplesColors(colorUp, colorLow);
    m_lowPixelLevel  = 0;
    m_highPixelLevel = m_vectorTuplesColors.size()-1;
}

void bscansSurfGlWidget::setupProperties(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan,  bool showFullBscans, bool resetSectPoints2dBscans, int materialId,
    int absStructInX, int absStructFnX
)
{
    m_materialId = materialId;
    m_filterId    = filterId;
    m_selectionId = selectionId;

    m_highLowOnBscan = highLowOnBscan;

    if (m_showFullBscans != showFullBscans) resetSectPoints2dBscans = true;
    m_showFullBscans = showFullBscans;

    m_quantImpulsesOfPacket = quantImpulsesOfPacket;

    if (m_showFullBscans == true) m_width   = bscansWorkData.bscansSurfWidth;
    else                          m_width   = bscansWorkData.bscansSurfMinWidth;

    m_height  = bscansWorkData.bscansSurfHeight;

    m_leftLatitude    = bscansWorkData.leftLatitude;
    m_rightLatitude   = bscansWorkData.rightLatitude;
    m_lowLongitude    = bscansWorkData.lowLongitude;
    m_upLongitude     = bscansWorkData.upLongitude;

    m_deltaLatitude  = bscansWorkData.deltaLatitude;
    m_deltaLongitude = bscansWorkData.deltaLongitude;

    m_wheelActive = false;

    m_absBscansStructInX = absStructInX;
    m_absBscansStructFnX = (absStructFnX >= m_width-1) ? m_width-1 : absStructFnX;

    defMaxDeep(bscansInfoDataMap, m_materialId, m_filterId, m_selectionId, m_maxDeep, m_absBscansStructInX, m_absBscansStructFnX);

    m_pixelsWidth  = m_absBscansStructFnX-m_absBscansStructInX+1;
    m_pixelsHeight = m_height;

    m_pixelsInX = m_absBscansStructInX;
    m_pixelsInY = 0;

    m_pixelsFnX = m_absBscansStructFnX;
    m_pixelsFnY = m_pixelsHeight-1;

    m_pixelsCentX = (m_pixelsInX + m_pixelsFnX)/2;
    m_pixelsCentY = (m_pixelsInY + m_pixelsFnY)/2;

    outputInitBscansSurfSection(resetSectPoints2dBscans);
}

void bscansSurfGlWidget::outputInitBscansSurfSection(bool resetSectPoints2dBscans)
{
    if (resetSectPoints2dBscans == true)
    {
        m_inSectPointX = m_absBscansStructInX;
        m_inSectPointY = 1;

        m_fnSectPointX = m_absBscansStructFnX;
        m_fnSectPointY = m_height-2;
    }
    if (m_section2dVisible == true) emit updateBscansSurfSections( m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY);
    emit signalFixBscansSurfPixels();
}

void bscansSurfGlWidget::slotCloseSection2dAction(bool section2dVisible)
{
    m_section2dVisible = section2dVisible;
    if (m_section2dVisible == true) emit updateBscansSurfSections( m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY);
    else emit hideBscansSurfSections();
    emit signalFixBscansSurfPixels();
}

void bscansSurfGlWidget::setupBscansSurfPixels(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData)
{
    if (m_pixels != nullptr) delete [] m_pixels;
    m_pixels = new GLubyte [4*m_pixelsWidth*m_pixelsHeight];

    for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
    {
        for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
        {
            int levelColor = 0;
            double depthScanOnTrackBscan = 0.0;
            std::string nameBscan = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*j+i]);
            int kBscan            = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*j+i]);

            if (m_highLowOnBscan)
            {
                if (nameBscan != "") depthScanOnTrackBscan = defDepthScanUpLowBscan(bscansInfoDataMap, nameBscan, kBscan, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                levelColor = (int)((m_vectorTuplesColors.size()-1)*((
                    bscansWorkData.vectorBscansFnRelief[i][j] - bscansWorkData.minRelief + (m_maxDeep - depthScanOnTrackBscan)
                )/(bscansWorkData.maxRelief - bscansWorkData.minRelief + m_maxDeep)));
            }
            else
            {
                if (nameBscan != "") depthScanOnTrackBscan = defDepthScanLowUpBscan(bscansInfoDataMap, nameBscan, kBscan, m_quantImpulsesOfPacket, m_materialId, m_filterId, m_selectionId);
                double               fullDepthScanOnTrackBscan = 0.0;
                if (nameBscan == "") fullDepthScanOnTrackBscan = m_maxDeep;
                else                 fullDepthScanOnTrackBscan = defFullDepthScanLowUpBscan(bscansInfoDataMap, nameBscan, kBscan, m_materialId, m_filterId, m_selectionId);


                levelColor = (int)((m_vectorTuplesColors.size()-1)*((
                    bscansWorkData.vectorBscansFnRelief[i][j]  - bscansWorkData.minRelief +
                    (m_maxDeep - (fullDepthScanOnTrackBscan - depthScanOnTrackBscan))
                )/(bscansWorkData.maxRelief - bscansWorkData.minRelief + m_maxDeep)));
            }

            if (fabs(bscansWorkData.maxRelief - bscansWorkData.minRelief) < deltaParam) includeMaskColorToBscansSurfPixels(levelColor, i, j);
            else
            {
//                if ((fabs(depthScanOnTrackBscan) < deltaParam) && m_highLowOnBscan)
//                {
//                    int maskReliefColor = (int)(122*((bscansWorkData.maxRelief - bscansWorkData.vectorBscansFnRelief[i][j])/(bscansWorkData.maxRelief - bscansWorkData.minRelief)));

//                    m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+0] = 123 + maskReliefColor;
//                    m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+1] = 123 + maskReliefColor;
//                    m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+2] = 123 + maskReliefColor;
//                    m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 255;
//                }
//                else includeMaskColorToBscansSurfPixels(maskColor, i, j);
            }
        }
    }

    if (m_bscansSurfBWPalettes != nullptr) delete [] m_bscansSurfBWPalettes;
    m_bscansSurfBWPalettes = new GLubyte [4*(m_vectorTuplesColors.size())];
    for (int i = 0; i <= m_vectorTuplesColors.size()-1; i++)
    {
        m_bscansSurfBWPalettes[4*i+0] = std::get<0>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
        m_bscansSurfBWPalettes[4*i+1] = std::get<1>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
        m_bscansSurfBWPalettes[4*i+2] = std::get<2>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
        m_bscansSurfBWPalettes[4*i+3] = 255;
    }

    if (m_wheelActive == false)
    {
        m_vectorPairXY.clear();
        if (m_section2dVisible == true)
        {
            addLineToPixelsData(m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY, m_vectorPairXY);
            QString outBscansSurfInfo =
            QString::fromStdString("in sect Latitude ")   + QString::number(m_leftLatitude + m_inSectPointX*m_deltaLatitude) +
            QString::fromStdString(" in sect Longitude ") + QString::number(m_lowLongitude + m_inSectPointY*m_deltaLongitude) +
            QString::fromStdString(" fn sect Latitude ")  + QString::number(m_leftLatitude + m_fnSectPointX*m_deltaLatitude) +
            QString::fromStdString(" fn sect Longitude ") + QString::number(m_lowLongitude + m_fnSectPointY*m_deltaLongitude) +
            QString::fromStdString(".");
            emit sendOutBscansSurfInfo(outBscansSurfInfo);
        }
    }

    if ((m_vectorPairXY.size()> 0) && (m_section2dVisible == true))
    {
        outputVectorbscansSurfSelectArea(
            m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY,
            m_pixelsWidth,   m_vectorPairXY,  m_pixels
        );
    }
    update();
}

void bscansSurfGlWidget::includeMaskColorToBscansSurfPixels(int levelColor, int i, int j)
{
    if (levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel)
    {
        m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+0] =
                std::get<0>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
        m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+1] =
                std::get<1>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
        m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+2] =
                std::get<2>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);

        m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 255;
    }
    else
    {
        m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+0] = 0;
        m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+1] = 0;
        m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+2] = 0;
        m_pixels[4*m_pixelsWidth*((j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 255;
    }
}

void bscansSurfGlWidget::initializeGL()
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

void bscansSurfGlWidget::resizeGL(int width, int height)
{
    m_inX = (0.1/2.0)*width;
    m_inY = (0.25/2.0)*height;

    m_fnX = (1.6/2.0)*width;
    m_fnY = (1.75/2.0)*height;

    m_bscansSurfOpenGlHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void bscansSurfGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if ( m_pixels != nullptr  && (m_pixelsWidth > 0 && m_pixelsHeight > 0))
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)m_pixelsWidth, (GLsizei)m_pixelsHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
        outputTexture(-0.9, -0.75, 0.6, -0.75, 0.6,  0.75, -0.9, 0.75);

        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)1, (GLsizei)m_vectorTuplesColors.size()-1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bscansSurfBWPalettes);
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

void bscansSurfGlWidget::paintEvent(QPaintEvent*)
{
    makeCurrent();
    QPainter p(this);
    p.begin(this);
    p.restore();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    paintGL();

    if ( m_pixels != nullptr  && (m_pixelsWidth > 0 && m_pixelsHeight > 0))
    {
        //Заголовок
        p.setPen(QPen(Qt::white));
        p.setFont(QFont());
        p.drawText(412, 40, QString("All radarograms"));

        //по оси Y слева
        for (int i = 0; i < NH; i++ )
        {
            p.drawText(10, 118+70*i,
                QString::number(m_lowLongitude + (m_pixelsInY + (((double)(i))/((double)(NH-1))) * (m_pixelsFnY - m_pixelsInY))*m_deltaLongitude, 'f', 3)
            );
        }

        p.drawText(35,  100, QString::fromStdString("long"));

        //по оси X
        p.setPen(QPen(Qt::white));
        p.drawText(50,  0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + m_pixelsInX*m_deltaLatitude , 'f', 6));
        p.drawText(120, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 1.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(190, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 2.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(260, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 3.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(335, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 4.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(410, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 5.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(485, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 6.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(560, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 7.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(635, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 8.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(710, 0.90*m_bscansSurfOpenGlHeight, QString::number(m_leftLatitude + (m_pixelsInX + 9.0*(m_pixelsFnX - m_pixelsInX)/(NH-1))*m_deltaLatitude, 'f', 6));
        p.drawText(755, 0.88*m_bscansSurfOpenGlHeight, QString::fromStdString("lat"));

        //по палитре
        p.drawText(800, 100, QString::fromStdString("m"));

        p.drawText(820, 100, QString::number( 0.0, 'f', 4));
        for (int i = 1; i < NV-1; i++ ) p.drawText(820, 100+71*i,  QString::number( (0.0 - i*(m_maxDeep)/(double)(NV-1)) , 'f', 4));
        p.drawText(820, 100+71*(NV-1), QString::number( -1.0*(m_maxDeep), 'f', 4));
    }
    p.end();
}

void bscansSurfGlWidget::mousePressEvent(QMouseEvent* event)
{
    m_pressX = m_absBscansStructInX + (int)((event->x() - m_inX)*((double)m_pixelsWidth /(double)(m_fnX-m_inX)));
    m_pressY = (int)((m_bscansSurfOpenGlHeight - event->y() - m_inY)*((double)m_pixelsHeight/(double)(m_fnY-m_inY)));
}

void bscansSurfGlWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_wheelActive == false) if (m_section2dVisible == true) m_mouseMoveEventAct = true;
}

void bscansSurfGlWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_releaseX = m_absBscansStructInX + (int)((event->x()-m_inX)*((double)m_pixelsWidth/(double) (m_fnX-m_inX)));
    m_releaseY = (int)((m_bscansSurfOpenGlHeight-event->y()-m_inY)*((double)m_pixelsHeight/(double)(m_fnY-m_inY)));

    if (m_wheelActive == false)
    {
        if (m_mouseMoveEventAct == true && m_section2dVisible == true)
        {
            if ( (m_pressX   >= m_absBscansStructInX && m_pressX   <= m_absBscansStructFnX)  &&
                 (m_pressY   >= 0                      && m_pressY   <= m_height-1)      &&
                 (m_releaseX >= m_absBscansStructInX && m_releaseX <= m_absBscansStructFnX)  &&
                 (m_releaseY >= 0                      && m_releaseY <= m_height-1) )
            {
                m_inSectPointX = m_pressX;
                m_fnSectPointX = m_releaseX;

                m_inSectPointY = m_pressY;
                m_fnSectPointY = m_releaseY;

                outputInitBscansSurfSection(false);
            }
            m_mouseMoveEventAct = false;
        }
    }
    else
    {     
        moveOpengl(
            m_pressX, m_pressY, m_releaseX, m_releaseY,
            m_absBscansStructInX, m_absBscansStructFnX, m_height, m_pixelsWidth, m_pixelsHeight,
            m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, m_pixelsCentX,  m_pixelsCentY
        );

        emit signalSetupScrollHData(m_pixelsWidth-1,  m_pixelsFnX, m_absBscansStructFnX);
        emit signalSetupScrollVData(m_pixelsHeight-1, m_pixelsFnY, m_height-1);

        emit signalFixBscansSurfPixels();
    }
}

void bscansSurfGlWidget::wheelEvent(QWheelEvent* event)
{
    double eventDelta = event->delta();

    wheelOpengl(
         eventDelta,  m_allScaling,   m_hScaling,  m_vScaling, m_absBscansStructInX, m_absBscansStructFnX, m_height,
         m_pixelsWidth, m_pixelsHeight, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY,
         m_pixelsCentX, m_pixelsCentY
    );

    emit signalSetupScrollHData(m_pixelsWidth-1,  m_pixelsFnX, m_absBscansStructFnX);
    emit signalSetupScrollVData(m_pixelsHeight-1, m_pixelsFnY, m_height-1);

    if ((m_pixelsWidth == m_absBscansStructFnX-m_absBscansStructInX+1) && (m_pixelsHeight == m_height)) m_wheelActive = false;
    else                                                                                                          m_wheelActive = true;

    emit signalFixBscansSurfPixels();
}

void bscansSurfGlWidget::slotChangeBscansSurfPixelsFnX(int pixelsFnX)
{
    if (pixelsFnX != 0)
    {
        m_pixelsFnX = pixelsFnX;
        m_pixelsInX = m_pixelsFnX - m_pixelsWidth+1;
        m_pixelsCentX = (m_pixelsFnX + m_pixelsInX)/2;
        emit signalFixBscansSurfPixels();
    }
}

void bscansSurfGlWidget::slotChangeBscansSurfPixelsFnY(int pixelsFnY)
{
    if (pixelsFnY != 0)
    {
        m_pixelsFnY = pixelsFnY;
        m_pixelsInY = m_pixelsFnY - m_pixelsHeight+1;
        m_pixelsCentY = (m_pixelsFnY + m_pixelsInY)/2;
        emit signalFixBscansSurfPixels();
    }
}


void bscansSurfGlWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_lowPixelLevel = lowPixelLevel;
    emit signalFixBscansSurfPixels();
}

void bscansSurfGlWidget::setHighPixelLevel(int highPixelLevel)
{
    m_highPixelLevel = highPixelLevel;
    emit signalFixBscansSurfPixels();
}

void bscansSurfGlWidget::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    saveBoundPointsLinePoints(saveBoundPointsLinePointsFile, m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY);
}

void bscansSurfGlWidget::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    loadBoundPointsLinePoints(loadBoundPointsLinePointsFile, m_inSectPointX, m_inSectPointY, m_fnSectPointX, m_fnSectPointY);
    outputInitBscansSurfSection(false);
}

void bscansSurfGlWidget::setupTypeScaling(int typeScaling)
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

void bscansSurfGlWidget::outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)
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
