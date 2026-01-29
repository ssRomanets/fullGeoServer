#ifndef SECTIONCHARTVIEW_H
#define SECTIONCHARTVIEW_H

#include <QChart>
#include <QChartView>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QRubberBand>
#include <QWidget>

QT_CHARTS_USE_NAMESPACE

class sectionChartView : public QChartView
{
public:
    sectionChartView(QChart* chart, QWidget* parent);

protected:
    bool viewportEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

private:
    bool m_isTouching = false;
};

#endif // SECTIONCHARTVIEW_H
