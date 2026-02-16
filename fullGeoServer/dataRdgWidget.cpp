#include "dataRdgWidget.h"

#include <iostream>

dataRdgWidget::dataRdgWidget(QWidget* parent): QWidget(parent)
{
    m_sectionRdgWidget  = new sectionRdgWidget(parent);
    m_sectionRdgWidget->setTitle("");
    m_sectionRdgWidget->outputSectionChart("");
    m_tableRdgTrackDataWidget   = new tableRdgTrackDataWidget(parent);

    m_sectionDeepRdgWidget = new sectionDeepRdgWidget(parent);
    m_sectionDeepRdgWidget->setTitle("");
    m_sectionDeepRdgWidget->outputSectionChart("");
    m_tableDeepRdgTrackDataWidget = new tableDeepRdgTrackDataWidget(parent);

    m_tableRdgLogAutoDataWidget = new tableRdgLogAutoDataWidget(parent);

    m_tabWidget = new QTabWidget;
    m_tabWidget->addTab(m_sectionRdgWidget, tr("Сечение по треку Rdg"));
    m_tabWidget->addTab(m_tableRdgTrackDataWidget,   tr("Таблица по треку Rdg"));

    m_tabWidget->addTab(m_sectionDeepRdgWidget,   tr("Графическое представление сканирования по глубине"));
    m_tabWidget->addTab(m_tableDeepRdgTrackDataWidget,   tr("Табличное представление сканирования по глубине"));

    m_tabWidget->addTab(m_tableRdgLogAutoDataWidget,   tr("Автоматическое выделение по Log Rdg"));

    m_trackRdgSlider   = new QSlider(Qt::Horizontal);
    m_numTrackRdgLabel = new QLabel;
    m_numTrackRdgLabel->setText("Номер импульса rdg");
    m_numTrackRdgLabel->setMaximumHeight(30);

    m_horLayout = new QHBoxLayout;
    m_horLayout->addWidget(m_numTrackRdgLabel);
    m_horLayout->addWidget( m_trackRdgSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->addLayout(m_horLayout);
    setLayout(mainLayout);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &dataRdgWidget::setPageIndex);
    connect(m_trackRdgSlider, &QSlider::valueChanged, this, &dataRdgWidget::setTrackRdg);

    connect(this, &dataRdgWidget::sendVectorRdgPairXY,  m_tableRdgLogAutoDataWidget, &tableRdgLogAutoDataWidget::receiveVectorRdgPairXY);
    connect(this, &dataRdgWidget::sendLoadMapRdgPairXY, m_tableRdgLogAutoDataWidget, &tableRdgLogAutoDataWidget::receiveLoadMapRdgPairXY);
    trackElementsVisible(false);
}

dataRdgWidget::~dataRdgWidget(){}

void dataRdgWidget::saveRdgAutoSelectionData(QString saveRdgAutoSelectionDataFile)
{
    m_tableRdgLogAutoDataWidget->saveRdgAutoSelectionData(saveRdgAutoSelectionDataFile);
}

void dataRdgWidget::setRdgName(std::string rdgName)
{
    m_rdgName = rdgName;
}

void dataRdgWidget::setPageIndex(int pageIndex)
{
    m_pageIndex = pageIndex;
    if ((m_pageIndex <= 3) && (m_rdgName != "")) trackElementsVisible(true);
    else                                         trackElementsVisible(false);

    emit signalFixRdgTrackElements(m_rdgName);
    emit signalFixPageRdgData(m_rdgName);
}

void dataRdgWidget::setTrackRdg(int trackRdgNumber)
{
    m_trackRdgNumber = trackRdgNumber;
    m_numTrackRdgLabel->setText("Номер трека rdg " +QString::number(m_trackRdgNumber));
    m_trackRdgSlider->setValue(m_trackRdgNumber);
    emit signalFixRdgTrackElements(m_rdgName);
    emit signalFixPageRdgData(m_rdgName);
}

void dataRdgWidget::outputPageRdgData(
    int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, const st_rdgInfoData& rdgInfoData
)
{
    switch (m_pageIndex)
    {
        case 0:
        {
            m_sectionRdgWidget         ->outputLineRdgSection(rdgInfoData, m_trackRdgNumber, rdgPixelsInY, rdgPixelsFnY);
            break;
        }
        case 1:
        {
            m_tableRdgTrackDataWidget  ->showRdgTrackDataTable(rdgInfoData, m_trackRdgNumber, rdgPixelsInY, rdgPixelsFnY);
            break;
        }
        case 2:
        {
            m_sectionDeepRdgWidget ->outputLineDeepRdgSection(
                rdgInfoData, m_trackRdgNumber, rdgPixelsInY, rdgPixelsFnY, m_filterId, m_materialId, m_selectionId
            );
            break;
        }
        case 3:
        {
            m_tableDeepRdgTrackDataWidget  ->showDeepRdgTrackDataTable(
                rdgInfoData, m_trackRdgNumber, rdgPixelsInY, rdgPixelsFnY, m_filterId, m_materialId, m_selectionId
            );
            break;
        }
        case 4:
        {
            m_tableRdgLogAutoDataWidget->showRdgLogAutoDataTable(
                rdgPixelsInX, rdgPixelsInY, rdgPixelsFnX, rdgPixelsFnY, rdgInfoData
            );
            break;
        }
    }
}

void dataRdgWidget::receiveVectorRdgPairXY(int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, int filterId, const st_rdgInfoData& rdgInfoData)
{
    setupTrackElements(rdgInfoData);
    emit sendVectorRdgPairXY(rdgPixelsInX, rdgPixelsInY, rdgPixelsFnX, rdgPixelsFnY, filterId, rdgInfoData);
}

void dataRdgWidget::receiveLoadMapRdgPairXY(
    int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY,
    const st_rdgInfoData& rdgInfoData, const std::map<int, std::vector<QPair<int, int>> >& mapRdgPairXY
)
{
    setupTrackElements(rdgInfoData);
    emit sendLoadMapRdgPairXY(rdgPixelsInX, rdgPixelsInY, rdgPixelsFnX, rdgPixelsFnY, rdgInfoData, mapRdgPairXY);
}

void dataRdgWidget::slotHideTableRdgLogAutoData()
{
    m_tableRdgLogAutoDataWidget->removeRdgLogAutoDataTable();
}

void dataRdgWidget::setupTrackElements(const st_rdgInfoData& rdgInfoData)
{
    if ((m_pageIndex >= 0) && (m_pageIndex <= 3)) if (rdgInfoData.vectorRdgData.size() >0) trackElementsVisible(true); else trackElementsVisible(false);
    else trackElementsVisible(false);
}

void dataRdgWidget::trackElementsVisible(bool visible)
{
    m_numTrackRdgLabel->setVisible(visible);
    m_trackRdgSlider->setVisible(visible);
}

void dataRdgWidget::setupTrackRdgSliderData(int rdgPixelsInX, int rdgPixelsFnX)
{
    m_trackRdgNumber = rdgPixelsInX;
    m_numTrackRdgLabel->setText("Номер импульса rdg " + QString::number(m_trackRdgNumber) + " .");
    m_trackRdgSlider->setRange(rdgPixelsInX, rdgPixelsFnX);
    m_trackRdgSlider->setValue(rdgPixelsInX);
    m_trackRdgSlider->setSingleStep(1);
    m_trackRdgSlider->setTickPosition(QSlider::TicksRight);
}

void dataRdgWidget::activateWidgets(
    std::string rdgName, const st_rdgInfoData& rdgInfoData, int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY
)
{
    switch (m_pageIndex)
    {
        case 0:
        {
            m_sectionRdgWidget->outputRdgSection(rdgName, rdgInfoData, m_trackRdgNumber, rdgPixelsInY, rdgPixelsFnY);
            break;
        }
        case 1:
        {
            m_tableRdgTrackDataWidget->outputRdgTrackDataTable( rdgInfoData, m_trackRdgNumber,  rdgPixelsInY, rdgPixelsFnY);
            break;
        }
        case 2:
        {
            m_sectionDeepRdgWidget ->outputLineDeepRdgSection(
                rdgInfoData, m_trackRdgNumber, rdgPixelsInY, rdgPixelsFnY, m_filterId, m_materialId, m_selectionId
            );
            break;
        }
        case 3:
        {
            m_tableDeepRdgTrackDataWidget->outputDeepRdgTrackDataTable(
                rdgInfoData, m_trackRdgNumber,  rdgPixelsInY, rdgPixelsFnY, m_filterId, m_materialId, m_selectionId
            );
            break;
        }
        case 4:
        {
            m_tableRdgLogAutoDataWidget->outputRdgLogAutoDataTable(rdgPixelsInX, rdgPixelsInY, rdgPixelsFnX, rdgPixelsFnY, rdgInfoData);
            break;
        }
    }
}
