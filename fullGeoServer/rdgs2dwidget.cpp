#include <QtWidgets>

#include "rdgs2dWidget.h"
#include "rdgsSurfGlWidget.h"
#include "rdgsData.h"

#include <iostream>

rdgs2dWidget::rdgs2dWidget(QWidget* parent): QWidget(parent)
{
    m_leftVLayout = new QVBoxLayout;
    m_rdgsSurfGlWidget = new rdgsSurfGlWidget();

    m_scrollHRdgsSurfBar = new QScrollBar(Qt::Horizontal);
    m_scrollHRdgsSurfBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollHRdgsSurfData(0, 0, 0);

    m_scrollVRdgsSurfBar = new QScrollBar(Qt::Vertical);
    m_scrollVRdgsSurfBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollVRdgsSurfData(0, 0, 0);

    m_leftVLayout = new QVBoxLayout;
    m_leftVLayout->addWidget(m_rdgsSurfGlWidget);
    m_leftVLayout->addWidget(m_scrollHRdgsSurfBar);

    m_leftHLayout = new QHBoxLayout;
    m_leftHLayout->addLayout(m_leftVLayout);
    m_leftHLayout->addWidget(m_scrollVRdgsSurfBar);

    m_leftAllVLayout = new QVBoxLayout;
    m_leftAllVLayout->addLayout(m_leftHLayout);

    m_lowBrightLabel = new QLabel();
    m_lowBrightLabel->setText("Яркость низкая");

    m_lowBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_lowBrightPixelSlider->setRange(0, m_rdgsSurfGlWidget->m_vectorTuplesColorsRdgsSurf.size()-1);
    m_lowBrightPixelSlider->setSingleStep(1);
    m_lowBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_highBrightLabel = new QLabel();
    m_highBrightLabel->setText("Яркость высокая");

    m_highBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_highBrightPixelSlider->setRange(0, m_rdgsSurfGlWidget->m_vectorTuplesColorsRdgsSurf.size()-1);
    m_highBrightPixelSlider->setSingleStep(1);
    m_highBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_leftLowLayout  = new QHBoxLayout;
    m_leftLowLayout->addWidget(m_lowBrightLabel);
    m_leftLowLayout->addWidget(m_lowBrightPixelSlider);
    m_leftLowLayout->addWidget(m_highBrightLabel);
    m_leftLowLayout->addWidget(m_highBrightPixelSlider);
    m_leftAllVLayout->addLayout(m_leftLowLayout);

    m_sectionRdgsSurfWidget  = new sectionRdgsSurfWidget(parent);
    m_sectionRdgsSurfWidget->outputSectionsChart();
    m_sectionRdgsSurfWidget->resize(960, 920);
    m_sectionRdgsSurfWidget->setMinimumSize(QSize(960, 920));
    m_sectionRdgsSurfWidget->setMaximumSize(QSize(960, 920));

    m_rightLayout = new QVBoxLayout;
    m_rightLayout->addWidget(m_sectionRdgsSurfWidget);

    m_mainLayout = new QGridLayout;
    m_mainLayout->addLayout(m_leftAllVLayout, 0, 0);
    m_mainLayout->addLayout(m_rightLayout, 0, 1);

    connect(m_lowBrightPixelSlider,  &QSlider::valueChanged, this, &rdgs2dWidget::setLowPixelLevel);
    connect(m_highBrightPixelSlider, &QSlider::valueChanged, this, &rdgs2dWidget::setHighPixelLevel);

    connect(m_rdgsSurfGlWidget, &rdgsSurfGlWidget::updateRdgsSurfSections, m_sectionRdgsSurfWidget, &sectionRdgsSurfWidget::updateRdgsSurfSections);
    connect(m_rdgsSurfGlWidget, &rdgsSurfGlWidget::hideRdgsSurfSections,   m_sectionRdgsSurfWidget, &sectionRdgsSurfWidget::hideRdgsSurfSections);

    connect(m_rdgsSurfGlWidget, &rdgsSurfGlWidget::sendOutRdgsSurfInfo, this, &rdgs2dWidget::receiveOutRdgsSurfInfo);
    connect(this, &rdgs2dWidget::signalCloseSection2dAction,  m_rdgsSurfGlWidget, &rdgsSurfGlWidget::slotCloseSection2dAction);

    connect(m_rdgsSurfGlWidget, &rdgsSurfGlWidget::signalFixRdgsSurfPixels, this, &rdgs2dWidget::slotFixRdgsSurfPixels);

    connect(m_sectionRdgsSurfWidget, &sectionRdgsSurfWidget::signalFixRdgsSurfSection, this, &rdgs2dWidget::slotFixRdgsSurfSection);

    connect(m_rdgsSurfGlWidget, &rdgsSurfGlWidget::signalSetupScrollHRdgsSurfData, this, &rdgs2dWidget::slotSetupScrollHRdgsSurfData);
    connect(m_rdgsSurfGlWidget, &rdgsSurfGlWidget::signalSetupScrollVRdgsSurfData, this, &rdgs2dWidget::slotSetupScrollVRdgsSurfData);

    connect(m_scrollHRdgsSurfBar, &QScrollBar::valueChanged, m_rdgsSurfGlWidget, &rdgsSurfGlWidget::slotChangeRdgsSurfPixelsFnX);
    connect(m_scrollVRdgsSurfBar, &QScrollBar::valueChanged, m_rdgsSurfGlWidget, &rdgsSurfGlWidget::slotChangeRdgsSurfPixelsFnY);

    setLayout(m_mainLayout);
}

rdgs2dWidget::~rdgs2dWidget(){}

void rdgs2dWidget::slotFixRdgsSurfSection()
{
    emit signalFixRdgsSurfSection();
}

void rdgs2dWidget::slotFixRdgsSurfPixels()
{
    emit signalFixRdgsSurfPixels();
}

void rdgs2dWidget::slotSetupRdgsSurfPixels(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData)
{
    m_rdgsSurfGlWidget->setupRdgsSurfPixels(rdgsInfoDataMap, rdgsWorkData);
}

void rdgs2dWidget::slotOutputRdgsSurfSection(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData)
{
    m_sectionRdgsSurfWidget->outputRdgsSurfSection(rdgsInfoDataMap, rdgsWorkData);
}

void rdgs2dWidget::slotCloseSection2dAction(bool section2dVisible)
{
    emit signalCloseSection2dAction(section2dVisible);
}

void rdgs2dWidget::receiveOutRdgsSurfInfo(QString outRdgsSurfInfo)
{
    emit sendOutRdgsSurfInfo(outRdgsSurfInfo);
}

void rdgs2dWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_highBrightPixelSlider->setRange(m_lowBrightPixelSlider->value(), m_rdgsSurfGlWidget->m_vectorTuplesColorsRdgsSurf.size()-1);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(lowPixelLevel));
    m_rdgsSurfGlWidget->setLowPixelLevel(lowPixelLevel);
}

void rdgs2dWidget::setHighPixelLevel(int highPixelLevel)
{
    m_lowBrightPixelSlider->setRange(0, m_highBrightPixelSlider->value());
    m_highBrightLabel->setText("Яркость высокая "+QString::number(highPixelLevel));
    m_rdgsSurfGlWidget->setHighPixelLevel(highPixelLevel);
}

void rdgs2dWidget::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    m_rdgsSurfGlWidget->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
}

void rdgs2dWidget::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    m_rdgsSurfGlWidget->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
}

void rdgs2dWidget::setupInitPixelSlidersData()
{
    m_rdgsSurfGlWidget->m_lowPixelLevel  = 0;
    m_rdgsSurfGlWidget->m_highPixelLevel = m_rdgsSurfGlWidget->m_vectorTuplesColorsRdgsSurf.size()-1;

    m_lowBrightPixelSlider->setValue(m_rdgsSurfGlWidget->m_lowPixelLevel);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(m_rdgsSurfGlWidget->m_lowPixelLevel));
    m_highBrightPixelSlider->setValue(m_rdgsSurfGlWidget->m_highPixelLevel);
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_rdgsSurfGlWidget->m_highPixelLevel));
}

void rdgs2dWidget::showRdgsSurf(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg, bool showFullRdgs, bool resetSectPoints2dRdgs, int materialId,
    int absRdgsStructInX, int absRdgsStructFnX
)
{
    m_sectionRdgsSurfWidget->setRdgsSurfSectionMap(
        rdgsInfoDataMap,  quantImpulsesOfPacket, filterId, selectionId, highLowOnRdg, materialId,  absRdgsStructInX, absRdgsStructFnX
    );
    changeRdgsSurf(
        rdgsInfoDataMap, rdgsWorkData, quantImpulsesOfPacket, filterId, selectionId,
        highLowOnRdg, showFullRdgs, resetSectPoints2dRdgs, materialId, absRdgsStructInX, absRdgsStructFnX
    );
}

void rdgs2dWidget::changeRdgsSurf(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg, bool showFullRdgs, bool resetSectPoints2dRdgs,
    int materialId,int absRdgsStructInX, int absRdgsStructFnX
)
{
    slotSetupScrollHRdgsSurfData(0, 0, 0);
    slotSetupScrollVRdgsSurfData(0, 0, 0);

    m_rdgsSurfGlWidget->setupRdgsSurfProperties(
        rdgsInfoDataMap, rdgsWorkData, quantImpulsesOfPacket,  filterId, selectionId,  highLowOnRdg, showFullRdgs,
        resetSectPoints2dRdgs, materialId,  absRdgsStructInX, absRdgsStructFnX
    );
    setupInitPixelSlidersData();
}

void rdgs2dWidget::setupColorsRdgsSurfAllProps(const QColor& colorUp, const QColor& colorLow)
{
    m_rdgsSurfGlWidget->setupColorsRdgsSurfData(colorUp,  colorLow);
    m_lowBrightPixelSlider->setRange(0, m_rdgsSurfGlWidget->m_vectorTuplesColorsRdgsSurf.size()-1);
    m_highBrightPixelSlider->setRange(0, m_rdgsSurfGlWidget->m_vectorTuplesColorsRdgsSurf.size()-1);
    setupInitPixelSlidersData();
    emit signalFixRdgsSurfPixels();
}

void rdgs2dWidget::saveRdgsSurf()
{
    QImage img = m_rdgsSurfGlWidget->grabFrameBuffer();
    QString fileRdgsSurfName = QFileDialog::getSaveFileName(
        this, "Save as...", "name",
        "BMP (*.bmp);; CUR (*.cur);; ICNS (*.icns);; ICO (*.ico);; JPEG (*.jpeg);; JPG (*.jpg);; PBM (*.pbm);; PGM (*.pgm);; PNG (*.png);; PPM (*.ppm);; TIF (*.tif);; TIFF (*.tiff);; WBMP (*.wbmp);; WEBP (*.webp);; XBM (*.xbm);; XPM (*.xpm);;"
    );
    img.save(fileRdgsSurfName);
}

void rdgs2dWidget::slotSetupScrollHRdgsSurfData(int minScrollHRdgsSurfData, int scrollHRdgsSurfData, int maxScrollHRdgsSurfData)
{
    m_scrollHRdgsSurfBar->setRange(minScrollHRdgsSurfData, maxScrollHRdgsSurfData);
    m_scrollHRdgsSurfBar->setValue(scrollHRdgsSurfData);
}

void rdgs2dWidget::slotSetupScrollVRdgsSurfData(int minScrollVRdgsSurfData, int scrollVRdgsSurfData, int maxScrollVRdgsSurfData)
{
    m_scrollVRdgsSurfBar->setRange(minScrollVRdgsSurfData, maxScrollVRdgsSurfData);
    m_scrollVRdgsSurfBar->setValue(scrollVRdgsSurfData);
}
