#include "bscansTransGlWidget.h"
#include "transformation.h"
#include <iostream>

#define NV 10
#define NH 10

bscansTransGlWidget::bscansTransGlWidget()
{
    QColor colorUp  = QColor(0, 255, 0);
    QColor colorLow = QColor(0,   0, 255);

    setupColorsBscansTransData(colorUp, colorLow);
}

bscansTransGlWidget::~bscansTransGlWidget()
{
    if (m_pixels != nullptr)     delete [] m_pixels;
    if (m_bscansTransBWPalettes != nullptr) delete [] m_bscansTransBWPalettes;
}

void bscansTransGlWidget::setupColorsBscansTransData(const QColor& colorUp, const QColor& colorLow)
{
    m_vectorTuplesColors = defVectorTuplesColors(colorUp, colorLow);
    m_lowPixelLevel  = 0;
    m_highPixelLevel = m_vectorTuplesColors.size()-1;
}

void bscansTransGlWidget::setInfoDataMap(int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int materialId)
{
    m_filterId = filterId;
    m_materialId = materialId;
    m_quantImpulsesOfPacket    = quantImpulsesOfPacket;
    m_maxQuantImpulsesOfPacket = maxQuantImpulsesOfPacket;
}

void bscansTransGlWidget::setShowLogBscansTrans(bool showLogBscansTrans)
{
    m_showLogBscansTrans = showLogBscansTrans;
    if (m_showLogBscansTrans) m_kindInit = "log BscansTrans"; else m_kindInit = "init BscansTrans";
}

void bscansTransGlWidget::initializeGL()
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

void bscansTransGlWidget::resizeGL(int width, int height)
{
    m_inX = (0.1/2.0)*width;
    m_inY = (0.1/2.0)*height;

    m_fnX = (1.6/2.0)*width;
    m_fnY = (1.6/2.0)*height;

    m_bscansTransOpenGlHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void bscansTransGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_showFullBscans == true)
    {
        if (m_pixels != nullptr && (m_pixelsWidth > 0 && m_pixelsHeight > 0))
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)m_pixelsWidth, (GLsizei)m_pixelsHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
            outputTexture(-0.9, -0.75, 0.6, -0.75, 0.6,  0.75, -0.9, 0.75);

            glBindTexture(GL_TEXTURE_2D, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)1, (GLsizei)m_vectorTuplesColors.size()-1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bscansTransBWPalettes);
            outputTexture(0.75, -0.75, 0.8, -0.75,  0.8,  0.75, 0.75, 0.75);
            glDisable(GL_TEXTURE_2D);

            //разметочные линии
            glBegin(GL_LINES);

            //c левой стороны
            glVertex3d(-0.9, -0.75, 1.0);
            glColor3d (  1.0,   1.0,  1.0);
            glVertex3d(-0.9,  0.75, 1.0);
            glColor3d (  1.0,   1.0, 1.0);

            for (int i = 0; i < NV; i++)
            {
                glVertex3d(-0.925,  -0.75 + 1.5*i/(NV-1), 1.0);
                glColor3d(  1.0,               1.0, 1.0);
                glVertex3d(-0.9, -0.75 + 1.5*i/(NV-1), 1.0);
                glColor3d ( 1.0,               1.0, 1.0);
            }

            //cнизу
            glVertex3d(-0.9, -0.75, 1.0);
            glColor3d (  1.0,   1.0,  1.0);
            glVertex3d(0.6,  -0.75, 1.0);
            glColor3d (  1.0,   1.0, 1.0);

            for (int i = 0; i < NH; i++)
            {
                glVertex3d(-0.9 + 1.5*i/(NH-1),  -0.775, 1.0);
                glColor3d(  1.0,               1.0, 1.0);
                glVertex3d(-0.9 + 1.5*i/(NH-1), -0.75, 1.0);
                glColor3d ( 1.0,               1.0, 1.0);
            }

            //c правой стороны
            glVertex3d( 0.6, -0.75, 1.0);
            glColor3d (  1.0,   1.0,  1.0);
            glVertex3d( 0.6,  0.75, 1.0);
            glColor3d (  1.0,   1.0, 1.0);

            for (int i = 0; i < NV; i++)
            {
                glVertex3d(0.625,  0.75 - (1.5)*i/(NV-1), 1.0);
                glColor3d (1.0,  1.0,               1.0);
                glVertex3d(0.6, 0.75 - (1.5)*i/(NV-1), 1.0);
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
}

void bscansTransGlWidget::paintEvent(QPaintEvent*)
{
    makeCurrent();
    QPainter p(this);
    p.begin(this);
    p.restore();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    paintGL();

    if (m_showFullBscans == true)
    {
        if (m_pixels != nullptr && (m_pixelsWidth > 0 && m_pixelsHeight > 0))
        {
            //Заголовок
            p.setPen(QPen(Qt::white));
            p.drawText(312, 50, QString::fromStdString(m_kindInit));

            //по оси X
            p.drawText(35, 100, QString::fromStdString("ref"));

            //по оси Y слева
            if (m_highLowOnBscan)
                for (int i = 0; i < NV; i++) p.drawText(10, 110+71*i, QString::number(m_pixelsInY + i*(m_pixelsFnY-m_pixelsInY)/(NV-1)));
            else
                for (int i = 0; i < NV; i++) p.drawText(10, 110+71*i, QString::number(m_quantImpulsesOfPacket + m_pixelsInY + i*(m_pixelsFnY-m_pixelsInY)/(NV-1)));

            p.setPen(QPen(Qt::white));
            p.drawText(33,  780, QString::number( m_leftGlLatitude + m_pixelsInX*m_deltaGlLatitude , 'f', 6));
            p.drawText(33,  795, QString::number( m_lowGlLongitude + m_pixelsInX*m_deltaGlLongitude, 'f', 6));

            p.drawText(100,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 1.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(100,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 1.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(177,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 2.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(177,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 2.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(250,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 3.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(250,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 3.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(330,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 4.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(330,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 4.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(405,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 5.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(405,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 5.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(477,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 6.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(477,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 6.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(550,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 7.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(550,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 7.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(625,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 8.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(625,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 8.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(700,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_pixelsInX + 9.0*(m_pixelsFnX - m_pixelsInX)/(NH-1) ), 'f', 6));
            p.drawText(700,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_pixelsInX + 9.0*(m_pixelsFnX - m_pixelsInX)/(NH-1)), 'f', 6));

            p.drawText(770,  780, QString::fromStdString("lat"));
            p.drawText(770,  795, QString::fromStdString("long"));

            p.drawText(700, 100, QString::fromStdString("ns"));

            //по оси Y справа
            if (m_highLowOnBscan)
                for (int i = 0; i < NV; i++)
                    p.drawText(715, 105+71*i, QString::number((m_pixelsInY + i*(m_pixelsFnY-m_pixelsInY)/(NV-1))*m_time_step_ns, 'f', 2));
            else
                for (int i = 0; i < NV; i++)
                    p.drawText(715, 105+71*i, QString::number((m_quantImpulsesOfPacket + m_pixelsInY + i*(m_pixelsFnY-m_pixelsInY)/(NV-1))*m_time_step_ns, 'f', 2));

            //по палитре
            if      (m_showLogBscansTrans == false) p.drawText(765, 100, QString::fromStdString("m"));
            else if (m_showLogBscansTrans == true)  p.drawText(780, 100, QString::fromStdString(""));

            double metricKoeff = m_showLogBscansTrans ? 1.0 : bscanMetricKoeff;
            p.drawText(800, 105, QString::number( metricKoeff*m_maxImpulse, 'f', 4));
            for (int i = 1; i < NV-1; i++ )
                p.drawText(800, 105+71*i,  QString::number(metricKoeff*(m_maxImpulse + i*(m_minImpulse-m_maxImpulse)/(double)(NV-1)) , 'f', 4));
            p.drawText(800, 105+71*(NV-1), QString::number(metricKoeff*m_minImpulse, 'f', 4));
        }
    }
    p.end();
}

void bscansTransGlWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_wheelActive== true)
    {
        m_pressX = (int)((event->x()-m_inX)*                            ((double)m_pixelsWidth/(double) (m_fnX-m_inX)));
        m_pressY = (int)((m_bscansTransOpenGlHeight - event->y() - m_inY)*((double)m_pixelsHeight/(double)(m_fnY-m_inY)));
    }
}

void bscansTransGlWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_wheelActive == true)
    {
        m_releaseX = (int)(((int)event->x()-m_inX)*                          ((double)m_pixelsWidth /(double)(m_fnX-m_inX)));
        m_releaseY = (int)((m_bscansTransOpenGlHeight - (int)event->y()-m_inY)*((double)m_pixelsHeight/(double)(m_fnY-m_inY)));

        moveOpengl(
            m_pressX, m_pressY, m_releaseX, m_releaseY,
            0, m_width-1, m_height, m_pixelsWidth, m_pixelsHeight,
            m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY, m_pixelsCentX,  m_pixelsCentY
        );

        emit signalSetupScrollHData(m_pixelsWidth-1,  m_pixelsFnX, m_width-1);
        emit signalSetupScrollVData(m_pixelsHeight-1, m_pixelsFnY, m_height-1);

        fixBscansTrans();
    }
}

void bscansTransGlWidget::wheelEvent(QWheelEvent* event)
{
    if ((m_allScaling != false) || (m_hScaling != false) || (m_vScaling!= false))
    {
        double eventDelta = event->delta();
        wheelOpengl(
             eventDelta,  m_allScaling,   m_hScaling,  m_vScaling, 0, m_width-1, m_height,
             m_pixelsWidth, m_pixelsHeight, m_pixelsInX, m_pixelsInY, m_pixelsFnX, m_pixelsFnY,
             m_pixelsCentX, m_pixelsCentY
        );

        emit signalSetupScrollHData(m_pixelsWidth-1,  m_pixelsFnX, m_width-1);
        emit signalSetupScrollVData(m_pixelsHeight-1, m_pixelsFnY, m_height-1);

        if ((m_pixelsWidth == m_width) && (m_pixelsHeight == m_height)) m_wheelActive = false;
        else                                                            m_wheelActive = true;

        fixBscansTrans();
    }
}

void bscansTransGlWidget::slotChangeBscansTransPixelsFnX(int pixelsFnX)
{
    if (pixelsFnX != 0)
    {
        m_pixelsFnX = pixelsFnX;
        m_pixelsInX = m_pixelsFnX - m_pixelsWidth+1;
        m_pixelsCentX = (m_pixelsFnX + m_pixelsInX)/2;
        fixBscansTrans();
    }
}

void bscansTransGlWidget::slotChangeBscansTransPixelsFnY(int pixelsFnY)
{
    if (pixelsFnY != 0)
    {
        m_pixelsFnY = pixelsFnY;
        m_pixelsInY = m_pixelsFnY - m_pixelsHeight+1;
        m_pixelsCentY = (m_pixelsFnY + m_pixelsInY)/2;
        fixBscansTrans();
    }
}

void bscansTransGlWidget::receiveBscansTransGlWidgetPoints(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
    const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
)
{
    m_showFullBscans = true;
    m_highLowOnBscan = highLowOnBscan;
    m_vectorBscansTransLineSectionFinalPoints = vectorBscansTransLineSectionFinalPoints;

    std::map<std::string, st_bscanInfoData>::const_iterator itBscansInfoDataMap = bscansInfoDataMap.begin();

    m_time_step_ns = itBscansInfoDataMap->second.vectorBscanData[0].time_step_ns;

    m_width  = vectorBscansTransLineSectionFinalPoints.size();
    if (m_highLowOnBscan) m_height = m_quantImpulsesOfPacket;
    else m_height = m_maxQuantImpulsesOfPacket - m_quantImpulsesOfPacket;

    m_pixelsInX = 0;
    m_pixelsInY = 0;

    m_pixelsFnX = m_width-1;
    m_pixelsFnY = m_height-1;

    m_pixelsWidth  = m_width;
    m_pixelsHeight = m_height;

    m_pixelsCentX = (m_pixelsInX + m_pixelsFnX)/2;
    m_pixelsCentY = (m_pixelsInY + m_pixelsFnY)/2;

    m_wheelActive = false;

    m_vectorBscansTransResultImpulses.resize(m_width);
    for (int i = 0; i < m_width; i++)  m_vectorBscansTransResultImpulses[i].resize(m_height);

    representBscansTransGlWidget(bscansInfoDataMap, bscansWorkData);
}

void bscansTransGlWidget::representBscansTransGlWidget(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData)
{
    for (int i = 0; i < m_width; i++) for (int j = 0; j < m_height; j++)  m_vectorBscansTransResultImpulses[i][j] = 0.0;

    m_leftGlLatitude  = 0.0;
    m_rightGlLatitude = 0.0;
    m_lowGlLongitude  = 0.0;
    m_upGlLongitude   = 0.0;

    int leftBound  = 0;
    int rightBound = 0;
    for (int count1 = 0; count1 < m_width; count1++)
    {
        int i = (int)m_vectorBscansTransLineSectionFinalPoints[count1].first;
        int j = (int)m_vectorBscansTransLineSectionFinalPoints[count1].second;

        if (count1 == 0)
        {
            m_leftGlLatitude  = bscansWorkData.leftLatitude + bscansWorkData.deltaLatitude*i;
            m_rightGlLatitude = bscansWorkData.leftLatitude + bscansWorkData.deltaLatitude*i;
            m_lowGlLongitude  = bscansWorkData.lowLongitude + bscansWorkData.deltaLongitude*j;
            m_upGlLongitude   = bscansWorkData.lowLongitude + bscansWorkData.deltaLongitude*j;
        }
        else
        {
            if (m_leftGlLatitude  > bscansWorkData.leftLatitude + bscansWorkData.deltaLatitude*i)
                m_leftGlLatitude  = bscansWorkData.leftLatitude + bscansWorkData.deltaLatitude*i;
            if (m_rightGlLatitude < bscansWorkData.leftLatitude + bscansWorkData.deltaLatitude*i)
                m_rightGlLatitude = bscansWorkData.leftLatitude + bscansWorkData.deltaLatitude*i;

            if (m_lowGlLongitude  > bscansWorkData.lowLongitude + bscansWorkData.deltaLongitude*j)
                m_lowGlLongitude = bscansWorkData.lowLongitude + bscansWorkData.deltaLongitude*j;
            if (m_upGlLongitude   < bscansWorkData.lowLongitude + bscansWorkData.deltaLongitude*j)
                m_upGlLongitude = bscansWorkData.lowLongitude + bscansWorkData.deltaLongitude*j;
        }

        std::string nameBscan = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*j+i]);
        int kBscan            = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*j+i]);

        if (nameBscan != "")
        {
            auto itBscansInfoDataMap = bscansInfoDataMap.find(nameBscan);
            if (itBscansInfoDataMap != bscansInfoDataMap.end())
            {
                if ((itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId]).size() >0)
                {
                    if (m_showLogBscansTrans == false)
                    {
                        if (m_highLowOnBscan)
                        {
                            for (int count2 = 0; count2 < m_quantImpulsesOfPacket; count2++)
                            {
                                m_vectorBscansTransResultImpulses[count1][count2] =
                                (itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId])[count2];
                            }
                            if (leftBound  == 0) leftBound = count1;
                            if (rightBound == 0) rightBound = count1; else if (count1 > rightBound) rightBound = count1;
                        }
                        else
                        {
                            for (int count2 = m_quantImpulsesOfPacket; count2 < (itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId]).size(); count2++)
                            {
                                m_vectorBscansTransResultImpulses[count1][count2-m_quantImpulsesOfPacket] =
                                (itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId])[count2];
                            }
                            if (leftBound  == 0) leftBound = count1;
                            if (rightBound == 0) rightBound = count1; else if (count1 > rightBound) rightBound = count1;
                        }
                    }
                    else
                    {
                        if (m_highLowOnBscan)
                        {
                            for (int count2 = 0; count2 < m_quantImpulsesOfPacket; count2++)
                            {
                                if ((itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId])[count2] > 0.0)
                                    m_vectorBscansTransResultImpulses[count1][count2] =
                                    log10(1 + (itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId])[count2]);
                                else
                                    m_vectorBscansTransResultImpulses[count1][count2] = -1.0*
                                    log10(1 + fabs((itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId])[count2]));
                            }
                            if (leftBound  == 0) leftBound = count1;
                            if (rightBound == 0) rightBound = count1; else if (count1 > rightBound) rightBound = count1;
                        }
                        else
                        {
                            for (int count2 = m_quantImpulsesOfPacket; count2 < (itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId]).size(); count2++)
                            {
                                if ((itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId])[count2] > 0.0)
                                    m_vectorBscansTransResultImpulses[count1][count2] =
                                    log10(1 + (itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId])[count2]);
                                else
                                    m_vectorBscansTransResultImpulses[count1][count2] = -1.0*
                                    log10(1 + fabs((itBscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDoubleData[m_filterId])[count2]));
                            }
                            if (leftBound  == 0) leftBound = count1;
                            if (rightBound == 0) rightBound = count1; else if (count1 > rightBound) rightBound = count1;
                        }
                    }
                }
            }
        }
    }

    if (rightBound > leftBound + 1)
    {
        int innerLeftBound  = 0;
        int innerRightBound = 0;

        for (int count3 = leftBound; count3 <= rightBound; count3++)
        {
            double sum = 0.0;
            for (int count4 = 0; count4 < m_height; count4++)
            {
                sum = sum + m_vectorBscansTransResultImpulses[count3][count4];
            }
            if (fabs(sum) < deltaParam)
            {
                if (innerLeftBound == 0) innerLeftBound = count3;
                if (innerRightBound == 0) innerRightBound = count3; else if (count3 > innerRightBound) innerRightBound = count3;
            }
            else if (innerLeftBound > 0 && innerRightBound > 0 && innerRightBound > innerLeftBound )
            {
                for (int count5 = innerLeftBound; count5 <= innerRightBound; count5++)
                {
                    for (int count6 = 0; count6 < m_height; count6++)
                    {
                        m_vectorBscansTransResultImpulses[count5][count6] =
                        ((double)(innerRightBound - count5)/(double)(innerRightBound - innerLeftBound))*
                        m_vectorBscansTransResultImpulses[innerLeftBound -1][count6] +
                        ((double)(count5  - innerLeftBound)/(double)(innerRightBound - innerLeftBound))*
                        m_vectorBscansTransResultImpulses[innerRightBound+1][count6];
                    }
                }
                innerLeftBound  = 0;
                innerRightBound = 0;
            }
        }
    }

    m_deltaGlLatitude   = (m_rightGlLatitude - m_leftGlLatitude)/(m_width-1);
    m_deltaGlLongitude  = (m_upGlLongitude   - m_lowGlLongitude)/(m_width-1);

    fixBscansTrans();
}

void bscansTransGlWidget::slotClearBscansTransGlWidget()
{
    m_showFullBscans = false;
    update();
}

void bscansTransGlWidget::setupBscansTransPixels()
{
    if (m_width > 0 && m_height > 0)
    {
        m_maxImpulse = 0.0;
        m_minImpulse = 0.0;

        for (int i = 0; i < m_width; i++)
        {
            for (int j = 0; j < m_height; j++)
            {
                if (i == 0 && j == 0)                                          m_maxImpulse = m_vectorBscansTransResultImpulses[i][j];
                else if (m_maxImpulse < m_vectorBscansTransResultImpulses[i][j]) m_maxImpulse = m_vectorBscansTransResultImpulses[i][j];

                if (i == 0 && j == 0)                                          m_minImpulse = m_vectorBscansTransResultImpulses[i][j];
                else if (m_minImpulse > m_vectorBscansTransResultImpulses[i][j]) m_minImpulse = m_vectorBscansTransResultImpulses[i][j];
            }
        }

        if (m_pixels != nullptr) delete [] m_pixels;
        m_pixels = new GLubyte [4*m_pixelsWidth*m_pixelsHeight];

        int levelColor = 0;
        for (int i = m_pixelsInX; i <= m_pixelsFnX; i++)
        {
            for (int j = m_pixelsInY; j <= m_pixelsFnY; j++)
            {
                levelColor = (int)((m_vectorTuplesColors.size()-1)*((m_vectorBscansTransResultImpulses[i][j]- m_minImpulse)/(m_maxImpulse - m_minImpulse)));
                if (levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel)
                {
                    m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+0] =
                            std::get<0>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                    m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+1] =
                            std::get<1>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                    m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+2] =
                            std::get<2>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-levelColor]);
                    m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 255;
                }
                else
                {
                    m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+0] = 0;
                    m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+1] = 0;
                    m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+2] = 0;
                    m_pixels[4*m_pixelsWidth*(m_pixelsHeight-1-(j-m_pixelsInY))+4*(i-m_pixelsInX)+3] = 255;
                }
            }
        }

        if (m_bscansTransBWPalettes != nullptr) delete [] m_bscansTransBWPalettes;
        m_bscansTransBWPalettes = new GLubyte [4*(m_vectorTuplesColors.size())];

        for (int i = 0; i <= m_vectorTuplesColors.size()-1; i++)
        {
            m_bscansTransBWPalettes[4*i+0] = std::get<0>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
            m_bscansTransBWPalettes[4*i+1] = std::get<1>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
            m_bscansTransBWPalettes[4*i+2] = std::get<2>(m_vectorTuplesColors[m_vectorTuplesColors.size()-1-i]);
            m_bscansTransBWPalettes[4*i+3] = 255;
        }
    }
}

void bscansTransGlWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_lowPixelLevel = lowPixelLevel;
    fixBscansTrans();
}

void bscansTransGlWidget::setHighPixelLevel(int highPixelLevel)
{
    m_highPixelLevel = highPixelLevel;
    fixBscansTrans();
}

void bscansTransGlWidget::setupTypeScaling(int typeScaling)
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

void bscansTransGlWidget::outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)
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

void bscansTransGlWidget::fixBscansTrans()
{
    setupBscansTransPixels();
    update();
}
