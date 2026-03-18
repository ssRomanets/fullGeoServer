#include "correctBscansSurfDialog.h"
#include "iostream"

correctBscansSurfDialog::correctBscansSurfDialog()
{
    m_colorBscansSurf1Dialog = new QColorDialog(this);
    m_colorBscansSurf2Dialog = new QColorDialog(this);

    m_color1Button = new QPushButton(tr("def Color1"), this);
    m_color2Button = new QPushButton(tr("def Color2"), this);
    m_sendColorsButton = new QPushButton(tr("send Colors"), this);
    m_delBscanNameButton = new QPushButton(tr("del Bscan Name"), this);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(new QLabel(tr("Color1:")), 0, 0);
    mainLayout->addWidget(m_color1Button, 0, 1);
    mainLayout->addWidget(new QLabel(tr("Color2:")), 1, 0);
    mainLayout->addWidget(m_color2Button, 1, 1);
    mainLayout->addWidget(m_sendColorsButton, 2, 0);
    mainLayout->addWidget(m_delBscanNameButton, 2, 1);

    setLayout(mainLayout);

    connect(m_color1Button,     &QPushButton::clicked, this, &correctBscansSurfDialog::defColor1);
    connect(m_color2Button,     &QPushButton::clicked, this, &correctBscansSurfDialog::defColor2);
    connect(m_sendColorsButton, &QPushButton::clicked, this, &correctBscansSurfDialog::sendColors);
    connect(m_delBscanNameButton, &QPushButton::clicked, this, &correctBscansSurfDialog::slotsSendDeleteBscanName);
}

void correctBscansSurfDialog::defColor1()
{
    defColor(m_colorBscansSurf1Dialog, m_colorBscansSurf1);
}

void correctBscansSurfDialog::defColor2()
{
    defColor(m_colorBscansSurf2Dialog, m_colorBscansSurf2);
}

void correctBscansSurfDialog::defColor(QColorDialog* colorBscansSurfDialog, QColor& colorBscansSurf)
{
    colorBscansSurfDialog->setCurrentColor(colorBscansSurf);
    colorBscansSurfDialog->exec();
    colorBscansSurf = colorBscansSurfDialog->selectedColor();
}

void correctBscansSurfDialog::sendColors()
{
    emit sendColorsBscansSurf(m_colorBscansSurf1, m_colorBscansSurf2);
    this->close();
}

void correctBscansSurfDialog::slotsSendDeleteBscanName()
{
    emit signalSendDeleteBscanName(m_deleteBscanName);
    this->close();
}
