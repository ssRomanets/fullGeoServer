#include <QtWidgets>

#include "rdg2dWidget.h"
#include "rdgGlWidget.h"
#include "transformation.h"
#include <iostream>

rdg2dWidget::rdg2dWidget(QWidget* parent): QWidget(parent)
{
    m_accomplishment = new Accomplishment();
    m_rdgGlWidget = new rdgGlWidget();

    m_scrollHRdgBar = new QScrollBar(Qt::Horizontal);
    m_scrollHRdgBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollHRdgData(0, 0, 0);

    m_scrollVRdgBar = new QScrollBar(Qt::Vertical);
    m_scrollVRdgBar->setFocusPolicy(Qt::StrongFocus);
    slotSetupScrollVRdgData(0, 0, 0);

    m_leftVLayout = new QVBoxLayout;
    m_leftVLayout->addWidget(m_rdgGlWidget);
    m_leftVLayout->addWidget(m_scrollHRdgBar);

    m_leftHLayout = new QHBoxLayout;
    m_leftHLayout->addLayout(m_leftVLayout);
    m_leftHLayout->addWidget(m_scrollVRdgBar);

    m_leftAllVLayout = new QVBoxLayout;
    m_leftAllVLayout->addLayout(m_leftHLayout);

    m_lowBrightLabel = new QLabel();
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));

    m_lowBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_lowBrightPixelSlider->setRange(0, m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1);
    m_lowBrightPixelSlider->setSingleStep(1);
    m_lowBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_highBrightLabel = new QLabel();
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1));

    m_highBrightPixelSlider = new QSlider(Qt::Horizontal);
    m_highBrightPixelSlider->setRange(0, m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1);
    m_highBrightPixelSlider->setSingleStep(1);
    m_highBrightPixelSlider->setTickPosition(QSlider::TicksRight);

    m_contrastLog10Label = new QLabel();
    m_contrastLog10Label->setText("Log10 Контраст "+QString::number(0));

    m_contrastLog10Slider = new QSlider(Qt::Horizontal);
    m_contrastLog10Slider->setRange(0, 100);
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

    m_dataRdgWidget  = new dataRdgWidget(parent);

    m_rightLayout = new QVBoxLayout;
    m_rightLayout->addWidget(m_dataRdgWidget);

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->addLayout(m_leftAllVLayout);
    m_mainLayout->addLayout(m_rightLayout);
    setLayout(m_mainLayout);

    connect(m_lowBrightPixelSlider,   &QSlider::valueChanged, this, &rdg2dWidget::setLowPixelLevel);
    connect(m_highBrightPixelSlider,  &QSlider::valueChanged, this, &rdg2dWidget::setHighPixelLevel);
    connect(m_contrastLog10Slider,    &QSlider::valueChanged, this, &rdg2dWidget::setContrastLog10Level);

    connect(m_dataRdgWidget->m_sectionRdgWidget,                       &sectionRdgWidget::fixTrackIndex, m_rdgGlWidget, &rdgGlWidget::remarkTrackRdg);
    connect(m_dataRdgWidget->m_tableRdgTrackDataWidget,         &tableRdgTrackDataWidget::fixTrackIndex, m_rdgGlWidget, &rdgGlWidget::remarkTrackRdg);

    connect(m_dataRdgWidget->m_sectionDeepRdgWidget,               &sectionDeepRdgWidget::fixTrackIndex, m_rdgGlWidget, &rdgGlWidget::remarkTrackRdg);
    connect(m_dataRdgWidget->m_tableDeepRdgTrackDataWidget, &tableDeepRdgTrackDataWidget::fixTrackIndex, m_rdgGlWidget, &rdgGlWidget::remarkTrackRdg);

    connect(m_rdgGlWidget, &rdgGlWidget::sendVectorRdgPairXY,           m_dataRdgWidget, &dataRdgWidget::receiveVectorRdgPairXY);
    connect(m_rdgGlWidget, &rdgGlWidget::sendLoadMapRdgPairXY,          m_dataRdgWidget, &dataRdgWidget::receiveLoadMapRdgPairXY);

    connect(m_rdgGlWidget, &rdgGlWidget::signalHideTableRdgLogAutoData, m_dataRdgWidget, &dataRdgWidget::slotHideTableRdgLogAutoData);

    connect(m_rdgGlWidget, &rdgGlWidget::signalFixRdgPixels,    this,         &rdg2dWidget::slotFixRdgPixels);
    connect( this,         &rdg2dWidget::signalSetupRdgPixels, m_rdgGlWidget, &rdgGlWidget::slotSetupRdgPixels);

    connect(m_rdgGlWidget, &rdgGlWidget::signalFixRdgInfo,              this, &rdg2dWidget::slotFixRdgInfo);
    connect( this,         &rdg2dWidget::signalSendOutRdgInfo, m_rdgGlWidget, &rdgGlWidget::slotSendOutRdgInfo);

    connect(m_rdgGlWidget, &rdgGlWidget::sendOutRdgInfo, this, &rdg2dWidget::receiveOutRdgInfo);

    connect(m_dataRdgWidget->m_sectionRdgWidget, &sectionRdgWidget::signalFixRdgSection, this, &rdg2dWidget::slotFixRdgSection);
    connect(m_dataRdgWidget, &dataRdgWidget::signalFixPageRdgData, this, &rdg2dWidget::slotFixPageRdgData);

    connect(m_accomplishment, &Accomplishment::signalActivateAccompThread, this, &rdg2dWidget::slotActivateAccompThread);

    connect(m_rdgGlWidget, &rdgGlWidget::signalCreateMapRdgTypeRdgSelectionInfo, this, &rdg2dWidget::slotCreateMapRdgTypeRdgSelectionInfo);

    connect(m_rdgGlWidget, &rdgGlWidget::signalFixDataRdgWidgets, this, &rdg2dWidget::slotFixDataRdgWidgets);

    connect(m_rdgGlWidget, &rdgGlWidget::signalSetupScrollHRdgData, this, &rdg2dWidget::slotSetupScrollHRdgData);
    connect(m_rdgGlWidget, &rdgGlWidget::signalSetupScrollVRdgData, this, &rdg2dWidget::slotSetupScrollVRdgData);

    connect(m_scrollHRdgBar, &QScrollBar::valueChanged, m_rdgGlWidget, &rdgGlWidget::slotChangeRdgPixelsFnX);
    connect(m_scrollVRdgBar, &QScrollBar::valueChanged, m_rdgGlWidget, &rdgGlWidget::slotChangeRdgPixelsFnY);

    setEnabledOpenGlControls(false);
}

rdg2dWidget::~rdg2dWidget(){}

void rdg2dWidget::receiveOutRdgInfo(QString outRdgInfo)
{
    emit sendOutRdgInfo(outRdgInfo);
}

void rdg2dWidget::setLowPixelLevel(int lowPixelLevel)
{
    m_highBrightPixelSlider->setRange(m_lowBrightPixelSlider->value(), m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(lowPixelLevel));
    m_rdgGlWidget->setLowPixelLevel(lowPixelLevel);
}

void rdg2dWidget::setHighPixelLevel(int highPixelLevel)
{
    m_lowBrightPixelSlider->setRange(0, m_highBrightPixelSlider->value());
    m_highBrightLabel->setText("Яркость высокая "+QString::number(highPixelLevel));
    m_rdgGlWidget->setHighPixelLevel(highPixelLevel);
}

void rdg2dWidget::outputNewImage(const std::string& fileName, int materialId, int filterId, int selectionId)
{
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));
    m_highBrightPixelSlider->setValue(m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1);
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1));

    m_dataRdgWidget->m_materialId  = materialId;
    m_dataRdgWidget->m_filterId    = filterId;
    m_dataRdgWidget->m_selectionId = selectionId;

    if (fileName != "" && m_accomplishment->m_thread->m_rdgsInfoDataMap[fileName].quantImpulsesOfPacket != 0)
    {
        m_rdgGlWidget->m_filterId = filterId;
        m_rdgGlWidget->m_materialId = materialId;

        m_rdgGlWidget->m_lowPixelLevel  = 0;
        m_rdgGlWidget->m_highPixelLevel = m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1;

        m_contrastLog10Label->setText("Log10 Контраст "+QString::number(m_accomplishment->m_thread->m_rdgsInfoDataMap[fileName].contrastLog10RdgKoeff));
        m_contrastLog10Slider->setValue(m_accomplishment->m_thread->m_rdgsInfoDataMap[fileName].contrastLog10RdgKoeff*percentsParam);
        setContrastLog10Level(m_accomplishment->m_thread->m_rdgsInfoDataMap[fileName].contrastLog10RdgKoeff*percentsParam);

        rdgInput(fileName, 0, 0);
    }
    else
    {
        m_dataRdgWidget->m_sectionRdgWidget->removeRdgSection();
        m_dataRdgWidget->m_sectionDeepRdgWidget->removeDeepRdgSection();

        m_rdgGlWidget->m_rdgPixelsWidth  = 0;
        m_rdgGlWidget->m_rdgPixelsHeight = 0;

        m_rdgGlWidget->setAppearingRdg(false);
        setEnabledOpenGlControls(false);

        m_dataRdgWidget->trackElementsVisible(false);
        m_dataRdgWidget->setRdgName("");

        m_rdgGlWidget->update();

        m_dataRdgWidget->m_tableRdgTrackDataWidget    ->removeRdgTrackDataTable();
        m_dataRdgWidget->m_tableDeepRdgTrackDataWidget->removeDeepRdgTrackDataTable();
        m_dataRdgWidget->m_tableRdgLogAutoDataWidget  ->removeRdgLogAutoDataTable();
    }
}

void rdg2dWidget::setShowLogRdg(bool showLogRdg)
{
    m_rdgGlWidget->setShowLogRdg(showLogRdg);
    m_dataRdgWidget->m_sectionRdgWidget->setShowLogRdg(showLogRdg);
    m_dataRdgWidget->m_tableRdgTrackDataWidget->setShowLogRdg(showLogRdg);
}

void rdg2dWidget::setLogRdgMouseSelection(bool logRdgMouseSelection)
{
    m_rdgGlWidget->setLogRdgMouseSelection(logRdgMouseSelection);
}

void rdg2dWidget::settingsRdgSelection(bool logRdgAutoSelection, bool clearSelectRdgPoints, std::string rdgName)
{
    m_rdgGlWidget->settingsRdgSelection(logRdgAutoSelection, clearSelectRdgPoints, rdgName);
}

void rdg2dWidget::setShowInitRdgSelection(bool showInitRdgSelection)
{
    m_rdgGlWidget->setShowInitRdgSelection(showInitRdgSelection);
}

void rdg2dWidget::resetRdgSelection(bool clearSelectRdgPoints, std::string rdgName)
{
    m_rdgGlWidget->resetRdgSelection(clearSelectRdgPoints, rdgName);
}

void rdg2dWidget::saveRdgAutoSelectionData(QString saveRdgAutoSelectionDataFile)
{
    m_dataRdgWidget->saveRdgAutoSelectionData(saveRdgAutoSelectionDataFile);
}

void rdg2dWidget::loadRdgAutoSelectionData(QString loadRdgAutoSelectionDataFile, std::string rdgName)
{
    m_rdgGlWidget->loadRdgAutoSelectionData(loadRdgAutoSelectionDataFile, rdgName);
}

void rdg2dWidget::setupRdgScaling(int typeRdgScaling)
{
    m_rdgGlWidget->setupRdgScaling(typeRdgScaling);
}

void rdg2dWidget::setupInitPixelSlidersData()
{
    m_rdgGlWidget->m_lowPixelLevel  = 0;
    m_rdgGlWidget->m_highPixelLevel = m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1;
    m_lowBrightPixelSlider->setValue(0);
    m_lowBrightLabel->setText("Яркость низкая "+QString::number(0));
    m_highBrightPixelSlider->setValue(m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1);
    m_highBrightLabel->setText("Яркость высокая "+QString::number(m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1));
}

void rdg2dWidget::setupRdgOpenglControls()
{
    if (m_lowBrightLabel->isEnabled() == false)
    {
        setEnabledOpenGlControls(true);
        setupInitPixelSlidersData();
    }
}

void rdg2dWidget::setupColorsRdgAllProps(const QColor& colorUp, const QColor& colorLow)
{
    m_rdgGlWidget->setupColorsRdgData(colorUp,  colorLow);
    m_lowBrightPixelSlider->setRange(0, m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1);
    m_highBrightPixelSlider->setRange(0, m_rdgGlWidget->m_vectorTuplesColorsRdg.size()-1);
    setupInitPixelSlidersData();
    emit signalSetupRdgPixels(m_accomplishment->m_thread->m_rdgsInfoDataMap[m_rdgGlWidget->m_rdgName]);
}

void rdg2dWidget::setupTrzRdgs(const QStringList& filesRdgNamesTrz, std::vector<int> trzNumAntennasVector,  std::vector<std::pair<std::string, std::string>>& rdgsNamesVectorPairs)
{
    m_accomplishment->m_thread->m_executeParserData = false;

    if (filesRdgNamesTrz.size() > 0)
    {
        m_accomplishment->m_thread->m_trzDataSampling = true;
        m_accomplishment->m_thread->m_csvDataSampling = false;
        m_accomplishment->m_thread->m_hdf5DataSampling = false;
        m_accomplishment->m_thread->m_trzNumAntennasVector.swap(trzNumAntennasVector);

        std::vector<std::string> fullFileNameTrzVector;
        std::vector<std::string> fileNameTrzVector;

        for (int count = 0; count <= filesRdgNamesTrz.size()-1; count++)
        {
            fullFileNameTrzVector.push_back(filesRdgNamesTrz.at(count).toStdString());
            fileNameTrzVector.push_back((QString::fromStdString(filesRdgNamesTrz.at(count).toStdString())).split("/").at((QString::fromStdString(filesRdgNamesTrz.at(count).toStdString())).split("/").size()-1).toStdString());
        }

        m_accomplishment->openFilesTrz(fullFileNameTrzVector);
        m_accomplishment->createRdgsDataTrz(rdgsNamesVectorPairs, fullFileNameTrzVector, fileNameTrzVector);
    }
}

void rdg2dWidget::setupCsvRdgs(const QStringList& filesRdgNamesCsv, std::vector<std::pair<std::string, std::string>>& rdgsNamesVectorPairs)
{
    if (filesRdgNamesCsv.size() > 0)
    {
        QStringList filesRdgNamesPropsCsv;
        QStringList filesRdgNamesDataCsv;

        createCsvRdgsFiles(filesRdgNamesCsv, filesRdgNamesPropsCsv, filesRdgNamesDataCsv);

        if ( (filesRdgNamesPropsCsv.size() > 0) && (filesRdgNamesDataCsv.size() > 0) &&  (filesRdgNamesPropsCsv.size() == filesRdgNamesDataCsv.size()) )
        {
            m_accomplishment->m_thread->m_trzDataSampling = false;
            m_accomplishment->m_thread->m_csvDataSampling = true;
            m_accomplishment->m_thread->m_hdf5DataSampling = false;

            std::vector<std::string> fullFileNamePropsCsvVector;
            std::vector<std::string> fullFileNameDataCsvVector ;
            std::vector<std::string> fileNameDataCsvVector     ;

            for (int count = 0; count <= filesRdgNamesPropsCsv.size()-1; count++)
            {
                fullFileNamePropsCsvVector.push_back(filesRdgNamesPropsCsv.at(count).toStdString());
                fullFileNameDataCsvVector.push_back(filesRdgNamesDataCsv.at(count).toStdString());
                fileNameDataCsvVector.push_back((QString::fromStdString(filesRdgNamesDataCsv.at(count).toStdString())).split("/").at((QString::fromStdString(filesRdgNamesDataCsv.at(count).toStdString())).split("/").size()-1).toStdString());
            }

            m_accomplishment->openFilesCsv(fullFileNamePropsCsvVector, fullFileNameDataCsvVector);
            m_accomplishment->createRdgsDataCsv(rdgsNamesVectorPairs, fileNameDataCsvVector);
        }
    }
}

void rdg2dWidget::setupHdf5Rdgs(
    const QStringList& infoRdgsHdf5Names, std::vector<std::pair<std::string, std::string>>& rdgsNamesVectorPairs
)
{
    std::vector<std::pair<std::string, std::vector<std::string>>> filesRdgNamesHdf5VectorPair;

    std::pair<std::string, std::vector<std::string>> pairHdf5RdgFiles;
    pairHdf5RdgFiles.first = "";
    pairHdf5RdgFiles.second.resize(0);

    for (int count = 0; count < infoRdgsHdf5Names.count(); count++)
    {
        QString paperRdgName = "";

        if ((infoRdgsHdf5Names.at(count).split(".out")).size() > 1 || (infoRdgsHdf5Names.at(count).split(".hdf5")).size() > 1)
        {
            paperRdgName = "";
            for (int count1 = 0; count1 <= (infoRdgsHdf5Names.at(count).split("/")).size()-2; count1++)  paperRdgName += (infoRdgsHdf5Names.at(count).split("/")).at(count1);

            if (pairHdf5RdgFiles.first == ""  )  pairHdf5RdgFiles.first = paperRdgName.toStdString();

            if (pairHdf5RdgFiles.first != "" && pairHdf5RdgFiles.first != paperRdgName.toStdString())
            {
                filesRdgNamesHdf5VectorPair.push_back(pairHdf5RdgFiles);
                pairHdf5RdgFiles.first = paperRdgName.toStdString();
                pairHdf5RdgFiles.second.resize(0);
            }

            pairHdf5RdgFiles.second.push_back(infoRdgsHdf5Names.at(count).toStdString());

            if (count == infoRdgsHdf5Names.count()-1)
            {
                filesRdgNamesHdf5VectorPair.push_back(pairHdf5RdgFiles);
                pairHdf5RdgFiles.first = "";
                pairHdf5RdgFiles.second.resize(0);
            }
        }
        else
        {
            paperRdgName = infoRdgsHdf5Names.at(count);

            QDir currentFolder(paperRdgName);
            currentFolder.setFilter(QDir::Dirs | QDir::Files);
            currentFolder.setSorting(QDir::Name);

            QFileInfoList folderItems(currentFolder.entryInfoList());

            pairHdf5RdgFiles.first = paperRdgName.toStdString();
            pairHdf5RdgFiles.second.resize(0);

            for (int count1 = 0; count1 <= folderItems.size()-1; count1++)   createPairHdf5RdgFiles(folderItems, paperRdgName, count1, pairHdf5RdgFiles);
            filesRdgNamesHdf5VectorPair.push_back(pairHdf5RdgFiles);
        }
    }

    if (filesRdgNamesHdf5VectorPair.size() > 0)
    {
        m_accomplishment->m_thread->m_executeParserData = false;

        m_accomplishment->m_thread->m_trzDataSampling  = false;
        m_accomplishment->m_thread->m_csvDataSampling  = false;
        m_accomplishment->m_thread->m_hdf5DataSampling = true;

        std::vector<std::string>              fullFileNameHdf5Vector   ;
        std::vector<std::vector<std::string>> filesRdgNamesHdf5Vectors ;

        for (int count = 0; count <= filesRdgNamesHdf5VectorPair.size()-1; count++)
        {
            fullFileNameHdf5Vector.push_back(filesRdgNamesHdf5VectorPair[count].first);
            filesRdgNamesHdf5Vectors.push_back(filesRdgNamesHdf5VectorPair[count].second);
        }

        m_accomplishment->createRdgsDataHdf5(rdgsNamesVectorPairs, fullFileNameHdf5Vector, filesRdgNamesHdf5Vectors);
    }
}

void rdg2dWidget::showNewRdg(const std::string& rdgName)
{
    setupRdgOpenglControls();
    rdgInput(rdgName, 0, 0);
    m_contrastLog10Label->setText("Log10 Контраст "+QString::number(m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName].contrastLog10RdgKoeff));
    m_contrastLog10Slider->setValue(m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName].contrastLog10RdgKoeff);
    setContrastLog10Level(m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName].contrastLog10RdgKoeff);
}

void rdg2dWidget::slotActivateAccompThread( std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, RdgFileFormat rdgFileFormat)
{
    accomplishmentThreadTerminate();
    emit signalActivateAccompThread(rdgsNamesVectorPairs, rdgFileFormat);
}

void rdg2dWidget::slotClearRdgsInfoDataMap()
{
    m_accomplishment->m_thread->m_rdgsInfoDataMap.clear();
}

void rdg2dWidget::slotEraseRdgsInfoDataMap(const std::string& rdgName)
{
    m_accomplishment->m_thread->m_rdgsInfoDataMap.erase(rdgName);
}

void rdg2dWidget::slotFixRdgPixels(const std::string& rdgName)
{
    emit signalSetupRdgPixels(m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName]);
}

void rdg2dWidget::slotFixRdgInfo(const std::string& rdgName)
{
    emit signalSendOutRdgInfo(m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName]);
}

void rdg2dWidget::slotFixRdgTrackElements(const std::string& rdgName)
{
    m_dataRdgWidget->setupTrackElements(m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName]);
}

void rdg2dWidget::slotFixRdgSection(const std::string& rdgName)
{
    m_dataRdgWidget->m_sectionRdgWidget->createRdgSection(m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName]);
}

void rdg2dWidget::slotFixPageRdgData(const std::string& rdgName)
{
    m_dataRdgWidget->outputPageRdgData(
        m_rdgGlWidget->m_rdgPixelsInX, m_rdgGlWidget->m_rdgPixelsInY, m_rdgGlWidget->m_rdgPixelsFnX, m_rdgGlWidget->m_rdgPixelsFnY,
        m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName]
    );
}

void rdg2dWidget::rdgInput(const std::string& rdgName, int absRdgPixelsInX, int absRdgPixelsFnX)
{
    m_dataRdgWidget->setRdgName(rdgName);
    m_rdgGlWidget->setupRdgProperties(rdgName, m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName], absRdgPixelsInX, absRdgPixelsFnX);

    if (m_dataRdgWidget->m_trackRdgSlider->isHidden() == true && m_dataRdgWidget->m_pageIndex <= 1) m_dataRdgWidget->trackElementsVisible(true);

    m_dataRdgWidget->m_sectionRdgWidget->setTitle("сечение по rdg");
    m_dataRdgWidget->m_sectionRdgWidget->outputSectionChart(QString::fromStdString(rdgName));
    m_dataRdgWidget->m_sectionRdgWidget->m_filterId = m_rdgGlWidget->m_filterId;
    m_dataRdgWidget->m_sectionRdgWidget->m_materialId = m_rdgGlWidget->m_materialId;

    m_dataRdgWidget->m_tableRdgTrackDataWidget->m_filterId = m_rdgGlWidget->m_filterId;
    m_dataRdgWidget->m_tableRdgTrackDataWidget->m_materialId = m_rdgGlWidget->m_materialId;

    m_dataRdgWidget->m_sectionDeepRdgWidget->setTitle("глубина по rdg (m)");
    m_dataRdgWidget->m_sectionDeepRdgWidget->outputSectionChart(QString::fromStdString(rdgName));

    m_dataRdgWidget->setupTrackRdgSliderData(m_rdgGlWidget->m_rdgPixelsInX, m_rdgGlWidget->m_rdgPixelsFnX);

    m_dataRdgWidget->activateWidgets(
        rdgName, m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName],
        m_rdgGlWidget->m_rdgPixelsInX, m_rdgGlWidget->m_rdgPixelsInY, m_rdgGlWidget->m_rdgPixelsFnX, m_rdgGlWidget->m_rdgPixelsFnY
    );
}

void rdg2dWidget::slotFixDataRdgWidgets(std::string rdgName)
{
    m_dataRdgWidget->setupTrackRdgSliderData(m_rdgGlWidget->m_rdgPixelsInX, m_rdgGlWidget->m_rdgPixelsFnX);
    m_dataRdgWidget->activateWidgets(
        rdgName, m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName],
        m_rdgGlWidget->m_rdgPixelsInX, m_rdgGlWidget->m_rdgPixelsInY, m_rdgGlWidget->m_rdgPixelsFnX, m_rdgGlWidget->m_rdgPixelsFnY
    );
}

void rdg2dWidget::slotCreateMapRdgTypeRdgSelectionInfo(
    int selectionId, int rdgHeight, std::string rdgName, const std::vector<QPair<int, int> >& vectorRdgPairXY
)
{
    std::map<int, std::vector<QPair<int, int> > >  mapRdgPairXY;
    mapRdgPairXY.clear();
    createMapRdgPairXY(vectorRdgPairXY, rdgHeight, m_rdgGlWidget->m_absRdgPixelsInX, m_rdgGlWidget->m_absRdgPixelsFnX, mapRdgPairXY);
    createMapRdgTypeRdgSelectionInfo(
        rdgHeight, m_rdgGlWidget->m_absRdgPixelsInX, m_rdgGlWidget->m_absRdgPixelsFnX, m_rdgGlWidget->m_materialId, m_rdgGlWidget->m_filterId,
        selectionId,  mapRdgPairXY, m_accomplishment->m_thread->m_rdgsInfoDataMap[rdgName]
    );
}

void rdg2dWidget::setEnabledOpenGlControls(boolean enabled)
{
    m_contrastLog10Label    ->setEnabled(enabled);
    m_contrastLog10Slider   ->setEnabled(enabled);
    m_lowBrightLabel        ->setEnabled(enabled);
    m_lowBrightPixelSlider  ->setEnabled(enabled);
    m_highBrightLabel       ->setEnabled(enabled);
    m_highBrightPixelSlider ->setEnabled(enabled);
}

void rdg2dWidget::slotSetupScrollHRdgData(int minScrollHRdgData, int scrollHRdgData, int maxScrollHRdgData)
{
    m_scrollHRdgBar->setRange(minScrollHRdgData, maxScrollHRdgData);
    m_scrollHRdgBar->setValue(scrollHRdgData);
}

void rdg2dWidget::slotSetupScrollVRdgData(int minScrollVRdgData, int scrollVRdgData, int maxScrollVRdgData)
{
    m_scrollVRdgBar->setRange(minScrollVRdgData, maxScrollVRdgData);
    m_scrollVRdgBar->setValue(scrollVRdgData);
}

void rdg2dWidget::setContrastLog10Level(int contrastRdgLevel)
{
    m_accomplishment->m_thread->m_rdgsInfoDataMap[m_rdgGlWidget->m_rdgName].contrastLog10RdgKoeff = contrastRdgLevel/(double)percentsParam;
    m_contrastLog10Label->setText("Log10 Контраст "+QString::number(contrastRdgLevel/percentsParam));
    changeRdgRightDataAtShift();
}

void rdg2dWidget::accomplishmentThreadTerminate()
{
    m_accomplishment->m_thread->terminate();
    m_accomplishment->m_thread->wait();
}

void rdg2dWidget::saveRdg()
{
    QImage img = m_rdgGlWidget->grabFrameBuffer();
    QString fileRdgName = QFileDialog::getSaveFileName(
        this,
        "Save as...",
        "name",
        "BMP (*.bmp);; CUR (*.cur);; ICNS (*.icns);; ICO (*.ico);; JPEG (*.jpeg);; JPG (*.jpg);; PBM (*.pbm);; PGM (*.pgm);; PNG (*.png);; PPM (*.ppm);; TIF (*.tif);; TIFF (*.tiff);; WBMP (*.wbmp);; WEBP (*.webp);; XBM (*.xbm);; XPM (*.xpm);;"
    );
    img.save(fileRdgName);
}

void rdg2dWidget::changeRdgRightDataAtShift()
{
    executeTypeRdgSelectionInfo(
        m_rdgGlWidget->m_materialId, m_rdgGlWidget->m_filterId, 2, m_rdgGlWidget->m_rdgHeight, m_rdgGlWidget->m_absRdgPixelsInX, m_rdgGlWidget->m_absRdgPixelsFnX,
        m_accomplishment->m_thread->m_rdgsInfoDataMap[m_rdgGlWidget->m_rdgName]
    );

    m_dataRdgWidget->activateWidgets(
        m_rdgGlWidget->m_rdgName, m_accomplishment->m_thread->m_rdgsInfoDataMap[m_rdgGlWidget->m_rdgName],
        m_rdgGlWidget->m_rdgPixelsInX, m_rdgGlWidget->m_rdgPixelsInY, m_rdgGlWidget->m_rdgPixelsFnX, m_rdgGlWidget->m_rdgPixelsFnY
    );
}

void rdg2dWidget::changeRdgImage(int absRdgPixelsInX, int absRdgPixelsFnX)
{
    slotSetupScrollHRdgData(0, 0, 0);
    slotSetupScrollVRdgData(0, 0, 0);

    rdgInput(m_rdgGlWidget->m_rdgName, absRdgPixelsInX, absRdgPixelsFnX);
    changeRdgRightDataAtShift();
}
