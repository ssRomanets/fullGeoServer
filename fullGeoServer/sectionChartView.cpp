#include "sectionChartView.h"

sectionChartView::sectionChartView(QChart* chart, QWidget* parent)
    : QChartView(chart, parent)
{
    setRubberBand(QChartView::RectangleRubberBand);
}

bool sectionChartView::viewportEvent(QEvent* event)
{
    if (event->type() == QEvent::TouchBegin) {
       m_isTouching = true;
       chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void sectionChartView::mousePressEvent(QMouseEvent* event)
{
    if (m_isTouching) return;
    QChartView::mousePressEvent(event);
}

void sectionChartView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isTouching) return;
    QChartView::mouseMoveEvent(event);
}

void sectionChartView::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_isTouching) m_isTouching = false;

    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}

void sectionChartView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Plus:
        {
            chart()->zoomIn();
            break;
        }
        case Qt::Key_Minus:
        {
            chart()->zoomOut();
            break;
        }
        case Qt::Key_Left:
        {
            chart()->scroll(-10, 0);
            break;
        }
        case Qt::Key_Right:
        {
            chart()->scroll(10, 0);
            break;
        }
        case Qt::Key_Up:
        {
            chart()->scroll(0, 10);
            break;
        }
        case Qt::Key_Down:
        {
            chart()->scroll(0, -10);
            break;
        }
        default:
        {
            QGraphicsView::keyPressEvent(event);
            break;
        }
    }
}
