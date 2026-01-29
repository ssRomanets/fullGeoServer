#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QResizeEvent>
#include <QWidget>
#include <QPointer>

#include "sectionChartView.h"

/**
 * @brief Виджет для одномерных графиков
*/
class contentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit contentWidget(QWidget* parent = nullptr);
    void createDefaultChartView(QChart* chart);

protected:
    void resizeEvent(QResizeEvent* event) override;

    QPointer<sectionChartView> m_sectionChartView { nullptr};
};

#endif // CONTENTWIDGET_H
