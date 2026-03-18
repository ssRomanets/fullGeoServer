#include "bscanDeep2dWidget.h"
#include "transformation.h"

bscanDeep2dWidget::bscanDeep2dWidget(QWidget* parent): QWidget(parent)
{ 
    m_bscanDeepGlWidget = new bscanDeepGlWidget();

    m_scrollHBar = new QScrollBar(Qt::Horizontal);
    m_scrollHBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollHData(0, 0, 0);

    m_scrollVBar = new QScrollBar(Qt::Vertical);
    m_scrollVBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollVData(0, 0, 0);

    m_leftVLayout = new QVBoxLayout;
    m_leftVLayout->addWidget(m_bscanDeepGlWidget);
    m_leftVLayout->addWidget(m_scrollHBar);

    m_leftHLayout = new QHBoxLayout;
    m_leftHLayout->addLayout(m_leftVLayout);
    m_leftHLayout->addWidget(m_scrollVBar);

    m_leftAllVLayout = new QVBoxLayout;
    m_leftAllVLayout->addLayout(m_leftHLayout);

    m_lowBrightLabel = new QLabel();
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));

    m_lowBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_lowBrightPixelSlider->setRange(0, m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1);
    m_lowBrightPixelSlider->setSingleStep(1);
    m_lowBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_highBrightLabel = new QLabel();
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1));

    m_highBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_highBrightPixelSlider->setRange(0, m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightPixelSlider->setSingleStep(1);
    m_highBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_leftLowLayout  = new QHBoxLayout;
    m_leftLowLayout->addWidget(m_lowBrightLabel);
    m_leftLowLayout->addWidget(m_lowBrightPixelSlider);
    m_leftLowLayout->addWidget(m_highBrightLabel);
    m_leftLowLayout->addWidget(m_highBrightPixelSlider);
    m_leftLowLayout->addWidget(m_contrastLog10Label);
    m_leftLowLayout->addWidget(m_contrastLog10Slider);
    m_leftAllVLayout->addLayout(m_leftHLayout);
    m_leftAllVLayout->addLayout(m_leftLowLayout);

    m_ascanDeepDataWidget  = new ascanDeepDataWidget(parent);

    m_rightLayout = new QVBoxLayout;
    m_rightLayout->addWidget(m_ascanDeepDataWidget);

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->addLayout(m_leftAllVLayout);
    m_mainLayout->addLayout(m_rightLayout);
    setLayout(m_mainLayout);

    connect(m_bscanDeepGlWidget, &bscanDeepGlWidget::signalFixPixels,   this,                &bscanDeep2dWidget::slotFixPixels);
    connect(m_bscanDeepGlWidget, &bscanDeepGlWidget::signalFixDataBscanDeepWidgets,   this,  &bscanDeep2dWidget::slotlFixDataBscanDeepWidgets);

    connect( this,               &bscanDeep2dWidget::signalSetupPixels, m_bscanDeepGlWidget, &bscanDeepGlWidget::slotSetupPixels);

    connect(m_ascanDeepDataWidget->m_ascanDeepWidget,                 &ascanDeepWidget::fixTrackIndex, m_bscanDeepGlWidget, &bscanDeepGlWidget::remarkTrack);
    connect(m_ascanDeepDataWidget->m_tableAscanDeepDataWidget,        &tableAscanDeepDataWidget::fixTrackIndex, m_bscanDeepGlWidget, &bscanDeepGlWidget::remarkTrack);

    connect(m_bscanDeepGlWidget, &bscanDeepGlWidget::signalSetupScrollHData, this, &bscanDeep2dWidget::slotSetupScrollHData);
    connect(m_bscanDeepGlWidget, &bscanDeepGlWidget::signalSetupScrollVData, this, &bscanDeep2dWidget::slotSetupScrollVData);

    connect(m_scrollHBar, &QScrollBar::valueChanged, m_bscanDeepGlWidget, &bscanDeepGlWidget::slotChangePixelsFnX);
    connect(m_scrollVBar, &QScrollBar::valueChanged, m_bscanDeepGlWidget, &bscanDeepGlWidget::slotChangePixelsFnY);

    connect(m_ascanDeepDataWidget,                &ascanDeepDataWidget::signalFixPageData, this, &bscanDeep2dWidget::slotFixPageData);

    connect(m_ascanDeepDataWidget->m_ascanDeepWidget,          &ascanDeepWidget::fixTrackIndex,          m_bscanDeepGlWidget, &bscanDeepGlWidget::remarkTrack);
    connect(m_ascanDeepDataWidget->m_tableAscanDeepDataWidget, &tableAscanDeepDataWidget::fixTrackIndex, m_bscanDeepGlWidget, &bscanDeepGlWidget::remarkTrack);

    setEnabledOpenGlControls(false);
}

bscanDeep2dWidget::~bscanDeep2dWidget()
{
}

void bscanDeep2dWidget::outputNewImage(const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int materialId, int filterId, int selectionId)
{
    setupOpenglControls();
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));
    m_highBrightPixelSlider->setValue(m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1));

    m_ascanDeepDataWidget->m_materialId  = materialId;
    m_ascanDeepDataWidget->m_filterId    = filterId;
    m_ascanDeepDataWidget->m_selectionId = selectionId;

    m_bscanDeepGlWidget->m_filterId = filterId;
    m_bscanDeepGlWidget->m_materialId = materialId;
    m_bscanDeepGlWidget->m_selectionId = selectionId;

    if (bscanName != "" && bscanInfoData.quantImpulsesOfPacket != 0)
    {
        m_bscanDeepGlWidget->m_lowPixelLevel  = 0;
        m_bscanDeepGlWidget->m_highPixelLevel = m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1;
        bscanDeepInput(bscanName, bscanInfoData, m_bscanDeepGlWidget->m_absPixelsInX,  m_bscanDeepGlWidget->m_absPixelsFnX);
    }
}

void bscanDeep2dWidget::outputEmptyImage()
{
    m_bscanDeepGlWidget->m_pixelsWidth  = 0;
    m_bscanDeepGlWidget->m_pixelsHeight = 0;
    m_bscanDeepGlWidget->m_appearingBscan = false;

    setEnabledOpenGlControls(false);

    m_ascanDeepDataWidget->trackElementsVisible(false);
    m_ascanDeepDataWidget->setBscanName("");

    m_bscanDeepGlWidget->update();

    m_ascanDeepDataWidget->m_ascanDeepWidget->removeDeepSection();
    m_ascanDeepDataWidget->m_tableAscanDeepDataWidget->removeAscanDeepDataTable();
}

void bscanDeep2dWidget::bscanDeepInput(const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int absPixelsInX, int absPixelsFnX)
{
    m_ascanDeepDataWidget->setBscanName(bscanName);
    m_bscanDeepGlWidget->setupProperties(bscanName, bscanInfoData, absPixelsInX, absPixelsFnX);

    m_ascanDeepDataWidget->trackElementsVisible(true);

    m_ascanDeepDataWidget->m_ascanDeepWidget->setTitle("глубина по bscan (m)");
    m_ascanDeepDataWidget->m_ascanDeepWidget->outputSectionChart(QString::fromStdString(bscanName));

    m_ascanDeepDataWidget->setupTrackSliderData(m_bscanDeepGlWidget->m_pixelsInX, m_bscanDeepGlWidget->m_pixelsFnX);
    m_ascanDeepDataWidget->activateWidgets( bscanInfoData, m_bscanDeepGlWidget->m_pixelsInY, m_bscanDeepGlWidget->m_pixelsFnY);
}

void bscanDeep2dWidget::showNewBscanDeep(const std::string& bscanName, const st_bscanInfoData& bscanInfoData)
{
    setupOpenglControls();
    bscanDeepInput(bscanName, bscanInfoData, m_bscanDeepGlWidget->m_absPixelsInX,  m_bscanDeepGlWidget->m_absPixelsFnX);
}

void bscanDeep2dWidget::setupTypeScaling(int typeScaling)
{
    m_bscanDeepGlWidget->setupTypeScaling(typeScaling);
}

void bscanDeep2dWidget::setupColorsBscanDeepAllProps(const QColor& colorUp, const QColor& colorLow, const st_bscanInfoData& bscanInfoData)
{
    m_bscanDeepGlWidget->setupColorsData(colorUp,  colorLow);
    m_lowBrightPixelSlider->setRange(0, m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightPixelSlider->setRange(0, m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1);
    setupInitPixelSlidersData();
    emit signalSetupPixels(bscanInfoData);
}

void bscanDeep2dWidget::setEnabledOpenGlControls(boolean enabled)
{
    m_lowBrightLabel        ->setEnabled(enabled);
    m_lowBrightPixelSlider  ->setEnabled(enabled);
    m_highBrightLabel       ->setEnabled(enabled);
    m_highBrightPixelSlider ->setEnabled(enabled);
}

void bscanDeep2dWidget::setupInitPixelSlidersData()
{
    m_bscanDeepGlWidget->m_lowPixelLevel  = 0;
    m_bscanDeepGlWidget->m_highPixelLevel = m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1;
    m_lowBrightPixelSlider->setValue(0);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));
    m_highBrightPixelSlider->setValue(m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_bscanDeepGlWidget->m_vectorTuplesColors.size()-1));
}

void bscanDeep2dWidget::setupOpenglControls()
{
    if (m_lowBrightLabel->isEnabled() == false)
    {
        setEnabledOpenGlControls(true);
        setupInitPixelSlidersData();
    }
}

void bscanDeep2dWidget::slotSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData)
{
    m_scrollHBar->setRange(minScrollHData, maxScrollHData);
    m_scrollHBar->setValue(scrollHData);
}

void bscanDeep2dWidget::slotSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData)
{
    m_scrollVBar->setRange(minScrollVData, maxScrollVData);
    m_scrollVBar->setValue(scrollVData);
}

void bscanDeep2dWidget::slotFixPixels()
{
    emit signalFixPixels();
}

void bscanDeep2dWidget::slotlFixDataBscanDeepWidgets()
{
    emit signalFixDataBscanDeepWidgets();
}

void bscanDeep2dWidget::slotSetupPixels(const st_bscanInfoData& bscanInfoData)
{
    emit signalSetupPixels(bscanInfoData);
}

void bscanDeep2dWidget::slotSetupFixDataWidgets(const st_bscanInfoData& bscanInfoData)
{
    m_ascanDeepDataWidget->setupTrackSliderData(m_bscanDeepGlWidget->m_pixelsInX, m_bscanDeepGlWidget->m_pixelsFnX);
    m_ascanDeepDataWidget->activateWidgets(bscanInfoData, m_bscanDeepGlWidget->m_pixelsInY, m_bscanDeepGlWidget->m_pixelsFnY);
}

void bscanDeep2dWidget::slotFixPageData(std::string bscanName)
{
    emit signalFixPageData(bscanName);
}

void bscanDeep2dWidget::slotSetupBscanDeepPageData(const st_bscanInfoData& bscanInfoData)
{
    m_ascanDeepDataWidget->outputPageData(m_bscanDeepGlWidget->m_pixelsInY, m_bscanDeepGlWidget->m_pixelsFnY, bscanInfoData);
}

void bscanDeep2dWidget::saveBscanDeep()
{
    QImage img = m_bscanDeepGlWidget->grabFrameBuffer();
    QString filebscanName = QFileDialog::getSaveFileName(
        this,
        "Save as...",
        "name",
        "BMP (*.bmp);; CUR (*.cur);; ICNS (*.icns);; ICO (*.ico);; JPEG (*.jpeg);; JPG (*.jpg);; PBM (*.pbm);; PGM (*.pgm);; PNG (*.png);; PPM (*.ppm);; TIF (*.tif);; TIFF (*.tiff);; WBMP (*.wbmp);; WEBP (*.webp);; XBM (*.xbm);; XPM (*.xpm);;"
    );
    img.save(filebscanName);
}

void bscanDeep2dWidget::changeBscanDeepImage(const st_bscanInfoData& bscanInfoData, int absPixelsInX, int absPixelsFnX)
{
    slotSetupScrollHData(0, 0, 0);
    slotSetupScrollVData(0, 0, 0);
    bscanDeepInput(m_bscanDeepGlWidget->m_bscanName, bscanInfoData, absPixelsInX, absPixelsFnX);
}
