#ifndef SECTIONBscanSURFWIDGET_H
#define SECTIONBscanSURFWIDGET_H

#include <QChart>
#include <QValueAxis>
#include <QCheckBox>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QResizeEvent>
#include <QLabel>
#include <QSlider>
#include <QPointer>

#include "contentwidget.h"
#include "bscansData.h"

class sectionBscansSurfWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sectionBscansSurfWidget(QWidget* parent = nullptr);
    ~sectionBscansSurfWidget();

    void outputSectionsChart();
    void setBscansSurfSectionMap(
        const std::map<std::string, st_bscanInfoData>& BscansInfoDataMap, int quantImpulsesOfPacket, int filterId, int selectionId,
        bool highLowOnBscan, int absBscansStructInX, int absBscansStructFnX, int materialId
    );
    void outputBscansSurfSection(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData);

signals:
    void signalFixBscansSurfSection();

public slots:
    void incExcBscansSurfNorm();
    void updateBscansSurfSections( int inPosX, int inPosY, int fnPosX, int fnPosY);
    void hideBscansSurfSections();

private:
    int m_quantImpulsesOfPacket {0};

    QPointer<QChart> m_chart;
    QPointer<QLineSeries> m_bscansSurfLineSeries;

    QPointer<contentWidget> m_contentWidget;

    QPointer<QVBoxLayout> m_verLayout;

    double m_inPosX {0.0};
    double m_inPosY {0.0};

    double m_fnPosX {0.0};
    double m_fnPosY {0.0};

    double m_maxDeep;

    bool m_highLowOnBscan{true};

    int m_filterId    {0};
    int m_selectionId {0};
    int m_materialId  {0};
};

#endif // SECTIONBscanSSURFWIDGET_H
