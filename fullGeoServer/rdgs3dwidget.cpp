#include <QtWidgets>

#include "rdgs3dWidget.h"
#include "structureRdgsWidget.h"
#include "RdgsTransGlWidget.h"
#include "iostream"
#include "string"

rdgs3dWidget::rdgs3dWidget(QWidget* parent): QWidget(parent)
{
    m_leftLayout       = new QVBoxLayout;
    m_structRdgsWidget = new structureRdgsWidget(parent);
    m_structRdgsWidget->resize(980, 920);
    m_structRdgsWidget->setMinimumSize(QSize(980, 920));
    m_structRdgsWidget->setMaximumSize(QSize(980, 920));
    m_leftLayout->addWidget(m_structRdgsWidget);

    m_rightVLayout    = new QVBoxLayout;
    m_rdgsTransGlWidget = new rdgsTransGlWidget();

    m_scrollHRdgsTransBar = new QScrollBar(Qt::Horizontal);
    m_scrollHRdgsTransBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollHRdgsTransData(0, 0, 0);

    m_rightVLayout = new QVBoxLayout;
    m_rightVLayout->addWidget(m_rdgsTransGlWidget);
    m_rightVLayout->addWidget(m_scrollHRdgsTransBar);

    m_scrollVRdgsTransBar = new QScrollBar(Qt::Vertical);
    m_scrollVRdgsTransBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollVRdgsTransData(0, 0, 0);

    m_rightHLayout = new QHBoxLayout;
    m_rightHLayout->addLayout(m_rightVLayout);
    m_rightHLayout->addWidget(m_scrollVRdgsTransBar);

    m_rightAllVLayout = new QVBoxLayout;
    m_rightAllVLayout->addLayout(m_rightHLayout);

    m_lowBrightLabel = new QLabel();
    m_lowBrightLabel->setText("Низкая яркость");

    m_lowBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_lowBrightPixelSlider->setRange(0, m_rdgsTransGlWidget->m_vectorTuplesColorsRdgsTrans.size()-1);
    m_lowBrightPixelSlider->setSingleStep(1);
    m_lowBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_highBrightLabel = new QLabel();
    m_highBrightLabel->setText("Высокая яркость");

    m_highBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_highBrightPixelSlider->setRange(0, m_rdgsTransGlWidget->m_vectorTuplesColorsRdgsTrans.size()-1);
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

    connect(m_lowBrightPixelSlider,  &QSlider::valueChanged, this, &rdgs3dWidget::setLowPixelLevel);
    connect(m_highBrightPixelSlider, &QSlider::valueChanged, this, &rdgs3dWidget::setHighPixelLevel);

    connect(m_structRdgsWidget, &structureRdgsWidget::updateRdgsTransGlWidget, this,  &rdgs3dWidget::receiveRdgsTransGlWidgetPoints);
    connect(this, &rdgs3dWidget::updateRdgsTransGlWidget, m_rdgsTransGlWidget,  &rdgsTransGlWidget::receiveRdgsTransGlWidgetPoints);

    connect(m_structRdgsWidget, &structureRdgsWidget::signalClearRdgsTransGlWidget, m_rdgsTransGlWidget,  &rdgsTransGlWidget::slotClearRdgsTransGlWidget);

    connect(m_structRdgsWidget, &structureRdgsWidget::sendDeleteRdgName, this, &rdgs3dWidget::receiveDeleteRdgName);
    connect(m_structRdgsWidget, &structureRdgsWidget::signalFixDeleteRdgName, this, &rdgs3dWidget::slotFixDeleteRdgName);
    connect(m_structRdgsWidget, &structureRdgsWidget::signalFixFillRdgs, this, &rdgs3dWidget::slotFixFillRdgs);

    connect(this, &rdgs3dWidget::signalCloseCut3dAction,  m_structRdgsWidget, &structureRdgsWidget::slotCloseCut3dAction);
    connect(this, &rdgs3dWidget::signalDefDeleteRdgName,  m_structRdgsWidget, &structureRdgsWidget::slotDefDeleteRdgName);

    connect(this, &rdgs3dWidget::signalFillRdgsExecute,  m_structRdgsWidget, &structureRdgsWidget::slotFillRdgsExecute);

    connect(m_rdgsTransGlWidget, &rdgsTransGlWidget::signalSetupScrollHRdgsTransData, this, &rdgs3dWidget::slotSetupScrollHRdgsTransData);
    connect(m_rdgsTransGlWidget, &rdgsTransGlWidget::signalSetupScrollVRdgsTransData, this, &rdgs3dWidget::slotSetupScrollVRdgsTransData);

    connect(m_scrollHRdgsTransBar, &QScrollBar::valueChanged, m_rdgsTransGlWidget, &rdgsTransGlWidget::slotChangeRdgsTransPixelsFnX);
    connect(m_scrollVRdgsTransBar, &QScrollBar::valueChanged, m_rdgsTransGlWidget, &rdgsTransGlWidget::slotChangeRdgsTransPixelsFnY);
}

rdgs3dWidget::~rdgs3dWidget(){}

void rdgs3dWidget::showRdgsImage(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
    int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg, bool showFullRdgs,
    bool resetCutPointsRdgs, int materialId, int absRdgsStructInX, int absRdgsStructFnX
)
{
    m_rdgsTransGlWidget->setInfoDataMap(maxQuantImpulsesOfPacket,  quantImpulsesOfPacket, filterId,  materialId);
    changeRdgsImage(
        rdgsInfoDataMap, rdgsWorkData,  quantImpulsesOfPacket, filterId, selectionId, highLowOnRdg,
        showFullRdgs, resetCutPointsRdgs, materialId, absRdgsStructInX, absRdgsStructFnX
    );
}

void rdgs3dWidget::changeRdgsImage(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
    int quantImpulsesOfPacket, int filterId, int selectionId, bool highLowOnRdg, bool showFullRdgs, bool resetCutPointsRdgs, int materialId,
    int absRdgsStructInX, int absRdgsStructFnX
)
{
    slotSetupScrollHRdgsTransData(0, 0, 0);
    slotSetupScrollVRdgsTransData(0, 0, 0);
    m_structRdgsWidget->showRdgsSurf(
        rdgsInfoDataMap, rdgsWorkData,  quantImpulsesOfPacket, filterId, selectionId, highLowOnRdg, showFullRdgs, resetCutPointsRdgs,
        materialId, absRdgsStructInX, absRdgsStructFnX
    );
}

void rdgs3dWidget::slotFixDeleteRdgName(double fixLatitude, double  fixLongitude)
{
    emit signalFixDeleteRdgName(fixLatitude, fixLongitude);
}

void rdgs3dWidget::slotDefDeleteRdgName(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, double fixLatitude, double fixLongitude)
{
    emit signalDefDeleteRdgName(rdgsInfoDataMap,  fixLatitude, fixLongitude);
}

void rdgs3dWidget::slotFillRdgsExecute(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData, bool resetCutPointsRdgs)
{
    emit signalFillRdgsExecute(rdgsInfoDataMap, rdgsWorkData, resetCutPointsRdgs);
}

void rdgs3dWidget::receiveDeleteRdgName(const std::string& deleteRdgName)
{
    emit sendDeleteRdgName(deleteRdgName);
}

void rdgs3dWidget::slotCloseCut3dAction(bool cut3dVisible)
{
    emit signalCloseCut3dAction(cut3dVisible);
}

void rdgs3dWidget::setShowLogRdgsTrans(bool showLogRdgsTrans)
{
    m_rdgsTransGlWidget->setShowLogRdgsTrans(showLogRdgsTrans);
}

void rdgs3dWidget::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    m_structRdgsWidget->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
}

void rdgs3dWidget::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    m_structRdgsWidget->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
}

void rdgs3dWidget::representRdgsTransGlWidget(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData)
{
    m_rdgsTransGlWidget->representRdgsTransGlWidget(rdgsInfoDataMap, rdgsWorkData);
}

void rdgs3dWidget::setupRdgsTransScaling(int typeRdgsTransScaling)
{
    m_rdgsTransGlWidget->setupRdsgTransScaling(typeRdgsTransScaling);
}

void rdgs3dWidget::slotFixFillRdgs(bool resetCutPointsRdgs)
{
    emit signalFixFillRdgs(resetCutPointsRdgs);
}

void rdgs3dWidget::setupInitPixelSlidersData()
{
    m_lowBrightPixelSlider->setValue(0);
    m_highBrightPixelSlider->setValue(m_rdgsTransGlWidget->m_vectorTuplesColorsRdgsTrans.size()-1);
    m_rdgsTransGlWidget->m_lowPixelLevel  = 0;
    m_rdgsTransGlWidget->m_highPixelLevel = m_rdgsTransGlWidget->m_vectorTuplesColorsRdgsTrans.size()-1;
}

void rdgs3dWidget::receiveRdgsTransGlWidgetPoints(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
    const std::vector<std::pair<int, int>>& vectorRdgsTransLineSectionFinalPoints, bool highLowOnRdg
)
{
    setupInitPixelSlidersData();
    emit updateRdgsTransGlWidget( rdgsInfoDataMap, rdgsWorkData, vectorRdgsTransLineSectionFinalPoints, highLowOnRdg);
}

void rdgs3dWidget::setupColorsRdgsTransAllProps(const QColor& colorUp, const QColor& colorLow)
{
    m_rdgsTransGlWidget->setupColorsRdgsTransData(colorUp,  colorLow);

    m_lowBrightPixelSlider ->setRange(0, m_rdgsTransGlWidget->m_vectorTuplesColorsRdgsTrans.size()-1);
    m_highBrightPixelSlider->setRange(0, m_rdgsTransGlWidget->m_vectorTuplesColorsRdgsTrans.size()-1);
    setupInitPixelSlidersData();
    m_rdgsTransGlWidget->setupRdgsTransPixels();
}

void rdgs3dWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_highBrightPixelSlider->setRange(m_lowBrightPixelSlider->value(), m_rdgsTransGlWidget->m_vectorTuplesColorsRdgsTrans.size()-1);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(lowPixelLevel));
    m_rdgsTransGlWidget->setLowPixelLevel(lowPixelLevel);
}

void rdgs3dWidget::setHighPixelLevel(int highPixelLevel)
{
    m_lowBrightPixelSlider->setRange(0, m_highBrightPixelSlider->value());
    m_highBrightLabel->setText("Яркость высокая "+QString::number(highPixelLevel));
    m_rdgsTransGlWidget->setHighPixelLevel(highPixelLevel);
}

void rdgs3dWidget::saveRdgsStruct()
{
    QImage img = m_structRdgsWidget->m_graph->renderToImage();
    QString fileRdgsStructName = QFileDialog::getSaveFileName(
        this,
        "Save as...",
        "name",
        "BMP (*.bmp);; CUR (*.cur);; ICNS (*.icns);; ICO (*.ico);; JPEG (*.jpeg);; JPG (*.jpg);; PBM (*.pbm);; PGM (*.pgm);; PNG (*.png);; PPM (*.ppm);; TIF (*.tif);; TIFF (*.tiff);; WBMP (*.wbmp);; WEBP (*.webp);; XBM (*.xbm);; XPM (*.xpm);;"
    );
    img.save(fileRdgsStructName);
}

void rdgs3dWidget::slotSetupScrollHRdgsTransData(int minScrollHRdgsTransData, int scrollHRdgsTransData, int maxScrollHRdgsTransData)
{
    m_scrollHRdgsTransBar->setRange(minScrollHRdgsTransData,  maxScrollHRdgsTransData);
    m_scrollHRdgsTransBar->setValue(scrollHRdgsTransData);
}

void rdgs3dWidget::slotSetupScrollVRdgsTransData(int minScrollVRdgsTransData, int scrollVRdgsTransData, int maxScrollVRdgsTransData)
{
    m_scrollVRdgsTransBar->setRange(minScrollVRdgsTransData,  maxScrollVRdgsTransData);
    m_scrollVRdgsTransBar->setValue(scrollVRdgsTransData);
}
