#include "structureBscansWidget.h"
#include "structureBscansGraph.h"
#include "transformation.h"
#include "iostream"

structureBscansWidget::structureBscansWidget(QWidget* parent): QWidget(parent)
{
    QPointer<QWidget> container { QWidget::createWindowContainer(m_graph)};

    QSize   screenSize = m_graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    m_out3dCutBscansDataLabel = new QLabel("Линия разреза ");

    QPointer<QWidget>     widget  { new QWidget};
    QPointer<QVBoxLayout> vLayout { new QVBoxLayout(widget)};
    vLayout->addWidget(container);
    vLayout->addWidget(m_out3dCutBscansDataLabel);
    setLayout(vLayout);

    m_correctBscansSurfDialog = new correctBscansSurfDialog();

    connect(m_modifier, &structureBscansGraph::updateBscansTransGlWidget,     this,  &structureBscansWidget::receiveBscansTransGlWidgetPoints);
    connect(m_modifier, &structureBscansGraph::signalClearBscansTransGlWidget, this,  &structureBscansWidget::slotClearBscansTransGlWidget);

    connect(this,       &structureBscansWidget::signalCloseCut3dAction,    m_modifier,  &structureBscansGraph::slotCloseCut3dAction);
    connect(m_modifier, &structureBscansGraph::showCorrectBscansSurfWidget,  this,      &structureBscansWidget::showCorrectBscansSurfWidget);
    connect(m_modifier, &structureBscansGraph::closeCorrectBscansSurfWidget, this,      &structureBscansWidget::closeCorrectBscansSurfWidget);
    connect(m_modifier, &structureBscansGraph::sendOutBscansSurfInfo,        this,      &structureBscansWidget::receiveOutBscansSurfInfo);

    connect(m_correctBscansSurfDialog, &correctBscansSurfDialog::sendColorsBscansSurf, m_modifier, &structureBscansGraph::receiveColorsBscansSurf);
    connect(m_correctBscansSurfDialog, &correctBscansSurfDialog::signalSendDeleteBscanName, this,  &structureBscansWidget::receiveDeleteBscanName);

    connect(m_modifier, &structureBscansGraph::signalFixFillBscans,      this,  &structureBscansWidget::slotFixFillBscans);
}

structureBscansWidget::~structureBscansWidget(){}

void structureBscansWidget::receiveOutBscansSurfInfo(QString outBscansSurfInfo)
{
    m_out3dCutBscansDataLabel->setText("Линия разреза " + outBscansSurfInfo);
}

void structureBscansWidget::showCorrectBscansSurfWidget(QColor& colorBscansSurf1, QColor& colorBscansSurf2, double fixLatitude, double fixLongitude)
{
    m_correctBscansSurfDialog->m_colorBscansSurf1 = colorBscansSurf1;
    m_correctBscansSurfDialog->m_colorBscansSurf2 = colorBscansSurf2;
    emit signalFixDeleteBscanName(fixLatitude, fixLongitude);
}

void structureBscansWidget::slotDefDeleteBscanName(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, double fixLatitude, double fixLongitude)
{
    m_correctBscansSurfDialog->m_deleteBscanName = defDeleteBscanName(fixLatitude, fixLongitude, bscansInfoDataMap);
    if (!m_correctBscansSurfDialog->isVisible())  m_correctBscansSurfDialog->show();
}

void structureBscansWidget::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    m_modifier->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
}

void structureBscansWidget::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    m_modifier->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
}

void structureBscansWidget::closeCorrectBscansSurfWidget()
{
    if (m_correctBscansSurfDialog->isVisible()) m_correctBscansSurfDialog->close();
}

void structureBscansWidget::showBscansSurf(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
    int quantImpulsesOfPacket,  int filterId, int selectionId,  bool highLowOnBscan,
    bool showFullBscans, bool resetCutPointsBscans,int materialId,
    int absBscansStructInX, int absBscansStructFnX
)
{
    m_modifier->fillBscans(
        bscansInfoDataMap, bscansWorkData, quantImpulsesOfPacket, filterId,  selectionId,  highLowOnBscan,
        showFullBscans, resetCutPointsBscans, materialId, absBscansStructInX, absBscansStructFnX
    );
}

void structureBscansWidget::slotFillBscansExecute(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap,
    const st_bscansWorkData& bscansWorkData, bool resetCutPointsBscans
)
{
    m_modifier->fillBscansExecute(bscansInfoDataMap, bscansWorkData, resetCutPointsBscans);
}

void structureBscansWidget::receiveBscansTransGlWidgetPoints(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData,
    const std::vector<std::pair<int, int>>& vectorBscansTransLineSectionFinalPoints, bool highLowOnBscan
)
{
    emit updateBscansTransGlWidget(bscansInfoDataMap, bscansWorkData, vectorBscansTransLineSectionFinalPoints, highLowOnBscan);
}

void structureBscansWidget::slotClearBscansTransGlWidget()
{
    emit signalClearBscansTransGlWidget();
}

void structureBscansWidget::slotCloseCut3dAction(bool cut3dVisible)
{
    emit signalCloseCut3dAction(cut3dVisible);
}

void structureBscansWidget::receiveDeleteBscanName(const std::string& deleteBscanName)
{
    emit sendDeleteBscanName(deleteBscanName);
}

void structureBscansWidget::slotFixFillBscans(bool resetCutPointsBscans)
{
    emit signalFixFillBscans(resetCutPointsBscans);
}
