#ifndef SECTIONRDGWIDGET_H
#define SECTIONRDGWIDGET_H

#include "contentwidget.h"
#include "rdgsData.h"

#include <QChart>
#include <QCheckBox>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QWidget>
#include <QResizeEvent>
#include <QLabel>
#include <QSlider>
#include <QPointer>

class sectionRdgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sectionRdgWidget(QWidget* parent = nullptr);
    ~sectionRdgWidget();

    void setTitle(const QString& titleSection);
    void outputSectionChart(const QString& rdgsFileCurrent);
    void outputRdgSection(
        const std::string& rdgName, const st_rdgInfoData& rdgInfoData, int trackRdgNumber,
        int rdgPixelsInY, int rdgPixelsFnY, int pageRdgDataIndex, int materialId
    );

    void removeRdgSection();
    void outputLineRdgSection(const st_rdgInfoData& rdgInfoData, int trackRdgNumber, int rdgPixelsInY, int rdgPixelsFnY);
    void setShowLogRdg(bool showLogRdg);
    void createRdgSection(const st_rdgInfoData& rdgInfoData);

public:
    int m_filterId{0};

public slots:
    void incExcRdgNorm();

signals:
    void fixTrackIndex(const int& trackRdgNumber);
    void signalFixRdgSection(const std::string& rdgName);

private:
    QPointer<QChart> m_chart;
    QPointer<QLineSeries> m_rdgLineSeries;

    QString m_titleSection;
    QPointer<contentWidget> m_contentWidget;

    QPointer<QVBoxLayout> m_verLayout;
    QPointer<QCheckBox> m_normActCheckBox;

    bool m_showLogRdg{false};

    int m_trackRdgNumber;
    int m_rdgPixelsInY;
    int m_rdgPixelsFnY;

    int m_materialId{0};
    std::string m_rdgName{""};

    QPointer<QValueAxis> m_axisX;
    QPointer<QValueAxis> m_axisY;
};

#endif // SECTIONRDGWIDGET_H
