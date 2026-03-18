#include <QtWidgets>

#include "bscans3dWidget.h"
#include "structureBscansWidget.h"
#include "bscansTransGlWidget.h"
#include "iostream"
#include "string"

bscans3dWidget::bscans3dWidget(QWidget* parent): QWidget(parent)
{
    m_leftLayout       = new QVBoxLayout;
    m_structBscansWidget = new structureBscansWidget(parent);
    m_structBscansWidget->resize(980, 920);
    m_structBscansWidget->setMinimumSize(QSize(980, 920));
    m_structBscansWidget->setMaximumSize(QSize(980, 920));
    m_leftLayout->addWidget(m_structBscansWidget);

    m_rightVLayout    = new QVBoxLayout;
    m_bscansTransGlWidget = new bscansTransGlWidget();

    m_scrollHBar = new QScrollBar(Qt::Horizontal);
    m_scrollHBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollHData(0, 0, 0);

    m_rightVLayout = new QVBoxLayout;
    m_rightVLayout->addWidget(m_bscansTransGlWidget);
    m_rightVLayout->addWidget(m_scrollHBar);

    m_scrollVBar = new QScrollBar(Qt::Vertical);
    m_scrollVBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollVData(0, 0, 0);

    m_rightHLayout = new QHBoxLayout;
    m_rightHLayout->addLayout(m_rightVLayout);
    m_rightHLayout->addWidget(m_scrollVBar);

    m_rightAllVLayout = new QVBoxLayout;
    m_rightAllVLayout->addLayout(m_rightHLayout);

    m_lowBrightLabel = new QLabel();
    m_lowBrightLabel->setText("Низкая яркость");

    m_lowBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_lowBrightPixelSlider->setRange(0, m_bscansTransGlWidget->m_vectorTuplesColors.size()-1);
    m_lowBrightPixelSlider->setSingleStep(1);
    m_lowBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_highBrightLabel = new QLabel();
    m_highBrightLabel->setText("Высокая яркость");

    m_highBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_highBrightPixelSlider->setRange(0, m_bscansTransGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightPixelSlider->setSingleStep(1);
    m_highBrightPixelSlider->setTickPosition(QSlider::TicksRight);


    m_rightLowLayout  = new QHBoxLayout;
    m_rightLowLayout->addWidget(m_lowBrightLabel);
    m_rightLowLayout->addWidget(m_lowBrightPixelSlider);
    m_rightLowLayout->addWidget(m_highBrightLabel);
    m_rightLowLayout->addWidget(m_highBrightPixelSlider);
    m_rightAllVLayout->addLayout(m_rightLowLayout);

    m_mainLayout = new QGridLayout;
    m_mainLayout->addLayout(m_leftLayout,  0, 0);
    m_mainLayout->addLayout(m_rightAllVLayout, 0, 1);
    setLayout(m_mainLayout);

    connect(m_lowBrightPixelSlider,  &QSlider::valueChanged, this, &bscans3dWidget::setLowPixelLevel);
    connect(m_highBrightPixelSlider, &QSlider::valueChanged, this, &bscans3dWidget::setHighPixelLevel);

    connect(m_structBscansWidget, &structureBscansWidget::updateBscansTransGlWidget, this,  &bscans3dWidget::receiveBscansTransGlWidgetPoints);
    connect(this, &bscans3dWidget::updateBscansTransGlWidget, m_bscansTransGlWidget,  &bscansTransGlWidget::receiveBscansTransGlWidgetPoints);

    connect(m_structBscansWidget, &structureBscansWidget::signalClearBscansTransGlWidget, m_bscansTransGlWidget,  &bscansTransGlWidget::slotClearBscansTransGlWidget);

    connect(m_structBscansWidget, &structureBscansWidget::sendDeleteBscanName, this, &bscans3dWidget::receiveDeleteBscanName);
    connect(m_structBscansWidget, &structureBscansWidget::signalFixDeleteBscanName, this, &bscans3dWidget::slotFixDeleteBscanName);
    connect(m_structBscansWidget, &structureBscansWidget::signalFixFillBscans, this, &bscans3dWidget::slotFixFillBscans);

    connect(this, &bscans3dWidget::signalCloseCut3dAction,  m_structBscansWidget, &structureBscansWidget::slotCloseCut3dAction);
    connect(this, &bscans3dWidget::signalDefDeleteBscanName,  m_structBscansWidget, &structureBscansWidget::slotDefDeleteBscanName);


    connect(this, &bscans3dWidget::signalFillBscansExecute,  m_structBscansWidget, &structureBscansWidget::slotFillBscansExecute);

    connect(m_bscansTransGlWidget, &bscansTransGlWidget::signalSetupScrollHData, this, &bscans3dWidget::slotSetupScrollHData);
    connect(m_bscansTransGlWidget, &bscansTransGlWidget::signalSetupScrollVData, this, &bscans3dWidget::slotSetupScrollVData);

    connect(m_scrollHBar, &QScrollBar::valueChanged, m_bscansTransGlWidget, &bscansTransGlWidget::slotChangeBscansTransPixelsFnX);
    connect(m_scrollVBar, &QScrollBar::valueChanged, m_bscansTransGlWidget, &bscansTransGlWidget::slotChangeBscansTransPixelsFnY);
}

bscans3dWidget::~bscans3dWidget(){}

void bscans3dWidget::showBscansImage(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
    int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan, bool showFullBscans,
    bool resetCutPointsBscans, int materialId, int absBscansStructInX, int absBscansStructFnX
)
{
    m_bscansTransGlWidget->setInfoDataMap(maxQuantImpulsesOfPacket,  quantImpulsesOfPacket, filterId,  materialId);
    changeBscansImage(
        bscansInfoDataMap, bscansWorkData,  quantImpulsesOfPacket, filterId, selectionId, highLowOnBscan,
        showFullBscans, resetCutPointsBscans, materialId, absBscansStructInX, absBscansStructFnX
    );
}

void bscans3dWidget::changeBscansImage(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnBscan, bool showFullBscans, bool resetCutPointsBscans, int materialId,
    int absBscansStructInX, int absBscansStructFnX
)
{
    slotSetupScrollHData(0, 0, 0);
    slotSetupScrollVData(0, 0, 0);
    m_structBscansWidget->showBscansSurf(
        bscansInfoDataMap, bscansWorkData,  quantImpulsesOfPacket, filterId, selectionId, highLowOnBscan, showFullBscans, resetCutPointsBscans,
        materialId, absBscansStructInX, absBscansStructFnX
    );
}

void bscans3dWidget::slotFixDeleteBscanName(double fixLatitude, double  fixLongitude)
{
    emit signalFixDeleteBscanName(fixLatitude, fixLongitude);
}

void bscans3dWidget::slotDefDeleteBscanName(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, double fixLatitude, double fixLongitude)
{
    emit signalDefDeleteBscanName(bscansInfoDataMap,  fixLatitude, fixLongitude);
}

void bscans3dWidget::slotFillBscansExecute(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData, bool resetCutPointsBscans)
{
    emit signalFillBscansExecute(bscansInfoDataMap, bscansWorkData, resetCutPointsBscans);
}

void bscans3dWidget::receiveDeleteBscanName(const std::string& deleteBscanName)
{
    emit sendDeleteBscanName(deleteBscanName);
}

void bscans3dWidget::slotCloseCut3dAction(bool cut3dVisible)
{
    emit signalCloseCut3dAction(cut3dVisible);
}

void bscans3dWidget::setShowLogBscansTrans(bool showLogBscansTrans)
{
    m_bscansTransGlWidget->setShowLogBscansTrans(showLogBscansTrans);
}

void bscans3dWidget::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    m_structBscansWidget->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
}

void bscans3dWidget::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    m_structBscansWidget->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
}

void bscans3dWidget::representBscansTransGlWidget(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData)
{
    m_bscansTransGlWidget->representBscansTransGlWidget(bscansInfoDataMap, bscansWorkData);
}

void bscans3dWidget::setupTypeScaling(int typeScaling)
{
    m_bscansTransGlWidget->setupTypeScaling(typeScaling);
}

void bscans3dWidget::slotFixFillBscans(bool resetCutPointsBscans)
{
    emit signalFixFillBscans(resetCutPointsBscans);
}

void bscans3dWidget::setupInitPixelSlidersData()
{
    m_lowBrightPixelSlider->setValue(0);
    m_highBrightPixelSlider->setValue(m_bscansTransGlWidget->m_vectorTuplesColors.size()-1);
    m_bscansTransGlWidget->m_lowPixelLevel  = 0;
    m_bscansTransGlWidget->m_highPixelLevel = m_bscansTransGlWidget->m_vectorTuplesColors.size()-1;
}

void bscans3dWidget::receiveBscansTransGlWidgetPoints(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
    const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
)
{
    setupInitPixelSlidersData();
    emit updateBscansTransGlWidget(bscansInfoDataMap, bscansWorkData, vectorBscansTransLineSectionFinalPoints, highLowOnBscan);
}

void bscans3dWidget::setupColorsBscansTransAllProps(const QColor& colorUp, const QColor& colorLow)
{
    m_bscansTransGlWidget->setupColorsBscansTransData(colorUp,  colorLow);

    m_lowBrightPixelSlider ->setRange(0, m_bscansTransGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightPixelSlider->setRange(0, m_bscansTransGlWidget->m_vectorTuplesColors.size()-1);
    setupInitPixelSlidersData();
    m_bscansTransGlWidget->setupBscansTransPixels();
}

void bscans3dWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_highBrightPixelSlider->setRange(m_lowBrightPixelSlider->value(), m_bscansTransGlWidget->m_vectorTuplesColors.size()-1);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(lowPixelLevel));
    m_bscansTransGlWidget->setLowPixelLevel(lowPixelLevel);
}

void bscans3dWidget::setHighPixelLevel(int highPixelLevel)
{
    m_lowBrightPixelSlider->setRange(0, m_highBrightPixelSlider->value());
    m_highBrightLabel->setText("Яркость высокая "+QString::number(highPixelLevel));
    m_bscansTransGlWidget->setHighPixelLevel(highPixelLevel);
}

void bscans3dWidget::saveBscansStruct()
{
    QImage img = m_structBscansWidget->m_graph->renderToImage();
    QString fileBscansStructName = QFileDialog::getSaveFileName(
        this,
        "Save as...",
        "name",
        "BMP (*.bmp);; CUR (*.cur);; ICNS (*.icns);; ICO (*.ico);; JPEG (*.jpeg);; JPG (*.jpg);; PBM (*.pbm);; PGM (*.pgm);; PNG (*.png);; PPM (*.ppm);; TIF (*.tif);; TIFF (*.tiff);; WBMP (*.wbmp);; WEBP (*.webp);; XBM (*.xbm);; XPM (*.xpm);;"
    );
    img.save(fileBscansStructName);
}

void bscans3dWidget::slotSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData)
{
    m_scrollHBar->setRange(minScrollHData,  maxScrollHData);
    m_scrollHBar->setValue(scrollHData);
}

void bscans3dWidget::slotSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData)
{
    m_scrollVBar->setRange(minScrollVData,  maxScrollVData);
    m_scrollVBar->setValue(scrollVData);
}
