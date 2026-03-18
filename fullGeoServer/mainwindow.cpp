#include "mainwindow.h"
#include "transformation.h"
#include <iostream>

Q_DECLARE_METATYPE(QStringList)

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<BscanFileFormat>("BscanFileFormat");
    qRegisterMetaType<QColor>("QColor");
    qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");
    qRegisterMetaType<std::vector<std::pair<std::string, std::string>>>("std::vector<std::pair<std::string, std::string>>");
    qRegisterMetaTypeStreamOperators<QStringList>("QStringList");

    m_stackedWidget = new QStackedWidget;
    m_stackedWidget->addWidget(m_bscan2dWidget);
    m_stackedWidget->addWidget(m_bscanDeep2dWidget);
    m_stackedWidget->addWidget(m_bscans2dWidget);
    m_stackedWidget->addWidget(m_bscans3dWidget);

    m_bscansNamesVectorPairs.resize(0);
    m_recentBscansFileActsVector.resize(0);

    init();

    connect(this, &MainWindow::signalClearBscansInfoDataMap, m_bscan2dWidget, &bscan2dWidget::slotClearBscansInfoDataMap);
    connect(this, &MainWindow::signalEraseBscansInfoDataMap, m_bscan2dWidget, &bscan2dWidget::slotEraseBscansInfoDataMap);

    connect(this, &MainWindow::closeSection2dAction,   m_bscans2dWidget, &bscans2dWidget::slotCloseSection2dAction);
    connect(this, &MainWindow::closeCut3dAction,   m_bscans3dWidget, &bscans3dWidget::slotCloseCut3dAction);
    connect(m_bscans3dWidget, &bscans3dWidget::sendDeleteBscanName, this, &MainWindow::receiveDeleteBscanName);

    connect(m_bscans2dWidget, &bscans2dWidget::signalFixBscansSurfPixels,    this, &MainWindow::slotFixBscansSurfPixels);
    connect(this, &MainWindow::signalSetupBscansSurfPixels, m_bscans2dWidget, &bscans2dWidget::slotSetupBscansSurfPixels);

    connect(m_bscans3dWidget, &bscans3dWidget::signalFixDeleteBscanName, this, &MainWindow::slotFixDeleteBscanName);
    connect(m_bscans3dWidget, &bscans3dWidget::signalFixFillBscans,    this, &MainWindow::slotFixFillBscans);

    connect(this, &MainWindow::signalDefDeleteBscanName,  m_bscans3dWidget, &bscans3dWidget::slotDefDeleteBscanName);
    connect(this, &MainWindow::signalFillBscansExecute, m_bscans3dWidget, &bscans3dWidget::slotFillBscansExecute);

    m_progressBar = new QProgressBar;

    m_mainWidget = new QWidget();
    m_mainLayout  = new QVBoxLayout;
    m_mainLayout->addWidget(m_stackedWidget);

    m_materialLabel  = new QLabel("Тип грунта");

    m_materialComboBox = new QComboBox(this);
    m_materialComboBox->addItem("Лед");
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

    m_labelHBscansLengthBar  = new QLabel("Переход по кадрам радарограммы");
    m_scrollHBscansLengthBar = new QScrollBar(Qt::Horizontal);
    m_scrollHBscansLengthBar->setFocusPolicy(Qt::StrongFocus);
    m_scrollHBscansLengthBar->setFixedHeight(20);
    m_scrollHBscansLengthBar->setFixedWidth(300);
    m_scrollHBscansLengthBar->setValue(0);
    m_scrollHBscansLengthBar->setRange(0,0);

    m_bscanQuantImpulsesLabel = new QLabel("Число отсчетов ");
    m_bscanQuantImpulsesSlider = new QSlider(Qt::Horizontal);
    m_bscanQuantImpulsesSlider->setSingleStep(1);
    m_bscanQuantImpulsesSlider->setTickPosition(QSlider::TicksRight);

    m_bscansTransitLabel  = new QLabel("Переход по радарограммам ");
    m_bscansTransitSlider = new QSlider(Qt::Horizontal);
    m_bscansTransitSlider->setSingleStep(1);
    m_bscansTransitSlider->setTickPosition(QSlider::TicksRight);
    m_leftBscansTransitButton  = new QPushButton(tr("<-"), this);
    m_rightBscansTransitButton = new QPushButton(tr("->"), this);

    m_materialLabel->setVisible(false);
    m_materialComboBox->setVisible(false);
    fixScrollHBscansLengthBar();
    bscansTransitControlsVisible(false);
    impulsesBscanControlsVisible(false);

    m_lowLayout  = new QHBoxLayout;
    m_lowLayout->addWidget(m_materialLabel);
    m_lowLayout->addWidget(m_materialComboBox);
    m_lowLayout->addWidget(m_labelHBscansLengthBar);
    m_lowLayout->addWidget(m_scrollHBscansLengthBar);

    m_lowLayout->addWidget(m_bscanQuantImpulsesLabel);
    m_lowLayout->addWidget(m_bscanQuantImpulsesSlider);

    m_lowLayout->addWidget(m_bscansTransitLabel);
    m_lowLayout->addWidget(m_leftBscansTransitButton);
    m_lowLayout->addWidget(m_bscansTransitSlider);
    m_lowLayout->addWidget(m_rightBscansTransitButton);
    m_mainLayout->addLayout(m_lowLayout);

    m_mainWidget->setLayout( m_mainLayout);
    setCentralWidget(m_mainWidget);

    connect(m_bscan2dWidget->m_accomplishment->m_thread, &accomplishmentThread::sendSurfBscansWorkData, this, &MainWindow::receiveBscansWorkData);
    connect(m_bscan2dWidget->m_accomplishment->m_thread, &accomplishmentThread::sendBscansZData,        this, &MainWindow::receiveBscansZData);
    connect(m_bscan2dWidget->m_accomplishment->m_thread, &accomplishmentThread::sendProgressData,     this, &MainWindow::receiveProgressData);

    connect(m_bscanDeep2dWidget, &bscanDeep2dWidget::signalFixPixels,               this,                &MainWindow::slotSetupBscanDeepPixels);
    connect(this,                &MainWindow::signalSetupBscanDeepPixels,      m_bscanDeep2dWidget, &bscanDeep2dWidget::slotSetupPixels);

    connect(m_bscanDeep2dWidget, &bscanDeep2dWidget::signalFixDataBscanDeepWidgets, this,                &MainWindow::slotFixDataBscanDeepWidgets);
    connect(this,                &MainWindow::signalSetupFixDataBscanDeepWidgets,   m_bscanDeep2dWidget, &bscanDeep2dWidget::slotSetupFixDataWidgets);

    connect(m_bscanDeep2dWidget, &bscanDeep2dWidget::signalFixPageData,     this,                &MainWindow::slotSetupBscanDeepPageData);
    connect(this,                &MainWindow::signalSetupBscanDeepPageData, m_bscanDeep2dWidget, &bscanDeep2dWidget::slotSetupBscanDeepPageData);

    connect(m_bscanQuantImpulsesSlider, &QSlider::valueChanged, this, &MainWindow::setQuantImpulsesOfPacketSlider);

    connect(m_bscans2dWidget, &bscans2dWidget::sendOutBscansSurfInfo, this, &MainWindow::receiveOutBscansSurfInfo);

    connect(m_bscan2dWidget, &bscan2dWidget::sendOutBscanInfo, this, &MainWindow::receiveOutBscanInfo);
    connect(m_bscan2dWidget, &bscan2dWidget::signalActivateAccompThread, this, &MainWindow::slotActivateAccompThread);

    connect(m_bscans2dWidget, &bscans2dWidget::signalFixBscansSurfSection, this, &MainWindow::slotFixBscansSurfSection);
    connect(this, &MainWindow::signalOutputBscansSurfSection, m_bscans2dWidget, &bscans2dWidget::slotOutputBscansSurfSection);

    connect(m_materialComboBox, SIGNAL(activated(int)), SLOT(materialIdChanged(int)));

    connect(m_bscansTransitSlider,      &QSlider::valueChanged, this, &MainWindow::changeBscan);
    connect(m_leftBscansTransitButton,  &QPushButton::clicked,  this, &MainWindow::leftChangeBscan);
    connect(m_rightBscansTransitButton, &QPushButton::clicked,  this, &MainWindow::rightChangeBscan);

    connect(m_scrollHBscansLengthBar, &QScrollBar::sliderPressed, this, &MainWindow::scrollHBscansLengthBarPressed);
    connect(m_scrollHBscansLengthBar, &QScrollBar::sliderMoved, this, &MainWindow::scrollHBscansLengthBarMoved);
    connect(m_scrollHBscansLengthBar, &QScrollBar::sliderReleased, this, &MainWindow::scrollHBscansLengthBarReleased);
    connect(m_scrollHBscansLengthBar, &QScrollBar::valueChanged, this, &MainWindow::scrollHBscansLengthBarChanged);

    m_colorsPaletteDialog = new colorsPaletteDialog();
    connect(m_colorsPaletteDialog, &colorsPaletteDialog::sendColorsPalette, this, &MainWindow::receiveColorsPalette);

    readLastBscansDirsFromSettings(m_trzDir, m_csvDir, m_hdf5Dir);
    setupLastBscans();
}

MainWindow::~MainWindow(){}

void MainWindow::setupLastBscans()
{
    m_filesBscanNamesTrz.clear();
    m_trzNumAntennasVector.resize(0);

    m_filesBscanNamesCsv.clear();
    m_infoBscansHdf5Names.clear();

    readLastNamesBscansFromSettings(m_filesBscanNamesTrz, m_trzNumAntennasVector, m_filesBscanNamesCsv, m_infoBscansHdf5Names);

    if      (m_trzNumAntennasVector.size() > 0)   m_bscan2dWidget->setupTrzBscans (m_filesBscanNamesTrz, m_trzNumAntennasVector, m_bscansNamesVectorPairs);
    else if (m_filesBscanNamesCsv.size()     > 0) m_bscan2dWidget->setupCsvBscans (m_filesBscanNamesCsv, m_bscansNamesVectorPairs);
    else if (m_infoBscansHdf5Names.size()    > 0) m_bscan2dWidget->setupHdf5Bscans(m_infoBscansHdf5Names, m_bscansNamesVectorPairs);
}

void MainWindow::receiveDeleteBscanName(const std::string& delBscanName)
{
    deleteFile(delBscanName);
}

void MainWindow::slotFixBscansSurfPixels()
{
    emit signalSetupBscansSurfPixels(m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData);
}

void MainWindow::slotFixBscansSurfSection()
{
    emit signalOutputBscansSurfSection(m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData);
}

void MainWindow::slotFixDeleteBscanName(double fixLatitude, double fixLongitude)
{
    emit signalDefDeleteBscanName(m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, fixLatitude, fixLongitude);
}

void MainWindow::slotFixFillBscans(bool resetCutPointsBscans)
{
    emit signalFillBscansExecute(
        m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData, resetCutPointsBscans
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
    m_stackedWidget->setCurrentWidget(m_bscan2dWidget);
}

void MainWindow::receiveOutBscansSurfInfo(QString outBscansSurfInfo)
{
     statusBar()->showMessage(outBscansSurfInfo);
}

void MainWindow::receiveOutBscanInfo(QString outBscanInfo)
{
    statusBar()->showMessage(outBscanInfo);
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

    QPointer<QAction> deleteAllBscansAct = new QAction(tr("&Удалить все радарограммы"), this);
    connect(deleteAllBscansAct, &QAction::triggered, this, &MainWindow::deleteAllFiles);
    fileMenu->addAction(deleteAllBscansAct);

    QPointer<QAction> saveAct = new QAction(tr("&Сохранить"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить "));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);

    fileMenu->addSeparator();

    m_recentFilesBscanMenu = fileMenu->addMenu(tr("Недавние..."));
    m_deleteFilesBscanMenu = fileMenu->addMenu(tr("Удалить..."));

    QPointer<QAction> closeAct = fileMenu->addAction(tr("&Закрыть"), this, &MainWindow::close);
    closeAct->setShortcut(tr("Ctrl+W"));
    closeAct->setStatusTip(tr("Закрыть это окно"));

    QPointer<QMenu> imagesMenu = menuBar()->addMenu(tr("&Изображения"));

    QAction* bscan2DImageAct = new QAction(tr("&Изображение bscan"), this);
    connect(bscan2DImageAct, &QAction::triggered, this, &MainWindow::openImageBscan);
    imagesMenu->addAction(bscan2DImageAct);

    QAction* bscanPaletteAct = new QAction(tr("&Палитра bscan"), this);
    connect(bscanPaletteAct, &QAction::triggered, this, &MainWindow::defPaletteBscan);
    imagesMenu->addAction(bscanPaletteAct);

    QAction* bscanDeep2DImageAct = new QAction(tr("&Изображение глубины по bscan"), this);
    connect(bscanDeep2DImageAct, &QAction::triggered, this, &MainWindow::openImageDeepBscan);
    imagesMenu->addAction(bscanDeep2DImageAct);

    QAction* bscanDeepPaletteAct = new QAction(tr("&Палитра по глубине bscan"), this);
    connect(bscanDeepPaletteAct, &QAction::triggered, this, &MainWindow::defPaletteBscanDeep);
    imagesMenu->addAction(bscanDeepPaletteAct);

    QPointer<QAction> bscans2DImageAct = new QAction(tr("&2D Изображение профилей по bscans"), this);
    connect( bscans2DImageAct, &QAction::triggered, this, &MainWindow::open2dImageBscans);
    imagesMenu->addAction(bscans2DImageAct);

    QPointer<QAction> bscansSurfPaletteAct = new QAction(tr("&Палитра по профилям bscans"), this);
    connect(bscansSurfPaletteAct, &QAction::triggered, this, &MainWindow::defPaletteSurfBscans);
    imagesMenu->addAction(bscansSurfPaletteAct);

    QPointer<QAction> bscans3DImageAct = new QAction(tr("&3D Изображение профилей по bscans"), this);
    connect( bscans3DImageAct, &QAction::triggered, this, &MainWindow::open3dImageBscans);
    imagesMenu->addAction(bscans3DImageAct);

    QPointer<QAction> bscansTransPaletteAct = new QAction(tr("&Палитра по переходу между bscans"), this);
    connect(bscansTransPaletteAct, &QAction::triggered, this, &MainWindow::defPaletteTransBscans);
    imagesMenu->addAction(bscansTransPaletteAct);

    m_includeFilterOnBscanAction = new QAction(tr("&Включаем фильтр Перона и Малика"), this);
    m_includeFilterOnBscanAction->setCheckable(true);
    m_includeFilterOnBscanAction->setChecked(false);
    imagesMenu->addAction(m_includeFilterOnBscanAction);
    connect(m_includeFilterOnBscanAction, &QAction::toggled, this, &MainWindow::includeFilterOnBscan);

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

    m_highLowImpulsesBscansAction  = new QAction(tr("&Сверху-вниз/снизу-вверх по импульсам "), this);
    m_highLowImpulsesBscansAction->setCheckable(true);
    m_highLowImpulsesBscansAction->setChecked(true);
    toolsMenu->addAction(m_highLowImpulsesBscansAction);
    connect(m_highLowImpulsesBscansAction, &QAction::triggered, this, &MainWindow::changeHighLowOnBscans);

    m_showFullBscansAction         = new QAction(tr("&Учет полных bscans"), this);
    m_showFullBscansAction->setCheckable(true);
    m_showFullBscansAction->setChecked(true);
    toolsMenu->addAction(m_showFullBscansAction);
    connect(m_showFullBscansAction, &QAction::triggered, this, &MainWindow::showFullBscansData);

    enabledCutActions(false, false, false);

    QPointer<QMenu> actionsBscanMenu = menuBar()->addMenu(tr("&Операции с bscan"));
    m_showInitBscanAct = new QAction(tr("&Исходный bscan"), this);
    connect(m_showInitBscanAct, &QAction::triggered, this, &MainWindow::setInitBscan);
    actionsBscanMenu->addAction(m_showInitBscanAct);

    m_showLogBscanAct = new QAction(tr("&Логарифмический bscan"), this);
    connect(m_showLogBscanAct, &QAction::triggered, this, &MainWindow::setLogBscan);
    actionsBscanMenu->addAction(m_showLogBscanAct);

    m_logBscanMouseSelectionAct = new QAction(tr("&Выделяем область мышкой по Log Bscan"), this);
    connect(m_logBscanMouseSelectionAct, &QAction::triggered, this, &MainWindow::setLogBscanMouseSelection);
    m_logBscanMouseSelectionAct->setCheckable(true);
    actionsBscanMenu->addAction(m_logBscanMouseSelectionAct);

    m_logBscanAutoSelectionAct = new QAction(tr("&Автоматическое выделение области по Log Bscan"), this);
    connect(m_logBscanAutoSelectionAct, &QAction::triggered, this, &MainWindow::setLogBscanAutoSelection);
    m_logBscanAutoSelectionAct->setCheckable(true);
    actionsBscanMenu->addAction(m_logBscanAutoSelectionAct);

    m_showInitBscanSelectionAct = new QAction(tr("&Показываем выделенные области на Init Bscan"), this);
    connect(m_showInitBscanSelectionAct, &QAction::triggered, this, &MainWindow::setShowInitBscanSelection);
    m_showInitBscanSelectionAct->setCheckable(true);
    actionsBscanMenu->addAction(m_showInitBscanSelectionAct);

    m_resetBscanSelectionAct = new QAction(tr("&Убираем выделенные области со всех Bscan"), this);
    connect(m_resetBscanSelectionAct, &QAction::triggered, this, &MainWindow::slotResetBscanSelection);
    actionsBscanMenu->addAction(m_resetBscanSelectionAct);

    m_saveBscanAutoSelectionDataAct = new QAction(tr("&Сохраняем автоматически-выделенные области"), this);
    connect(m_saveBscanAutoSelectionDataAct, &QAction::triggered, this, &MainWindow::saveBscanAutoSelectionData);
    actionsBscanMenu->addAction(m_saveBscanAutoSelectionDataAct);

    m_loadBscanAutoSelectionDataAct = new QAction(tr("&Загружаем автоматически-выделенные области"), this);
    connect(m_loadBscanAutoSelectionDataAct, &QAction::triggered, this, &MainWindow::loadBscanAutoSelectionData);
    actionsBscanMenu->addAction(m_loadBscanAutoSelectionDataAct);

    QPointer<QMenu> selectionsUseBscanMenu = menuBar()->addMenu(tr("&Активирование способа выделения областей по log образу."));
    m_useVoidSelectionAct  = new QAction(tr("&нет выделенией по bscan"), this);
    m_useVoidSelectionAct->setData(QString::fromStdString("void selection is activated"));
    m_useVoidSelectionAct->setCheckable(true);
    m_useVoidSelectionAct->setChecked(true);
    connect(m_useVoidSelectionAct, &QAction::triggered, this, &MainWindow::useSelectionOnBscan);
    selectionsUseBscanMenu->addAction(m_useVoidSelectionAct);

    m_useMouseSelectionAct = new QAction(tr("&выделение мышью по bscan"), this);
    m_useMouseSelectionAct->setData(QString::fromStdString("mouse selection is activated"));
    m_useMouseSelectionAct->setCheckable(true);
    connect(m_useMouseSelectionAct, &QAction::triggered, this, &MainWindow::useSelectionOnBscan);
    selectionsUseBscanMenu->addAction(m_useMouseSelectionAct);

    m_useAutoSelectionAct  = new QAction(tr("&автоматическое выделение по bscan"), this);
    m_useAutoSelectionAct->setData(QString::fromStdString("auto selection is activated"));
    m_useAutoSelectionAct->setCheckable(true);
    connect(m_useAutoSelectionAct, &QAction::triggered, this, &MainWindow::useSelectionOnBscan);
    selectionsUseBscanMenu->addAction(m_useAutoSelectionAct);

    QPointer<QMenu> scalingsBscansMenu = menuBar()->addMenu(tr("&Масштабирование"));

    m_bscanAllScalingAct  = new QAction(tr("&общее масштабирование bscan"), this);
    m_bscanAllScalingAct->setData(QString::fromStdString("all bscan scaling"));
    m_bscanAllScalingAct->setCheckable(true);
    m_bscanAllScalingAct->setChecked(true);
    connect(m_bscanAllScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscan);
    scalingsBscansMenu->addAction(m_bscanAllScalingAct);

    m_bscanHScalingAct  = new QAction(tr("&горизонтальное масштабирование bscan"), this);
    m_bscanHScalingAct->setData(QString::fromStdString("h bscan scaling"));
    m_bscanHScalingAct->setCheckable(true);
    m_bscanHScalingAct->setChecked(false);
    connect(m_bscanHScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscan);
    scalingsBscansMenu->addAction(m_bscanHScalingAct);

    m_bscanVScalingAct  = new QAction(tr("&вертикальное масштабирование bscan"), this);
    m_bscanVScalingAct->setData(QString::fromStdString("v bscan scaling"));
    m_bscanVScalingAct->setCheckable(true);
    m_bscanVScalingAct->setChecked(false);
    connect(m_bscanVScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscan);
    scalingsBscansMenu->addAction(m_bscanVScalingAct);
    scalingsBscansMenu->addSeparator();

    m_bscanDeepAllScalingAct  = new QAction(tr("&общее масштабирование по bscan deep"), this);
    m_bscanDeepAllScalingAct->setData(QString::fromStdString("all bscan deep scaling"));
    m_bscanDeepAllScalingAct->setCheckable(true);
    m_bscanDeepAllScalingAct->setChecked(true);
    connect(m_bscanDeepAllScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscanDeep);
    scalingsBscansMenu->addAction(m_bscanDeepAllScalingAct);

    m_bscanDeepHScalingAct  = new QAction(tr("&горизонтальное масштабирование по bscan deep"), this);
    m_bscanDeepHScalingAct->setData(QString::fromStdString("h bscan deep scaling"));
    m_bscanDeepHScalingAct->setCheckable(true);
    m_bscanDeepHScalingAct->setChecked(false);
    connect(m_bscanDeepHScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscanDeep);
    scalingsBscansMenu->addAction(m_bscanDeepHScalingAct);

    m_bscanDeepVScalingAct  = new QAction(tr("&вертикальное масштабирование по deep bscan"), this);
    m_bscanDeepVScalingAct->setData(QString::fromStdString("v bscan deep scaling"));
    m_bscanDeepVScalingAct->setCheckable(true);
    m_bscanDeepVScalingAct->setChecked(false);
    connect(m_bscanDeepVScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscanDeep);
    scalingsBscansMenu->addAction(m_bscanDeepVScalingAct);
    scalingsBscansMenu->addSeparator();

    m_bscansSurfAllScalingAct  = new QAction(tr("&общее масштабирование поверхности bscans"), this);
    m_bscansSurfAllScalingAct->setData(QString::fromStdString("all surf bscan scaling"));
    m_bscansSurfAllScalingAct->setCheckable(true);
    m_bscansSurfAllScalingAct->setChecked(true);
    connect(m_bscansSurfAllScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscansSurf);
    scalingsBscansMenu->addAction(m_bscansSurfAllScalingAct);

    m_bscansSurfHScalingAct  = new QAction(tr("&горизонтальное масштабирование поверхности bscans"), this);
    m_bscansSurfHScalingAct->setData(QString::fromStdString("h surf bscan scaling"));
    m_bscansSurfHScalingAct->setCheckable(true);
    m_bscansSurfHScalingAct->setChecked(false);
    connect(m_bscansSurfHScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscansSurf);
    scalingsBscansMenu->addAction(m_bscansSurfHScalingAct);

    m_bscansSurfVScalingAct  = new QAction(tr("&вертикальное масштабирование поверхности bscans"), this);
    m_bscansSurfVScalingAct->setData(QString::fromStdString("v surf bscan scaling"));
    m_bscansSurfVScalingAct->setCheckable(true);
    m_bscansSurfVScalingAct->setChecked(false);
    connect(m_bscansSurfVScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscansSurf);
    scalingsBscansMenu->addAction(m_bscansSurfVScalingAct);

    scalingsBscansMenu->addSeparator();

    m_bscansTransAllScalingAct  = new QAction(tr("&общее масштабирование между bscans"), this);
    m_bscansTransAllScalingAct->setData(QString::fromStdString("all trans bscans scaling"));
    m_bscansTransAllScalingAct->setCheckable(true);
    m_bscansTransAllScalingAct->setChecked(true);
    connect(m_bscansTransAllScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscansTrans);
    scalingsBscansMenu->addAction(m_bscansTransAllScalingAct);

    m_bscansTransHScalingAct  = new QAction(tr("&горизонтальное масштабирование между bscans"), this);
    m_bscansTransHScalingAct->setData(QString::fromStdString("h trans bscans scaling"));
    m_bscansTransHScalingAct->setCheckable(true);
    m_bscansTransHScalingAct->setChecked(false);
    connect(m_bscansTransHScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscansTrans);
    scalingsBscansMenu->addAction(m_bscansTransHScalingAct);

    m_bscansTransVScalingAct  = new QAction(tr("&вертикальное масштабирование между bscans"), this);
    m_bscansTransVScalingAct->setData(QString::fromStdString("v trans bscans scaling"));
    m_bscansTransVScalingAct->setCheckable(true);
    m_bscansTransVScalingAct->setChecked(false);
    connect(m_bscansTransVScalingAct, &QAction::triggered, this, &MainWindow::setupScalingOnBscansTrans);
    scalingsBscansMenu->addAction(m_bscansTransVScalingAct);

    enabledShowBscanActions(false);
}

void MainWindow::openTrz()
{
    if (m_trzDir == "") m_trzDir = QCoreApplication::applicationDirPath();

    m_filesBscanNamesTrz = QFileDialog::getOpenFileNames(this, tr("Select Multiple Files"), m_trzDir, tr("Image Files(*.trz)"));
    m_trzNumAntennasVector.resize(0);
    if (m_filesBscanNamesTrz.size() > 0)
    {
        m_trzDir = "";
        for (int count = 0; count <= m_filesBscanNamesTrz.at(0).split("/").size()-2; count++)
        {
            if (count != m_filesBscanNamesTrz.at(0).split("/").size()-2) m_trzDir = m_trzDir + m_filesBscanNamesTrz.at(0).split("/").at(count) + "/";
            else                                                       m_trzDir = m_trzDir + m_filesBscanNamesTrz.at(0).split("/").at(count) ;
        }
        writeLastBscansDirs(m_trzDir, m_csvDir, m_hdf5Dir);
        m_bscan2dWidget->setupTrzBscans(m_filesBscanNamesTrz, m_trzNumAntennasVector, m_bscansNamesVectorPairs);
    }
}

void MainWindow::openCsv()
{   
    if (m_csvDir == "") m_csvDir = QCoreApplication::applicationDirPath();

    m_bscan2dWidget->m_accomplishment->m_thread->m_executeParserData = false;
    m_filesBscanNamesCsv = QFileDialog::getOpenFileNames(this, tr("Select Multiple Files"), m_csvDir, tr("Image Files(*.csv)"));
    if (m_filesBscanNamesCsv.size() > 0)
    {
        m_csvDir = "";
        for (int count = 0; count <= m_filesBscanNamesCsv.at(0).split("/").size()-2; count++)
        {
            if (count != m_filesBscanNamesCsv.at(0).split("/").size()-2) m_csvDir = m_csvDir + m_filesBscanNamesCsv.at(0).split("/").at(count) + "/";
            else                                                       m_csvDir = m_csvDir + m_filesBscanNamesCsv.at(0).split("/").at(count) + "/";
        }
        writeLastBscansDirs(m_trzDir, m_csvDir, m_hdf5Dir);
        m_bscan2dWidget->setupCsvBscans(m_filesBscanNamesCsv, m_bscansNamesVectorPairs);
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

    m_infoBscansHdf5Names = hdf5dlg->selectedFiles();

    writeLastBscansDirs(m_trzDir, m_csvDir, m_hdf5Dir);

    m_bscan2dWidget->setupHdf5Bscans(m_infoBscansHdf5Names, m_bscansNamesVectorPairs);
    hdf5dlg->close();
}

void MainWindow::setupBscanCurrentName()
{
    if (m_bscanCurrentName == "")  m_bscanCurrentName = m_bscansNamesVectorPairs[0].first;
}

void MainWindow::setupBscanControls()
{
    m_bscan2dWidget->showNewBscan(m_bscanCurrentName);
    m_bscanDeep2dWidget->showNewBscanDeep(m_bscanCurrentName, m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName]);

    m_materialLabel->setVisible(true);
    m_materialComboBox->setVisible(true);
    if (m_stackedWidget->currentIndex() <=1)
    {
        enabledShowBscanActions(true);
        fixScrollHBscansLengthBar();
        if (m_bscansNamesVectorPairs.size() > 1)
        {
            setupBscansSliderSetup(0, m_bscansNamesVectorPairs.size());
            bscansTransitControlsVisible(true);
        }
    }
    createActionsFilesMenu();
}

void MainWindow::controlBscansSamplingData( bool& dataSampling, bool& executeParserData, BscanFileFormat BscanFileFormat)
{
    switch(BscanFileFormat)
    {
        case BscanFileFormat::Trz:
        {
            m_filesBscanNamesTrz.clear();
            m_trzNumAntennasVector.clear();
            if (m_filesBscanNamesCsv.size()     > 0)      m_bscan2dWidget->setupCsvBscans(m_filesBscanNamesCsv, m_bscansNamesVectorPairs);
            else if (m_infoBscansHdf5Names.size()   > 0)  m_bscan2dWidget->setupHdf5Bscans(m_infoBscansHdf5Names, m_bscansNamesVectorPairs);
            break;
        }
        case BscanFileFormat::Csv:
        {
            m_filesBscanNamesCsv.clear();
            if (m_infoBscansHdf5Names.size()   > 0) m_bscan2dWidget->setupHdf5Bscans(m_infoBscansHdf5Names, m_bscansNamesVectorPairs);
            break;
        }
        case BscanFileFormat::Hdf5:
        {
            m_infoBscansHdf5Names.clear();
            break;
        }
    }

    if (m_filesBscanNamesTrz.size() == 0 && m_trzNumAntennasVector.size() == 0 && m_filesBscanNamesCsv.size() == 0 && m_infoBscansHdf5Names.size() == 0)
    {
        setupBscanCurrentName();
        setupBscanControls();
        if (dataSampling == true) dataSampling = false;
        if (m_bscansNamesVectorPairs.size() > 1)
        {
            executeParserData = true;
            m_bscan2dWidget->m_accomplishment->m_thread->start();
        }
    }
}

void MainWindow::slotActivateAccompThread(std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs, BscanFileFormat bscanFileFormat)
{
    m_bscansNamesVectorPairs.swap(bscansNamesVectorPairs);
    if      (bscanFileFormat == BscanFileFormat::Trz)
        controlBscansSamplingData(m_bscan2dWidget->m_accomplishment->m_thread->m_trzDataSampling, m_bscan2dWidget->m_accomplishment->m_thread->m_executeParserData, bscanFileFormat);
    else if (bscanFileFormat == BscanFileFormat::Csv)
        controlBscansSamplingData( m_bscan2dWidget->m_accomplishment->m_thread->m_csvDataSampling, m_bscan2dWidget->m_accomplishment->m_thread->m_executeParserData, bscanFileFormat);
    else if (bscanFileFormat == BscanFileFormat::Hdf5)
        controlBscansSamplingData(m_bscan2dWidget->m_accomplishment->m_thread->m_hdf5DataSampling, m_bscan2dWidget->m_accomplishment->m_thread->m_executeParserData, bscanFileFormat);
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
        defBscansInRelief(fileName, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.vectorBscansInRelief);
        if (m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.leftLatitude > 0.0)
        {
            m_bscan2dWidget->m_accomplishment->m_thread->m_defBscansFnRelief = true;
            m_bscan2dWidget->m_accomplishment->m_thread->m_executeParserData = true;
            m_bscan2dWidget->m_accomplishment->m_thread->start();
        }
    }
}

void MainWindow::openImageBscan()
{
    enabledCutActions(false, false, false);
    m_stackedWidget->setCurrentWidget(m_bscan2dWidget);
    if (m_bscanCurrentName != "")
    {  
        impulsesBscanControlsVisible(false);
        if (m_bscansNamesVectorPairs.size() > 1)
        {
            fixScrollHBscansLengthBar();
            bscansTransitControlsVisible(true);
        }
        statusBar()->show();
        enabledShowBscanActions(true);
        showBscanImage(true);
    }
}

void  MainWindow::openImageDeepBscan()
{
    enabledCutActions(false, false, false);
    m_stackedWidget->setCurrentWidget(m_bscanDeep2dWidget);
    if (m_bscanCurrentName != "")
    {   
        impulsesBscanControlsVisible(false);
        if (m_bscansNamesVectorPairs.size() > 1)
        {
            fixScrollHBscansLengthBar();
            bscansTransitControlsVisible(true);
        }
        statusBar()->show();
        enabledShowBscanActions(false);
        showBscanImage(true);
    }
}

void MainWindow::slotSetupBscanDeepPixels()
{
    emit signalSetupBscanDeepPixels(m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName]);
}

void MainWindow::slotFixDataBscanDeepWidgets()
{
    emit signalSetupFixDataBscanDeepWidgets(m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName]);
}

void MainWindow::defPaletteBscan()
{
    if (m_bscanCurrentName != "" && m_stackedWidget->currentIndex() == 0 && !m_colorsPaletteDialog->isVisible()) m_colorsPaletteDialog->exec();
}

void MainWindow::defPaletteBscanDeep()
{
    if (m_bscanCurrentName != "" && m_stackedWidget->currentIndex() == 1 && !m_colorsPaletteDialog->isVisible()) m_colorsPaletteDialog->exec();
}

void MainWindow::defAbsBscansStructInXAndFnX(int& absBscansStructInX, int& absBscansStructFnX)
{
    if ( (m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfWidth > limitBscanWidth) && (m_showFullBscans == true) )
    {
        absBscansStructInX = m_scrollHBscansLengthBar->value()-limitBscanWidth+1;
        absBscansStructFnX = m_scrollHBscansLengthBar->value();
    }
    else if (m_showFullBscans == true)
    {
        absBscansStructInX = 0;
        absBscansStructFnX = m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfWidth-1;
    }

    if  ((m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfMinWidth > limitBscanWidth) && (m_showFullBscans == false) )
    {
        absBscansStructInX = m_scrollHBscansLengthBar->value()-limitBscanWidth+1;
        absBscansStructFnX = m_scrollHBscansLengthBar->value();
    }
    else if (m_showFullBscans == false)
    {
        absBscansStructInX = 0;
        absBscansStructFnX = m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfMinWidth-1;
    }
}

void MainWindow::scrollHBscansLengthBarResetDataAtBscansImages()
{
    if (m_showFullBscans == true)
    {
        if  (m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfWidth > limitBscanWidth)
            fixScrollHBscansLengthBar();
        else
            samplingAllLog10Bscans(
                m_materialId, m_filterId, m_bscansNamesVectorPairs, m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
                0, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfWidth-1
            );
    }
    else
    {
        if  (m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfMinWidth > limitBscanWidth)
            fixScrollHBscansLengthBar();
        else
            samplingAllLog10Bscans(
                m_materialId, m_filterId, m_bscansNamesVectorPairs, m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
                0, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfMinWidth-1
            );
    }
}

void MainWindow::open2dImageBscans()
{
    if (m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap.size()>1)
    {
        m_stackedWidget->setCurrentWidget(m_bscans2dWidget);

        int absBscansStructInX {0};
        int absBscansStructFnX {0};

        enabledCutActions(true, false, true);
        impulsesBscanControlsVisible(true);
        bscansTransitControlsVisible(false);

        statusBar()->show();
        enabledShowBscanActions(false);

        scrollHBscansLengthBarResetDataAtBscansImages();
        defAbsBscansStructInXAndFnX(absBscansStructInX, absBscansStructFnX);
        m_bscans2dWidget->showBscansSurf(
            m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
            m_quantImpulsesOfPacket, m_filterId, m_selectionId, m_highLowOnBscan, m_showFullBscans, m_resetSectPoints2dBscans, m_materialId,
            absBscansStructInX,  absBscansStructFnX
        );
    }
}

void  MainWindow::defPaletteSurfBscans()
{
    if (m_stackedWidget->currentIndex() == 2 && !m_colorsPaletteDialog->isVisible()) m_colorsPaletteDialog->exec();
}

void MainWindow::open3dImageBscans()
{
    if (m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap.size()>1)
    {
        m_stackedWidget->setCurrentWidget(m_bscans3dWidget);

        int absBscansStructInX {0};
        int absBscansStructFnX {0};

        enabledCutActions(false, true, true);
        impulsesBscanControlsVisible(true);
        bscansTransitControlsVisible(false);

        statusBar()->hide();
        enabledShowBscanActions(true);

        scrollHBscansLengthBarResetDataAtBscansImages();
        defAbsBscansStructInXAndFnX(absBscansStructInX, absBscansStructFnX);
        m_bscans3dWidget->showBscansImage(
            m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId, m_highLowOnBscan, m_showFullBscans, m_resetCutPointsBscans,
            m_materialId, absBscansStructInX,  absBscansStructFnX
        );
    }
}

void  MainWindow::defPaletteTransBscans()
{
    if (m_stackedWidget->currentIndex() == 3 && !m_colorsPaletteDialog->isVisible()) m_colorsPaletteDialog->exec();
}

void MainWindow::receiveColorsPalette(QColor& colorUp, QColor& colorLow)
{
    if (m_bscanCurrentName != "")
    {
        switch (m_stackedWidget->currentIndex())
        {
            case 0:
            {
                m_bscan2dWidget->setupColorsBscanAllProps(colorUp, colorLow);
                break;
            }
            case 1:
            {
                m_bscanDeep2dWidget->setupColorsBscanDeepAllProps(colorUp, colorLow, m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName]);
                break;
            }
            case 2:
            {
                m_bscans2dWidget->setupColorsBscansSurfAllProps(colorUp, colorLow);
                break;
            }
            case 3:
            {
                m_bscans3dWidget->setupColorsBscansTransAllProps(colorUp, colorLow);
                break;
            }
        }
    }
}

void MainWindow::openRecentFile()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender()))
    {
        m_bscanCurrentName = action->data().toString().toStdString();
        if (m_stackedWidget->currentIndex() <= 1)
        {
            auto vectorPairsIt = std::find_if(
                m_bscansNamesVectorPairs.begin(), m_bscansNamesVectorPairs.end(), [&](const std::pair<std::string, std::string>& stringPair) { return (stringPair.first == m_bscanCurrentName); }
            );
            
            setupBscansSliderSetup(std::distance(m_bscansNamesVectorPairs.begin(), vectorPairsIt), m_bscansNamesVectorPairs.size());
            showBscanImage(true);
        }
    }
}

void MainWindow::deleteSelectedFile()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))  deleteFile(action->data().toString().toStdString());
}

void MainWindow::deleteAllFiles()
{
    for (int count = 0; count < m_bscansNamesVectorPairs.size(); count++)
    {      
        emit signalEraseBscansInfoDataMap(m_bscansNamesVectorPairs[count].first);
        m_bscan2dWidget->m_bscanGlWidget->clearSelectedData(m_bscansNamesVectorPairs[count].first);
    }

    m_bscansNamesVectorPairs.clear();
    createActionsFilesMenu();

    emit signalClearBscansInfoDataMap();

    if (m_stackedWidget->currentIndex() > 1) m_stackedWidget->setCurrentWidget(m_bscan2dWidget);

    m_bscanCurrentName = "";
    m_bscan2dWidget->m_bscanGlWidget->m_bscanName = "";
    m_bscan2dWidget->m_accomplishment->m_thread->clearWorkData();

    m_bscanDeep2dWidget->m_bscanDeepGlWidget->m_bscanName = "";
    enabledShowBscanActions(false);

    showBscanImage(true);
    m_bscanQuantImpulsesLabel->setText("Число отсчетов ");
    m_materialLabel->setVisible(false);
    m_materialComboBox->setVisible(false);

    fixScrollHBscansLengthBar();
    impulsesBscanControlsVisible(false);
    bscansTransitControlsVisible(false);
    enabledCutActions(false, false, false);
}

void MainWindow::save()
{
    if (m_bscanCurrentName != "")
    {
        switch (m_stackedWidget->currentIndex())
        {
            case 0:
            {
                m_bscan2dWidget->saveBscan();
                break;
            }
            case 1:
            {
                m_bscanDeep2dWidget->saveBscanDeep();
                break;
            }
            case 2:
            {
                m_bscans2dWidget->saveBscansSurf();
                break;
            }
            case 3:
            {
                m_bscans3dWidget->saveBscansStruct();
                break;
            }
        }
    }
}

void MainWindow::deleteFile(const std::string& fileName)
{
    m_recentBscansFileActsVector.resize(0);
    m_deleteBscansFileActsVector.resize(0);

    emit signalEraseBscansInfoDataMap(fileName);

    m_bscan2dWidget->m_bscanGlWidget->clearSelectedData(fileName);

    auto vectorPairsIt = std::find_if(
        m_bscansNamesVectorPairs.begin(), m_bscansNamesVectorPairs.end(), [&](const std::pair<std::string, std::string>& stringPair) { return (stringPair.first == fileName); }
    );
    
    m_bscansNamesVectorPairs.erase(vectorPairsIt);

    if (m_bscansNamesVectorPairs.size()>=0)  createActionsFilesMenu();

    if (m_bscansNamesVectorPairs.size()>1)
    {
        if (m_bscanCurrentName == fileName) m_bscanCurrentName = m_bscansNamesVectorPairs[0].first;
        if (m_stackedWidget->currentIndex() <= 1 && m_bscanCurrentName != fileName)
        {
            showBscanImage(true);
            fixScrollHBscansLengthBar();
        }

        setupBscansSliderSetup(0, m_bscansNamesVectorPairs.size());

        m_bscan2dWidget->m_accomplishment->m_thread->m_executeParserData = true;
        m_bscan2dWidget->m_accomplishment->m_thread->m_nameDeleteBscan     = fileName;
        m_bscan2dWidget->m_accomplishment->m_thread->m_executeDeleteBscan  = true;
        m_bscan2dWidget->m_accomplishment->m_thread->start();
    }
    else
    {
        if (m_stackedWidget->currentIndex()>1) m_stackedWidget->setCurrentWidget(m_bscan2dWidget);
        if (m_bscansNamesVectorPairs.size() == 1)
        {
            m_bscanCurrentName = m_bscansNamesVectorPairs[0].first;
            enabledCutActions(false, false, false);
            enabledShowBscanActions(true);
        }
        else
        {
            m_bscanCurrentName = "";
            m_bscan2dWidget->m_bscanGlWidget->m_bscanName = "";
            m_bscanDeep2dWidget->m_bscanDeepGlWidget->m_bscanName = "";
            enabledShowBscanActions(false);
            m_materialLabel->setVisible(false);
            m_materialComboBox->setVisible(false);
            m_bscan2dWidget->m_accomplishment->m_thread->clearWorkData();
        }

        showBscanImage(true);
        m_bscanQuantImpulsesLabel->setText("Число отсчетов ");
        fixScrollHBscansLengthBar();
        impulsesBscanControlsVisible(false);
        bscansTransitControlsVisible(false);
        enabledCutActions(false, false, false);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::showBscanImage(bool changeBscan)
{
    if (changeBscan == true) resetBscanSelection(false);
    else
    {
        m_bscan2dWidget->outputNewImage(m_bscanCurrentName, m_materialId, m_filterId, m_selectionId);
        if (m_bscanCurrentName != "")
        {
            std::cout<<"m_bscanDeep2dWidget->outputNewImage"<<std::endl;
            m_bscanDeep2dWidget->outputNewImage(
                m_bscanCurrentName, m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName], m_materialId, m_filterId, m_selectionId
            );
        }
        else
        {
            std::cout<<"m_bscanDeep2dWidget->outputEmptyImage();"<<std::endl;
            m_bscanDeep2dWidget->outputEmptyImage();
        }
    }
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
        if      (m_stackedWidget->currentIndex() == 2) m_bscans2dWidget->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
        else if (m_stackedWidget->currentIndex() == 3) m_bscans3dWidget->saveBoundPointsLineAction(saveBoundPointsLinePointsFile);
    }
}

void MainWindow::loadBoundPointsLineAction()
{
    QString loadBoundPointsLinePointsFile = QFileDialog::getOpenFileName(this, tr("Open csv file"), "cutLinePointsFile", tr("CSV File(*.csv);"));
    if (loadBoundPointsLinePointsFile != "")
    {
        if      (m_stackedWidget->currentIndex() == 2)  m_bscans2dWidget->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
        else if (m_stackedWidget->currentIndex() == 3)  m_bscans3dWidget->loadBoundPointsLineAction(loadBoundPointsLinePointsFile);
    }
}

void MainWindow::createActionsFilesMenu()
{
    m_recentFilesBscanMenu->clear();
    m_deleteFilesBscanMenu->clear();

    std::vector<std::string> bscansNamesVector;
    bscansNamesVector.resize(0);
    for (int count = 0; count < m_bscansNamesVectorPairs.size(); count++)
    {
        bscansNamesVector.push_back(m_bscansNamesVectorPairs[count].first);
    }
    std::sort(bscansNamesVector.begin(), bscansNamesVector.end());

    m_recentBscansFileActsVector.resize(bscansNamesVector.size());
    m_deleteBscansFileActsVector.resize(bscansNamesVector.size());

    for (int i = 0; i < m_recentBscansFileActsVector.size(); i++)
    {
        m_recentBscansFileActsVector[i] =
        m_recentFilesBscanMenu         ->addAction(QString::fromStdString(bscansNamesVector[i]), this, &MainWindow::openRecentFile);
        m_recentBscansFileActsVector[i]->  setData(QString::fromStdString(bscansNamesVector[i]));
        m_recentBscansFileActsVector[i]->setVisible(true);

        m_deleteBscansFileActsVector[i] =
        m_deleteFilesBscanMenu         ->addAction(QString::fromStdString(bscansNamesVector[i]), this, &MainWindow::deleteSelectedFile);
        m_deleteBscansFileActsVector[i]->  setData(QString::fromStdString(bscansNamesVector[i]));
        m_deleteBscansFileActsVector[i]->setVisible(true);
    }

    writeLastNamesBscansToSettings(m_bscansNamesVectorPairs);
}

void MainWindow::receiveBscansWorkData()
{
    m_bscan2dWidget->accomplishmentThreadTerminate();

    if (m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.vectorBscansInRelief.size() == 0)
    {
        bscanImpulsesSliderActivate();
        m_resetSectPoints2dBscans = true;
        m_resetCutPointsBscans = true;
        outputBscansData(
            m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
    }
    else
    {
        m_bscan2dWidget->m_accomplishment->m_thread->m_defBscansFnRelief = true;
        m_bscan2dWidget->m_accomplishment->m_thread->m_executeParserData = true;
        m_bscan2dWidget->m_accomplishment->m_thread->start();
    }
}

void MainWindow::receiveBscansZData()
{
    m_bscan2dWidget->m_accomplishment->m_thread->m_defBscansFnRelief = false;
    m_bscan2dWidget->accomplishmentThreadTerminate();

    bscanImpulsesSliderActivate();
    m_resetSectPoints2dBscans = true;
    m_resetCutPointsBscans = true;
    outputBscansData(
        m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
        m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
    );
}

void MainWindow::outputBscansData(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData,
    int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int selectionId
)
{
    int absBscansStructInX {0};
    int absBscansStructFnX {0};

    if (m_stackedWidget->currentIndex() == 2)
    {
        defAbsBscansStructInXAndFnX(absBscansStructInX, absBscansStructFnX);
        m_bscans2dWidget->showBscansSurf(
            bscansInfoDataMap, bscansWorkData, quantImpulsesOfPacket, filterId, selectionId,
            m_highLowOnBscan, m_showFullBscans, m_resetSectPoints2dBscans, m_materialId, absBscansStructInX, absBscansStructFnX
        );
        if (m_resetSectPoints2dBscans == true) m_resetSectPoints2dBscans = false;
    }
    else if (m_stackedWidget->currentIndex() == 3)
    {
        defAbsBscansStructInXAndFnX(absBscansStructInX, absBscansStructFnX);
        m_bscans3dWidget->showBscansImage(
            bscansInfoDataMap, bscansWorkData,  maxQuantImpulsesOfPacket, quantImpulsesOfPacket, filterId, selectionId,
            m_highLowOnBscan, m_showFullBscans, m_resetCutPointsBscans, m_materialId, absBscansStructInX, absBscansStructFnX
        );
        if (m_resetCutPointsBscans == true) m_resetCutPointsBscans = false;
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
        m_highLowImpulsesBscansAction->setEnabled(true);
        m_showFullBscansAction       ->setEnabled(true);
    }
    else
    {
        m_highLowImpulsesBscansAction->setEnabled(false);
        m_showFullBscansAction       ->setEnabled(false);
    }
}

void MainWindow::enabledShowBscanActions(bool enabled)
{
    m_showInitBscanAct->setEnabled(enabled);
    m_showLogBscanAct->setEnabled(enabled);
    if (m_stackedWidget->currentIndex() == 3)
    {
        m_logBscanMouseSelectionAct->setEnabled(!enabled);
        m_logBscanAutoSelectionAct->setEnabled(!enabled);
        m_showInitBscanSelectionAct->setEnabled(!enabled);
        m_resetBscanSelectionAct->setEnabled(!enabled);
        m_saveBscanAutoSelectionDataAct->setEnabled(!enabled);
        m_loadBscanAutoSelectionDataAct->setEnabled(!enabled);
    }
    else
    {
        m_logBscanMouseSelectionAct->setEnabled(enabled);
        m_logBscanAutoSelectionAct->setEnabled(enabled);
        m_showInitBscanSelectionAct->setEnabled(enabled);
        m_resetBscanSelectionAct->setEnabled(enabled);
        m_saveBscanAutoSelectionDataAct->setEnabled(enabled);
        m_loadBscanAutoSelectionDataAct->setEnabled(enabled);
    }
}

void MainWindow::bscanImpulsesSliderActivate()
{
    m_bscanImpulsesSliderActivate = true;
    defMaxQuantImpulsesOfPacket(m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap, m_maxQuantImpulsesOfPacket);
    m_quantImpulsesOfPacket = m_maxQuantImpulsesOfPacket-1;

    m_bscanQuantImpulsesSlider->setRange(1, m_maxQuantImpulsesOfPacket-1);
    m_bscanQuantImpulsesSlider->setValue(m_quantImpulsesOfPacket);
    m_bscanQuantImpulsesSlider->setSingleStep(1);
    m_bscanQuantImpulsesSlider->setTickPosition(QSlider::TicksRight);
    m_bscanQuantImpulsesLabel->setText("Число отсчетов " + QString::number(m_quantImpulsesOfPacket) + " .");
}

void MainWindow::setQuantImpulsesOfPacketSlider(int quantImpulsesOfPacket)
{
    if (m_bscanImpulsesSliderActivate == false)
    {
        m_quantImpulsesOfPacket = quantImpulsesOfPacket;
        m_bscanQuantImpulsesLabel->setText("Число отсчетов " + QString::number(m_quantImpulsesOfPacket) + " .");

        m_resetSectPoints2dBscans = false;
        m_resetCutPointsBscans = false;
        outputBscansData(
            m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
            m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
        m_resetSectPoints2dBscans = true;
        m_resetCutPointsBscans = true;
    }
    else  m_bscanImpulsesSliderActivate = false;
}

void MainWindow::changeHighLowOnBscans()
{
    if (m_stackedWidget->currentIndex() > 1)
    {
        if (m_highLowImpulsesBscansAction->isChecked() == false) m_highLowOnBscan = false;  else m_highLowOnBscan = true;
        m_resetSectPoints2dBscans = false;
        m_resetCutPointsBscans = false;
        outputBscansData(
            m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
            m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
        m_resetSectPoints2dBscans = true;
        m_resetCutPointsBscans = true;
    }
}

void MainWindow::showFullBscansData()
{
    if (m_stackedWidget->currentIndex() > 1)
    {
        m_showFullBscans = m_showFullBscansAction->isChecked();

        m_resetSectPoints2dBscans = false;
        m_resetCutPointsBscans = false;

        int bscansSurfWidth    = m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfWidth   ;
        int bscansSurfWidthMin = m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfMinWidth;

        if (m_showFullBscans)
        {
            if (bscansSurfWidth > limitBscanWidth)
            {
                setupScrollHBscansLengthBarData(limitBscanWidth-1, limitBscanWidth-1, bscansSurfWidth-1);
            }
            else
            {
                setupScrollHBscansLengthBarData(0,                 0,                 0);
                outputBscansData(
                    m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
                    m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
                    m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
                );
            }
        }
        else if (!m_showFullBscans)
        {
            if (bscansSurfWidthMin > limitBscanWidth)
            {
                setupScrollHBscansLengthBarData(limitBscanWidth-1, limitBscanWidth-1, bscansSurfWidthMin-1);
            }
            else
            {
                setupScrollHBscansLengthBarData(0,                 0,                 0);
                outputBscansData(
                    m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
                    m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
                    m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
                );
            }
        }

        m_resetSectPoints2dBscans = true;
        m_resetCutPointsBscans = true;
    }
}

void MainWindow::setInitBscan()
{
    m_bscan2dWidget->setShowLogBscan(false);
    m_bscans3dWidget->setShowLogBscansTrans(false);
    if      (m_stackedWidget->currentIndex() == 0)
    {
        showBscanImage(false);
    }
    else if (m_stackedWidget->currentIndex() == 3) m_bscans3dWidget->representBscansTransGlWidget(
        m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
        m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData
    );
}

void MainWindow::setLogBscan()
{
    m_bscan2dWidget->setShowLogBscan(true);
    m_bscans3dWidget->setShowLogBscansTrans(true);
    if (m_stackedWidget->currentIndex() == 0)
    {
        showBscanImage(false);
    }
    else if (m_stackedWidget->currentIndex() == 3) m_bscans3dWidget->representBscansTransGlWidget(
        m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
        m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData
    );
}

void MainWindow::setLogBscanMouseSelection()
{
    m_bscan2dWidget->setLogBscanMouseSelection(m_logBscanMouseSelectionAct->isChecked());
    if (m_logBscanAutoSelectionAct->isChecked())
    {
        m_bscan2dWidget->settingsBscanSelection(false, false, m_bscanCurrentName);
        m_logBscanAutoSelectionAct->setChecked(false);
    }
    closeInitBscanSelection();
}

void MainWindow::setLogBscanAutoSelection()
{
    m_bscan2dWidget->settingsBscanSelection(m_logBscanAutoSelectionAct->isChecked(), false, m_bscanCurrentName);
    if (m_logBscanMouseSelectionAct->isChecked())
    {
        m_bscan2dWidget->setLogBscanMouseSelection(false);
        m_logBscanMouseSelectionAct->setChecked(false);
    }
    closeInitBscanSelection();
}

void MainWindow::setShowInitBscanSelection()
{
    m_bscan2dWidget->setShowInitBscanSelection(m_showInitBscanSelectionAct->isChecked());
    showBscanImage(false);
}

void MainWindow::slotResetBscanSelection()
{
    resetBscanSelection(true);
}

void MainWindow::resetBscanSelection(bool clearSelectBscanPoints)
{
    m_bscan2dWidget->resetBscanSelection(clearSelectBscanPoints, m_bscanCurrentName);

    if (m_showInitBscanSelectionAct->isChecked())
    {
        m_showInitBscanSelectionAct->setChecked(false);
        m_bscan2dWidget->setShowInitBscanSelection(false);
    }
    if (m_logBscanMouseSelectionAct->isChecked())
    {
        m_logBscanMouseSelectionAct->setChecked(false);
        m_bscan2dWidget->setLogBscanMouseSelection(false);
    }
    if (m_logBscanAutoSelectionAct->isChecked()){  m_logBscanAutoSelectionAct->setChecked(false); }

    showBscanImage(false);
}

void MainWindow::closeInitBscanSelection()
{
    m_showInitBscanSelectionAct->setChecked(false);
    setShowInitBscanSelection();
}

void MainWindow::impulsesBscanControlsVisible(bool visible)
{
    m_bscanQuantImpulsesLabel->setVisible(visible);
    m_bscanQuantImpulsesSlider->setVisible(visible);
}

void MainWindow::bscansTransitControlsVisible(bool visible)
{
    m_bscansTransitLabel->setVisible(visible);
    m_bscansTransitSlider->setVisible(visible);
    m_leftBscansTransitButton->setVisible(visible);
    m_rightBscansTransitButton->setVisible(visible);
}

void MainWindow::saveBscanAutoSelectionData()
{
    QString saveBscanAutoSelectionDataFile = QFileDialog::getSaveFileName(this, tr("Save As"), "bscanAutoSelectionData", tr("CSV File(*.csv);"));
    if (saveBscanAutoSelectionDataFile != "")   m_bscan2dWidget->saveBscanAutoSelectionData(saveBscanAutoSelectionDataFile);
}

void MainWindow::loadBscanAutoSelectionData()
{
    QString loadBscanAutoSelectionDataFile = QFileDialog::getOpenFileName(this, tr("Open csv file"), "", tr("CSV File(*.csv);"));
    if (loadBscanAutoSelectionDataFile != "" && m_bscanCurrentName != "") m_bscan2dWidget->loadBscanAutoSelectionData(loadBscanAutoSelectionDataFile, m_bscanCurrentName);
}

void MainWindow::materialIdChanged(int materialId)
{
    m_materialId = materialId;
    if (m_stackedWidget->currentIndex() <= 1 && m_bscanCurrentName != "")  showBscanImage(false);
    else if (m_stackedWidget->currentIndex() > 1 && m_recentBscansFileActsVector.size() > 1)
    {
        scrollHBscansLengthBarResetDataAtBscansImages();
        outputBscansData(
            m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
            m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
    }
}

void MainWindow::includeFilterOnBscan()
{
    if (m_includeFilterOnBscanAction->isChecked()) m_filterId = 1;
    else                                           m_filterId = 0;

    if      (m_stackedWidget->currentIndex() <=1 && m_bscanCurrentName != "")    showBscanImage(false);
    else if (m_stackedWidget->currentIndex() > 1 && m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap.size()>1)
    {
        scrollHBscansLengthBarResetDataAtBscansImages();
        outputBscansData(
            m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
            m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
            m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
        );
    }
}

void MainWindow::useSelectionOnBscan()
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

        if (m_stackedWidget->currentIndex() <= 1)
        {
            m_bscan2dWidget->outputNewImage(m_bscanCurrentName, m_materialId, m_filterId, m_selectionId);
            m_bscanDeep2dWidget->outputNewImage(
                m_bscanCurrentName, m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName],
                m_materialId, m_filterId, m_selectionId
            );
        }
        else
        {
            outputBscansData(
                m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
                m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
                m_maxQuantImpulsesOfPacket, m_quantImpulsesOfPacket, m_filterId, m_selectionId
            );
        }

        statusBar()->showMessage(action->data().toString());
    }
}

void MainWindow::setupScalingOnBscan()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))
    {
        int   typeScaling {0};
        if (action->data().toString().toStdString()      == "all bscan scaling")
        {
            typeScaling = 0;
            m_bscanAllScalingAct->setChecked(true);
            m_bscanHScalingAct->setChecked(false);
            m_bscanVScalingAct->setChecked(false);
        }
        else if (action->data().toString().toStdString() ==  "h bscan scaling")
        {
            typeScaling = 1;
            m_bscanAllScalingAct->setChecked(false);
            m_bscanHScalingAct->setChecked(true);
            m_bscanVScalingAct->setChecked(false);
        }
        else if(action->data().toString().toStdString()  == "v bscan scaling")
        {
            typeScaling = 2;
            m_bscanAllScalingAct->setChecked(false);
            m_bscanHScalingAct->setChecked(false);
            m_bscanVScalingAct->setChecked(true);
        }
        m_bscan2dWidget->setupTypeScaling(typeScaling);
    }
}

void MainWindow::setupScalingOnBscanDeep()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))
    {
        int   typeScaling {0};
        if (action->data().toString().toStdString()      == "all bscan deep scaling")
        {
            typeScaling = 0;
            m_bscanDeepAllScalingAct->setChecked(true);
            m_bscanDeepHScalingAct->setChecked(false);
            m_bscanDeepVScalingAct->setChecked(false);
        }
        else if (action->data().toString().toStdString() ==  "h bscan deep scaling")
        {
            typeScaling = 1;
            m_bscanDeepAllScalingAct->setChecked(false);
            m_bscanDeepHScalingAct->setChecked(true);
            m_bscanDeepVScalingAct->setChecked(false);
        }
        else if(action->data().toString().toStdString()  == "v bscan deep scaling")
        {
            typeScaling = 2;
            m_bscanDeepAllScalingAct->setChecked(false);
            m_bscanDeepHScalingAct->setChecked(false);
            m_bscanDeepVScalingAct->setChecked(true);
        }
        m_bscanDeep2dWidget->setupTypeScaling(typeScaling);
    }
}

void MainWindow::setupScalingOnBscansSurf()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))
    {
        int   typeScaling {0};
        if (action->data().toString().toStdString()      == "all surf scaling")
        {
            typeScaling = 0;
            m_bscansSurfAllScalingAct->setChecked(true);
            m_bscansSurfHScalingAct->setChecked(false);
            m_bscansSurfVScalingAct->setChecked(false);
        }
        else if (action->data().toString().toStdString() ==  "h surf scaling")
        {
            typeScaling = 1;
            m_bscansSurfAllScalingAct->setChecked(false);
            m_bscansSurfHScalingAct->setChecked(true);
            m_bscansSurfVScalingAct->setChecked(false);
        }
        else if(action->data().toString().toStdString()  == "v surf scaling")
        {
            typeScaling = 2;
            m_bscansSurfAllScalingAct->setChecked(false);
            m_bscansSurfHScalingAct->setChecked(false);
            m_bscansSurfVScalingAct->setChecked(true);
        }
        m_bscans2dWidget->setupTypeScaling(typeScaling);
    }
}

void MainWindow::setupScalingOnBscansTrans()
{
    if (const QAction* action = qobject_cast<const QAction*>(sender()))
    {
        int   typeScaling {0};
        if (action->data().toString().toStdString() == "all trans scaling")
        {
            typeScaling = 0;
            m_bscansTransAllScalingAct->setChecked(true);
            m_bscansTransHScalingAct->setChecked(false);
            m_bscansTransVScalingAct->setChecked(false);
        }
        else if (action->data().toString().toStdString() ==  "h trans scaling")
        {
            typeScaling = 1;
            m_bscansTransAllScalingAct->setChecked(false);
            m_bscansTransHScalingAct->setChecked(true);
            m_bscansTransVScalingAct->setChecked(false);
        }
        else if(action->data().toString().toStdString() == "v trans scaling")
        {
            typeScaling = 2;
            m_bscansTransAllScalingAct->setChecked(false);
            m_bscansTransHScalingAct->setChecked(false);
            m_bscansTransVScalingAct->setChecked(true);
        }
        m_bscans3dWidget->setupTypeScaling(typeScaling);
    }
}

void MainWindow::setupBscansSliderSetup(int bscanId, int bscansCount)
{
    m_bscansTransitSlider->setRange(0, bscansCount-1);
    m_bscansTransitSlider->setSingleStep(1);
    m_bscansTransitSlider->setValue(bscanId);
}

void MainWindow::changeBscan(int bscanId)
{
    m_bscan2dWidget->m_ascanDataWidget->m_trackSlider->setValue(0);
    m_bscanCurrentName = m_bscansNamesVectorPairs[bscanId].first;

    m_bscan2dWidget->showNewBscan(m_bscanCurrentName);
    m_bscanDeep2dWidget->showNewBscanDeep(m_bscanCurrentName, m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName]);
    fixScrollHBscansLengthBar();
}

void MainWindow::leftChangeBscan()
{
    int bscanId = m_bscansTransitSlider->value();
    if (bscanId-1 >=0)
    {
        m_bscansTransitSlider->setValue(bscanId-1);
    }
}

void MainWindow::rightChangeBscan()
{
    int bscanId = m_bscansTransitSlider->value();
    if (bscanId+1 <= m_bscansTransitSlider->maximum())
    {
        m_bscansTransitSlider->setValue(bscanId+1);
    }
}

void MainWindow::fixScrollHBscansLengthBar()
{   
    setupScrollHBscansLengthBarData(0,                 0,                 0);

    if (m_bscanCurrentName != "")
    {
        switch (m_stackedWidget->currentIndex())
        {
            case 0:
            case 1:
            {
                int bscanWidth = m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName].vectorBscanData.size();
                if (bscanWidth > limitBscanWidth)   setupScrollHBscansLengthBarData(limitBscanWidth-1, limitBscanWidth-1, bscanWidth-1);
                break;
            }
            case 2:
            case 3:
            {
                int bscansSurfWidth    = m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfWidth   ;
                int bscansSurfWidthMin = m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData.bscansSurfMinWidth;
                if (m_showFullBscansAction->isChecked() == true)
                    if (bscansSurfWidth > limitBscanWidth) setupScrollHBscansLengthBarData(limitBscanWidth-1, limitBscanWidth-1, bscansSurfWidth-1);
                else if (m_showFullBscansAction->isChecked() == false)
                    if (bscansSurfWidthMin > limitBscanWidth)    setupScrollHBscansLengthBarData(limitBscanWidth-1, limitBscanWidth-1, bscansSurfWidthMin-1);
                break;
            }
        }
    }
}


void MainWindow::setupScrollHBscansLengthBarData(int minScrollHBscansLengthBarData, int scrollHBscansLengthBarData, int maxScrollHBscansLengthBarData)
{
    m_scrollHBscansLengthBar->setRange(minScrollHBscansLengthBarData, maxScrollHBscansLengthBarData);
    m_scrollHBscansLengthBar->setValue(scrollHBscansLengthBarData);
    moveOnAnyBscansImage(scrollHBscansLengthBarData);
}

void MainWindow::scrollHBscansLengthBarPressed()
{
    m_scrollHBscansLengthBarAct = true;
}

void MainWindow::scrollHBscansLengthBarMoved(int scrollHBscansLengthBarPos)
{
    m_scrollHBscansLengthBarPos = scrollHBscansLengthBarPos;
    m_scrollHBscansLengthBarAct = true;
}

void MainWindow::scrollHBscansLengthBarReleased()
{
    moveOnAnyBscansImage(m_scrollHBscansLengthBarPos);
    m_scrollHBscansLengthBarAct = false;
}

void MainWindow::scrollHBscansLengthBarChanged(int scrollHBscansLengthBarPos)
{
    if ( m_scrollHBscansLengthBar->maximum() > 0)
    {
        m_scrollHBscansLengthBarPos = scrollHBscansLengthBarPos;
        if (m_scrollHBscansLengthBarAct == false)
        {
            moveOnAnyBscansImage(m_scrollHBscansLengthBarPos);
        }
    }
}

void MainWindow::moveOnAnyBscansImage(int scrollHBscansLengthBarPos)
{
    if (m_bscanCurrentName != "" && scrollHBscansLengthBarPos >= limitBscanWidth-1)
    {
        switch (m_stackedWidget->currentIndex())
        {
            case 0:
            case 1:
            {
                m_bscan2dWidget->changeBscanImage(scrollHBscansLengthBarPos-limitBscanWidth+1, scrollHBscansLengthBarPos);
                m_bscanDeep2dWidget->changeBscanDeepImage(
                    m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanCurrentName],
                    scrollHBscansLengthBarPos-limitBscanWidth+1, scrollHBscansLengthBarPos
                );
                break;
            }
            case 2:
            case 3:
            {
                //обработка логарифмического образа по всем радарограммам (она обязательна)
                samplingAllLog10Bscans(
                    m_materialId, m_filterId, m_bscansNamesVectorPairs, m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
                    scrollHBscansLengthBarPos-limitBscanWidth+1, scrollHBscansLengthBarPos
                );

                if (m_stackedWidget->currentIndex() == 2)
                    m_bscans2dWidget->changeBscansSurf(
                        m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
                        m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
                        m_quantImpulsesOfPacket, m_filterId, m_selectionId, m_highLowOnBscan, m_showFullBscans, m_resetSectPoints2dBscans,
                        m_materialId, scrollHBscansLengthBarPos-limitBscanWidth+1, scrollHBscansLengthBarPos
                    );
                else if (m_stackedWidget->currentIndex() == 3)
                    m_bscans3dWidget->changeBscansImage(
                        m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap,
                        m_bscan2dWidget->m_accomplishment->m_thread->m_st_bscansWorkData,
                        m_quantImpulsesOfPacket, m_filterId, m_selectionId, m_highLowOnBscan,  m_showFullBscans, true,
                        m_materialId, scrollHBscansLengthBarPos-limitBscanWidth+1, scrollHBscansLengthBarPos
                    );
                break;
            }
        }
    }
}

void MainWindow::slotSetupBscanDeepPageData(const std::string bscanName)
{
    emit signalSetupBscanDeepPageData(m_bscan2dWidget->m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName]);
}
