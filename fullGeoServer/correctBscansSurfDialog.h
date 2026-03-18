#ifndef CORRECTBscansSURDialog_H
#define CORRECTBscansSURDialog_H

#include <QColorDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QWidget>

class correctBscansSurfDialog: public QDialog
{
    Q_OBJECT

public:
    correctBscansSurfDialog();

    QColor        m_colorBscansSurf1;
    QColor        m_colorBscansSurf2;
    std::string   m_deleteBscanName;

signals:
    void sendColorsBscansSurf(QColor& colorBscansSurf1, QColor& colorBscansSurf2);
    void signalSendDeleteBscanName(const std::string& deleteBscanName);

private:
    QPointer<QColorDialog> m_colorBscansSurf1Dialog;
    QPointer<QColorDialog> m_colorBscansSurf2Dialog;

    QPointer<QPushButton> m_color1Button;
    QPointer<QPushButton> m_color2Button;

    QPointer<QPushButton> m_sendColorsButton;
    QPointer<QPushButton> m_delBscanNameButton;

    void defColor(QColorDialog* colorBscansSurfDialog, QColor& colorBscansSurf);

private slots:
    void defColor1();
    void defColor2();

    void sendColors();
    void slotsSendDeleteBscanName();
};

#endif // CORRECTBscansSURDialog_H
