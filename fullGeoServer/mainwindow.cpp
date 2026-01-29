#include "mainwindow.h"
#include "transformation.h"
#include <iostream>

Q_DECLARE_METATYPE(QStringList)

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<RdgFileFormat>("RdgFileFormat");
    qRegisterMetaType<QColor>("QColor");
    qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");
    qRegisterMetaType<std::vector<std::pair<std::string, std::string>>>("std::vector<std::pair<std::string, std::string>>");
    qRegisterMetaTypeStreamOperators<QStringList>("QStringList");

    m_stackedWidget = new QStackedWidget;
    m_stackedWidget->addWidget(m_rdg2dWidget);
    m_stackedWidget->addWidget(m_rdgs2dWidget);
    m_stackedWidget->addWidget(m_rdgs3dWidget);

    m_rdgsNamesVectorPairs.resize(0);
    m_recentRdgsFileActsVector.resize(0);

    init();

    connect(this, &MainWindow::signalClearRdgsInfoDataMap, m_rdg2dWidget, &rdg2dWidget::slotClearRdgsInfoDataMap);
    connect(this, &MainWindow::signalEraseRdgsInfoDataMap, m_rdg2dWidget, &rdg2dWidget::slotEraseRdgsInfoDataMap);

    connect(this, &MainWindow::closeSection2dAction,   m_rdgs2dWidget, &rdgs2dWidget::slotCloseSection2dAction);
    connect(this, &MainWindow::closeCut3dAction,   m_rdgs3dWidget, &rdgs3dWidget::slotCloseCut3dAction);
    connect(m_rdgs3dWidget, &rdgs3dWidget::sendDeleteRdgName, this, &MainWindow::receiveDeleteRdgName);

    connect(m_rdgs2dWidget, &rdgs2dWidget::signalFixRdgsSurfPixels,    this, &MainWindow::slotFixRdgsSurfPixels);
    connect(this, &MainWindow::signalSetupRdgsSurfPixels, m_rdgs2dWidget, &rdgs2dWidget::slotSetupRdgsSurfPixels);

    connect(m_rdgs3dWidget, &rdgs3dWidget::signalFixDeleteRdgName, this, &MainWindow::slotFixDeleteRdgName);
    connect(m_rdgs3dWidget, &rdgs3dWidget::signalFixFillRdgs,    this, &MainWindow::slotFixFillRdgs);

    connect(this, &MainWindow::signalDefDeleteRdgName,  m_rdgs3dWidget, &rdgs3dWidget::slotDefDeleteRdgName);
    connect(this, &MainWindow::signalFillRdgsExecute, m_rdgs3dWidget, &rdgs3dWidget::slotFillRdgsExecute);

    m_progressBar = new QProgressBar;

    m_mainWidget = new QWidget();
    m_mainLayout  = new QVBoxLayout;
    m_mainLayout->addWidget(m_stackedWidget);

    m_materialComboBox = new QComboBox(this);
    m_materialComboBox->addItem("Пресная вода");
    m_materialComboBox->addItem("Морская вода");
    m_materialComboBox->addItem("Песчанная почва сухая");
    m_materialComboBox->addItem("Песчанная почва влажная");
    m_materialComboBox->addItem("Суглинок сухой");
    m_materialComboBox->addItem("Суглинок влажный");
    m_materialComboBox->addItem("Глинистая почва сухая");
    m_materialComboBox->addItem("Глинистая почва влажная");
    m_materialComboBox->addItem("Базальт влажный");
    m_materialComboBox->addItem("Гранит");
    m_materialComboBox->addItem("Гранитный сланец влажный");
    m_materialComboBox->addItem("Песчаник влажный");
    m_materialComboBox->addItem("Известняк влажный");
    
    m_materialComboBox->setCurrentIndex(m_materialId);
    m_materialComboBox->setFixedHeight(20);
    m_materialComboBox->setFixedWidth(200);

    m_scrollHRdgsLengthBar = new QScrollBar(Qt::Horizontal);
    m_scrollHRdgsLengthBar->setFocusPolicy(Qt::StrongFocus);
    m_scrollHRdgsLengthBar->setFixedHeight(20);
    m_scrollHRdgsLengthBar->setFixedWidth(300);
    m_scrollHRdgsLengthBar->setVisible(false);

    m_rdgQuantImpulsesLabel = new QLabel("Число отсчетов rdg ");
    m_rdgQuantImpulsesSlider = new QSlider(Qt::Horizontal);
    m_rdgQuantImpulsesSlider->setSingleStep(1);
    m_rdgQuantImpulsesSlider->setTickPosition(QSlider::TicksRight);

    m_rdgsTransitSlider = new QSlider(Qt::Horizontal);
    m_rdgsTransitSlider->setSingleStep(1);
    m_rdgsTransitSlider->setTickPosition(QSlider::TicksRight);
    m_leftRdgsTransitButton  = new QPushButton(tr("<-"), this);
    m_rightRdgsTransitButton = new QPushButton(tr("->"), this);

    m_materialComboBox->setVisible(false);
    openScrollHRdgsLengthBar(false);
    rdgsTransitControlsVisible(false);
    impulsesRdgControlsVisible(false);

    m_lowLayout  = new QHBoxLayout;
    m_lowLayout->addWidget(m_materialComboBox);
    m_lowLayout->addWidget(m_scrollHRdgsLengthBar);

    m_lowLayout->addWidget(m_rdgQuantImpulsesLabel);
    m_lowLayout->addWidget(m_rdgQuantImpulsesSlider);

    m_lowLayout->addWidget(m_leftRdgsTransitButton);
    m_lowLayout->addWidget(m_rdgsTransitSlider);
    m_lowLayout->addWidget(m_rightRdgsTransitButton);
    m_mainLayout->addLayout(m_lowLayout);

    m_mainWidget->setLayout( m_mainLayout);
    setCentralWidget(m_mainWidget);

    connect(m_rdg2dWidget->m_accomplishment->m_thread, &accomplishmentThread::sendSurfRdgsWorkData, this, &MainWindow::receiveRdgsWorkData);
    connect(m_rdg2dWidget->m_accomplishment->m_thread, &accomplishmentThread::sendRdgsZData,        this, &MainWindow::receiveRdgsZData);
    connect(m_rdg2dWidget->m_accomplishment->m_thread, &accomplishmentThread::sendProgressData,     this, &MainWindow::receiveProgressData);

    connect(m_rdgQuantImpulsesSlider, &QSlider::valueChanged, this, &MainWindow::setQuantImpulsesOfPacketSlider);

    connect(m_rdgs2dWidget, &rdgs2dWidget::sendOutRdgsSurfInfo, this, &MainWindow::receiveOutRdgsSurfInfo);

    connect(m_rdg2dWidget, &rdg2dWidget::sendOutRdgInfo, this, &MainWindow::receiveOutRdgInfo);
    connect(m_rdg2dWidget, &rdg2dWidget::signalActivateAccompThread, this, &MainWindow::slotActivateAccompThread);

    connect(m_rdgs2dWidget, &rdgs2dWidget::signalFixRdgsSurfSection, this, &MainWindow::slotFixRdgsSurfSection);
    connect(this, &MainWindow::signalOutputRdgsSurfSection, m_rdgs2dWidget, &rdgs2dWidget::slotOutputRdgsSurfSection);

    connect(m_materialComboBox, SIGNAL(activated(int)), SLOT(materialIdChanged(int)));

    connect(m_rdgsTransitSlider,      &QSlider::valueChanged, this, &MainWindow::changeRdg);
    connect(m_leftRdgsTransitButton,  &QPushButton::clicked,  this, &MainWindow::leftChangeRdg);
    connect(m_rightRdgsTransitButton, &QPushButton::clicked,  this, &MainWindow::rightChangeRdg);

    connect(m_scrollHRdgsLengthBar, &QScrollBar::sliderPressed, this, &MainWindow::scrollHRdgsLengthBarPressed);
    connect(m_scrollHRdgsLengthBar, &QScrollBar::sliderMoved, this, &MainWindow::scrollHRdgsLengthBarMoved);
    connect(m_scrollHRdgsLengthBar, &QScrollBar::sliderReleased, this, &MainWindow::scrollHRdgsLengthBarReleased);
    connect(m_scrollHRdgsLengthBar, &QScrollBar::valueChanged, this, &MainWindow::scrollHRdgsLengthBarChanged);

    m_colorsPaletteDialog = new colorsPaletteDialog();
    connect(m_colorsPaletteDialog, &colorsPaletteDialog::sendColorsPalette, this, &MainWindow::receiveColorsPalette);

    readLastRdgsDirsFromSettings(m_trzDir, m_csvDir, m_hdf5Dir);
    setupLastRdgs();
}

MainWindow::~MainWindow(){}

void MainWindow::setupLastRdgs()
{
    m_filesRdgNamesTrz.clear();
    m_trzNumAntennasVector.resize(0);

    m_filesRdgNamesCsv.clear();
    m_infoRdgsHdf5Names.clear();

    readLastNamesRdgsFromSettings(m_filesRdgNamesTrz, m_trzNumAntennasVector, m_filesRdgNamesCsv, m_infoRdgsHdf5Names);

    if      (m_trzNumAntennasVector.size() > 0) m_rdg2dWidget->setupTrzRdgs(m_filesRdgNamesTrz, m_trzNumAntennasVector, m_rdgsNamesVectorPairs);
    else if (m_filesRdgNamesCsv.size()     > 0) m_rdg2dWidget->setupCsvRdgs(m_filesRdgNamesCsv, m_rdgsNamesVectorPairs);
    else if (m_infoRdgsHdf5Names.size()    > 0) m_rdg2dWidget->setupHdf5Rdgs(m_infoRdgsHdf5Names, m_rdgsNamesVectorPairs);
}

void MainWindow::receiveDeleteRdgName(const std::string& delRdgName)
{
    deleteFile(delRdgName);
}

void MainWindow::slotFixRdgsSurfPixels()
{
    emit signalSetupRdgsSurfPixels(m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData);
}

void MainWindow::slotFixRdgsSurfSection()
{
    emit signalOutputRdgsSurfSection(m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData);
}

void MainWindow::slotFixDeleteRdgName(double fixLatitude, double fixLongitude)
{
    emit signalDefDeleteRdgName(m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, fixLatitude, fixLongitude);
}

void MainWindow::slotFixFillRdgs(bool resetCutPointsRdgs)
{
    emit signalFillRdgsExecute(
        m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData, resetCutPointsRdgs
    );
}

void MainWindow::receiveProgressData(int progressPos, int progressMax)
{
    if (progressPos == 0)
    {
        m_progressBar->setMinimum(progressPos);
        m_progressBar->setMaximum(progressMax);
        m_progressBar->setValue(progressPos);
        m_progressBar->show();
    }
    else if (progressPos != progressMax) m_progressBar->setValue(progressPos);
    else if (progressPos == progressMax) m_progressBar->close();
}

void MainWindow::init()
{
    createActions();
    m_stackedWidget->setCurrentWidget(m_rdg2dWidget);
}

void MainWindow::receiveOutRdgsSurfInfo(QString outRdgsSurfInfo)
{
     statusBar()->showMessage(outRdgsSurfInfo);
}

void MainWindow::receiveOutRdgInfo(QString outRdgInfo)
{
    statusBar()->showMessage(outRdgInfo);
}

void MainWindow::createActions()
{
    QPointer<QMenu> fileMenu = menuBar()->addMenu(tr("&Файл"));

    QPointer<QAction> openTrzAct = new QAction(tr("&Открыть trz файлы..."), this);
    connect(openTrzAct, &QAction::triggered, this, &MainWindow::openTrz);
    fileMenu->addAction(openTrzAct);

    QPointer<QAction> openCsvAct = new QAction(tr("&Открыть csv файлы..."), this);
    connect(openCsvAct, &QAction::triggered, this, &MainWindow::openCsv);
    fileMenu->addAction(openCsvAct);

    QPointer<QAction> openHdf5Act = new QAction(tr("&Открыть hdf5 папки..."), this);
    connect(openHdf5Act, &QAction::triggered, this, &MainWindow::openHdf5);
    fileMenu->addAction(openHdf5Act);

    QPointer<QAction> openKmlAct = new QAction(tr("&Открыть kml файл..."), this);
    connect(openKmlAct, &QAction::triggered, this, &MainWindow::openKml);
    fileMenu->addAction(openKmlAct);

    QPointer<QAction> deleteAllRdgsAct = new QAction(tr("&Удалить все радарограммы"), this);
    connect(deleteAllRdgsAct, &QAction::triggered, this, &MainWindow::deleteAllFiles);
    fileMenu->addAction(deleteAllRdgsAct);

    QPointer<QAction> saveAct = new QAction(tr("&Сохранить"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить "));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);

    fileMenu->addSeparator();

    m_recentFilesRdgMenu = fileMenu->addMenu(tr("Недавние..."));
    m_deleteFilesRdgMenu = fileMenu->addMenu(tr("Удалить..."));

    QPointer<QAction> closeAct = fileMenu->addAction(tr("&Закрыть"), this, &MainWindow::close);
    closeAct->setShortcut(tr("Ctrl+W"));
    closeAct->setStatusTip(tr("Закрыть это окно"));

    QPointer<QMenu> imagesMenu = menuBar()->addMenu(tr("&Изображения"));

    QAction* rdg2DImageAct = new QAction(tr("&Изображение радарограммы"), this);
    connect(rdg2DImageAct, &QAction::triggered, this, &MainWindow::openImageRdg);
    imagesMenu->addAction(rdg2DImageAct);

    QAction* rdgPaletteAct = new QAction(tr("&Палитра радарограммы"), this);
    connect(rdgPaletteAct, &QAction::triggered, this, &MainWindow::defPaletteRdg);
    imagesMenu->addAction(rdgPaletteAct);

    QPointer<QAction> rdgs2DImageAct = new QAction(tr("&2D Изображение профилей по радарограммам"), this);
    connect( rdgs2DImageAct, &QAction::triggered, this, &MainWindow::open2dImageRdgs);
    imagesMenu->addAction(rdgs2DImageAct);

    QPointer<QAction> rdgsSurfPaletteAct = new QAction(tr("&Палитра по профилям радарограмм"), this);
    connect(rdgsSurfPaletteAct, &QAction::triggered, this, &MainWindow::defPaletteSurfRdgs);
    imagesMenu->addAction(rdgsSurfPaletteAct);

    QPointer<QAction> rdgs3DImageAct = new QAction(tr("&3D Изображение профилей по радарограммам"), this);
    connect( rdgs3DImageAct, &QAction::triggered, this, &MainWindow::open3dImageRdgs);
    imagesMenu->addAction(rdgs3DImageAct);

    QPointer<QAction> rdgsTransPaletteAct = new QAction(tr("&Палитра по переходу между радарограммами"), this);
    connect(rdgsTransPaletteAct, &QAction::triggered, this, &MainWindow::defPaletteTransRdgs);
    imagesMenu->addAction(rdgsTransPaletteAct);

    m_includeFilterOnRdgAction = new QAction(tr("&Включаем фильтр Перона и Малика"), this);
    m_includeFilterOnRdgAction->setCheckable(true);
    m_includeFilterOnRdgAction->setChecked(false);
    imagesMenu->addAction(m_includeFilterOnRdgAction);
    connect(m_includeFilterOnRdgAction, &QAction::toggled, this, &MainWindow::includeFilterOnRdg);

    QPointer<QMenu> toolsMenu = menuBar()->addMenu(tr("&Инструменты по 2D-3D"));

    m_closeSection2dAct = new QAction(tr("&Скрываем сечение 2D поверхности"), this);
    m_closeSection2dAct->setCheckable(true);
    m_closeSection2dAct->setChecked(!m_section2dVisible);
    toolsMenu->addAction(m_closeSection2dAct);
    connect(m_closeSection2dAct, &QAction::toggled, this, &MainWindow::setCloseSection2dAction);

    m_closeCut3dAct = new QAction(tr("&Скрываем разрез 3D структуры"), this);
    m_closeCut3dAct->setCheckable(true);
    m_closeCut3dAct->setChecked(!m_cut3dVisible);
    toolsMenu->addAction(m_closeCut3dAct);
    connect(m_closeCut3dAct, &QAction::toggled, this, &MainWindow::setCloseCut3dAction);

    m_saveBoundPointsLineAct = new QAction(tr("&Сохранение точек линии сечения/плоскости разреза"), this);
    toolsMenu->addAction(m_saveBoundPointsLineAct);
    connect(m_saveBoundPointsLineAct, &QAction::triggered, this, &MainWindow::saveBoundPointsLineAction);

    m_loadBoundPointsLineAct = new QAction(tr("&Загрузка точек линии сечения/плоскости разреза"), this);
    toolsMenu->addAction(m_loadBoundPointsLineAct);
    connect(m_loadBoundPointsLineAct, &QAction::triggered, this, &MainWindow::loadBoundPointsLineAction);

    toolsMenu->addSeparator();

    m_highLowImpulsesRdgsAction  = new QAction(tr("&Сверху-вниз/снизу-вверх по импульсам rdg"), this);
    m_highLowImpulsesRdgsAction->setCheckable(true);
    m_highLowImpulsesRdgsAction->setChecked(true);
    toolsMenu->addAction(m_highLowImpulsesRdgsAction);
    connect(m_highLowImpulsesRdgsAction, &QAction::triggered, this, &MainWindow::changeHighLowOnRdgs);

    m_showFullRdgsAction         = new QAction(tr("&Учет полных радарограмм"), this);
    m_showFullRdgsAction->setCheckable(true);
    m_showFullRdgsAction->setChecked(true);
    toolsMenu->addAction(m_showFullRdgsAction);
    connect(m_showFullRdgsAction, &QAction::triggered, this, &MainWindow::showFullRdgsData);

    enabledCutActions(false, false, false);

    QPointer<QMenu> actionsRdgMenu = menuBar()->addMenu(tr("&Операции с радарограммой"));
    m_showInitRdgAct = new QAction(tr("&Исходная радарограмма"), this);
    connect(m_showInitRdgAct, &QAction::triggered, this, &MainWindow::setInitRdg);
    actionsRdgMenu->addAction(m_showInitRdgAct);

    m_showLogRdgAct = new QAction(tr("&Логарифмическая радарограмма"), this);
    connect(m_showLogRdgAct, &QAction::triggered, this, &MainWindow::setLogRdg);
    actionsRdgMenu->addAction(m_showLogRdgAct);

    m_logRdgMouseSelectionAct = new QAction(tr("&Выделяем область мышкой по Log Rdg"), this);
    connect(m_logRdgMouseSelectionAct, &QAction::triggered, this, &MainWindow::setLogRdgMouseSelection);
    m_logRdgMouseSelectionAct->setCheckable(true);
    actionsRdgMenu->addAction(m_logRdgMouseSelectionAct);

    m_logRdgAutoSelectionAct = new QAction(tr("&Автоматическое выделение области по Log Rdg"), this);
    connect(m_logRdgAutoSelectionAct, &QAction::triggered, this, &MainWindow::setLogRdgAutoSelection);
    m_logRdgAutoSelectionAct->setCheckable(true);
    actionsRdgMenu->addAction(m_logRdgAutoSelectionAct);

    m_showInitRdgSelectionAct = new QAction(tr("&Показываем выделенные области на Init Rdg"), this);
    connect(m_showInitRdgSelectionAct, &QAction::triggered, this, &MainWindow::setShowInitRdgSelection);
    m_showInitRdgSelectionAct->setCheckable(true);
    actionsRdgMenu->addAction(m_showInitRdgSelectionAct);

    m_resetRdgSelectionAct = new QAction(tr("&Убираем выделенные области со всех Rdg"), this);
    connect(m_resetRdgSelectionAct, &QAction::triggered, this, &MainWindow::slotResetRdgSelection);
    actionsRdgMenu->addAction(m_resetRdgSelectionAct);

    m_saveRdgAutoSelectionDataAct = new QAction(tr("&Сохраняем автоматически-выделенные области"), this);
    connect(m_saveRdgAutoSelectionDataAct, &QAction::triggered, this, &MainWindow::saveRdgAutoSelectionData);
    actionsRdgMenu->addAction(m_saveRdgAutoSelectionDataAct);

    m_loadRdgAutoSelectionDataAct = new QAction(tr("&Загружаем автоматически-выделенные области"), this);
    connect(m_loadRdgAutoSelectionDataAct, &QAction::triggered, this, &MainWindow::loadRdgAutoSelectionData);
    actionsRdgMenu->addAction(m_loadRdgAutoSelectionDataAct);

    QPointer<QMenu> selectionsUseRdgMenu = menuBar()->addMenu(tr("&Активирование способа выделения областей по log образу rdg."));
    m_useVoidSelectionAct  = new QAction(tr("&нет выделенией по радарограмме"), this);
    m_useVoidSelectionAct->setData(QString::fromStdString("void selection is activated"));
    m_useVoidSelectionAct->setCheckable(true);
    m_useVoidSelectionAct->setChecked(true);
    connect(m_useVoidSelectionAct, &QAction::triggered, this, &MainWindow::useSelectionOnRdg);
    selectionsUseRdgMenu->addAction(m_useVoidSelectionAct);

    m_useMouseSelectionAct = new QAction(tr("&выделение мышью по радарограмме"), this);
    m_useMouseSelectionAct->setData(QString::fromStdString("mouse selection is activated"));
    m_useMouseSelectionAct->setCheckable(true);
    connect(m_useMouseSelectionAct, &QAction::triggered, this, &MainWindow::useSelectionOnRdg);
    selectionsUseRdgMenu->addAction(m_useMouseSelectionAct);

    m_useAutoSelectionAct  = new QAction(tr("&автоматическое выделение по радарограмме"), this);
    m_useAutoSelectionAct->setData(QString::fromStdString("auto selection is activated"));
    m_useAutoSelectionAct->setCheckable(true);
    connect(m_useAutoSelectionAct, &QAction::triggered, this, &MainWindow::useSelectionOnRdg);
    selectionsUseRdgMenu->addAction(m_useAutoSelectionAct);

    QPointer<QMenu> scalingsRdgsMenu = menuBar()->addMenu(tr("&Масштабирование"));
    m_rdgAllScalingAct  = new QAction(tr("&общее масштабирование радарограммы"), this);
    m_rdgAllScalingAct->setData(QString::fromStdString("all rdg scaling"));
    m_rdgAllScalingAct->setCheckable(true);
    m_rdgAllScalingAct->setChecked(true);
    connect(m_rdgAllScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdg);
    scalingsRdgsMenu->addAction(m_rdgAllScalingAct);

    m_rdgHScalingAct  = new QAction(tr("&горизонтальное масштабирование радарограммы"), this);
    m_rdgHScalingAct->setData(QString::fromStdString("h rdg scaling"));
    m_rdgHScalingAct->setCheckable(true);
    m_rdgHScalingAct->setChecked(false);
    connect(m_rdgHScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdg);
    scalingsRdgsMenu->addAction(m_rdgHScalingAct);

    m_rdgVScalingAct  = new QAction(tr("&вертикальное масштабирование радарограммы"), this);
    m_rdgVScalingAct->setData(QString::fromStdString("v rdg scaling"));
    m_rdgVScalingAct->setCheckable(true);
    m_rdgVScalingAct->setChecked(false);
    connect(m_rdgVScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdg);
    scalingsRdgsMenu->addAction(m_rdgVScalingAct);

    scalingsRdgsMenu->addSeparator();

    m_rdgsSurfAllScalingAct  = new QAction(tr("&общее масштабирование поверхности"), this);
    m_rdgsSurfAllScalingAct->setData(QString::fromStdString("all rdgs surf scaling"));
    m_rdgsSurfAllScalingAct->setCheckable(true);
    m_rdgsSurfAllScalingAct->setChecked(true);
    connect(m_rdgsSurfAllScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdgsSurf);
    scalingsRdgsMenu->addAction(m_rdgsSurfAllScalingAct);

    m_rdgsSurfHScalingAct  = new QAction(tr("&горизонтальное масштабирование поверхности"), this);
    m_rdgsSurfHScalingAct->setData(QString::fromStdString("h rdgs surf scaling"));
    m_rdgsSurfHScalingAct->setCheckable(true);
    m_rdgsSurfHScalingAct->setChecked(false);
    connect(m_rdgsSurfHScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdgsSurf);
    scalingsRdgsMenu->addAction(m_rdgsSurfHScalingAct);

    m_rdgsSurfVScalingAct  = new QAction(tr("&вертикальное масштабирование поверхности"), this);
    m_rdgsSurfVScalingAct->setData(QString::fromStdString("v rdgs surf scaling"));
    m_rdgsSurfVScalingAct->setCheckable(true);
    m_rdgsSurfVScalingAct->setChecked(false);
    connect(m_rdgsSurfVScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdgsSurf);
    scalingsRdgsMenu->addAction(m_rdgsSurfVScalingAct);

    scalingsRdgsMenu->addSeparator();

    m_rdgsTransAllScalingAct  = new QAction(tr("&общее масштабирование между радарограммами"), this);
    m_rdgsTransAllScalingAct->setData(QString::fromStdString("all rdgs trans scaling"));
    m_rdgsTransAllScalingAct->setCheckable(true);
    m_rdgsTransAllScalingAct->setChecked(true);
    connect(m_rdgsTransAllScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdgsTrans);
    scalingsRdgsMenu->addAction(m_rdgsTransAllScalingAct);

    m_rdgsTransHScalingAct  = new QAction(tr("&горизонтальное масштабирование между радарограммами"), this);
    m_rdgsTransHScalingAct->setData(QString::fromStdString("h rdgs trans scaling"));
    m_rdgsTransHScalingAct->setCheckable(true);
    m_rdgsTransHScalingAct->setChecked(false);
    connect(m_rdgsTransHScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdgsTrans);
    scalingsRdgsMenu->addAction(m_rdgsTransHScalingAct);

    m_rdgsTransVScalingAct  = new QAction(tr("&вертикальное масштабирование между радарограммами"), this);
    m_rdgsTransVScalingAct->setData(QString::fromStdString("v rdgs trans scaling"));
    m_rdgsTransVScalingAct->setCheckable(true);
    m_rdgsTransVScalingAct->setChecked(false);
    connect(m_rdgsTransVScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnRdgsTrans);
    scalingsRdgsMenu->addAction(m_rdgsTransVScalingAct);

    enabledShowRdgActions(false);
}

void MainWindow::openTrz()
{
    if (m_trzDir == "") m_trzDir = QCoreApplication::applicationDirPath();

    m_filesRdgNamesTrz = QFileDialog::getOpenFileNames(this, tr("Select Multiple Files"), m_trzDir, tr("Image Files(*.trz)"));
    m_trzNumAntennasVector.resize(0);
    if (m_filesRdgNamesTrz.size() > 0)
    {
        m_trzDir = "";
        for (int count = 0; count <= m_filesRdgNamesTrz.at(0).split("/").size()-2; count++)
        {
            if (count != m_filesRdgNamesTrz.at(0).split("/").size()-2) m_trzDir = m_trzDir + m_filesRdgNamesTrz.at(0).split("/").at(count) + "/";
            else                                                       m_trzDir = m_trzDir + m_filesRdgNamesTrz.at(0).split("/").at(count) ;
        }
        writeLastRdgsDirs(m_trzDir, m_csvDir, m_hdf5Dir);
        m_rdg2dWidget->setupTrzRdgs(m_filesRdgNamesTrz, m_trzNumAntennasVector, m_rdgsNamesVectorPairs);
    }
}

void MainWindow::openCsv()
{   
    if (m_csvDir == "") m_csvDir = QCoreApplication::applicationDirPath();

    m_rdg2dWidget->m_accomplishment->m_thread->m_executeParserData = false;
    m_filesRdgNamesCsv = QFileDialog::getOpenFileNames(this, tr("Select Multiple Files"), m_csvDir, tr("Image Files(*.csv)"));
    if (m_filesRdgNamesCsv.size() > 0)
    {
        m_csvDir = "";
        for (int count = 0; count <= m_filesRdgNamesCsv.at(0).split("/").size()-2; count++)
        {
            if (count != m_filesRdgNamesCsv.at(0).split("/").size()-2) m_csvDir = m_csvDir + m_filesRdgNamesCsv.at(0).split("/").at(count) + "/";
            else                                                       m_csvDir = m_csvDir + m_filesRdgNamesCsv.at(0).split("/").at(count) + "/";
        }
        writeLastRdgsDirs(m_trzDir, m_csvDir, m_hdf5Dir);
        m_rdg2dWidget->setupCsvRdgs(m_filesRdgNamesCsv, m_rdgsNamesVectorPairs);
    }
}

void MainWindow::openHdf5()
{
    if (m_hdf5Dir == "") m_hdf5Dir = QCoreApplication::applicationDirPath();

    QSharedPointer<QFileDialog> hdf5dlg { new QFileDialog(this)};

    hdf5dlg->setFileMode(QFileDialog::Directory);
    hdf5dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    hdf5dlg->setDirectory(m_hdf5Dir);

    QSharedPointer<QTreeView> treeHdf5 { this->findChild<QTreeView*>()};
    if (treeHdf5)  treeHdf5->setSelectionMode(QAbstractItemView::ExtendedSelection);

    hdf5dlg->exec();

    m_hdf5Dir = hdf5dlg->directory().canonicalPath();

    m_infoRdgsHdf5Names = hdf5dlg->selectedFiles();

    writeLastRdgsDirs(m_trzDir, m_csvDir, m_hdf5Dir);

    m_rdg2dWidget->setupHdf5Rdgs(m_infoRdgsHdf5Names, m_rdgsNamesVectorPairs);
    hdf5dlg->close();
}

void MainWindow::setupRdgCurrentName()
{
    if (m_rdgCurrentName == "")  m_rdgCurrentName = m_rdgsNamesVectorPairs[0].first;  
}

void MainWindow::setupRdgControls()
{
    m_rdg2dWidget->showNewRdg(m_rdgCurrentName);
    m_materialComboBox->setVisible(true);
    if (m_stackedWidget->currentIndex() == 0)
    {
        enabledShowRdgActions(true);
        openScrollHRdgsLengthBar(true);
        if (m_rdgsNamesVectorPairs.size() > 1)
        {
            setupRdgsSliderSetup(0, m_rdgsNamesVectorPairs.size());
            rdgsTransitControlsVisible(true);
        }
    }
    createActionsFilesMenu();
}

void MainWindow::controlRdgsSamplingData( bool& dataSampling, bool& executeParserData, RdgFileFormat rdgFileFormat)
{
    switch(rdgFileFormat)
    {
        case RdgFileFormat::Trz:
        {
            m_filesRdgNamesTrz.clear();
            m_trzNumAntennasVector.clear();
            if (m_filesRdgNamesCsv.size()     > 0)      m_rdg2dWidget->setupCsvRdgs(m_filesRdgNamesCsv, m_rdgsNamesVectorPairs);
            else if (m_infoRdgsHdf5Names.size()   > 0) m_rdg2dWidget->setupHdf5Rdgs(m_infoRdgsHdf5Names, m_rdgsNamesVectorPairs);
            break;
        }
        case RdgFileFormat::Csv:
        {
            m_filesRdgNamesCsv.clear();
            if (m_infoRdgsHdf5Names.size()   > 0) m_rdg2dWidget->setupHdf5Rdgs(m_infoRdgsHdf5Names, m_rdgsNamesVectorPairs);
            break;
        }
        case RdgFileFormat::Hdf5:
        {
            m_infoRdgsHdf5Names.clear();
            break;
        }
    }

    if (m_filesRdgNamesTrz.size() == 0 && m_trzNumAntennasVector.size() == 0 && m_filesRdgNamesCsv.size() == 0 && m_infoRdgsHdf5Names.size() == 0)
    {
        setupRdgCurrentName();
        setupRdgControls();
        if (dataSampling == true) dataSampling = false;
        if (m_rdgsNamesVectorPairs.size() > 1)
        {
            executeParserData = true;
            m_rdg2dWidget->m_accomplishment->m_thread->start();
        }
    }
}

void MainWindow::slotActivateAccompThread(std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, RdgFileFormat rdgFileFormat)
{
    m_rdgsNamesVectorPairs.swap(rdgsNamesVectorPairs);

    if      (rdgFileFormat == RdgFileFormat::Trz)  controlRdgsSamplingData(m_rdg2dWidget->m_accomplishment->m_thread->m_trzDataSampling, m_rdg2dWidget->m_accomplishment->m_thread->m_executeParserData, rdgFileFormat);
    else if (rdgFileFormat == RdgFileFormat::Csv)  controlRdgsSamplingData( m_rdg2dWidget->m_accomplishment->m_thread->m_csvDataSampling, m_rdg2dWidget->m_accomplishment->m_thread->m_executeParserData, rdgFileFormat);
    else if (rdgFileFormat == RdgFileFormat::Hdf5) controlRdgsSamplingData(m_rdg2dWidget->m_accomplishment->m_thread->m_hdf5DataSampling, m_rdg2dWidget->m_accomplishment->m_thread->m_executeParserData, rdgFileFormat);
}

void MainWindow::openKml()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select Kml File"),
        QCoreApplication::applicationDirPath(),
        tr("Image Files(*.kml)")
    );

    if (fileName.isNull() || fileName.isEmpty())  return;
    else
    {
        defRdgsInRelief(fileName, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.vectorRdgsInRelief);
        if (m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.leftLatitude > 0.0)
        {
            m_rdg2dWidget->m_accomplishment->m_thread->m_defRdgsFnRelief = true;
            m_rdg2dWidget->m_accomplishment->m_thread->m_executeParserData = true;
            m_rdg2dWidget->m_accomplishment->m_thread->start();
        }
    }
}

void MainWindow::openImageRdg()
{
    enabledCutActions(false, false, false);
    if (m_rdgCurrentName != "")
    {
        m_stackedWidget->setCurrentWidget(m_rdg2dWidget);
        impulsesRdgControlsVisible(false);
        if (m_rdgsNamesVectorPairs.size() > 1)
        {
            openScrollHRdgsLengthBar(true);
            rdgsTransitControlsVisible(true);
        }
        statusBar()->show();
        enabledShowRdgActions(true);
        showRdgImage(true);
    }
}

void MainWindow::defPaletteRdg()
{
    if (m_rdgCurrentName != "" && m_stackedWidget->currentIndex() == 0 && !m_colorsPaletteDialog->isVisible()) m_colorsPaletteDialog->exec();
}

void MainWindow::defAbsRdgsStructInXAndFnX(int& absRdgsStructInX, int& absRdgsStructFnX)
{
    if ( (m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfWidth > limitRdgWidth) && (m_showFullRdgs == true) )
    {
        absRdgsStructInX = m_scrollHRdgsLengthBar->value()-limitRdgWidth+1;
        absRdgsStructFnX = m_scrollHRdgsLengthBar->value();
    }
    else if (m_showFullRdgs == true)
    {
        absRdgsStructInX = 0;
        absRdgsStructFnX = m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfWidth-1;
    }

    if  ((m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfMinWidth > limitRdgWidth) && (m_showFullRdgs == false) )
    {
        absRdgsStructInX = m_scrollHRdgsLengthBar->value()-limitRdgWidth+1;
        absRdgsStructFnX = m_scrollHRdgsLengthBar->value();
    }
    else if (m_showFullRdgs == false)
    {
        absRdgsStructInX = 0;
        absRdgsStructFnX = m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfMinWidth-1;
    }
}

void MainWindow::scrollHRdgsLengthBarResetDataAtRdgsImages()
{
    if (m_showFullRdgs == true)
    {
        if  (m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfWidth > limitRdgWidth)
            openScrollHRdgsLengthBar(true);
        else
            samplingAllLog10Rdgs(
                m_materialId, m_filterId, m_rdgsNamesVectorPairs, m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap,
                0, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfWidth-1
            );
    }
    else
    {
        if  (m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfMinWidth > limitRdgWidth)
            openScrollHRdgsLengthBar(true);
        else
            samplingAllLog10Rdgs(
                m_materialId, m_filterId, m_rdgsNamesVectorPairs, m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap,
                0, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfMinWidth-1
            );
    }
}

void MainWindow::open2dImageRdgs()
{
    if (m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap.size()>1)
    {
        m_stackedWidget->setCurrentWidget(m_rdgs2dWidget);

        int absRdgsStructInX {0};
        int absRdgsStructFnX {0};

        enabledCutActions(true, false, true);   
        impulsesRdgControlsVisible(true);
        rdgsTransitControlsVisible(false);

        statusBar()->show();
        enabledShowRdgActions(false);

        scrollHRdgsLengthBarResetDataAtRdgsImages();
        defAbsRdgsStructInXAndFnX(absRdgsStructInX, absRdgsStructFnX);
        m_rdgs2dWidget->showRdgsSurf(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_quantImpulsesOfPacket, m_filterId, m_selectionId, m_highLowOnRdg, m_showFullRdgs, m_resetSectPoints2dRdgs, m_materialId,
            absRdgsStructInX,  absRdgsStructFnX
        );

    }
}

void  MainWindow::defPaletteSurfRdgs()
{
    if (m_stackedWidget->currentIndex() == 1 && !m_colorsPaletteDialog->isVisible()) m_colorsPaletteDialog->exec();
}

void MainWindow::open3dImageRdgs()
{
    if (m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap.size()>1)
    {
        m_stackedWidget->setCurrentWidget(m_rdgs3dWidget);

        int absRdgsStructInX {0};
        int absRdgsStructFnX {0};

        enabledCutActions(false, true, true);
        impulsesRdgControlsVisible(true);
        rdgsTransitControlsVisible(false);

        statusBar()->hide();
        enabledShowRdgActions(true);

        scrollHRdgsLengthBarResetDataAtRdgsImages();
        defAbsRdgsStructInXAndFnX(absRdgsStructInX, absRdgsStructFnX);
        m_rdgs3dWidget->showRdgsImage(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId, m_highLowOnRdg, m_showFullRdgs, m_resetCutPointsRdgs,
            m_materialId, absRdgsStructInX,  absRdgsStructFnX
        );
    }
}

void  MainWindow::defPaletteTransRdgs()
{
    if (m_stackedWidget->currentIndex() == 2 && !m_colorsPaletteDialog->isVisible()) m_colorsPaletteDialog->exec();
}

void MainWindow::receiveColorsPalette(QColor& colorUp, QColor& colorLow)
{
    if (m_rdgCurrentName != "")
    {
        switch (m_stackedWidget->currentIndex())
        {
            case 0:
            {
                m_rdg2dWidget->setupColorsRdgAllProps(colorUp, colorLow);
                break;
            }
            case 1:
            {
                m_rdgs2dWidget->setupColorsRdgsSurfAllProps(colorUp, colorLow);
                break;
            }
            case 2:
            {
                m_rdgs3dWidget->setupColorsRdgsTransAllProps(colorUp, colorLow);
                break;
            }
        }
    }
}

void MainWindow::openRecentFile()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender()))
    {
        m_rdgCurrentName = action->data().toString().toStdString();
        if (m_stackedWidget->currentIndex() == 0)
        {
            auto vectorPairsIt = std::find_if(
                m_rdgsNamesVectorPairs.begin(), m_rdgsNamesVectorPairs.end(), [&](const std::pair<std::string, std::string>& stringPair) { return (stringPair.first == m_rdgCurrentName); }
            );
            
            setupRdgsSliderSetup(std::distance(m_rdgsNamesVectorPairs.begin(), vectorPairsIt), m_rdgsNamesVectorPairs.size());
            showRdgImage(true);
        }
    }
}

void MainWindow::deleteSelectedFile()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))  deleteFile(action->data().toString().toStdString());
}

void MainWindow::deleteAllFiles()
{
    for (int count = 0; count < m_rdgsNamesVectorPairs.size(); count++)
    {      
        emit signalEraseRdgsInfoDataMap(m_rdgsNamesVectorPairs[count].first);
        m_rdg2dWidget->m_rdgGlWidget->clearSelectedData(m_rdgsNamesVectorPairs[count].first);
    }

    m_rdgsNamesVectorPairs.clear();
    createActionsFilesMenu();

    emit signalClearRdgsInfoDataMap();

    m_stackedWidget->setCurrentWidget(m_rdg2dWidget);
    m_rdgCurrentName = "";
    m_rdg2dWidget->m_rdgGlWidget->m_rdgName = "";
    enabledShowRdgActions(false);

    showRdgImage(true);
    m_rdgQuantImpulsesLabel->setText("Число отсчетов rdg");
    m_materialComboBox->setVisible(false);
    openScrollHRdgsLengthBar(false);
    impulsesRdgControlsVisible(false);
    rdgsTransitControlsVisible(false);
    enabledCutActions(false, false, false);

    m_rdg2dWidget->m_accomplishment->m_thread->clearWorkData();
}

void MainWindow::save()
{
    if (m_rdgCurrentName != "")
    {
        switch (m_stackedWidget->currentIndex())
        {
            case 0:
            {
                m_rdg2dWidget->saveRdg();
                break;
            }
            case 1:
            {
                m_rdgs2dWidget->saveRdgsSurf();
                break;
            }
            case 2:
            {
                m_rdgs3dWidget->saveRdgsStruct();
                break;
            }
        }
    }
}

void MainWindow::deleteFile(const std::string& fileName)
{
    m_recentRdgsFileActsVector.resize(0);
    m_deleteRdgsFileActsVector.resize(0);

    emit signalEraseRdgsInfoDataMap(fileName);

    m_rdg2dWidget->m_rdgGlWidget->clearSelectedData(fileName);

    auto vectorPairsIt = std::find_if(
        m_rdgsNamesVectorPairs.begin(), m_rdgsNamesVectorPairs.end(), [&](const std::pair<std::string, std::string>& stringPair) { return (stringPair.first == fileName); }
    );
    
    m_rdgsNamesVectorPairs.erase(vectorPairsIt);

    if (m_rdgsNamesVectorPairs.size()>=0)  createActionsFilesMenu();

    if (m_rdgsNamesVectorPairs.size()>1)
    {
        if (m_rdgCurrentName == fileName) m_rdgCurrentName = m_rdgsNamesVectorPairs[0].first;
        if (m_stackedWidget->currentIndex() == 0 && m_rdgCurrentName != fileName)
        {
            showRdgImage(true);
            openScrollHRdgsLengthBar(true);   
        }

        setupRdgsSliderSetup(0, m_rdgsNamesVectorPairs.size());

        m_rdg2dWidget->m_accomplishment->m_thread->m_executeParserData = true;
        m_rdg2dWidget->m_accomplishment->m_thread->m_nameDeleteRdg     = fileName;
        m_rdg2dWidget->m_accomplishment->m_thread->m_executeDeleteRdg  = true;
        m_rdg2dWidget->m_accomplishment->m_thread->start();
    }
    else
    {
        m_stackedWidget->setCurrentWidget(m_rdg2dWidget);
        if (m_rdgsNamesVectorPairs.size() == 1)
        {
            m_rdgCurrentName = m_rdgsNamesVectorPairs[0].first;
            enabledCutActions(false, false, false);
            enabledShowRdgActions(true);
        }
        else
        {
            m_rdgCurrentName = "";
            m_rdg2dWidget->m_rdgGlWidget->m_rdgName = "";
            enabledShowRdgActions(false);
            m_materialComboBox->setVisible(false);
            m_rdg2dWidget->m_accomplishment->m_thread->clearWorkData();
        }

        showRdgImage(true);
        m_rdgQuantImpulsesLabel->setText("Число отсчетов rdg");
        openScrollHRdgsLengthBar(false);
        impulsesRdgControlsVisible(false);
        rdgsTransitControlsVisible(false);
        enabledCutActions(false, false, false);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::showRdgImage(bool changeRdg)
{
    if (changeRdg == true) resetRdgSelection(false);
    m_rdg2dWidget->outputNewImage(m_rdgCurrentName, m_materialId, m_filterId);
}

void MainWindow::setCloseSection2dAction()
{
    m_section2dVisible = !m_section2dVisible;
    emit closeSection2dAction(m_section2dVisible);
}

void MainWindow::setCloseCut3dAction()
{
    m_cut3dVisible = !m_cut3dVisible;
    emit closeCut3dAction(m_cut3dVisible);
}

void MainWindow::saveBoundPointsLineAction()
{
    QString saveBoundPointsLinePointsFile = QFileDialog::getSaveFileName(this, tr("Save As"), "cutLinePointsFile", tr("CSV File(*.csv);"));
    if (saveBoundPointsLinePointsFile != "")
    {
        if      (m_stackedWidget->currentIndex() == 1) m_rdgs2dWidget->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
        else if (m_stackedWidget->currentIndex() == 2) m_rdgs3dWidget->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
    }
}

void MainWindow::loadBoundPointsLineAction()
{
    QString loadBoundPointsLinePointsFile = QFileDialog::getOpenFileName(this, tr("Open csv file"), "cutLinePointsFile", tr("CSV File(*.csv);"));
    if (loadBoundPointsLinePointsFile != "")
    {
        if      (m_stackedWidget->currentIndex() == 1)  m_rdgs2dWidget->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
        else if (m_stackedWidget->currentIndex() == 2)  m_rdgs3dWidget->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
    }
}

void MainWindow::createActionsFilesMenu()
{
    m_recentFilesRdgMenu->clear();
    m_deleteFilesRdgMenu->clear();

    std::vector<std::string> rdgsNamesVector;
    rdgsNamesVector.resize(0);
    for (int count = 0; count < m_rdgsNamesVectorPairs.size(); count++)
    {
        rdgsNamesVector.push_back(m_rdgsNamesVectorPairs[count].first);
    }
    std::sort(rdgsNamesVector.begin(), rdgsNamesVector.end());

    m_recentRdgsFileActsVector.resize(rdgsNamesVector.size());
    m_deleteRdgsFileActsVector.resize(rdgsNamesVector.size());

    for (int i = 0; i < m_recentRdgsFileActsVector.size(); i++)
    {
        m_recentRdgsFileActsVector[i] =
        m_recentFilesRdgMenu         ->addAction(QString::fromStdString(rdgsNamesVector[i]), this, &MainWindow::openRecentFile);
        m_recentRdgsFileActsVector[i]->  setData(QString::fromStdString(rdgsNamesVector[i]));
        m_recentRdgsFileActsVector[i]->setVisible(true);

        m_deleteRdgsFileActsVector[i] =
        m_deleteFilesRdgMenu         ->addAction(QString::fromStdString(rdgsNamesVector[i]), this, &MainWindow::deleteSelectedFile);
        m_deleteRdgsFileActsVector[i]->  setData(QString::fromStdString(rdgsNamesVector[i]));
        m_deleteRdgsFileActsVector[i]->setVisible(true);
    }

    writeLastNamesRdgsToSettings(m_rdgsNamesVectorPairs);
}

void MainWindow::receiveRdgsWorkData()
{
    m_rdg2dWidget->accomplishmentThreadTerminate();

    if (m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.vectorRdgsInRelief.size() == 0)
    {
        rdgImpulsesSliderActivate();
        m_resetSectPoints2dRdgs = true;
        m_resetCutPointsRdgs = true;
        outputRdgsData(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
    }
    else
    {
        m_rdg2dWidget->m_accomplishment->m_thread->m_defRdgsFnRelief = true;
        m_rdg2dWidget->m_accomplishment->m_thread->m_executeParserData = true;
        m_rdg2dWidget->m_accomplishment->m_thread->start();
    }
}

void MainWindow::receiveRdgsZData()
{
    m_rdg2dWidget->m_accomplishment->m_thread->m_defRdgsFnRelief = false;
    m_rdg2dWidget->accomplishmentThreadTerminate();

    rdgImpulsesSliderActivate();
    m_resetSectPoints2dRdgs = true;
    m_resetCutPointsRdgs = true;
    outputRdgsData(
        m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
        m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
    );
}

void MainWindow::outputRdgsData(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData,
    int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int selectionId
)
{
    int absRdgsStructInX {0};
    int absRdgsStructFnX {0};

    if (m_stackedWidget->currentIndex() == 1)
    {
        defAbsRdgsStructInXAndFnX(absRdgsStructInX, absRdgsStructFnX);
        m_rdgs2dWidget->showRdgsSurf(
            rdgsInfoDataMap, rdgsWorkData, quantImpulsesOfPacket, filterId, selectionId,
            m_highLowOnRdg, m_showFullRdgs, m_resetSectPoints2dRdgs, m_materialId, absRdgsStructInX, absRdgsStructFnX
        );
        if (m_resetSectPoints2dRdgs == true) m_resetSectPoints2dRdgs = false;
    }
    else if (m_stackedWidget->currentIndex() == 2)
    {
        defAbsRdgsStructInXAndFnX(absRdgsStructInX, absRdgsStructFnX);
        m_rdgs3dWidget->showRdgsImage(
            rdgsInfoDataMap, rdgsWorkData,  maxQuantImpulsesOfPacket, quantImpulsesOfPacket, filterId, selectionId,
            m_highLowOnRdg, m_showFullRdgs, m_resetCutPointsRdgs, m_materialId, absRdgsStructInX, absRdgsStructFnX
        );
        if (m_resetCutPointsRdgs == true) m_resetCutPointsRdgs = false;
    }
}

void MainWindow::enabledCutActions(bool enabled2d, bool enabled3d, bool enabledSL)
{
    m_closeSection2dAct->setEnabled(enabled2d);
    m_closeCut3dAct->setEnabled(enabled3d);
    m_saveBoundPointsLineAct->setEnabled(enabledSL);
    m_loadBoundPointsLineAct->setEnabled(enabledSL);

    if (enabled2d == true  || enabled3d == true)
    {
        m_highLowImpulsesRdgsAction->setEnabled(true);
        m_showFullRdgsAction       ->setEnabled(true);
    }
    else
    {
        m_highLowImpulsesRdgsAction->setEnabled(false);
        m_showFullRdgsAction       ->setEnabled(false);
    }
}

void MainWindow::enabledShowRdgActions(bool enabled)
{
    m_showInitRdgAct->setEnabled(enabled);
    m_showLogRdgAct->setEnabled(enabled);
    if (m_stackedWidget->currentIndex() == 2)
    {
        m_logRdgMouseSelectionAct->setEnabled(!enabled);
        m_logRdgAutoSelectionAct->setEnabled(!enabled);
        m_showInitRdgSelectionAct->setEnabled(!enabled);
        m_resetRdgSelectionAct->setEnabled(!enabled);
        m_saveRdgAutoSelectionDataAct->setEnabled(!enabled);
        m_loadRdgAutoSelectionDataAct->setEnabled(!enabled);
    }
    else
    {
        m_logRdgMouseSelectionAct->setEnabled(enabled);
        m_logRdgAutoSelectionAct->setEnabled(enabled);
        m_showInitRdgSelectionAct->setEnabled(enabled);
        m_resetRdgSelectionAct->setEnabled(enabled);
        m_saveRdgAutoSelectionDataAct->setEnabled(enabled);
        m_loadRdgAutoSelectionDataAct->setEnabled(enabled);
    }
}

void MainWindow::rdgImpulsesSliderActivate()
{
    m_rdgImpulsesSliderActivate = true;
    defMaxQuantImpulsesOfPacket(m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_maxQuantImpulsesOfPacket);
    m_quantImpulsesOfPacket = m_maxQuantImpulsesOfPacket-1;

    m_rdgQuantImpulsesSlider->setRange(1, m_maxQuantImpulsesOfPacket-1);
    m_rdgQuantImpulsesSlider->setValue(m_quantImpulsesOfPacket);
    m_rdgQuantImpulsesSlider->setSingleStep(1);
    m_rdgQuantImpulsesSlider->setTickPosition(QSlider::TicksRight);
    m_rdgQuantImpulsesLabel->setText("Число отсчетов rdg " + QString::number(m_quantImpulsesOfPacket) + " .");
}

void MainWindow::setQuantImpulsesOfPacketSlider(int quantImpulsesOfPacket)
{
    if (m_rdgImpulsesSliderActivate == false)
    {
        m_quantImpulsesOfPacket = quantImpulsesOfPacket;
        m_rdgQuantImpulsesLabel->setText("Число отсчетов rdg " + QString::number(m_quantImpulsesOfPacket) + " .");

        m_resetSectPoints2dRdgs = false;
        m_resetCutPointsRdgs = false;
        outputRdgsData(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
        m_resetSectPoints2dRdgs = true;
        m_resetCutPointsRdgs = true;
    }
    else  m_rdgImpulsesSliderActivate = false;
}

void MainWindow::changeHighLowOnRdgs()
{
    if (m_stackedWidget->currentIndex() > 0)
    {
        if (m_highLowImpulsesRdgsAction->isChecked() == false) m_highLowOnRdg = false;  else m_highLowOnRdg = true;
        m_resetSectPoints2dRdgs = false;
        m_resetCutPointsRdgs = false;
        outputRdgsData(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
        m_resetSectPoints2dRdgs = true;
        m_resetCutPointsRdgs = true;
    }
}

void MainWindow::showFullRdgsData()
{
    if (m_stackedWidget->currentIndex() > 0)
    {
        if (m_showFullRdgsAction->isChecked() == false) m_showFullRdgs = false;  else m_showFullRdgs = true;

        m_resetSectPoints2dRdgs = false;
        m_resetCutPointsRdgs = false;
        outputRdgsData(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );

        openScrollHRdgsLengthBar(true);
        moveOnAnyRdgsImage(limitRdgWidth-1);

        m_resetSectPoints2dRdgs = true;
        m_resetCutPointsRdgs = true;
    }
}

void MainWindow::setInitRdg()
{
    m_rdg2dWidget->setShowLogRdg(false);
    m_rdgs3dWidget->setShowLogRdgsTrans(false);
    if      (m_stackedWidget->currentIndex() == 0) showRdgImage(false);
    else if (m_stackedWidget->currentIndex() == 2) m_rdgs3dWidget->representRdgsTransGlWidget(
        m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap,
        m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData
    );
}

void MainWindow::setLogRdg()
{
    m_rdg2dWidget->setShowLogRdg(true);
    m_rdgs3dWidget->setShowLogRdgsTrans(true);
    if (m_stackedWidget->currentIndex() == 0) showRdgImage(false);
    else if (m_stackedWidget->currentIndex() == 2) m_rdgs3dWidget->representRdgsTransGlWidget(
        m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap,
        m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData
    );
}

void MainWindow::setLogRdgMouseSelection()
{
    m_rdg2dWidget->setLogRdgMouseSelection(m_logRdgMouseSelectionAct->isChecked());
    if (m_logRdgAutoSelectionAct->isChecked())
    {
        m_rdg2dWidget->settingsRdgSelection(false, false, m_rdgCurrentName);
        m_logRdgAutoSelectionAct->setChecked(false);
    }
    closeInitRdgSelection();
}

void MainWindow::setLogRdgAutoSelection()
{
    m_rdg2dWidget->settingsRdgSelection(m_logRdgAutoSelectionAct->isChecked(), false, m_rdgCurrentName);
    if (m_logRdgMouseSelectionAct->isChecked())
    {
        m_rdg2dWidget->setLogRdgMouseSelection(false);
        m_logRdgMouseSelectionAct->setChecked(false);
    }
    closeInitRdgSelection();
}

void MainWindow::setShowInitRdgSelection()
{
    m_rdg2dWidget->setShowInitRdgSelection(m_showInitRdgSelectionAct->isChecked());
    showRdgImage(false);
}

void MainWindow::slotResetRdgSelection()
{
    resetRdgSelection(true);
}

void MainWindow::resetRdgSelection(bool clearSelectRdgPoints)
{
    m_rdg2dWidget->resetRdgSelection(clearSelectRdgPoints, m_rdgCurrentName);
    if (m_showInitRdgSelectionAct->isChecked())
    {
        m_showInitRdgSelectionAct->setChecked(false);
        m_rdg2dWidget->setShowInitRdgSelection(false);
    }
    if (m_logRdgMouseSelectionAct->isChecked())
    {
        m_logRdgMouseSelectionAct->setChecked(false);
        m_rdg2dWidget->setLogRdgMouseSelection(false);
    }
    if (m_logRdgAutoSelectionAct->isChecked()){  m_logRdgAutoSelectionAct->setChecked(false); }
    showRdgImage(false);
}

void MainWindow::closeInitRdgSelection()
{
    m_showInitRdgSelectionAct->setChecked(false);
    setShowInitRdgSelection();
}

void MainWindow::impulsesRdgControlsVisible(bool visible)
{
    m_rdgQuantImpulsesLabel->setVisible(visible);
    m_rdgQuantImpulsesSlider->setVisible(visible);
}

void MainWindow::rdgsTransitControlsVisible(bool visible)
{
    m_rdgsTransitSlider->setVisible(visible);
    m_leftRdgsTransitButton->setVisible(visible);
    m_rightRdgsTransitButton->setVisible(visible);
}

void MainWindow::saveRdgAutoSelectionData()
{
    QString saveRdgAutoSelectionDataFile = QFileDialog::getSaveFileName(this, tr("Save As"), "rdgAutoSelectionData", tr("CSV File(*.csv);"));
    if (saveRdgAutoSelectionDataFile != "")   m_rdg2dWidget->saveRdgAutoSelectionData(saveRdgAutoSelectionDataFile);
}

void MainWindow::loadRdgAutoSelectionData()
{
    QString loadRdgAutoSelectionDataFile = QFileDialog::getOpenFileName(this, tr("Open csv file"), "", tr("CSV File(*.csv);"));
    if (loadRdgAutoSelectionDataFile != "" && m_rdgCurrentName != "") m_rdg2dWidget->loadRdgAutoSelectionData(loadRdgAutoSelectionDataFile, m_rdgCurrentName);
}

void MainWindow::materialIdChanged(int materialId)
{
    m_materialId = materialId;

    scrollHRdgsLengthBarResetDataAtRdgsImages();

    if (m_stackedWidget->currentIndex() == 0 && m_rdgCurrentName != "")  showRdgImage(false);
    else if (m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap.size()>1)
        outputRdgsData(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
}

void MainWindow::includeFilterOnRdg()
{
    if (m_includeFilterOnRdgAction->isChecked()) m_filterId = 1;
    else                                         m_filterId = 0;

    if (m_stackedWidget->currentIndex() == 0 && m_rdgCurrentName != "")
    {
        openScrollHRdgsLengthBar(true);
        showRdgImage(false);
    }
    else if (m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap.size()>1)
    {
        scrollHRdgsLengthBarResetDataAtRdgsImages();
        outputRdgsData(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
    }
}

void MainWindow::useSelectionOnRdg()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))
    {
        if (action->data().toString().toStdString() == "void selection is activated" )
        {
            m_selectionId = 0;
            m_useVoidSelectionAct->setChecked(true);
            m_useMouseSelectionAct->setChecked(false);
            m_useAutoSelectionAct->setChecked(false);

        }
        if (action->data().toString().toStdString() == "mouse selection is activated")
        {
            m_selectionId = 1;
            m_useVoidSelectionAct->setChecked(false);
            m_useMouseSelectionAct->setChecked(true);
            m_useAutoSelectionAct->setChecked(false);

        }
        if (action->data().toString().toStdString() == "auto selection is activated" )
        {
            m_selectionId = 2;
            m_useVoidSelectionAct->setChecked(false);
            m_useMouseSelectionAct->setChecked(false);
            m_useAutoSelectionAct->setChecked(true);
        }

        outputRdgsData(
            m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
        statusBar()->showMessage(action->data().toString());
    }
}

void MainWindow::setupScalingOnRdg()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))
    {
        int   typeRdgScaling {0};
        if (action->data().toString().toStdString()      == "all rdg scaling")
        {
            typeRdgScaling = 0;
            m_rdgAllScalingAct->setChecked(true);
            m_rdgHScalingAct->setChecked(false);
            m_rdgVScalingAct->setChecked(false);
        }
        else if (action->data().toString().toStdString() ==  "h rdg scaling")
        {
            typeRdgScaling = 1;
            m_rdgAllScalingAct->setChecked(false);
            m_rdgHScalingAct->setChecked(true);
            m_rdgVScalingAct->setChecked(false);
        }
        else if(action->data().toString().toStdString()  == "v rdg scaling")
        {
            typeRdgScaling = 2;
            m_rdgAllScalingAct->setChecked(false);
            m_rdgHScalingAct->setChecked(false);
            m_rdgVScalingAct->setChecked(true);
        }
        m_rdg2dWidget->setupRdgScaling(typeRdgScaling);
    }
}

void MainWindow::setupScalingOnRdgsSurf()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))
    {
        int   typeRdgsSurfScaling {0};
        if (action->data().toString().toStdString()      == "all rdgs surf scaling")
        {
            typeRdgsSurfScaling = 0;
            m_rdgsSurfAllScalingAct->setChecked(true);
            m_rdgsSurfHScalingAct->setChecked(false);
            m_rdgsSurfVScalingAct->setChecked(false);
        }
        else if (action->data().toString().toStdString() ==  "h rdgs surf scaling")
        {
            typeRdgsSurfScaling = 1;
            m_rdgsSurfAllScalingAct->setChecked(false);
            m_rdgsSurfHScalingAct->setChecked(true);
            m_rdgsSurfVScalingAct->setChecked(false);
        }
        else if(action->data().toString().toStdString()  == "v rdgs surf scaling")
        {
            typeRdgsSurfScaling = 2;
            m_rdgsSurfAllScalingAct->setChecked(false);
            m_rdgsSurfHScalingAct->setChecked(false);
            m_rdgsSurfVScalingAct->setChecked(true);
        }
    }
}

void MainWindow::setupScalingOnRdgsTrans()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))
    {
        int   typeRdgsTransScaling {0};
        if (action->data().toString().toStdString() == "all rdgs trans scaling")
        {
            typeRdgsTransScaling = 0;
            m_rdgsTransAllScalingAct->setChecked(true);
            m_rdgsTransHScalingAct->setChecked(false);
            m_rdgsTransVScalingAct->setChecked(false);
        }
        else if (action->data().toString().toStdString() ==  "h rdgs trans scaling")
        {
            typeRdgsTransScaling = 1;
            m_rdgsTransAllScalingAct->setChecked(false);
            m_rdgsTransHScalingAct->setChecked(true);
            m_rdgsTransVScalingAct->setChecked(false);
        }
        else if(action->data().toString().toStdString() == "v rdgs trans scaling")
        {
            typeRdgsTransScaling = 2;
            m_rdgsTransAllScalingAct->setChecked(false);
            m_rdgsTransHScalingAct->setChecked(false);
            m_rdgsTransVScalingAct->setChecked(true);
        }
        m_rdgs3dWidget->setupRdgsTransScaling(typeRdgsTransScaling);
    }
}

void MainWindow::setupRdgsSliderSetup(int rdgId, int rdgsCount)
{
    m_rdgsTransitSlider->setRange(0, rdgsCount-1);
    m_rdgsTransitSlider->setSingleStep(1);
    m_rdgsTransitSlider->setValue(rdgId);
}

void MainWindow::changeRdg(int rdgId)
{
    m_rdg2dWidget->m_dataRdgWidget->m_trackRdgSlider->setValue(0);
    m_rdgCurrentName = m_rdgsNamesVectorPairs[rdgId].first;

    showRdgImage(true);
    openScrollHRdgsLengthBar(true);
}

void MainWindow::leftChangeRdg()
{
    int rdgId = m_rdgsTransitSlider->value();
    if (rdgId-1 >=0)
    {
        m_rdgsTransitSlider->setValue(rdgId-1);
    }
}

void MainWindow::rightChangeRdg()
{
    int rdgId = m_rdgsTransitSlider->value();
    if (rdgId+1 <= m_rdgsTransitSlider->maximum())
    {
        m_rdgsTransitSlider->setValue(rdgId+1);
    }
}

void MainWindow::openScrollHRdgsLengthBar(bool visible)
{
    m_scrollHRdgsLengthBar->setVisible(false);

    if (visible == false) m_scrollHRdgsLengthBar->setVisible(visible);
    else if (m_rdgCurrentName != "")
    {
        switch (m_stackedWidget->currentIndex())
        {
            case 0:
            {
                int rdgWidth = m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap[m_rdgCurrentName].vectorRdgData.size();

                if (rdgWidth > limitRdgWidth)
                {
                    m_scrollHRdgsLengthBar->setVisible(visible);
                    setupScrollHRdgsLengthBarData(limitRdgWidth-1, limitRdgWidth-1 , rdgWidth-1);
                }
                break;
            }
            case 1:
            case 2:
            {
                int rdgsSurfWidth    = m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfWidth   ;
                int rdgsSurfWidthMin = m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData.rdgsSurfMinWidth;

                if (m_showFullRdgsAction->isChecked() && (rdgsSurfWidth > limitRdgWidth))
                {
                    m_scrollHRdgsLengthBar->setVisible(visible);
                    setupScrollHRdgsLengthBarData(limitRdgWidth-1,limitRdgWidth-1, rdgsSurfWidth-1);
                }
                else if (rdgsSurfWidthMin > limitRdgWidth)
                {
                    m_scrollHRdgsLengthBar->setVisible(visible);
                    setupScrollHRdgsLengthBarData(limitRdgWidth-1,limitRdgWidth-1, rdgsSurfWidthMin-1);
                }
                break;
            }
        }
    }
}


void MainWindow::setupScrollHRdgsLengthBarData(int minScrollHRdgsLengthBarData, int scrollHRdgsLengthBarData, int maxScrollHRdgsLengthBarData)
{
    m_scrollHRdgsLengthBar->setRange(minScrollHRdgsLengthBarData, maxScrollHRdgsLengthBarData);
    m_scrollHRdgsLengthBar->setValue(scrollHRdgsLengthBarData);
    moveOnAnyRdgsImage(scrollHRdgsLengthBarData);
}

void MainWindow::scrollHRdgsLengthBarPressed()
{
    m_scrollHRdgsLengthBarAct = true;
}

void MainWindow::scrollHRdgsLengthBarMoved(int scrollHRdgsLengthBarPos)
{
    m_scrollHRdgsLengthBarPos = scrollHRdgsLengthBarPos;
    m_scrollHRdgsLengthBarAct = true;
}

void MainWindow::scrollHRdgsLengthBarReleased()
{
    moveOnAnyRdgsImage(m_scrollHRdgsLengthBarPos);
    m_scrollHRdgsLengthBarAct = false;
}

void MainWindow::scrollHRdgsLengthBarChanged(int scrollHRdgsLengthBarPos)
{
    m_scrollHRdgsLengthBarPos = scrollHRdgsLengthBarPos;
    if (m_scrollHRdgsLengthBarAct == false) moveOnAnyRdgsImage(m_scrollHRdgsLengthBarPos);
}

void MainWindow::moveOnAnyRdgsImage(int scrollHRdgsLengthBarPos)
{
    switch (m_stackedWidget->currentIndex())
    {
        case 0:
        {
            m_rdg2dWidget->changeRdgImage(scrollHRdgsLengthBarPos-limitRdgWidth+1, scrollHRdgsLengthBarPos);
            break;
        }
        case 1:
        case 2:
        {
            //обработка логарифмического образа по всем радарограммам (она обязательна)
            samplingAllLog10Rdgs(
                m_materialId, m_filterId, m_rdgsNamesVectorPairs, m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap,
                scrollHRdgsLengthBarPos-limitRdgWidth+1, scrollHRdgsLengthBarPos
            );

            if (m_stackedWidget->currentIndex() == 1)
                m_rdgs2dWidget->changeRdgsSurf(
                    m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
                    m_quantImpulsesOfPacket, m_filterId, m_selectionId, m_highLowOnRdg, m_showFullRdgs, m_resetSectPoints2dRdgs,
                    m_materialId, scrollHRdgsLengthBarPos-limitRdgWidth+1, scrollHRdgsLengthBarPos
                );
            else
                m_rdgs3dWidget->changeRdgsImage(
                    m_rdg2dWidget->m_accomplishment->m_thread->m_rdgsInfoDataMap, m_rdg2dWidget->m_accomplishment->m_thread->m_st_rdgsWorkData,
                    m_quantImpulsesOfPacket, m_filterId, m_selectionId, m_highLowOnRdg,  m_showFullRdgs, true,
                     m_materialId, scrollHRdgsLengthBarPos-limitRdgWidth+1, scrollHRdgsLengthBarPos
                );

            break;
        }
    }
}
