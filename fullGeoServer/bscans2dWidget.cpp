#include <QtWidgets>

#include "bscans2dWidget.h"
#include "bscansSurfGlWidget.h"
#include "bscansData.h"

#include <iostream>

bscans2dWidget::bscans2dWidget(QWidget* parent): QWidget(parent)
{
    m_leftVLayout = new QVBoxLayout;
    m_bscansSurfGlWidget = new bscansSurfGlWidget();

    m_scrollHBar = new QScrollBar(Qt::Horizontal);
    m_scrollHBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollHData(0, 0, 0);

    m_scrollVBar = new QScrollBar(Qt::Vertical);
    m_scrollVBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollVData(0, 0, 0);

    m_leftVLayout = new QVBoxLayout;
    m_leftVLayout->addWidget(m_bscansSurfGlWidget);
    m_leftVLayout->addWidget(m_scrollHBar);

    m_leftHLayout = new QHBoxLayout;
    m_leftHLayout->addLayout(m_leftVLayout);
    m_leftHLayout->addWidget(m_scrollVBar);

    m_leftAllVLayout = new QVBoxLayout;
    m_leftAllVLayout->addLayout(m_leftHLayout);

    m_lowBrightLabel = new QLabel();
    m_lowBrightLabel->setText("Яркость низкая");

    m_lowBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_lowBrightPixelSlider->setRange(0, m_bscansSurfGlWidget->m_vectorTuplesColors.size()-1);
    m_lowBrightPixelSlider->setSingleStep(1);
    m_lowBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_highBrightLabel = new QLabel();
    m_highBrightLabel->setText("Яркость высокая");

    m_highBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_highBrightPixelSlider->setRange(0, m_bscansSurfGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightPixelSlider->setSingleStep(1);
    m_highBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_leftLowLayout  = new QHBoxLayout;
    m_leftLowLayout->addWidget(m_lowBrightLabel);
    m_leftLowLayout->addWidget(m_lowBrightPixelSlider);
    m_leftLowLayout->addWidget(m_highBrightLabel);
    m_leftLowLayout->addWidget(m_highBrightPixelSlider);
    m_leftAllVLayout->addLayout(m_leftLowLayout);

    m_sectionBscansSurfWidget  = new sectionBscansSurfWidget(parent);
    m_sectionBscansSurfWidget->outputSectionsChart();
    m_sectionBscansSurfWidget->resize(960, 920);
    m_sectionBscansSurfWidget->setMinimumSize(QSize(960, 920));
    m_sectionBscansSurfWidget->setMaximumSize(QSize(960, 920));

    m_rightLayout = new QVBoxLayout;
    m_rightLayout->addWidget(m_sectionBscansSurfWidget);

    m_mainLayout = new QGridLayout;
    m_mainLayout->addLayout(m_leftAllVLayout, 0, 0);
    m_mainLayout->addLayout(m_rightLayout, 0, 1);

    connect(m_lowBrightPixelSlider,  &QSlider::valueChanged, this, &bscans2dWidget::setLowPixelLevel);
    connect(m_highBrightPixelSlider, &QSlider::valueChanged, this, &bscans2dWidget::setHighPixelLevel);

    connect(m_bscansSurfGlWidget, &bscansSurfGlWidget::updateBscansSurfSections, m_sectionBscansSurfWidget, &sectionBscansSurfWidget::updateBscansSurfSections);
    connect(m_bscansSurfGlWidget, &bscansSurfGlWidget::hideBscansSurfSections,   m_sectionBscansSurfWidget, &sectionBscansSurfWidget::hideBscansSurfSections);

    connect(m_bscansSurfGlWidget, &bscansSurfGlWidget::sendOutBscansSurfInfo, this, &bscans2dWidget::receiveOutBscansSurfInfo);
    connect(this, &bscans2dWidget::signalCloseSection2dAction,  m_bscansSurfGlWidget, &bscansSurfGlWidget::slotCloseSection2dAction);

    connect(m_bscansSurfGlWidget, &bscansSurfGlWidget::signalFixBscansSurfPixels, this, &bscans2dWidget::slotFixBscansSurfPixels);

    connect(m_sectionBscansSurfWidget, &sectionBscansSurfWidget::signalFixBscansSurfSection, this, &bscans2dWidget::slotFixBscansSurfSection);

    connect(m_bscansSurfGlWidget, &bscansSurfGlWidget::signalSetupScrollHData, this, &bscans2dWidget::slotSetupScrollHData);
    connect(m_bscansSurfGlWidget, &bscansSurfGlWidget::signalSetupScrollVData, this, &bscans2dWidget::slotSetupScrollVData);

    connect(m_scrollHBar, &QScrollBar::valueChanged, m_bscansSurfGlWidget, &bscansSurfGlWidget::slotChangeBscansSurfPixelsFnX);
    connect(m_scrollVBar, &QScrollBar::valueChanged, m_bscansSurfGlWidget, &bscansSurfGlWidget::slotChangeBscansSurfPixelsFnY);

    setLayout(m_mainLayout);
}

bscans2dWidget::~bscans2dWidget(){}

void bscans2dWidget::slotFixBscansSurfSection()
{
    emit signalFixBscansSurfSection();
}

void bscans2dWidget::slotFixBscansSurfPixels()
{
    emit signalFixBscansSurfPixels();
}

void bscans2dWidget::slotSetupBscansSurfPixels(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData)
{
    m_bscansSurfGlWidget->setupBscansSurfPixels(bscansInfoDataMap, bscansWorkData);
}

void bscans2dWidget::slotOutputBscansSurfSection(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData)
{
    m_sectionBscansSurfWidget->outputBscansSurfSection(bscansInfoDataMap, bscansWorkData);
}

void bscans2dWidget::slotCloseSection2dAction(bool section2dVisible)
{
    emit signalCloseSection2dAction(section2dVisible);
}

void bscans2dWidget::receiveOutBscansSurfInfo(QString outBscansSurfInfo)
{
    emit sendOutBscansSurfInfo(outBscansSurfInfo);
}

void bscans2dWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_highBrightPixelSlider->setRange(m_lowBrightPixelSlider->value(), m_bscansSurfGlWidget->m_vectorTuplesColors.size()-1);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(lowPixelLevel));
    m_bscansSurfGlWidget->setLowPixelLevel(lowPixelLevel);
}

void bscans2dWidget::setHighPixelLevel(int highPixelLevel)
{
    m_lowBrightPixelSlider->setRange(0, m_highBrightPixelSlider->value());
    m_highBrightLabel->setText("Яркость высокая "+QString::number(highPixelLevel));
    m_bscansSurfGlWidget->setHighPixelLevel(highPixelLevel);
}

void bscans2dWidget::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    m_bscansSurfGlWidget->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
}

void bscans2dWidget::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    m_bscansSurfGlWidget->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
}

void bscans2dWidget::setupInitPixelSlidersData()
{
    m_bscansSurfGlWidget->m_lowPixelLevel  = 0;
    m_bscansSurfGlWidget->m_highPixelLevel = m_bscansSurfGlWidget->m_vectorTuplesColors.size()-1;

    m_lowBrightPixelSlider->setValue(m_bscansSurfGlWidget->m_lowPixelLevel);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(m_bscansSurfGlWidget->m_lowPixelLevel));
    m_highBrightPixelSlider->setValue(m_bscansSurfGlWidget->m_highPixelLevel);
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_bscansSurfGlWidget->m_highPixelLevel));
}

void bscans2dWidget::showBscansSurf(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan, bool showFullBscans, bool resetSectPoints2dBscans, int materialId,
    int absBscansStructInX, int absBscansStructFnX
)
{
    m_sectionBscansSurfWidget->setBscansSurfSectionMap(
        bscansInfoDataMap,  quantImpulsesOfPacket, filterId, selectionId, highLowOnBscan, materialId,  absBscansStructInX, absBscansStructFnX
    );
    changeBscansSurf(
        bscansInfoDataMap, bscansWorkData, quantImpulsesOfPacket, filterId, selectionId,
        highLowOnBscan, showFullBscans, resetSectPoints2dBscans, materialId, absBscansStructInX, absBscansStructFnX
    );
}

void bscans2dWidget::changeBscansSurf(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan, bool showFullBscans, bool resetSectPoints2dBscans,
    int materialId,int absBscansStructInX, int absBscansStructFnX
)
{
    slotSetupScrollHData(0, 0, 0);
    slotSetupScrollVData(0, 0, 0);

    m_bscansSurfGlWidget->setupProperties(
        bscansInfoDataMap, bscansWorkData, quantImpulsesOfPacket,  filterId, selectionId,  highLowOnBscan, showFullBscans,
        resetSectPoints2dBscans, materialId,  absBscansStructInX, absBscansStructFnX
    );
    setupInitPixelSlidersData();
}

void bscans2dWidget::setupTypeScaling(int typeScaling)
{
    m_bscansSurfGlWidget->setupTypeScaling(typeScaling);
}

void bscans2dWidget::setupColorsBscansSurfAllProps(const QColor& colorUp, const QColor& colorLow)
{
    m_bscansSurfGlWidget->setupColorsData(colorUp,  colorLow);
    m_lowBrightPixelSlider->setRange(0, m_bscansSurfGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightPixelSlider->setRange(0, m_bscansSurfGlWidget->m_vectorTuplesColors.size()-1);
    setupInitPixelSlidersData();
    emit signalFixBscansSurfPixels();
}

void bscans2dWidget::saveBscansSurf()
{
    QImage img = m_bscansSurfGlWidget->grabFrameBuffer();
    QString fileBscansSurfName = QFileDialog::getSaveFileName(
        this, "Save as...", "name",
        "BMP (*.bmp);; CUR (*.cur);; ICNS (*.icns);; ICO (*.ico);; JPEG (*.jpeg);; JPG (*.jpg);; PBM (*.pbm);; PGM (*.pgm);; PNG (*.png);; PPM (*.ppm);; TIF (*.tif);; TIFF (*.tiff);; WBMP (*.wbmp);; WEBP (*.webp);; XBM (*.xbm);; XPM (*.xpm);;"
    );
    img.save(fileBscansSurfName);
}

void bscans2dWidget::slotSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData)
{
    m_scrollHBar->setRange(minScrollHData, maxScrollHData);
    m_scrollHBar->setValue(scrollHData);
}

void bscans2dWidget::slotSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData)
{
    m_scrollVBar->setRange(minScrollVData, maxScrollVData);
    m_scrollVBar->setValue(scrollVData);
}
