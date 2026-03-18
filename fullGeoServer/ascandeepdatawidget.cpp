#include "ascanDeepDataWidget.h"

#include <iostream>

ascanDeepDataWidget::ascanDeepDataWidget(QWidget *parent)
{
    m_ascanDeepWidget = new ascanDeepWidget(parent);
    m_ascanDeepWidget->setTitle("");
    m_ascanDeepWidget->outputSectionChart("");
    m_tableAscanDeepDataWidget = new tableAscanDeepDataWidget(parent);

    m_tabWidget = new QTabWidget;

    m_tabWidget->addTab(m_ascanDeepWidget,   tr("Графическое представление сканирования по глубине"));
    m_tabWidget->addTab(m_tableAscanDeepDataWidget,   tr("Табличное представление сканирования по глубине"));

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

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &ascanDeepDataWidget::setPageIndex);
    connect(m_trackSlider, &QSlider::valueChanged, this, &ascanDeepDataWidget::setTrackBscan);
}

ascanDeepDataWidget::~ascanDeepDataWidget(){}

void ascanDeepDataWidget::setBscanName(std::string bscanName) { m_bscanName = bscanName;}

void ascanDeepDataWidget::trackElementsVisible(bool visible)
{
    m_numTrackLabel->setVisible(visible);
    m_trackSlider->setVisible(visible);
}

void ascanDeepDataWidget::setPageIndex(int pageIndex)
{
    m_pageIndex = pageIndex;
    emit signalFixPageData(m_bscanName);
}

void ascanDeepDataWidget::outputPageData(int pixelsInY, int pixelsFnY, const st_bscanInfoData& bscanInfoData)
{
    switch (m_pageIndex)
    {
        case 0:
        {
            m_ascanDeepWidget -> outputLineDeepSection( bscanInfoData, m_trackNumber, pixelsInY, pixelsFnY, m_filterId, m_materialId, m_selectionId );
            break;
        }
        case 1:
        {
            m_tableAscanDeepDataWidget ->showAscanDeepDataTable( bscanInfoData, m_trackNumber, pixelsInY, pixelsFnY, m_filterId, m_materialId, m_selectionId);
            break;
        }
    }
}

void ascanDeepDataWidget::setTrackBscan(int trackNumber)
{
    m_trackNumber = trackNumber;
    m_numTrackLabel->setText("Номер трека " +QString::number(m_trackNumber));
    m_trackSlider->setValue(m_trackNumber);
    emit signalFixPageData(m_bscanName);
}

void ascanDeepDataWidget::setupTrackSliderData(int pixelsInX, int pixelsFnX)
{
    m_trackNumber = pixelsInX;
    m_numTrackLabel->setText("Номер трека " + QString::number(m_trackNumber) + " .");
    m_trackSlider->setRange(pixelsInX, pixelsFnX);
    m_trackSlider->setValue(pixelsInX);
    m_trackSlider->setSingleStep(1);
    m_trackSlider->setTickPosition(QSlider::TicksRight);
}

void ascanDeepDataWidget::activateWidgets( const st_bscanInfoData& bscanInfoData, int pixelsInY, int pixelsFnY)
{
    switch (m_pageIndex)
    {
        case 0:
        {
            m_ascanDeepWidget ->outputLineDeepSection( bscanInfoData, m_trackNumber, pixelsInY, pixelsFnY, m_filterId, m_materialId, m_selectionId);
            break;
        }
        case 1:
        {
            m_tableAscanDeepDataWidget->outputAscanDeepDataTable(bscanInfoData, m_trackNumber,  pixelsInY, pixelsFnY, m_filterId, m_materialId, m_selectionId);
            break;
        }
    }
}
