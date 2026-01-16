#include "colorsPaletteDialog.h"
#include "iostream"

colorsPaletteDialog::colorsPaletteDialog()
{
    m_colorUpDialog = new QColorDialog(this);
    m_colorLowDialog = new QColorDialog(this);

    m_colorUpButton = new QPushButton(tr("def Color Up"), this);
    m_colorLowButton = new QPushButton(tr("def Color Low"), this);
    m_sendColorsButton = new QPushButton(tr("send Colors"), this);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(new QLabel(tr("Color Up:")), 0, 0);
    mainLayout->addWidget(m_colorUpButton, 0, 1);
    mainLayout->addWidget(new QLabel(tr("Color Low:")), 1, 0);
    mainLayout->addWidget(m_colorLowButton, 1, 1);
    mainLayout->addWidget(m_sendColorsButton, 2, 0);

    setLayout(mainLayout);

    m_colorUp  = QColor(255,255,255);
    m_colorLow = QColor(0,0,0);

    connect(m_colorUpButton,    &QPushButton::clicked, this, &colorsPaletteDialog::defColorUp);
    connect(m_colorLowButton,   &QPushButton::clicked, this, &colorsPaletteDialog::defColorLow);
    connect(m_sendColorsButton, &QPushButton::clicked, this, &colorsPaletteDialog::sendColors);
}

void colorsPaletteDialog::defColorUp()
{
    defColor(m_colorUpDialog, m_colorUp);
}

void colorsPaletteDialog::defColorLow()
{
    defColor(m_colorLowDialog, m_colorLow);
}

void colorsPaletteDialog::defColor(QColorDialog* colorDialog, QColor& color)
{
    colorDialog->setCurrentColor(color);
    colorDialog->exec();
    color = colorDialog->selectedColor();
}

void colorsPaletteDialog::sendColors()
{
    emit sendColorsPalette(m_colorUp, m_colorLow);
    this->close();
}
