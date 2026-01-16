#include "rdgsTransGlWidget.h"
#include "transformation.h"
#include <iostream>

#define NV 10
#define NH 10

rdgsTransGlWidget::rdgsTransGlWidget()
{
    QColor colorUp  = QColor(255, 255, 255);
    QColor colorLow = QColor(0,   0,   0);

    setupColorsRdgsTransData(colorUp, colorLow);
}

rdgsTransGlWidget::~rdgsTransGlWidget()
{
    if (m_rdgsTransPixels != nullptr)     delete [] m_rdgsTransPixels;
    if (m_rdgsTransBWPalettes != nullptr) delete [] m_rdgsTransBWPalettes;
}

void rdgsTransGlWidget::setupColorsRdgsTransData(const QColor& colorUp, const QColor& colorLow)
{
    m_vectorTuplesColorsRdgsTrans = defVectorTuplesColors(colorUp, colorLow);
    m_lowPixelLevel  = 0;
    m_highPixelLevel = m_vectorTuplesColorsRdgsTrans.size()-1;
}

void rdgsTransGlWidget::setInfoDataMap(int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int materialId)
{
    m_filterId = filterId;
    m_materialId = materialId;
    m_quantImpulsesOfPacket    = quantImpulsesOfPacket;
    m_maxQuantImpulsesOfPacket = maxQuantImpulsesOfPacket;
}

void rdgsTransGlWidget::setShowLogRdgsTrans(bool showLogRdgsTrans)
{
    m_showLogRdgsTrans = showLogRdgsTrans;
    if (m_showLogRdgsTrans) m_kindInit = "log RdgsTrans"; else m_kindInit = "init RdgsTrans";
}

void rdgsTransGlWidget::initializeGL()
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

void rdgsTransGlWidget::resizeGL(int width, int height)
{
    m_inX = (0.1/2.0)*width;
    m_inY = (0.1/2.0)*height;

    m_fnX = (1.6/2.0)*width;
    m_fnY = (1.6/2.0)*height;

    m_rdgsTransOpenGlHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void rdgsTransGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_showRdg == true)
    {
        if (m_rdgsTransPixels != nullptr && (m_rdgsTransPixelsWidth > 0 && m_rdgsTransPixelsHeight > 0))
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)m_rdgsTransPixelsWidth, (GLsizei)m_rdgsTransPixelsHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_rdgsTransPixels);
            outputTexture(-0.9, -0.75, 0.6, -0.75, 0.6,  0.75, -0.9, 0.75);

            glBindTexture(GL_TEXTURE_2D, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)1, (GLsizei)m_vectorTuplesColorsRdgsTrans.size()-1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_rdgsTransBWPalettes);
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

void rdgsTransGlWidget::paintEvent(QPaintEvent*)
{
    makeCurrent();
    QPainter p(this);
    p.begin(this);
    p.restore();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    paintGL();

    if (m_showRdg == true)
    {
        if (m_rdgsTransPixels != nullptr && (m_rdgsTransPixelsWidth > 0 && m_rdgsTransPixelsHeight > 0))
        {
            //Заголовок
            p.setPen(QPen(Qt::white));
            p.drawText(312, 50, QString::fromStdString(m_kindInit));

            //по оси X
            p.drawText(35, 100, QString::fromStdString("ref"));

            //по оси Y слева
            if (m_highLowOnRdg)
                for (int i = 0; i < NV; i++) p.drawText(10, 110+71*i, QString::number(m_rdgsTransPixelsInY + i*(m_rdgsTransPixelsFnY-m_rdgsTransPixelsInY)/(NV-1)));
            else
                for (int i = 0; i < NV; i++) p.drawText(10, 110+71*i, QString::number(m_quantImpulsesOfPacket + m_rdgsTransPixelsInY + i*(m_rdgsTransPixelsFnY-m_rdgsTransPixelsInY)/(NV-1)));

            p.setPen(QPen(Qt::white));
            p.drawText(33,  780, QString::number( m_leftGlLatitude + m_rdgsTransPixelsInX*m_deltaGlLatitude , 'f', 3));
            p.drawText(33,  795, QString::number( m_lowGlLongitude + m_rdgsTransPixelsInX*m_deltaGlLongitude, 'f', 3));

            p.drawText(100,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 1.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(100,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 1.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1)), 'f', 3));

            p.drawText(177,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 2.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(177,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 2.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1)), 'f', 3));

            p.drawText(250,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 3.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(250,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 3.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1)), 'f', 3));

            p.drawText(330,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 4.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(330,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 4.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1)), 'f', 3));

            p.drawText(405,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 5.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(405,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 5.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1)), 'f', 3));

            p.drawText(477,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 6.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(477,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 6.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1)), 'f', 3));

            p.drawText(550,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 7.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(550,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 7.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1)), 'f', 3));

            p.drawText(625,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 8.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(625,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 8.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));

            p.drawText(700,  780, QString::number(m_leftGlLatitude + m_deltaGlLatitude*(m_rdgsTransPixelsInX + 9.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1) ), 'f', 3));
            p.drawText(700,  795, QString::number(m_lowGlLongitude + m_deltaGlLongitude*(m_rdgsTransPixelsInX + 9.0*(m_rdgsTransPixelsFnX - m_rdgsTransPixelsInX)/(NH-1)), 'f', 3));

            p.drawText(740,  780, QString::fromStdString("lat"));
            p.drawText(740,  795, QString::fromStdString("long"));

            p.drawText(700, 100, QString::fromStdString("ns"));

            //по оси Y справа
            if (m_highLowOnRdg)
                for (int i = 0; i < NV; i++)
                    p.drawText(715, 105+71*i, QString::number((m_rdgsTransPixelsInY + i*(m_rdgsTransPixelsFnY-m_rdgsTransPixelsInY)/(NV-1))*m_time_step_ns, 'f', 2));
            else
                for (int i = 0; i < NV; i++)
                    p.drawText(715, 105+71*i, QString::number((m_quantImpulsesOfPacket + m_rdgsTransPixelsInY + i*(m_rdgsTransPixelsFnY-m_rdgsTransPixelsInY)/(NV-1))*m_time_step_ns, 'f', 2));

            //по палитре
            if      (m_showLogRdgsTrans == false) p.drawText(765, 100, QString::fromStdString("mm"));
            else if (m_showLogRdgsTrans == true)  p.drawText(780, 100, QString::fromStdString(""));

            p.drawText(800, 105, QString::number( m_maxImpulse, 'f', 2));
            for (int i = 1; i < NV-1; i++ ) p.drawText(800, 105+71*i,  QString::number( m_maxImpulse + i*(m_minImpulse-m_maxImpulse)/(double)(NV-1) , 'f', 2));
            p.drawText(800, 105+71*(NV-1), QString::number( m_minImpulse, 'f', 2));
        }
    }
    p.end();
}

void rdgsTransGlWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_wheelActive== true)
    {
        m_pressX = (int)((event->x()-m_inX)*                            ((double)m_rdgsTransPixelsWidth/(double) (m_fnX-m_inX)));
        m_pressY = (int)((m_rdgsTransOpenGlHeight - event->y() - m_inY)*((double)m_rdgsTransPixelsHeight/(double)(m_fnY-m_inY)));
    }
}

void rdgsTransGlWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_wheelActive == true)
    {
        m_releaseX = (int)(((int)event->x()-m_inX)*                          ((double)m_rdgsTransPixelsWidth /(double)(m_fnX-m_inX)));
        m_releaseY = (int)((m_rdgsTransOpenGlHeight - (int)event->y()-m_inY)*((double)m_rdgsTransPixelsHeight/(double)(m_fnY-m_inY)));

        moveOpengl(
            m_pressX, m_pressY, m_releaseX, m_releaseY,
            0, m_rdgsTransWidth-1, m_rdgsTransHeight, m_rdgsTransPixelsWidth, m_rdgsTransPixelsHeight,
            m_rdgsTransPixelsInX, m_rdgsTransPixelsInY, m_rdgsTransPixelsFnX, m_rdgsTransPixelsFnY, m_rdgsTransPixelsCentX,  m_rdgsTransPixelsCentY
        );

        emit signalSetupScrollHRdgsTransData(m_rdgsTransPixelsWidth-1,  m_rdgsTransPixelsFnX, m_rdgsTransWidth-1);
        emit signalSetupScrollVRdgsTransData(m_rdgsTransPixelsHeight-1, m_rdgsTransPixelsFnY, m_rdgsTransHeight-1);

        fixRdgsTrans();
    }
}

void rdgsTransGlWidget::wheelEvent(QWheelEvent* event)
{
    if ((m_rdgsTransAllScaling != false) || (m_rdgsTransHScaling != false) || (m_rdgsTransVScaling!= false))
    {
        double eventDelta = event->delta();
        wheelOpengl(
             eventDelta,  m_rdgsTransAllScaling,   m_rdgsTransHScaling,  m_rdgsTransVScaling, 0, m_rdgsTransWidth-1, m_rdgsTransHeight,
             m_rdgsTransPixelsWidth, m_rdgsTransPixelsHeight, m_rdgsTransPixelsInX, m_rdgsTransPixelsInY, m_rdgsTransPixelsFnX, m_rdgsTransPixelsFnY,
             m_rdgsTransPixelsCentX, m_rdgsTransPixelsCentY
        );

        emit signalSetupScrollHRdgsTransData(m_rdgsTransPixelsWidth-1,  m_rdgsTransPixelsFnX, m_rdgsTransWidth-1);
        emit signalSetupScrollVRdgsTransData(m_rdgsTransPixelsHeight-1, m_rdgsTransPixelsFnY, m_rdgsTransHeight-1);

        if ((m_rdgsTransPixelsWidth == m_rdgsTransWidth) && (m_rdgsTransPixelsHeight == m_rdgsTransHeight)) m_wheelActive = false;
        else                                                                                                m_wheelActive = true;

        fixRdgsTrans();
    }
}

void rdgsTransGlWidget::slotChangeRdgsTransPixelsFnX(int rdgsTransPixelsFnX)
{
    if (rdgsTransPixelsFnX != 0)
    {
        m_rdgsTransPixelsFnX = rdgsTransPixelsFnX;
        m_rdgsTransPixelsInX = m_rdgsTransPixelsFnX - m_rdgsTransPixelsWidth+1;
        m_rdgsTransPixelsCentX = (m_rdgsTransPixelsFnX + m_rdgsTransPixelsInX)/2;
        fixRdgsTrans();
    }
}

void rdgsTransGlWidget::slotChangeRdgsTransPixelsFnY(int rdgsTransPixelsFnY)
{
    if (rdgsTransPixelsFnY != 0)
    {
        m_rdgsTransPixelsFnY = rdgsTransPixelsFnY;
        m_rdgsTransPixelsInY = m_rdgsTransPixelsFnY - m_rdgsTransPixelsHeight+1;
        m_rdgsTransPixelsCentY = (m_rdgsTransPixelsFnY + m_rdgsTransPixelsInY)/2;
        fixRdgsTrans();
    }
}

void rdgsTransGlWidget::receiveRdgsTransGlWidgetPoints(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
    const std::vector<std::pair<int, int>>& vectorRdgsTransLineSectionFinalPoints, bool highLowOnRdg
)
{
    m_showRdg = true;
    m_highLowOnRdg = highLowOnRdg;
    m_vectorRdgsTransLineSectionFinalPoints = vectorRdgsTransLineSectionFinalPoints;

    std::map<std::string, st_rdgInfoData>::const_iterator itRdgsInfoDataMap = rdgsInfoDataMap.begin();

    m_time_step_ns = itRdgsInfoDataMap->second.vectorRdgData[0].time_step_ns;

    m_rdgsTransWidth  = vectorRdgsTransLineSectionFinalPoints.size();
    if (m_highLowOnRdg) m_rdgsTransHeight = m_quantImpulsesOfPacket;
    else m_rdgsTransHeight = m_maxQuantImpulsesOfPacket - m_quantImpulsesOfPacket;

    m_rdgsTransPixelsInX = 0;
    m_rdgsTransPixelsInY = 0;

    m_rdgsTransPixelsFnX = m_rdgsTransWidth-1;
    m_rdgsTransPixelsFnY = m_rdgsTransHeight-1;

    m_rdgsTransPixelsWidth  = m_rdgsTransWidth;
    m_rdgsTransPixelsHeight = m_rdgsTransHeight;

    m_rdgsTransPixelsCentX = (m_rdgsTransPixelsInX + m_rdgsTransPixelsFnX)/2;
    m_rdgsTransPixelsCentY = (m_rdgsTransPixelsInY + m_rdgsTransPixelsFnY)/2;

    m_wheelActive = false;

    m_vectorRdgsTransResultImpulses.resize(m_rdgsTransWidth);
    for (int i = 0; i < m_rdgsTransWidth; i++)  m_vectorRdgsTransResultImpulses[i].resize(m_rdgsTransHeight);

    representRdgsTransGlWidget(rdgsInfoDataMap, rdgsWorkData);
}

void rdgsTransGlWidget::representRdgsTransGlWidget(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData)
{
    for (int i = 0; i < m_rdgsTransWidth; i++) for (int j = 0; j < m_rdgsTransHeight; j++)  m_vectorRdgsTransResultImpulses[i][j] = 0.0;

    m_leftGlLatitude  = 0.0;
    m_rightGlLatitude = 0.0;
    m_lowGlLongitude  = 0.0;
    m_upGlLongitude   = 0.0;

    int leftBound  = 0;
    int rightBound = 0;
    for (int count1 = 0; count1 < m_rdgsTransWidth; count1++)
    {
        int i = (int)m_vectorRdgsTransLineSectionFinalPoints[count1].first;
        int j = (int)m_vectorRdgsTransLineSectionFinalPoints[count1].second;

        if (count1 == 0)
        {
            m_leftGlLatitude  = rdgsWorkData.leftLatitude + rdgsWorkData.deltaLatitude*i;
            m_rightGlLatitude = rdgsWorkData.leftLatitude + rdgsWorkData.deltaLatitude*i;
            m_lowGlLongitude  = rdgsWorkData.lowLongitude + rdgsWorkData.deltaLongitude*j;
            m_upGlLongitude   = rdgsWorkData.lowLongitude + rdgsWorkData.deltaLongitude*j;
        }
        else
        {
            if (m_leftGlLatitude  > rdgsWorkData.leftLatitude + rdgsWorkData.deltaLatitude*i)
                m_leftGlLatitude  = rdgsWorkData.leftLatitude + rdgsWorkData.deltaLatitude*i;
            if (m_rightGlLatitude < rdgsWorkData.leftLatitude + rdgsWorkData.deltaLatitude*i)
                m_rightGlLatitude = rdgsWorkData.leftLatitude + rdgsWorkData.deltaLatitude*i;

            if (m_lowGlLongitude  > rdgsWorkData.lowLongitude + rdgsWorkData.deltaLongitude*j)
                m_lowGlLongitude = rdgsWorkData.lowLongitude + rdgsWorkData.deltaLongitude*j;
            if (m_upGlLongitude   < rdgsWorkData.lowLongitude + rdgsWorkData.deltaLongitude*j)
                m_upGlLongitude = rdgsWorkData.lowLongitude + rdgsWorkData.deltaLongitude*j;
        }

        std::string nameRdg = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*j+i]);
        int kRdg            = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*j+i]);

        if (nameRdg != "")
        {
            auto itRdgsInfoDataMap = rdgsInfoDataMap.find(nameRdg);
            if (itRdgsInfoDataMap != rdgsInfoDataMap.end())
            {
                if ((itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDoubleData[m_filterId]).size() >0)
                {
                    if (m_showLogRdgsTrans == false)
                    {
                        if (m_highLowOnRdg)
                        {
                            for (int count2 = 0; count2 < m_quantImpulsesOfPacket; count2++)
                            {
                                m_vectorRdgsTransResultImpulses[count1][count2] =
                                (itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDoubleData[m_filterId])[count2]*(nanokoef*spc/epsdData(m_materialId));
                            }
                            if (leftBound  == 0) leftBound = count1;
                            if (rightBound == 0) rightBound = count1; else if (count1 > rightBound) rightBound = count1;
                        }
                        else
                        {
                            for (int count2 = m_quantImpulsesOfPacket; count2 < (itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDoubleData[m_filterId]).size(); count2++)
                            {
                                m_vectorRdgsTransResultImpulses[count1][count2-m_quantImpulsesOfPacket] =
                                (itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDoubleData[m_filterId])[count2]*(nanokoef*spc/epsdData(m_materialId));
                            }
                            if (leftBound  == 0) leftBound = count1;
                            if (rightBound == 0) rightBound = count1; else if (count1 > rightBound) rightBound = count1;
                        }
                    }
                    else
                    {
                        if (m_highLowOnRdg)
                        {
                            for (int count2 = 0; count2 < m_quantImpulsesOfPacket; count2++)
                            {
                                m_vectorRdgsTransResultImpulses[count1][count2] =
                                log10(1 + (itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDoubleData[m_filterId])[count2]);
                            }
                            if (leftBound  == 0) leftBound = count1;
                            if (rightBound == 0) rightBound = count1; else if (count1 > rightBound) rightBound = count1;
                        }
                        else
                        {
                            for (int count2 = m_quantImpulsesOfPacket; count2 < (itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDoubleData[m_filterId]).size(); count2++)
                            {
                                m_vectorRdgsTransResultImpulses[count1][count2-m_quantImpulsesOfPacket] =
                                log10(1 + (itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDoubleData[m_filterId])[count2]);
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
            for (int count4 = 0; count4 < m_rdgsTransHeight; count4++)
            {
                sum = sum + m_vectorRdgsTransResultImpulses[count3][count4];
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
                    for (int count6 = 0; count6 < m_rdgsTransHeight; count6++)
                    {
                        m_vectorRdgsTransResultImpulses[count5][count6] =
                        ((double)(innerRightBound - count5)/(double)(innerRightBound - innerLeftBound))*
                        m_vectorRdgsTransResultImpulses[innerLeftBound -1][count6] +
                        ((double)(count5  - innerLeftBound)/(double)(innerRightBound - innerLeftBound))*
                        m_vectorRdgsTransResultImpulses[innerRightBound+1][count6];
                    }
                }
                innerLeftBound  = 0;
                innerRightBound = 0;
            }
        }
    }

    m_deltaGlLatitude   = (m_rightGlLatitude - m_leftGlLatitude)/(m_rdgsTransWidth-1);
    m_deltaGlLongitude  = (m_upGlLongitude   - m_lowGlLongitude)/(m_rdgsTransWidth-1);

    fixRdgsTrans();
}

void rdgsTransGlWidget::slotClearRdgsTransGlWidget()
{
    m_showRdg = false;
    update();
}

void rdgsTransGlWidget::setupRdgsTransPixels()
{
    if (m_rdgsTransWidth > 0 && m_rdgsTransHeight > 0)
    {
        m_maxImpulse = 0.0;
        m_minImpulse = 0.0;

        for (int i = 0; i < m_rdgsTransWidth; i++)
        {
            for (int j = 0; j < m_rdgsTransHeight; j++)
            {
                if (i == 0 && j == 0)                                          m_maxImpulse = m_vectorRdgsTransResultImpulses[i][j];
                else if (m_maxImpulse < m_vectorRdgsTransResultImpulses[i][j]) m_maxImpulse = m_vectorRdgsTransResultImpulses[i][j];

                if (i == 0 && j == 0)                                          m_minImpulse = m_vectorRdgsTransResultImpulses[i][j];
                else if (m_minImpulse > m_vectorRdgsTransResultImpulses[i][j]) m_minImpulse = m_vectorRdgsTransResultImpulses[i][j];
            }
        }

        if (m_rdgsTransPixels != nullptr) delete [] m_rdgsTransPixels;
        m_rdgsTransPixels = new GLubyte [4*m_rdgsTransPixelsWidth*m_rdgsTransPixelsHeight];

        int levelColor = 0;
        for (int i = m_rdgsTransPixelsInX; i <= m_rdgsTransPixelsFnX; i++)
        {
            for (int j = m_rdgsTransPixelsInY; j <= m_rdgsTransPixelsFnY; j++)
            {
                levelColor = (int)((m_vectorTuplesColorsRdgsTrans.size()-1)*((m_vectorRdgsTransResultImpulses[i][j]- m_minImpulse)/(m_maxImpulse - m_minImpulse)));
                if (levelColor >= m_lowPixelLevel  && levelColor <= m_highPixelLevel)
                {
                    m_rdgsTransPixels[4*m_rdgsTransPixelsWidth*(m_rdgsTransPixelsHeight-1-(j-m_rdgsTransPixelsInY))+4*(i-m_rdgsTransPixelsInX)+0] = std::get<0>(m_vectorTuplesColorsRdgsTrans[m_vectorTuplesColorsRdgsTrans.size()-1-levelColor]);
                    m_rdgsTransPixels[4*m_rdgsTransPixelsWidth*(m_rdgsTransPixelsHeight-1-(j-m_rdgsTransPixelsInY))+4*(i-m_rdgsTransPixelsInX)+1] = std::get<1>(m_vectorTuplesColorsRdgsTrans[m_vectorTuplesColorsRdgsTrans.size()-1-levelColor]);
                    m_rdgsTransPixels[4*m_rdgsTransPixelsWidth*(m_rdgsTransPixelsHeight-1-(j-m_rdgsTransPixelsInY))+4*(i-m_rdgsTransPixelsInX)+2] = std::get<2>(m_vectorTuplesColorsRdgsTrans[m_vectorTuplesColorsRdgsTrans.size()-1-levelColor]);
                    m_rdgsTransPixels[4*m_rdgsTransPixelsWidth*(m_rdgsTransPixelsHeight-1-(j-m_rdgsTransPixelsInY))+4*(i-m_rdgsTransPixelsInX)+3] = 255;
                }
                else
                {
                    m_rdgsTransPixels[4*m_rdgsTransPixelsWidth*(m_rdgsTransPixelsHeight-1-(j-m_rdgsTransPixelsInY))+4*(i-m_rdgsTransPixelsInX)+0] = 0;
                    m_rdgsTransPixels[4*m_rdgsTransPixelsWidth*(m_rdgsTransPixelsHeight-1-(j-m_rdgsTransPixelsInY))+4*(i-m_rdgsTransPixelsInX)+1] = 0;
                    m_rdgsTransPixels[4*m_rdgsTransPixelsWidth*(m_rdgsTransPixelsHeight-1-(j-m_rdgsTransPixelsInY))+4*(i-m_rdgsTransPixelsInX)+2] = 0;
                    m_rdgsTransPixels[4*m_rdgsTransPixelsWidth*(m_rdgsTransPixelsHeight-1-(j-m_rdgsTransPixelsInY))+4*(i-m_rdgsTransPixelsInX)+3] = 255;
                }
            }
        }

        if (m_rdgsTransBWPalettes != nullptr) delete [] m_rdgsTransBWPalettes;
        m_rdgsTransBWPalettes = new GLubyte [4*(m_vectorTuplesColorsRdgsTrans.size())];

        for (int i = 0; i <= m_vectorTuplesColorsRdgsTrans.size()-1; i++)
        {
            m_rdgsTransBWPalettes[4*i+0] = std::get<0>(m_vectorTuplesColorsRdgsTrans[m_vectorTuplesColorsRdgsTrans.size()-1-i]);
            m_rdgsTransBWPalettes[4*i+1] = std::get<1>(m_vectorTuplesColorsRdgsTrans[m_vectorTuplesColorsRdgsTrans.size()-1-i]);
            m_rdgsTransBWPalettes[4*i+2] = std::get<2>(m_vectorTuplesColorsRdgsTrans[m_vectorTuplesColorsRdgsTrans.size()-1-i]);
            m_rdgsTransBWPalettes[4*i+3] = 255;
        }
    }
}

void rdgsTransGlWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_lowPixelLevel = lowPixelLevel;
    fixRdgsTrans();
}

void rdgsTransGlWidget::setHighPixelLevel(int highPixelLevel)
{
    m_highPixelLevel = highPixelLevel;
    fixRdgsTrans();
}

void rdgsTransGlWidget::setupRdsgTransScaling(int typeRdgsTransScaling)
{
    switch (typeRdgsTransScaling)
    {
        case (0) :
        {
            m_rdgsTransAllScaling = true;
            m_rdgsTransHScaling   = false;
            m_rdgsTransVScaling   = false;
            break;
        }
        case (1) :
        {
            m_rdgsTransAllScaling = false;
            m_rdgsTransHScaling   = true;
            m_rdgsTransVScaling   = false;
            break;
        }
        case (2) :
        {
            m_rdgsTransAllScaling = false;
            m_rdgsTransHScaling   = false;
            m_rdgsTransVScaling   = true;
            break;
        }
    }
}

void rdgsTransGlWidget::outputTexture(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)
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

void rdgsTransGlWidget::fixRdgsTrans()
{
    setupRdgsTransPixels();
    update();
}
