#include "contentwidget.h"
#include <QChart>
#include <QChartView>

contentWidget::contentWidget(QWidget *parent): QWidget(parent){}

void contentWidget::resizeEvent(QResizeEvent* event)
{
    if (m_sectionChartView) m_sectionChartView->resize(size());
}

void contentWidget::createDefaultChartView(QChart* chart)
{
    if (m_sectionChartView != nullptr) m_sectionChartView->setChart(chart);
    else m_sectionChartView = new sectionChartView(chart, this);
    m_sectionChartView->setRenderHint(QPainter::Antialiasing);
}
