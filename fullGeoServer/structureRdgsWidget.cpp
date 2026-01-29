#include "structureRdgsWidget.h"
#include "structureRdgsGraph.h"
#include "transformation.h"
#include "iostream"

structureRdgsWidget::structureRdgsWidget(QWidget* parent): QWidget(parent)
{
    QPointer<QWidget> container { QWidget::createWindowContainer(m_graph)};

    QSize   screenSize = m_graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    m_out3dCutRdgsDataLabel = new QLabel("Линия разреза ");

    QPointer<QWidget>     widget  { new QWidget};
    QPointer<QVBoxLayout> vLayout { new QVBoxLayout(widget)};
    vLayout->addWidget(container);
    vLayout->addWidget(m_out3dCutRdgsDataLabel);
    setLayout(vLayout);

    m_correctRdgsSurfDialog = new correctRdgsSurfDialog();

    connect(m_modifier, &structureRdgsGraph::updateRdgsTransGlWidget,      this,  &structureRdgsWidget::receiveRdgsTransGlWidgetPoints);
    connect(m_modifier, &structureRdgsGraph::signalClearRdgsTransGlWidget, this,  &structureRdgsWidget::slotClearRdgsTransGlWidget);

    connect(this,       &structureRdgsWidget::signalCloseCut3dAction,    m_modifier,  &structureRdgsGraph::slotCloseCut3dAction);
    connect(m_modifier, &structureRdgsGraph::showCorrectRdgsSurfWidget,  this,        &structureRdgsWidget::showCorrectRdgsSurfWidget);
    connect(m_modifier, &structureRdgsGraph::closeCorrectRdgsSurfWidget, this,        &structureRdgsWidget::closeCorrectRdgsSurfWidget);
    connect(m_modifier, &structureRdgsGraph::sendOutRdgsSurfInfo,        this,        &structureRdgsWidget::receiveOutRdgsSurfInfo);

    connect(m_correctRdgsSurfDialog, &correctRdgsSurfDialog::sendColorsRdgsSurf, m_modifier, &structureRdgsGraph::receiveColorsRdgsSurf);
    connect(m_correctRdgsSurfDialog, &correctRdgsSurfDialog::signalSendDeleteRdgName, this,  &structureRdgsWidget::receiveDeleteRdgName);

    connect(m_modifier, &structureRdgsGraph::signalFixFillRdgs,      this,  &structureRdgsWidget::slotFixFillRdgs);
}

structureRdgsWidget::~structureRdgsWidget(){}

void structureRdgsWidget::receiveOutRdgsSurfInfo(QString outRdgsSurfInfo)
{
    m_out3dCutRdgsDataLabel->setText("Линия разреза " + outRdgsSurfInfo);
}

void structureRdgsWidget::showCorrectRdgsSurfWidget(QColor& colorRdgsSurf1, QColor& colorRdgsSurf2, double fixLatitude, double fixLongitude)
{
    m_correctRdgsSurfDialog->m_colorRdgsSurf1 = colorRdgsSurf1;
    m_correctRdgsSurfDialog->m_colorRdgsSurf2 = colorRdgsSurf2;
    emit signalFixDeleteRdgName(fixLatitude, fixLongitude);
}

void structureRdgsWidget::slotDefDeleteRdgName(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, double fixLatitude, double fixLongitude)
{
    m_correctRdgsSurfDialog->m_deleteRdgName = defDeleteRdgName(fixLatitude, fixLongitude, rdgsInfoDataMap);
    if (!m_correctRdgsSurfDialog->isVisible())  m_correctRdgsSurfDialog->show();
}

void structureRdgsWidget::saveBoundPointsLineAction(const QString& saveBoundPointsLinePointsFile)
{
    m_modifier->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
}

void structureRdgsWidget::loadBoundPointsLineAction(const QString& loadBoundPointsLinePointsFile)
{
    m_modifier->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
}

void structureRdgsWidget::closeCorrectRdgsSurfWidget()
{
    if (m_correctRdgsSurfDialog->isVisible()) m_correctRdgsSurfDialog->close();
}

void structureRdgsWidget::showRdgsSurf(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
    int quantImpulsesOfPacket,  int filterId, int selectionId,  bool highLowOnRdg, bool showFullRdgs, bool resetCutPointsRdgs,int materialId,
    int absRdgsStructInX, int absRdgsStructFnX
)
{
    m_modifier->fillRdgs(
        rdgsInfoDataMap, rdgsWorkData, quantImpulsesOfPacket, filterId,  selectionId,  highLowOnRdg,
        showFullRdgs, resetCutPointsRdgs, materialId, absRdgsStructInX, absRdgsStructFnX
    );
}

void structureRdgsWidget::slotFillRdgsExecute(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData, bool resetCutPointsRdgs
)
{
    m_modifier->fillRdgsExecute(rdgsInfoDataMap, rdgsWorkData, resetCutPointsRdgs);
}

void structureRdgsWidget::receiveRdgsTransGlWidgetPoints(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData,
    const std::vector<std::pair<int, int>>& vectorRdgsTransLineSectionFinalPoints, bool highLowOnRdg
)
{
    emit updateRdgsTransGlWidget(rdgsInfoDataMap, rdgsWorkData, vectorRdgsTransLineSectionFinalPoints, highLowOnRdg);
}

void structureRdgsWidget::slotClearRdgsTransGlWidget()
{
    emit signalClearRdgsTransGlWidget();
}

void structureRdgsWidget::slotCloseCut3dAction(bool cut3dVisible)
{
    emit signalCloseCut3dAction(cut3dVisible);
}

void structureRdgsWidget::receiveDeleteRdgName(const std::string& deleteRdgName)
{
    emit sendDeleteRdgName(deleteRdgName);
}

void structureRdgsWidget::slotFixFillRdgs(bool resetCutPointsRdgs)
{
    emit signalFixFillRdgs(resetCutPointsRdgs);
}
