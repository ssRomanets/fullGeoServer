#include "bscanDeepGlWidget.h"

#include <QMouseEvent>
#include <iostream>
#include "transformation.h"

#define NV 10
#define NH 10

bscanDeepGlWidget::bscanDeepGlWidget()
{
    QColor colorUp  = QColor(0, 255, 0);
    QColor colorLow = QColor(0, 0, 255);
    setupColorsData(colorUp, colorLow);
}

bscanDeepGlWidget::~bscanDeepGlWidget()
{
    if (m_pixels   != nullptr) delete [] m_pixels;
    if (m_palette != nullptr) delete [] m_palette;
}

void bscanDeepGlWidget::setupColorsData(const QColor& colorUp, const QColor& colorLow)
{
    m_vectorTuplesColors = defVectorTuplesColors(colorUp, colorLow);
    m_lowPixelLevel  = 0;
    m_highPixelLevel = m_vectorTuplesColors.size()-1;
}

void bscanDeepGlWidget::setupProperties(const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int absPixelsInX, int absPixelsFnX)
{
    if (
        m_bscanName != bscanName                             ||
        m_width != bscanInfoData.vectorBscanData.size()   ||
        m_height != bscanInfoData.quantImpulsesOfPacket ||
        absPixelsInX != m_absPixelsInX             ||
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

    m_minDeep =  bscanInfoData.vectorBscanData[m_absPixelsInX].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId][m_pixelsInY];
    m_maxDeep =  bscanInfoData.vectorBscanData[m_absPixelsInX].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId][m_pixelsFnY];

    for (int i=m_absPixelsInX; i<=m_absPixelsFnX; i++)
    {
        if (bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId][m_pixelsInY] < m_minDeep)
            m_minDeep = bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId][m_pixelsInY];

        if (bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId][m_pixelsFnY] > m_maxDeep)
            m_maxDeep = bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId][m_pixelsFnY];
    }

    emit signalFixPixels();
}

void bscanDeepGlWidget::slotSetupPixels(const st_bscanInfoData& bscanInfoData)
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
        m_palette[4*i+0] = std::get<0>(m_vectorTuplesColors[i]);
        m_palette[4*i+1] = std::get<1>(m_vectorTuplesColors[i]);
        m_palette[4*i+2] = std::get<2>(m_vectorTuplesColors[i]);
        m_palette[4*i+3] = 255;
    }

    int levelColor   = 0;

    std::vector<int> maskVector;
    maskVector.resize(m_pixelsWidth*m_pixelsHeight);
    for (int i = 0; i <= m_pixelsWidth*m_pixelsHeight-1; i++) maskVector[i] = 0;

    for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
    {
        for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
        {
            levelColor = (int)((m_vectorTuplesColors.size()-1)*(((bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId])[j] - m_minDeep)/(m_maxDeep - m_minDeep)));
            if ((levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel))
            maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] = 1;
        }
    }

    m_minSmallDeep = 0.0;
    m_maxSmallDeep = 0.0;

    for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
    {
        for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
        {
            if (maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] == 1)
            {
                if (i == m_pixelsInX && j == m_pixelsInY)
                m_minSmallDeep = (bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId])[j];
                else if (m_minSmallDeep > (bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId])[j])
                m_minSmallDeep = (bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId])[j];

                if (i == m_pixelsInX && j == m_pixelsInY)
                m_maxSmallDeep = (bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId])[j];
                else if (m_maxSmallDeep < (bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId])[j])
                m_maxSmallDeep = (bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId])[j];
            }
        }
    }

    if ((fabs(m_minSmallDeep) > 0.0) || (fabs(m_maxSmallDeep) > 0.0))
    {
        for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
        {
            for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
            {
                if (maskVector[m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+(i-m_pixelsInX)] == 1)
                {
                    levelColor = (int)((m_vectorTuplesColors.size()-1)*
                    (((bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*m_materialId + countSelectors*m_filterId + m_selectionId])[j] - m_minSmallDeep)/(m_maxSmallDeep - m_minSmallDeep)));

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

    if (m_selectionId == 2)
    {
        outputMapBscanSelectArea(
            m_pixelsWidth, m_pixelsHeight, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, bscanInfoData.vectorMapAutoLog10BscanPairXY[m_filterId], m_pixels
        );
    }

    update();
}

void bscanDeepGlWidget::initializeGL()
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

void bscanDeepGlWidget::resizeGL(int width, int height)
{
    m_inX = (0.1/2.0)*width;
    m_inY = (0.25/2.0)*height;

    m_fnX = (1.6/2.0)*width;
    m_fnY = (1.75/2.0)*height;

    m_openGlHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void bscanDeepGlWidget::paintGL()
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

void bscanDeepGlWidget::paintEvent(QPaintEvent* )
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
        p.drawText(300, 40, QString::fromStdString(m_bscanName));

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
        p.drawText(780, 100, QString::fromStdString("imp"));

        p.drawText(835, 105, QString::number(m_minSmallDeep, 'f', 6));
        for (int i = 1; i < NV-1; i++ ) p.drawText(835, 105+71*i,  QString::number((m_minSmallDeep + i*(m_maxSmallDeep-m_minSmallDeep)/(double)(NV-1)), 'f', 6));
        p.drawText(835, 105+71*(NV-1), QString::number(m_maxSmallDeep, 'f', 6));
    }
    p.end();
}

void bscanDeepGlWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_appearingBscan == true)
    {
        m_pressX = (int)((event->x()-m_inX)*                   ((double)m_pixelsWidth/(double) (m_fnX-m_inX)));
        m_pressY = (int)((m_openGlHeight - event->y() - m_inY)*((double)m_pixelsHeight/(double)(m_fnY-m_inY)));
    }
}

void bscanDeepGlWidget::mouseMoveEvent(QMouseEvent* event){}

void bscanDeepGlWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_appearingBscan == true)
    {
        m_releaseX = (int)((event->x()-m_inX)*                 ((double)m_pixelsWidth /(double)(m_fnX-m_inX)));
        m_releaseY = (int)((m_openGlHeight - event->y()-m_inY)*((double)m_pixelsHeight/(double)(m_fnY-m_inY)));

        if (m_wheelActive == true)
        {
            moveOpengl(
                m_pressX, m_pressY, m_releaseX, m_releaseY,
                m_absPixelsInX, m_absPixelsFnX, m_height, m_pixelsWidth, m_pixelsHeight,
                m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, m_pixelsCentX,  m_pixelsCentY
            );

            emit signalSetupScrollHData(m_pixelsWidth-1,  m_pixelsFnX, m_absPixelsFnX);
            emit signalSetupScrollVData(m_pixelsHeight-1, m_pixelsFnY, m_height-1);

            fixBscanDeep();
        }
    }
}

void bscanDeepGlWidget::wheelEvent(QWheelEvent* event)
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

        fixBscanDeep();
    }
}

void bscanDeepGlWidget::slotChangePixelsFnX(int pixelsFnX)
{
    if (pixelsFnX != 0)
    {
        m_pixelsFnX = pixelsFnX;
        m_pixelsInX = m_pixelsFnX - m_pixelsWidth+1;
        m_pixelsCentX = (m_pixelsFnX + m_pixelsInX)/2;

        fixBscanDeep();
    }
}

void bscanDeepGlWidget::slotChangePixelsFnY(int pixelsFnY)
{
    if (pixelsFnY != 0)
    {
        m_pixelsFnY = pixelsFnY;
        m_pixelsInY = m_pixelsFnY - m_pixelsHeight+1;
        m_pixelsCentY = (m_pixelsFnY + m_pixelsInY)/2;
        fixBscanDeep();
    }
}

void bscanDeepGlWidget::fixBscanDeep()
{
    emit signalFixPixels();
    emit signalFixDataBscanDeepWidgets();
}

void bscanDeepGlWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_lowPixelLevel = lowPixelLevel;
    if (m_pixels != nullptr){ emit signalFixPixels(); }
}

void bscanDeepGlWidget::setHighPixelLevel(int highPixelLevel)
{
    m_highPixelLevel = highPixelLevel;
    if (m_pixels != nullptr){ emit signalFixPixels(); }
}

void bscanDeepGlWidget::remarkTrack(const int& trackNumber)
{
    m_trackNumber = trackNumber;
    if (m_pixels != nullptr) {
        std::cout<<"m_trackNumber emit signalFixPixels();"<<" "<<m_trackNumber<<std::endl;
        emit signalFixPixels();
    }
}

void bscanDeepGlWidget::setupTypeScaling(int typeScaling)
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

void bscanDeepGlWidget::outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)
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
