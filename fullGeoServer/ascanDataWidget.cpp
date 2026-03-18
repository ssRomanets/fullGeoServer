#include "ascanDataWidget.h"

#include <iostream>

ascanDataWidget::ascanDataWidget(QWidget* parent): QWidget(parent)
{
    m_ascanWidget  = new ascanWidget(parent);
    m_ascanWidget->setTitle("");
    m_ascanWidget->outputSectionChart("");
    m_tableAscanDataWidget   = new tableAscanDataWidget(parent);

    m_tableBscanLogAutoDataWidget = new tableBscanLogAutoDataWidget(parent);

    m_tabWidget = new QTabWidget;
    m_tabWidget->addTab(m_ascanWidget, tr("ascan"));
    m_tabWidget->addTab(m_tableAscanDataWidget,   tr("Таблица по ascan"));
    m_tabWidget->addTab(m_tableBscanLogAutoDataWidget,   tr("Автоматическое выделение по Log Bscan"));

    m_trackSlider   = new QSlider(Qt::Horizontal);
    m_numTrackLabel = new QLabel;
    m_numTrackLabel->setText("Номер трека ");
    m_numTrackLabel->setMaximumHeight(30);

    m_horLayout = new QHBoxLayout;
    m_horLayout->addWidget(m_numTrackLabel);
    m_horLayout->addWidget( m_trackSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->addLayout(m_horLayout);
    setLayout(mainLayout);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &ascanDataWidget::setPageIndex);
    connect(m_trackSlider, &QSlider::valueChanged, this, &ascanDataWidget::setTrackBscan);

    connect(this, &ascanDataWidget::sendVectorBscanPairXY,  m_tableBscanLogAutoDataWidget, &tableBscanLogAutoDataWidget::receiveVectorPairXY);
    connect(this, &ascanDataWidget::sendLoadMapBscanPairXY, m_tableBscanLogAutoDataWidget, &tableBscanLogAutoDataWidget::receiveLoadMapPairXY);
    trackElementsVisible(false);
}

ascanDataWidget::~ascanDataWidget(){}

void ascanDataWidget::saveBscanAutoSelectionData(QString saveAutoSelectionDataFile)
{
    m_tableBscanLogAutoDataWidget->saveAutoSelectionData(saveAutoSelectionDataFile);
}

void ascanDataWidget::setbscanName(std::string bscanName)
{
    m_bscanName = bscanName;
}

void ascanDataWidget::setPageIndex(int pageIndex)
{
    m_pageIndex = pageIndex;
    if ((m_pageIndex <= 1) && (m_bscanName != "")) trackElementsVisible(true);
    else                                           trackElementsVisible(false);
    emit signalFixPageData(m_bscanName);
}

void ascanDataWidget::setTrackBscan(int trackNumber)
{
    m_trackNumber = trackNumber;
    m_numTrackLabel->setText("Номер трека " +QString::number(m_trackNumber));
    m_trackSlider->setValue(m_trackNumber);
    emit signalFixPageData(m_bscanName);
}

void ascanDataWidget::outputPageData( int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY, const st_bscanInfoData& bscanInfoData)
{
    switch (m_pageIndex)
    {
        case 0:
        {
            m_ascanWidget           ->outputLineSection(bscanInfoData, m_trackNumber, pixelsInY, pixelsFnY);
            break;
        }
        case 1:
        {
            m_tableAscanDataWidget  ->showAscanDataTable(bscanInfoData, m_trackNumber, pixelsInY, pixelsFnY);
            break;
        }
        case 2:
        {
            m_tableBscanLogAutoDataWidget->showLogAutoDataTable(pixelsInX, pixelsInY, pixelsFnX, pixelsFnY, bscanInfoData);
            break;
        }
    }
}

void ascanDataWidget::receiveVectorBscanPairXY(int bscanPixelsInX, int bscanPixelsInY, int bscanPixelsFnX, int bscanPixelsFnY, int filterId, const st_bscanInfoData& bscanInfoData)
{
    setupTrackElements(bscanInfoData);
    emit sendVectorBscanPairXY(bscanPixelsInX, bscanPixelsInY, bscanPixelsFnX, bscanPixelsFnY, filterId, bscanInfoData);
}

void ascanDataWidget::receiveLoadMapBscanPairXY(
    int bscanPixelsInX, int bscanPixelsInY, int bscanPixelsFnX, int bscanPixelsFnY,
    const st_bscanInfoData& bscanInfoData, const std::map<int, std::vector<QPair<int, int>> >& mapBscanPairXY
)
{
    setupTrackElements(bscanInfoData);
    emit sendLoadMapBscanPairXY(bscanPixelsInX, bscanPixelsInY, bscanPixelsFnX, bscanPixelsFnY, bscanInfoData, mapBscanPairXY);
}

void ascanDataWidget::slotHideTableBscanLogAutoData()
{
    m_tableBscanLogAutoDataWidget->removeLogAutoDataTable();
}

void ascanDataWidget::setupTrackElements(const st_bscanInfoData& bscanInfoData)
{
    if ((m_pageIndex >= 0) && (m_pageIndex <= 3)) if (bscanInfoData.vectorBscanData.size() >0) trackElementsVisible(true); else trackElementsVisible(false);
    else trackElementsVisible(false);
}

void ascanDataWidget::trackElementsVisible(bool visible)
{
    m_numTrackLabel->setVisible(visible);
    m_trackSlider->setVisible(visible);
}

void ascanDataWidget::setupTrackSliderData(int pixelsInX, int pixelsFnX)
{
    m_trackNumber = pixelsInX;
    m_numTrackLabel->setText("Номер трека " + QString::number(m_trackNumber) + " .");
    m_trackSlider->setRange(pixelsInX, pixelsFnX);
    m_trackSlider->setValue(pixelsInX);
    m_trackSlider->setSingleStep(1);
    m_trackSlider->setTickPosition(QSlider::TicksRight);
}

void ascanDataWidget::activateWidgets(
    std::string bscanName, const st_bscanInfoData& bscanInfoData, int pixelsInX, int pixelsInY, int pixelsFnX, int pixelsFnY
)
{
    switch (m_pageIndex)
    {
        case 0:
        {
            m_ascanWidget->outputSection(bscanName, bscanInfoData, m_trackNumber, pixelsInY, pixelsFnY);
            break;
        }
        case 1:
        {
            m_tableAscanDataWidget->outputAscanDataTable(bscanInfoData, m_trackNumber,  pixelsInY, pixelsFnY);
            break;
        }
        case 2:
        {
            m_tableBscanLogAutoDataWidget->outputLogAutoDataTable(pixelsInX, pixelsInY, pixelsFnX, pixelsFnY, bscanInfoData);
            break;
        }
    }
}
