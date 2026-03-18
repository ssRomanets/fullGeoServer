#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bscan2dWidget.h"
#include "bscanDeep2dWidget.h"
#include "bscans2dWidget.h"
#include "bscans3dWidget.h"
#include "bscanGlWidget.h"
#include "colorsPaletteDialog.h"

#include <QMainWindow>
#include <QWidget>

#include <QCloseEvent>
#include <QCoreApplication>
#include <QContextMenuEvent>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QStackedWidget>
#include <QComboBox>

#include <QSlider>
#include <QStatusBar>

#include <algorithm>
#include <iostream>

/**
 * @brief Главное окно
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupLastBscans();
    void scrollHBscansLengthBarResetDataAtBscansImages();
    void defAbsBscansStructInXAndFnX(int& absBscansStructInX, int& absBscansStructFnX);

public slots:
    /**
     * @brief Слот получения имени удаляемой радарограммы
    */
    void receiveDeleteBscanName(const std::string& deleteBscanName);
    /**
     * @brief Слот для прогрессбара по формированию данных из имеющихся trz файлов
    */
    void receiveProgressData(int progressPos, int progressMax);
    /**
     * @brief Слот говорящий о том, что данные по радарограммам сформированы
    */
    void receiveBscansWorkData();
    /**
     * @brief Слот говорящий о том, что рельеф связанный с радарограммами определен
    */
    void receiveBscansZData();

    void slotFixBscansSurfPixels();
    void slotFixDeleteBscanName(double fixLatitude, double fixLongitude);
    void slotFixFillBscans(bool resetCutPointsBscans);

    void slotFixBscansSurfSection();
    void slotActivateAccompThread(std::vector<std::pair<std::string, std::string>> BscansNamesVectorPairs, BscanFileFormat bscanFileFormat);

    void receiveColorsPalette(QColor& colorUp, QColor& colorLow);

    void slotSetupBscanDeepPixels();
    void slotFixDataBscanDeepWidgets();

protected:
    /**
     * @brief функция закрытия главного окна
    */
    void closeEvent(QCloseEvent *event) override;

private:
    /**
     * @brief функция установки начального состояния главного окна
    */
    void init();
    /**
     * @brief функция определения основных действий проекта
    */
    void createActions();

    /**
     * @brief функция показа радарограммы в OpenGl
    */
    void showBscanImage(bool changeBscan);

    /**
     * @brief функция создания менюшек действий по файлам
    */
    void createActionsFilesMenu();

    /**
     * @brief функция астивации/деактивации действий по разрезам трехмерных структур или профильным сечениям поверхностей
    */
    void enabledCutActions(bool enabled2d, bool enabled3d, bool enabledSL);
    /**
     * @brief функция астивации/деактивации действий по одномерной радарограмме
    */
    void enabledShowBscanActions(bool enabled);

    void bscanImpulsesSliderActivate();
    void setQuantImpulsesOfPacketSlider(int quantImpulsesOfPacket);
    void receiveOutBscanInfo(QString outBscanInfo);
    void receiveOutBscansSurfInfo(QString outBscansSurfInfo);

    void resetBscanSelection(bool clearSelectBscanPoints);
    void setupBscansSliderSetup(int m_bscanId, int bscansCount);

    void setupBscanCurrentName();
    void setupBscanControls();

    void setupScrollHBscansLengthBarData(int minScrollHBscansLengthBarData, int scrollHBscansLengthBarData, int maxScrollHBscansLengthBarData);

signals:
    void signalSetupBscanDeepPixels        (const st_bscanInfoData& bscanInfoData);
    void signalSetupFixDataBscanDeepWidgets(const st_bscanInfoData& bscanInfoData);

    void signalClearBscansInfoDataMap();
    void signalEraseBscansInfoDataMap(const std::string& fileName);
    void closeSection2dAction(bool section2dVisible);
    void closeCut3dAction(bool cut3dVisible);

    void signalSetupBscansSurfPixels   (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData);
    void signalOutputBscansSurfSection (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData& bscansWorkData);
    void signalDefDeleteBscanName      (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, double fixLatitude, double fixLongitude);
    void signalFillBscansExecute       (const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData, bool resetCutPointsBscans);

    void signalSetupBscanDeepPageData(const st_bscanInfoData& bscansInfoData);
    void signalFixDataBscanDeepWidgets(const st_bscanInfoData& bscansInfoData);

private slots:
    /**
     * @brief Слот открытия trz файлов по радарограмму
    */
    void openTrz();
    void openCsv();
    void openHdf5();

    /**
     * @brief Слот открытия trz файлов по радарограмму
    */
    void openKml();
    void openImageBscan();
    void defPaletteBscan();
    void defPaletteBscanDeep();

    void openImageDeepBscan();

    void open2dImageBscans();
    void defPaletteSurfBscans();

    void open3dImageBscans();
    void defPaletteTransBscans();
    void includeFilterOnBscan();

    void openRecentFile();

    void deleteSelectedFile();
    void deleteAllFiles();

    void save();

    void setCloseSection2dAction();
    void setCloseCut3dAction();

    void saveBoundPointsLineAction();
    void loadBoundPointsLineAction();

    void setInitBscan();
    void setLogBscan();

    void setLogBscanMouseSelection();
    void setLogBscanAutoSelection();
    void setShowInitBscanSelection();
    void slotResetBscanSelection();
    void saveBscanAutoSelectionData();
    void loadBscanAutoSelectionData();

    void changeHighLowOnBscans();
    void showFullBscansData();

    void materialIdChanged(int materialId);

    void useSelectionOnBscan();

    void setupScalingOnBscan();
    void setupScalingOnBscanDeep();
    void setupScalingOnBscansSurf();
    void setupScalingOnBscansTrans();

    void changeBscan(int BscanId);
    void leftChangeBscan();
    void rightChangeBscan();
    
    void scrollHBscansLengthBarPressed();
    void scrollHBscansLengthBarMoved(int scrollHBscansLengthBarPos);
    void scrollHBscansLengthBarReleased();

    void scrollHBscansLengthBarChanged(int scrollHBscansLengthBarPos);

    void slotSetupBscanDeepPageData(const std::string bscanName);

private:
    void deleteFile(const std::string& fileName);
    void outputBscansData(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, const st_bscansWorkData&  bscansWorkData,
        int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int selectionId
    );
    void closeInitBscanSelection();

    void impulsesBscanControlsVisible(bool visible);
    void bscansTransitControlsVisible(bool visible);

    void controlBscansSamplingData(bool& dataSampling, bool& executeParserData, BscanFileFormat bscanFileFormat);

    void fixScrollHBscansLengthBar();

    void moveOnAnyBscansImage(int scrollHBscansLengthBarPos);

private:
    QPointer<QAction>        m_recentFileSeparator;
    QPointer<QMenu>          m_recentFilesBscanMenu;
    QPointer<QMenu>          m_deleteFilesBscanMenu;

    QPointer<QStackedWidget> m_stackedWidget;
    QPointer<QProgressBar>   m_progressBar;

    QPointer<QWidget>        m_mainWidget;
    QPointer<QVBoxLayout>    m_mainLayout;

    QPointer<QHBoxLayout>    m_lowLayout;

    QPointer<QLabel>         m_labelHBscansLengthBar;
    QPointer<QScrollBar>     m_scrollHBscansLengthBar;

    QPointer<QLabel>         m_materialLabel;
    QPointer<QComboBox>      m_materialComboBox;
    QPointer<QLabel>         m_bscanQuantImpulsesLabel;
    QPointer<QSlider>        m_bscanQuantImpulsesSlider;

    QPointer<QLabel>         m_bscansTransitLabel;
    QPointer<QSlider>        m_bscansTransitSlider;
    QPointer<QPushButton>    m_leftBscansTransitButton;
    QPointer<QPushButton>    m_rightBscansTransitButton;

    QPointer<QAction> m_includeFilterOnBscanAction;

    QPointer<QAction> m_closeSection2dAct;
    QPointer<QAction> m_closeCut3dAct;
    QPointer<QAction> m_saveBoundPointsLineAct;
    QPointer<QAction> m_loadBoundPointsLineAct;

    QPointer<QAction> m_highLowImpulsesBscansAction;
    QPointer<QAction> m_showFullBscansAction;

    QPointer<QAction> m_showInitBscanAct;
    QPointer<QAction> m_showLogBscanAct;

    QPointer<QAction> m_logBscanMouseSelectionAct;
    QPointer<QAction> m_logBscanAutoSelectionAct;
    QPointer<QAction> m_showInitBscanSelectionAct;
    QPointer<QAction> m_resetBscanSelectionAct;

    QPointer<QAction> m_saveBscanAutoSelectionDataAct;
    QPointer<QAction> m_loadBscanAutoSelectionDataAct;

    QPointer<QAction> m_useVoidSelectionAct;
    QPointer<QAction> m_useMouseSelectionAct;
    QPointer<QAction> m_useAutoSelectionAct;

    QPointer<QAction> m_bscanAllScalingAct;
    QPointer<QAction> m_bscanHScalingAct;
    QPointer<QAction> m_bscanVScalingAct;

    QPointer<QAction> m_bscanDeepAllScalingAct;
    QPointer<QAction> m_bscanDeepHScalingAct;
    QPointer<QAction> m_bscanDeepVScalingAct;

    QPointer<QAction> m_bscansSurfAllScalingAct;
    QPointer<QAction> m_bscansSurfHScalingAct;
    QPointer<QAction> m_bscansSurfVScalingAct;

    QPointer<QAction> m_bscansTransAllScalingAct;
    QPointer<QAction> m_bscansTransHScalingAct;
    QPointer<QAction> m_bscansTransVScalingAct;

    std::vector<std::pair<std::string, std::string>>  m_bscansNamesVectorPairs;

    std::vector<QPointer<QAction>> m_recentBscansFileActsVector;
    std::vector<QPointer<QAction>> m_deleteBscansFileActsVector;

    QPointer<bscan2dWidget>     m_bscan2dWidget     {new bscan2dWidget(this)};
    QPointer<bscanDeep2dWidget> m_bscanDeep2dWidget {new bscanDeep2dWidget(this)};
    QPointer<bscans2dWidget>    m_bscans2dWidget    {new bscans2dWidget(this)};
    QPointer<bscans3dWidget>    m_bscans3dWidget    {new bscans3dWidget(this)};

    QPointer<colorsPaletteDialog> m_colorsPaletteDialog;

    std::string m_bscanCurrentName{""};

    bool m_section2dVisible      {true};
    bool m_cut3dVisible      {true};

    bool m_bscanImpulsesSliderActivate{false};
    int m_quantImpulsesOfPacket{0};
    int m_maxQuantImpulsesOfPacket{0};

    bool m_resetSectPoints2dBscans {false};
    bool m_resetCutPointsBscans {false};
    bool m_highLowOnBscan {true};
    bool m_showFullBscans {true};

    int          m_filterId {0};
    int       m_selectionId {0};
    int        m_materialId {0};

    std::vector<int> m_trzNumAntennasVector;
    QStringList m_filesBscanNamesTrz;
    QString m_trzDir{""};

    QStringList m_filesBscanNamesCsv;
    QString m_csvDir{""};

    QStringList m_infoBscansHdf5Names;
    QString m_hdf5Dir{""};

    int m_scrollHBscansLengthBarPos{0};
    bool m_scrollHBscansLengthBarAct{false};
};

#endif // MAINWINDOW_H
