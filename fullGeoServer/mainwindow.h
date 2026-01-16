#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rdg2dWidget.h"
#include "rdgs2dWidget.h"
#include "rdgs3dWidget.h"
#include "rdgGlWidget.h"
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

    void setupLastRdgs();
    void scrollHRdgsLengthBarResetDataAtRdgsImages();
    void defAbsRdgsStructInXAndFnX(int& absRdgsStructInX, int& absRdgsStructFnX);

public slots:   
    /**
     * @brief Слот получения имени удаляемой радарограммы
    */
    void receiveDeleteRdgName(const std::string& deleteRdgName);
    /**
     * @brief Слот для прогрессбара по формированию данных из имеющихся trz файлов
    */
    void receiveProgressData(int progressPos, int progressMax);
    /**
     * @brief Слот говорящий о том, что данные по радарограммам сформированы
    */
    void receiveRdgsWorkData();
    /**
     * @brief Слот говорящий о том, что рельеф связанный с радарограммами определен
    */
    void receiveRdgsZData();

    void slotFixRdgsSurfPixels();
    void slotFixDeleteRdgName(double fixLatitude, double fixLongitude);
    void slotFixFillRdgs(bool resetCutPointsRdgs);

    void slotFixRdgsSurfSection();
    void slotActivateAccompThread(std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, RdgFileFormat rdgFileFormat);

    void receiveColorsPalette(QColor& colorUp, QColor& colorLow);

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
    void showRdgImage(bool changeRdg);
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
    void enabledShowRdgActions(bool enabled);

    void rdgImpulsesSliderActivate();
    void setQuantImpulsesOfPacketSlider(int quantImpulsesOfPacket);
    void receiveOutRdgInfo(QString outRdgInfo);
    void receiveOutRdgsSurfInfo(QString outRdgsSurfInfo);

    void resetRdgSelection(bool clearSelectRdgPoints);
    void setupRdgsSliderSetup(int m_rdgId, int rdgsCount);

    void setupRdgCurrentName();
    void setupRdgControls();

    void setupScrollHRdgsLengthBarData(int minScrollHRdgsLengthBarData, int scrollHRdgsLengthBarData, int maxScrollHRdgsLengthBarData);

signals:
    void signalClearRdgsInfoDataMap();
    void signalEraseRdgsInfoDataMap(const std::string& fileName);
    void closeSection2dAction(bool section2dVisible);
    void closeCut3dAction(bool cut3dVisible);

    void signalSetupRdgsSurfPixels   (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData);
    void signalOutputRdgsSurfSection (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData& rdgsWorkData);
    void signalDefDeleteRdgName      (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, double fixLatitude, double fixLongitude);
    void signalFillRdgsExecute       (const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData, bool resetCutPointsRdgs);

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
    void openImageRdg();
    void defPaletteRdg();

    void open2dImageRdgs();
    void defPaletteSurfRdgs();

    void open3dImageRdgs();
    void defPaletteTransRdgs();
    void includeFilterOnRdg();

    void openRecentFile();

    void deleteSelectedFile();
    void deleteAllFiles();

    void save();

    void setCloseSection2dAction();
    void setCloseCut3dAction();

    void saveBoundPointsLineAction();
    void loadBoundPointsLineAction();

    void setInitRdg();
    void setLogRdg();

    void setLogRdgMouseSelection();
    void setLogRdgAutoSelection();
    void setShowInitRdgSelection();
    void slotResetRdgSelection();
    void saveRdgAutoSelectionData();
    void loadRdgAutoSelectionData();

    void changeHighLowOnRdgs();
    void showFullRdgsData();

    void materialIdChanged(int materialId);

    void useSelectionOnRdg();
    void setupScalingOnRdg();
    void setupScalingOnRdgsSurf();
    void setupScalingOnRdgsTrans();

    void changeRdg(int rdgId);
    void leftChangeRdg();
    void rightChangeRdg();
    
    void scrollHRdgsLengthBarPressed();
    void scrollHRdgsLengthBarMoved(int scrollHRdgsLengthBarPos);
    void scrollHRdgsLengthBarReleased();

    void scrollHRdgsLengthBarChanged(int scrollHRdgsLengthBarPos);

private:
    void deleteFile(const std::string& fileName);
    void outputRdgsData(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, const st_rdgsWorkData&  rdgsWorkData,
        int maxQuantImpulsesOfPacket, int quantImpulsesOfPacket, int filterId, int selectionId
    );
    void closeInitRdgSelection();

    void impulsesRdgControlsVisible(bool visible);
    void rdgsTransitControlsVisible(bool visible);

    void controlRdgsSamplingData(bool& dataSampling, bool& executeParserData, RdgFileFormat rdgFileFormat);

    void openScrollHRdgsLengthBar(bool visible);

    void moveOnAnyRdgsImage(int scrollHRdgsLengthBarPos);

private:
    QPointer<QAction>        m_recentFileSeparator;
    QPointer<QMenu>          m_recentFilesRdgMenu;
    QPointer<QMenu>          m_deleteFilesRdgMenu;

    QPointer<QStackedWidget> m_stackedWidget;
    QPointer<QProgressBar>   m_progressBar;

    QPointer<QWidget>        m_mainWidget;
    QPointer<QVBoxLayout>    m_mainLayout;

    QPointer<QHBoxLayout>    m_lowLayout;

    QPointer<QScrollBar>     m_scrollHRdgsLengthBar;

    QPointer<QComboBox>      m_materialComboBox;
    QPointer<QLabel>         m_rdgQuantImpulsesLabel;
    QPointer<QSlider>        m_rdgQuantImpulsesSlider;

    QPointer<QSlider>        m_rdgsTransitSlider;
    QPointer<QPushButton>    m_leftRdgsTransitButton;
    QPointer<QPushButton>    m_rightRdgsTransitButton;

    QPointer<QAction> m_includeFilterOnRdgAction;

    QPointer<QAction> m_closeSection2dAct;
    QPointer<QAction> m_closeCut3dAct;
    QPointer<QAction> m_saveBoundPointsLineAct;
    QPointer<QAction> m_loadBoundPointsLineAct;

    QPointer<QAction> m_highLowImpulsesRdgsAction;
    QPointer<QAction> m_showFullRdgsAction;

    QPointer<QAction> m_showInitRdgAct;
    QPointer<QAction> m_showLogRdgAct;

    QPointer<QAction> m_logRdgMouseSelectionAct;
    QPointer<QAction> m_logRdgAutoSelectionAct;
    QPointer<QAction> m_showInitRdgSelectionAct;
    QPointer<QAction> m_resetRdgSelectionAct;

    QPointer<QAction> m_saveRdgAutoSelectionDataAct;
    QPointer<QAction> m_loadRdgAutoSelectionDataAct;

    QPointer<QAction> m_useVoidSelectionAct;
    QPointer<QAction> m_useMouseSelectionAct;
    QPointer<QAction> m_useAutoSelectionAct;

    QPointer<QAction> m_rdgAllScalingAct;
    QPointer<QAction> m_rdgHScalingAct;
    QPointer<QAction> m_rdgVScalingAct;

    QPointer<QAction> m_rdgsSurfAllScalingAct;
    QPointer<QAction> m_rdgsSurfHScalingAct;
    QPointer<QAction> m_rdgsSurfVScalingAct;

    QPointer<QAction> m_rdgsTransAllScalingAct;
    QPointer<QAction> m_rdgsTransHScalingAct;
    QPointer<QAction> m_rdgsTransVScalingAct;

    std::vector<std::pair<std::string, std::string>>  m_rdgsNamesVectorPairs;

    std::vector<QPointer<QAction>> m_recentRdgsFileActsVector;
    std::vector<QPointer<QAction>> m_deleteRdgsFileActsVector;

    QPointer<rdg2dWidget>  m_rdg2dWidget  {new rdg2dWidget(this)};
    QPointer<rdgs2dWidget> m_rdgs2dWidget {new rdgs2dWidget(this)};
    QPointer<rdgs3dWidget> m_rdgs3dWidget {new rdgs3dWidget(this)};

    QPointer<colorsPaletteDialog> m_colorsPaletteDialog;

    std::string m_rdgCurrentName{""};

    bool m_section2dVisible      {true};
    bool m_cut3dVisible      {true};

    bool m_rdgImpulsesSliderActivate{false};
    int m_quantImpulsesOfPacket{0};
    int m_maxQuantImpulsesOfPacket{0};

    bool m_resetSectPoints2dRdgs {false};
    bool m_resetCutPointsRdgs {false};
    bool m_highLowOnRdg {true};
    bool m_showFullRdgs {true};

    int          m_filterId {0};
    int       m_selectionId {0};
    int        m_materialId {0};

    std::vector<int> m_trzNumAntennasVector;
    QStringList m_filesRdgNamesTrz;
    QString m_trzDir{""};

    QStringList m_filesRdgNamesCsv;
    QString m_csvDir{""};

    QStringList m_infoRdgsHdf5Names;
    QString m_hdf5Dir{""};

    int m_scrollHRdgsLengthBarPos{0};
    bool m_scrollHRdgsLengthBarAct{false};
};

#endif // MAINWINDOW_H
