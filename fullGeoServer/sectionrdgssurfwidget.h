#ifndef SECTIONRDGSSURFWIDGET_H
#define SECTIONRDGSSURFWIDGET_H

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
#include "rdgsData.h"

class sectionRdgsSurfWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sectionRdgsSurfWidget(QWidget* parent = nullptr);
    ~sectionRdgsSurfWidget();

    void outputSectionsChart();
    void setRdgsSurfSectionMap(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, int quantImpulsesOfPacket, int filterId, int selectionId,
        bool highLowOnRdg, int absRdgsStructInX, int absRdgsStructFnX, int materialId
    );
    void outputRdgsSurfSection(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData);

signals:
    void signalFixRdgsSurfSection();

public slots:
    void incExcRdgsSurfNorm();
    void updateRdgsSurfSections( int inPosX, int inPosY, int fnPosX, int fnPosY);
    void hideRdgsSurfSections();

private:
    int m_quantImpulsesOfPacket {0};

    QPointer<QChart> m_chart;
    QPointer<QLineSeries> m_rdgsSurfLineSeries;

    QPointer<contentWidget> m_contentWidget;

    QPointer<QVBoxLayout> m_verLayout;

    double m_inPosX {0.0};
    double m_inPosY {0.0};

    double m_fnPosX {0.0};
    double m_fnPosY {0.0};

    double m_maxDeep;

    bool m_highLowOnRdg{true};

    int m_filterId    {0};
    int m_selectionId {0};
    int m_materialId  {0};
};

#endif // SECTIONRDGSSURFWIDGET_H
