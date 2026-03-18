#ifndef BSCANDEEP2DWIDGET_H
#define BSCANDEEP2DWIDGET_H

#include <QHBoxLayout>
#include <QPointer>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

#include "bscanDeepGlWidget.h"
#include "ascanDeepDataWidget.h"

class bscanDeep2dWidget : public QWidget
{
    Q_OBJECT
public:
    bscanDeep2dWidget(QWidget* parent = nullptr);
    ~bscanDeep2dWidget();

    void outputNewImage(const std::string& bscanName,  const st_bscanInfoData& bscanInfoData, int materialId, int filterId, int selectionId);
    void outputEmptyImage();

    void showNewBscanDeep(const std::string& bscanName, const st_bscanInfoData& bscanInfoData);

    void changeBscanDeepImage(const st_bscanInfoData& bscanInfoData, int absPixelsInX, int absPixelsFnX);

    void setupColorsBscanDeepAllProps(const QColor& colorUp, const QColor& colorLow, const st_bscanInfoData& bscanInfoData);

    void setupTypeScaling(int typeScaling);

signals:
    void signalFixPixels();
    void signalFixDataBscanDeepWidgets();
    void signalSetupPixels(const st_bscanInfoData& bscanInfoData);
    void signalFixPageData(const std::string& bscanName);

public slots:
    void slotSetupScrollHData(int minScrollHData, int scrollHData, int maxScrollHData);
    void slotSetupScrollVData(int minScrollVData, int scrollVData, int maxScrollVData);

    void slotFixPixels();
    void slotlFixDataBscanDeepWidgets();
    void slotSetupPixels(const st_bscanInfoData& bscanInfoData);
    void slotSetupFixDataWidgets(const st_bscanInfoData& bscanInfoData);
    void slotSetupBscanDeepPageData(const st_bscanInfoData& bscanInfoData);

    void slotFixPageData(std::string bscanName);
    void saveBscanDeep();

public:
    QPointer<bscanDeepGlWidget>    m_bscanDeepGlWidget;
    QPointer<QScrollBar>     m_scrollHBar;
    QPointer<QScrollBar>     m_scrollVBar;

private:  
    void bscanDeepInput(const std::string& bscanName, const st_bscanInfoData& bscanInfoData, int absPixelsInX, int absPixelsFnX);
    void setEnabledOpenGlControls(boolean enabled);
    void setupInitPixelSlidersData();
    void setupOpenglControls();


private:
    QPointer<ascanDeepDataWidget>  m_ascanDeepDataWidget;

    QPointer<QVBoxLayout> m_leftVLayout;
    QPointer<QHBoxLayout> m_leftHLayout;
    QPointer<QVBoxLayout> m_leftAllVLayout;

    QPointer<QSlider> m_contrastLog10Slider;
    QPointer<QLabel>  m_contrastLog10Label;

    QPointer<QSlider> m_lowBrightPixelSlider;
    QPointer<QLabel>  m_lowBrightLabel;

    QPointer<QSlider> m_highBrightPixelSlider;
    QPointer<QLabel>  m_highBrightLabel;

    QPointer<QVBoxLayout> m_rightLayout;
    QPointer<QHBoxLayout> m_leftLowLayout;
    QPointer<QHBoxLayout> m_mainLayout;
};

#endif // BSCANDEEP2DWIDGET_H
