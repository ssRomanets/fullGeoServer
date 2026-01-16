#include "mainwindow.h"
#include <QApplication>
#include <QImageWriter>
#include <QTextCodec>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    MainWindow mainWindow;
    mainWindow.resize(1920, 1000);
    mainWindow.setMinimumSize(QSize(1920, 1000));
    mainWindow.setMaximumSize(QSize(1920, 1000));
    mainWindow.show();

    return a.exec();
}
