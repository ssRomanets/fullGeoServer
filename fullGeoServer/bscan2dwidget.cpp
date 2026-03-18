#include <QtWidgets>

#include "bscan2dWidget.h"
#include "bscanGlWidget.h"
#include "transformation.h"
#include <iostream>

bscan2dWidget::bscan2dWidget(QWidget* parent): QWidget(parent)
{
    m_accomplishment = new Accomplishment();

    m_bscanGlWidget = new bscanGlWidget();

    m_scrollHBar = new QScrollBar(Qt::Horizontal);
    m_scrollHBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollHData(0, 0, 0);

    m_scrollVBar = new QScrollBar(Qt::Vertical);
    m_scrollVBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollVData(0, 0, 0);

    m_leftVLayout = new QVBoxLayout;
    m_leftVLayout->addWidget(m_bscanGlWidget);
    m_leftVLayout->addWidget(m_scrollHBar);

    m_leftHLayout = new QHBoxLayout;
    m_leftHLayout->addLayout(m_leftVLayout);
    m_leftHLayout->addWidget(m_scrollVBar);

    m_leftAllVLayout = new QVBoxLayout;
    m_leftAllVLayout->addLayout(m_leftHLayout);

    m_lowBrightLabel = new QLabel();
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));

    m_lowBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_lowBrightPixelSlider->setRange(0, m_bscanGlWidget->m_vectorTuplesColors.size()-1);
    m_lowBrightPixelSlider->setSingleStep(1);
    m_lowBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_highBrightLabel = new QLabel();
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_bscanGlWidget->m_vectorTuplesColors.size()-1));

    m_highBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_highBrightPixelSlider->setRange(0, m_bscanGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightPixelSlider->setSingleStep(1);
    m_highBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_contrastLog10Label = new QLabel();
    m_contrastLog10Label->setText("Log10 Contrast "+QString::number(0));

    m_contrastLog10Slider = new QSlider(Qt::Horizontal);
    m_contrastLog10Slider->setRange(0, 1000);
    m_contrastLog10Slider->setSingleStep(1);
    m_contrastLog10Slider->setTickPosition(QSlider::TicksRight);

    m_leftLowLayout  = new QHBoxLayout;
    m_leftLowLayout->addWidget(m_lowBrightLabel);
    m_leftLowLayout->addWidget(m_lowBrightPixelSlider);
    m_leftLowLayout->addWidget(m_highBrightLabel);
    m_leftLowLayout->addWidget(m_highBrightPixelSlider);
    m_leftLowLayout->addWidget(m_contrastLog10Label);
    m_leftLowLayout->addWidget(m_contrastLog10Slider);
    m_leftAllVLayout->addLayout(m_leftHLayout);
    m_leftAllVLayout->addLayout(m_leftLowLayout);

    m_ascanDataWidget  = new ascanDataWidget(parent);

    m_rightLayout = new QVBoxLayout;
    m_rightLayout->addWidget(m_ascanDataWidget);

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->addLayout(m_leftAllVLayout);
    m_mainLayout->addLayout(m_rightLayout);
    setLayout(m_mainLayout);

    connect(m_lowBrightPixelSlider,   &QSlider::valueChanged, this, &bscan2dWidget::setLowPixelLevel);
    connect(m_highBrightPixelSlider,  &QSlider::valueChanged, this, &bscan2dWidget::setHighPixelLevel);
    connect(m_contrastLog10Slider,    &QSlider::valueChanged, this, &bscan2dWidget::setContrastLog10Level);

    connect(m_ascanDataWidget->m_ascanWidget,                 &ascanWidget::fixTrackIndex, m_bscanGlWidget, &bscanGlWidget::remarkTrack);
    connect(m_ascanDataWidget->m_tableAscanDataWidget,        &tableAscanDataWidget::fixTrackIndex, m_bscanGlWidget, &bscanGlWidget::remarkTrack);

    connect(m_bscanGlWidget, &bscanGlWidget::sendVectorBscanPairXY,           m_ascanDataWidget, &ascanDataWidget::receiveVectorBscanPairXY);
    connect(m_bscanGlWidget, &bscanGlWidget::sendLoadMapBscanPairXY,          m_ascanDataWidget, &ascanDataWidget::receiveLoadMapBscanPairXY);

    connect(m_bscanGlWidget, &bscanGlWidget::signalHideTableBscanLogAutoData, m_ascanDataWidget, &ascanDataWidget::slotHideTableBscanLogAutoData);

    connect(m_bscanGlWidget, &bscanGlWidget::signalFixPixels,        this,       &bscan2dWidget::slotFixPixels);
    connect( this,           &bscan2dWidget::signalSetupPixels, m_bscanGlWidget, &bscanGlWidget::slotSetupPixels);

    connect(m_bscanGlWidget, &bscanGlWidget::signalFixBscanInfo,              this, &bscan2dWidget::slotFixBscanInfo);
    connect( this,          &bscan2dWidget::signalSendOutBscanInfo, m_bscanGlWidget, &bscanGlWidget::slotSendOutBscanInfo);

    connect(m_bscanGlWidget, &bscanGlWidget::sendOutBscanInfo, this, &bscan2dWidget::receiveOutBscanInfo);

    connect(m_ascanDataWidget->m_ascanWidget, &ascanWidget::signalFixSection,      this, &bscan2dWidget::slotFixBscanSection);
    connect(m_ascanDataWidget,                &ascanDataWidget::signalFixPageData, this, &bscan2dWidget::slotFixPageData);

    connect(m_accomplishment, &Accomplishment::signalActivateAccompThread, this, &bscan2dWidget::slotActivateAccompThread);

    connect(m_bscanGlWidget, &bscanGlWidget::signalCreateMapBscanTypeBscanSelectionInfo, this, &bscan2dWidget::slotCreateMapBscanTypeBscanSelectionInfo);

    connect(m_bscanGlWidget, &bscanGlWidget::signalFixDataBscanWidgets, this, &bscan2dWidget::slotFixDataBscanWidgets);

    connect(m_bscanGlWidget, &bscanGlWidget::signalSetupScrollHData, this, &bscan2dWidget::slotSetupScrollHData);
    connect(m_bscanGlWidget, &bscanGlWidget::signalSetupScrollVData, this, &bscan2dWidget::slotSetupScrollVData);

    connect(m_scrollHBar, &QScrollBar::valueChanged, m_bscanGlWidget, &bscanGlWidget::slotChangePixelsFnX);
    connect(m_scrollVBar, &QScrollBar::valueChanged, m_bscanGlWidget, &bscanGlWidget::slotChangePixelsFnY);

    setEnabledOpenGlControls(false);
}

bscan2dWidget::~bscan2dWidget(){}

void bscan2dWidget::receiveOutBscanInfo(QString outBscanInfo)
{
    emit sendOutBscanInfo(outBscanInfo);
}

void bscan2dWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_highBrightPixelSlider->setRange(m_lowBrightPixelSlider->value(), m_bscanGlWidget->m_vectorTuplesColors.size()-1);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(lowPixelLevel));
    m_bscanGlWidget->setLowPixelLevel(lowPixelLevel);
}

void bscan2dWidget::setHighPixelLevel(int highPixelLevel)
{
    m_lowBrightPixelSlider->setRange(0, m_highBrightPixelSlider->value());
    m_highBrightLabel->setText("Яркость высокая "+QString::number(highPixelLevel));
    m_bscanGlWidget->setHighPixelLevel(highPixelLevel);
}

void bscan2dWidget::outputNewImage(const std::string& fileName, int materialId, int filterId, int selectionId)
{
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));
    m_highBrightPixelSlider->setValue(m_bscanGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_bscanGlWidget->m_vectorTuplesColors.size()-1));

    m_ascanDataWidget->m_materialId  = materialId;
    m_ascanDataWidget->m_filterId    = filterId;
    m_ascanDataWidget->m_selectionId = selectionId;

    if (fileName != "" && m_accomplishment->m_thread->m_bscansInfoDataMap[fileName].quantImpulsesOfPacket != 0)
    {
        m_bscanGlWidget->m_filterId = filterId;
        m_bscanGlWidget->m_materialId = materialId;

        m_bscanGlWidget->m_lowPixelLevel  = 0;
        m_bscanGlWidget->m_highPixelLevel = m_bscanGlWidget->m_vectorTuplesColors.size()-1;

        if (m_bscanGlWidget->m_bscanName == "" || m_bscanGlWidget->m_bscanName == fileName)
        {
            m_contrastLog10Label->setText("Log10 Contrast "+QString::number(m_accomplishment->m_thread->m_bscansInfoDataMap[fileName].contrastLog10BscanKoeff));
            m_contrastLog10Slider->setValue(m_accomplishment->m_thread->m_bscansInfoDataMap[fileName].contrastLog10BscanKoeff*promilleParam);
            setContrastLog10Level(m_accomplishment->m_thread->m_bscansInfoDataMap[fileName].contrastLog10BscanKoeff*promilleParam);
        } 

        bscanInput(fileName, m_bscanGlWidget->m_absPixelsInX,  m_bscanGlWidget->m_absPixelsFnX);
    }
    else
    {

        m_bscanGlWidget->m_pixelsWidth  = 0;
        m_bscanGlWidget->m_pixelsHeight = 0;
        m_bscanGlWidget->m_appearingBscan = false;

        setEnabledOpenGlControls(false);

        m_ascanDataWidget->trackElementsVisible(false);
        m_ascanDataWidget->setbscanName("");

        m_bscanGlWidget->update();

        m_ascanDataWidget->m_ascanWidget->removeSection();
        m_ascanDataWidget->m_tableAscanDataWidget    ->removeAscanDataTable();
        m_ascanDataWidget->m_tableBscanLogAutoDataWidget  ->removeLogAutoDataTable();
    }
}

void bscan2dWidget::setShowLogBscan(bool showLogBscan)
{
    m_bscanGlWidget->setShowLogBscan(showLogBscan);
    m_ascanDataWidget->m_ascanWidget->setShowLog(showLogBscan);
    m_ascanDataWidget->m_tableAscanDataWidget->setShowLog(showLogBscan);
}

void bscan2dWidget::setLogBscanMouseSelection(bool logBscanMouseSelection)
{
    m_bscanGlWidget->setLogBscanMouseSelection(logBscanMouseSelection);
}

void bscan2dWidget::settingsBscanSelection(bool logBscanAutoSelection, bool clearSelectBscanPoints, std::string bscanName)
{
    m_bscanGlWidget->settingsBscanSelection(logBscanAutoSelection, clearSelectBscanPoints, bscanName);
}

void bscan2dWidget::setShowInitBscanSelection(bool showInitBscanSelection)
{
    m_bscanGlWidget->setShowInitBscanSelection(showInitBscanSelection);
}

void bscan2dWidget::resetBscanSelection(bool clearSelectBscanPoints, std::string bscanName)
{
    m_bscanGlWidget->resetBscanSelection(clearSelectBscanPoints, bscanName);
}

void bscan2dWidget::saveBscanAutoSelectionData(QString saveBscanAutoSelectionDataFile)
{
    m_ascanDataWidget->saveBscanAutoSelectionData(saveBscanAutoSelectionDataFile);
}

void bscan2dWidget::loadBscanAutoSelectionData(QString loadBscanAutoSelectionDataFile, std::string bscanName)
{
    m_bscanGlWidget->loadBscanAutoSelectionData(loadBscanAutoSelectionDataFile, bscanName);
}

void bscan2dWidget::setupTypeScaling(int typeScaling)
{
    m_bscanGlWidget->setupTypeScaling(typeScaling);
}

void bscan2dWidget::setEnabledOpenGlControls(boolean enabled)
{
    m_contrastLog10Label    ->setEnabled(enabled);
    m_contrastLog10Slider   ->setEnabled(enabled);
    m_lowBrightLabel        ->setEnabled(enabled);
    m_lowBrightPixelSlider  ->setEnabled(enabled);
    m_highBrightLabel       ->setEnabled(enabled);
    m_highBrightPixelSlider ->setEnabled(enabled);
}

void bscan2dWidget::setupInitPixelSlidersData()
{
    m_bscanGlWidget->m_lowPixelLevel  = 0;
    m_bscanGlWidget->m_highPixelLevel = m_bscanGlWidget->m_vectorTuplesColors.size()-1;
    m_lowBrightPixelSlider->setValue(0);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));
    m_highBrightPixelSlider->setValue(m_bscanGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_bscanGlWidget->m_vectorTuplesColors.size()-1));
}

void bscan2dWidget::setupOpenglControls()
{
    if (m_lowBrightLabel->isEnabled() == false)
    {
        setEnabledOpenGlControls(true);
        setupInitPixelSlidersData();
    }
}

void bscan2dWidget::setupColorsBscanAllProps(const QColor& colorUp, const QColor& colorLow)
{
    m_bscanGlWidget->setupColorsData(colorUp,  colorLow);
    m_lowBrightPixelSlider->setRange(0, m_bscanGlWidget->m_vectorTuplesColors.size()-1);
    m_highBrightPixelSlider->setRange(0, m_bscanGlWidget->m_vectorTuplesColors.size()-1);
    setupInitPixelSlidersData();
    emit signalSetupPixels(m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanGlWidget->m_bscanName]);
}

void bscan2dWidget::setupTrzBscans(const QStringList& filesbscanNamesTrz, std::vector<int> trzNumAntennasVector,  std::vector<std::pair<std::string, std::string>>& bscansNamesVectorPairs)
{
    m_accomplishment->m_thread->m_executeParserData = false;

    if (filesbscanNamesTrz.size() > 0)
    {
        m_accomplishment->m_thread->m_trzDataSampling = true;
        m_accomplishment->m_thread->m_csvDataSampling = false;
        m_accomplishment->m_thread->m_hdf5DataSampling = false;
        m_accomplishment->m_thread->m_trzNumAntennasVector.swap(trzNumAntennasVector);

        std::vector<std::string> fullFileNameTrzVector;
        std::vector<std::string> fileNameTrzVector;

        for (int count = 0; count <= filesbscanNamesTrz.size()-1; count++)
        {
            fullFileNameTrzVector.push_back(filesbscanNamesTrz.at(count).toStdString());
            fileNameTrzVector.push_back(
                (QString::fromStdString(filesbscanNamesTrz.at(count).toStdString())).split("/").at((QString::fromStdString(filesbscanNamesTrz.at(count).toStdString())).split("/").size()-1).toStdString()
            );
        }

        m_accomplishment->openFilesTrz(fullFileNameTrzVector);
        m_accomplishment->createBscansDataTrz(bscansNamesVectorPairs, fullFileNameTrzVector, fileNameTrzVector);
    }
}

void bscan2dWidget::setupCsvBscans(const QStringList& filesbscanNamesCsv, std::vector<std::pair<std::string, std::string>>& bscansNamesVectorPairs)
{
    if (filesbscanNamesCsv.size() > 0)
    {
        QStringList filesbscanNamesPropsCsv;
        QStringList filesbscanNamesDataCsv;

        createCsvBscansFiles(filesbscanNamesCsv, filesbscanNamesPropsCsv, filesbscanNamesDataCsv);

        if ( (filesbscanNamesPropsCsv.size() > 0) && (filesbscanNamesDataCsv.size() > 0) &&  (filesbscanNamesPropsCsv.size() == filesbscanNamesDataCsv.size()) )
        {
            m_accomplishment->m_thread->m_trzDataSampling = false;
            m_accomplishment->m_thread->m_csvDataSampling = true;
            m_accomplishment->m_thread->m_hdf5DataSampling = false;

            std::vector<std::string> fullFileNamePropsCsvVector;
            std::vector<std::string> fullFileNameDataCsvVector ;
            std::vector<std::string> fileNameDataCsvVector     ;

            for (int count = 0; count <= filesbscanNamesPropsCsv.size()-1; count++)
            {
                fullFileNamePropsCsvVector.push_back(filesbscanNamesPropsCsv.at(count).toStdString());
                fullFileNameDataCsvVector.push_back(filesbscanNamesDataCsv.at(count).toStdString());
                fileNameDataCsvVector.push_back((QString::fromStdString(filesbscanNamesDataCsv.at(count).toStdString())).split("/").at((QString::fromStdString(filesbscanNamesDataCsv.at(count).toStdString())).split("/").size()-1).toStdString());
            }

            m_accomplishment->openFilesCsv(fullFileNamePropsCsvVector, fullFileNameDataCsvVector);
            m_accomplishment->createBscansDataCsv(bscansNamesVectorPairs, fileNameDataCsvVector);
        }
    }
}

void bscan2dWidget::setupHdf5Bscans(
    const QStringList& infoBscansHdf5Names, std::vector<std::pair<std::string, std::string>>& bscansNamesVectorPairs
)
{
    std::vector<std::pair<std::string, std::vector<std::string>>> filesbscanNamesHdf5VectorPair;

    std::pair<std::string, std::vector<std::string>> pairHdf5BscanFiles;
    pairHdf5BscanFiles.first = "";
    pairHdf5BscanFiles.second.resize(0);

    for (int count = 0; count < infoBscansHdf5Names.count(); count++)
    {
        QString paperbscanName = "";

        if ((infoBscansHdf5Names.at(count).split(".out")).size() > 1 || (infoBscansHdf5Names.at(count).split(".hdf5")).size() > 1)
        {
            paperbscanName = "";
            for (int count1 = 0; count1 <= (infoBscansHdf5Names.at(count).split("/")).size()-2; count1++)  paperbscanName += (infoBscansHdf5Names.at(count).split("/")).at(count1);

            if (pairHdf5BscanFiles.first == ""  )  pairHdf5BscanFiles.first = paperbscanName.toStdString();

            if (pairHdf5BscanFiles.first != "" && pairHdf5BscanFiles.first != paperbscanName.toStdString())
            {
                filesbscanNamesHdf5VectorPair.push_back(pairHdf5BscanFiles);
                pairHdf5BscanFiles.first = paperbscanName.toStdString();
                pairHdf5BscanFiles.second.resize(0);
            }

            pairHdf5BscanFiles.second.push_back(infoBscansHdf5Names.at(count).toStdString());

            if (count == infoBscansHdf5Names.count()-1)
            {
                filesbscanNamesHdf5VectorPair.push_back(pairHdf5BscanFiles);
                pairHdf5BscanFiles.first = "";
                pairHdf5BscanFiles.second.resize(0);
            }
        }
        else
        {
            paperbscanName = infoBscansHdf5Names.at(count);

            QDir currentFolder(paperbscanName);
            currentFolder.setFilter(QDir::Dirs | QDir::Files);
            currentFolder.setSorting(QDir::Name);

            QFileInfoList folderItems(currentFolder.entryInfoList());

            pairHdf5BscanFiles.first = paperbscanName.toStdString();
            pairHdf5BscanFiles.second.resize(0);

            for (int count1 = 0; count1 <= folderItems.size()-1; count1++)   createPairHdf5BscanFiles(folderItems, paperbscanName, count1, pairHdf5BscanFiles);
            filesbscanNamesHdf5VectorPair.push_back(pairHdf5BscanFiles);
        }
    }

    if (filesbscanNamesHdf5VectorPair.size() > 0)
    {
        m_accomplishment->m_thread->m_executeParserData = false;

        m_accomplishment->m_thread->m_trzDataSampling  = false;
        m_accomplishment->m_thread->m_csvDataSampling  = false;
        m_accomplishment->m_thread->m_hdf5DataSampling = true;

        std::vector<std::string>              fullFileNameHdf5Vector   ;
        std::vector<std::vector<std::string>> filesbscanNamesHdf5Vectors ;

        for (int count = 0; count <= filesbscanNamesHdf5VectorPair.size()-1; count++)
        {
            fullFileNameHdf5Vector.push_back(filesbscanNamesHdf5VectorPair[count].first);
            filesbscanNamesHdf5Vectors.push_back(filesbscanNamesHdf5VectorPair[count].second);
        }

        m_accomplishment->createBscansDataHdf5(bscansNamesVectorPairs, fullFileNameHdf5Vector, filesbscanNamesHdf5Vectors);
    }
}

void bscan2dWidget::showNewBscan(const std::string& bscanName)
{
    setupOpenglControls();
    bscanInput(bscanName,  m_bscanGlWidget->m_absPixelsInX,  m_bscanGlWidget->m_absPixelsFnX);
    m_contrastLog10Label->setText("Log10 Contrast "+QString::number(m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName].contrastLog10BscanKoeff));
    m_contrastLog10Slider->setValue(m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName].contrastLog10BscanKoeff);
    setContrastLog10Level(m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName].contrastLog10BscanKoeff);
}

void bscan2dWidget::slotActivateAccompThread( std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs, BscanFileFormat bscanFileFormat)
{
    accomplishmentThreadTerminate();
    emit signalActivateAccompThread(bscansNamesVectorPairs, bscanFileFormat);
}

void bscan2dWidget::slotClearBscansInfoDataMap()
{
    m_accomplishment->m_thread->m_bscansInfoDataMap.clear();
}

void bscan2dWidget::slotEraseBscansInfoDataMap(const std::string& bscanName)
{
    m_accomplishment->m_thread->m_bscansInfoDataMap.erase(bscanName);
}

void bscan2dWidget::slotFixPixels(const std::string& bscanName)
{
    emit signalSetupPixels(m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName]);
}

void bscan2dWidget::slotFixBscanInfo(const std::string& bscanName)
{
    emit signalSendOutBscanInfo(m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName]);
}

void bscan2dWidget::slotFixBscanTrackElements(const std::string& bscanName)
{
    m_ascanDataWidget->setupTrackElements(m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName]);
}

void bscan2dWidget::slotFixBscanSection(const std::string& bscanName)
{
    m_ascanDataWidget->m_ascanWidget->createSection(m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName]);
}

void bscan2dWidget::slotFixPageData(const std::string& bscanName)
{
    m_ascanDataWidget->outputPageData(
        m_bscanGlWidget->m_pixelsInX, m_bscanGlWidget->m_pixelsInY, m_bscanGlWidget->m_pixelsFnX, m_bscanGlWidget->m_pixelsFnY,
        m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName]
    );
}

void bscan2dWidget::bscanInput(const std::string& bscanName, int absBscanPixelsInX, int absBscanPixelsFnX)
{
    m_ascanDataWidget->setbscanName(bscanName);
    m_bscanGlWidget->setupProperties(bscanName, m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName], absBscanPixelsInX, absBscanPixelsFnX);

    if (m_ascanDataWidget->m_trackSlider->isHidden() == true && m_ascanDataWidget->m_pageIndex <= 1) m_ascanDataWidget->trackElementsVisible(true);

    m_ascanDataWidget->m_ascanWidget->setTitle("сечение по bscan");
    m_ascanDataWidget->m_ascanWidget->outputSectionChart(QString::fromStdString(bscanName));
    m_ascanDataWidget->m_ascanWidget->m_filterId = m_bscanGlWidget->m_filterId;
    m_ascanDataWidget->m_tableAscanDataWidget->m_filterId = m_bscanGlWidget->m_filterId;

    m_ascanDataWidget->setupTrackSliderData(m_bscanGlWidget->m_pixelsInX, m_bscanGlWidget->m_pixelsFnX);

    m_ascanDataWidget->activateWidgets(
        bscanName, m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName],
        m_bscanGlWidget->m_pixelsInX, m_bscanGlWidget->m_pixelsInY, m_bscanGlWidget->m_pixelsFnX, m_bscanGlWidget->m_pixelsFnY
    );
}

void bscan2dWidget::slotFixDataBscanWidgets(std::string bscanName)
{
    m_ascanDataWidget->setupTrackSliderData(m_bscanGlWidget->m_pixelsInX, m_bscanGlWidget->m_pixelsFnX);
    m_ascanDataWidget->activateWidgets(
        bscanName, m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName],
        m_bscanGlWidget->m_pixelsInX, m_bscanGlWidget->m_pixelsInY, m_bscanGlWidget->m_pixelsFnX, m_bscanGlWidget->m_pixelsFnY
    );
}

void bscan2dWidget::slotCreateMapBscanTypeBscanSelectionInfo(
    int selectionId, int bscanHeight, std::string bscanName, const std::vector<QPair<int, int> >& vectorBscanPairXY
)
{
    std::map<int, std::vector<QPair<int, int> > >  mapBscanPairXY;
    mapBscanPairXY.clear();
    createMapBscanPairXY(vectorBscanPairXY, bscanHeight, m_bscanGlWidget->m_absPixelsInX, m_bscanGlWidget->m_absPixelsFnX, mapBscanPairXY);
    createMapBscanTypeBscanSelectionInfo(
        bscanHeight, m_bscanGlWidget->m_absPixelsInX, m_bscanGlWidget->m_absPixelsFnX, m_bscanGlWidget->m_materialId, m_bscanGlWidget->m_filterId,
        selectionId,  mapBscanPairXY, m_accomplishment->m_thread->m_bscansInfoDataMap[bscanName]
    );
}

void bscan2dWidget::slotSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData)
{
    m_scrollHBar->setRange(minScrollHData, maxScrollHData);
    m_scrollHBar->setValue(scrollHData);
}

void bscan2dWidget::slotSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData)
{
    m_scrollVBar->setRange(minScrollVData, maxScrollVData);
    m_scrollVBar->setValue(scrollVData);
}

void bscan2dWidget::setContrastLog10Level(int contrastBscanLevel)
{
    m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanGlWidget->m_bscanName].contrastLog10BscanKoeff = contrastBscanLevel/(double)promilleParam;
    m_contrastLog10Label->setText("Log10 Contrast‚ "+QString::number(contrastBscanLevel/promilleParam));

    executeTypeBscanSelectionInfo(
        m_bscanGlWidget->m_materialId, m_bscanGlWidget->m_filterId, 2, m_bscanGlWidget->m_height, m_bscanGlWidget->m_absPixelsInX, m_bscanGlWidget->m_absPixelsFnX,
        m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanGlWidget->m_bscanName]
    );

    m_ascanDataWidget->activateWidgets(
        m_bscanGlWidget->m_bscanName, m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanGlWidget->m_bscanName],
        m_bscanGlWidget->m_pixelsInX, m_bscanGlWidget->m_pixelsInY, m_bscanGlWidget->m_pixelsFnX, m_bscanGlWidget->m_pixelsFnY
    );
}

void bscan2dWidget::accomplishmentThreadTerminate()
{
    m_accomplishment->m_thread->terminate();
    m_accomplishment->m_thread->wait();
}

void bscan2dWidget::saveBscan()
{
    QImage img = m_bscanGlWidget->grabFrameBuffer();
    QString filebscanName = QFileDialog::getSaveFileName(
        this,
        "Save as...",
        "name",
        "BMP (*.bmp);; CUR (*.cur);; ICNS (*.icns);; ICO (*.ico);; JPEG (*.jpeg);; JPG (*.jpg);; PBM (*.pbm);; PGM (*.pgm);; PNG (*.png);; PPM (*.ppm);; TIF (*.tif);; TIFF (*.tiff);; WBMP (*.wbmp);; WEBP (*.webp);; XBM (*.xbm);; XPM (*.xpm);;"
    );
    img.save(filebscanName);
}

void bscan2dWidget::changeBscanImage(int absPixelsInX, int absPixelsFnX)
{
    slotSetupScrollHData(0, 0, 0);
    slotSetupScrollVData(0, 0, 0);
    executeTypeBscanSelectionInfo(
        m_bscanGlWidget->m_materialId, m_bscanGlWidget->m_filterId, 2, m_bscanGlWidget->m_height, absPixelsInX, absPixelsFnX,
        m_accomplishment->m_thread->m_bscansInfoDataMap[m_bscanGlWidget->m_bscanName]
    );
    bscanInput(m_bscanGlWidget->m_bscanName, absPixelsInX, absPixelsFnX);
}
