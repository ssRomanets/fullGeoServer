#include "correctRdgsSurfDialog.h"
#include "iostream"

correctRdgsSurfDialog::correctRdgsSurfDialog()
{
    m_colorRdgsSurf1Dialog = new QColorDialog(this);
    m_colorRdgsSurf2Dialog = new QColorDialog(this);

    m_color1Button = new QPushButton(tr("def Color1"), this);
    m_color2Button = new QPushButton(tr("def Color2"), this);
    m_sendColorsButton = new QPushButton(tr("send Colors"), this);
    m_delRdgNameButton = new QPushButton(tr("del Rdg Name"), this);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(new QLabel(tr("Color1:")), 0, 0);
    mainLayout->addWidget(m_color1Button, 0, 1);
    mainLayout->addWidget(new QLabel(tr("Color2:")), 1, 0);
    mainLayout->addWidget(m_color2Button, 1, 1);
    mainLayout->addWidget(m_sendColorsButton, 2, 0);
    mainLayout->addWidget(m_delRdgNameButton, 2, 1);

    setLayout(mainLayout);

    connect(m_color1Button,     &QPushButton::clicked, this, &correctRdgsSurfDialog::defColor1);
    connect(m_color2Button,     &QPushButton::clicked, this, &correctRdgsSurfDialog::defColor2);
    connect(m_sendColorsButton, &QPushButton::clicked, this, &correctRdgsSurfDialog::sendColors);
    connect(m_delRdgNameButton, &QPushButton::clicked, this, &correctRdgsSurfDialog::slotsSendDeleteRdgName);
}

void correctRdgsSurfDialog::defColor1()
{
    defColor(m_colorRdgsSurf1Dialog, m_colorRdgsSurf1);
}

void correctRdgsSurfDialog::defColor2()
{
    defColor(m_colorRdgsSurf2Dialog, m_colorRdgsSurf2);
}

void correctRdgsSurfDialog::defColor(QColorDialog* colorRdgsSurfDialog, QColor& colorRdgsSurf)
{
    colorRdgsSurfDialog->setCurrentColor(colorRdgsSurf);
    colorRdgsSurfDialog->exec();
    colorRdgsSurf = colorRdgsSurfDialog->selectedColor();
}

void correctRdgsSurfDialog::sendColors()
{
    emit sendColorsRdgsSurf(m_colorRdgsSurf1, m_colorRdgsSurf2);
    this->close();
}

void correctRdgsSurfDialog::slotsSendDeleteRdgName()
{
    emit signalSendDeleteRdgName(m_deleteRdgName);
    this->close();
}
