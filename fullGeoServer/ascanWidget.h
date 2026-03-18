#ifndef ascanWIDGET_H
#define ascanWIDGET_H

#include "contentwidget.h"
#include "bscansData.h"

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

class ascanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ascanWidget(QWidget* parent = nullptr);
    ~ascanWidget();

    void setTitle(const QString& titleSection);
    void outputSectionChart(const QString& fileCurrent);

    void outputSection(
        const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY, int pixelsFnY
    );

    void removeSection();
    void outputLineSection(const st_bscanInfoData& bscanInfoData, int trackNumber, int pixelsInY, int pixelsFnY);
    void setShowLog(bool showLog);
    void createSection(const st_bscanInfoData& bscanInfoData);

public:
    int m_filterId{0};

public slots:
    void incExcNorm();

signals:
    void fixTrackIndex(const int& trackNumber);
    void signalFixSection(const std::string& bscanName);

private:
    QPointer<QChart> m_chart;
    QPointer<QLineSeries> m_lineSeries;

    QString m_titleSection;
    QPointer<contentWidget> m_contentWidget;

    QPointer<QVBoxLayout> m_verLayout;
    QPointer<QCheckBox> m_normActCheckBox;

    bool m_showLog{false};

    int m_trackNumber;
    int m_pixelsInY;
    int m_pixelsFnY;

    std::string m_bscanName{""};

    QPointer<QValueAxis> m_axisX;
    QPointer<QValueAxis> m_axisY;
};

#endif // SECTIONBscanWIDGET_H
