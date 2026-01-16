#ifndef CORRECTRdgsSURDialog_H
#define CORRECTRdgsSURDialog_H

#include <QColorDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QWidget>

class correctRdgsSurfDialog: public QDialog
{
    Q_OBJECT

public:
    correctRdgsSurfDialog();

    QColor        m_colorRdgsSurf1;
    QColor        m_colorRdgsSurf2;
    std::string   m_deleteRdgName;

signals:
    void sendColorsRdgsSurf(QColor& colorRdgsSurf1, QColor& colorRdgsSurf2);
    void signalSendDeleteRdgName(const std::string& deleteRdgName);

private:
    QPointer<QColorDialog> m_colorRdgsSurf1Dialog;
    QPointer<QColorDialog> m_colorRdgsSurf2Dialog;

    QPointer<QPushButton> m_color1Button;
    QPointer<QPushButton> m_color2Button;

    QPointer<QPushButton> m_sendColorsButton;
    QPointer<QPushButton> m_delRdgNameButton;

    void defColor(QColorDialog* colorRdgsSurfDialog, QColor& colorRdgsSurf);

private slots:
    void defColor1();
    void defColor2();

    void sendColors();
    void slotsSendDeleteRdgName();
};

#endif // CORRECTRdgsSURDialog_H
