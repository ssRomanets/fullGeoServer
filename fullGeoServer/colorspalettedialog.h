#ifndef COLORSPALETTEDIALOG_H
#define COLORSPALETTEDIALOG_H

#include <QColorDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QWidget>

class colorsPaletteDialog: public QDialog
{
    Q_OBJECT

public:
    colorsPaletteDialog();

    QColor        m_colorUp;
    QColor        m_colorLow;

signals:
    void sendColorsPalette(QColor& colorUp, QColor& colorLow);

private:
    QPointer<QColorDialog> m_colorUpDialog;
    QPointer<QColorDialog> m_colorLowDialog;

    QPointer<QPushButton> m_colorUpButton;
    QPointer<QPushButton> m_colorLowButton;

    QPointer<QPushButton> m_sendColorsButton;

    void defColor(QColorDialog* colorRdgsSurfDialog, QColor& colorRdgsSurf);

private slots:
    void defColorUp();
    void defColorLow();

    void sendColors();
};

#endif // COLORSPALETTEDIALOG_H
