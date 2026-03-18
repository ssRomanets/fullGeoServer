#ifndef ACCOMPLISHMENT_H
#define ACCOMPLISHMENT_H

#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QObject>
#include <QPointer>

#include "accomplishmentThread.h"
#include <bscansData.h>

class Accomplishment : public QObject
{
    Q_OBJECT
public:
    Accomplishment(QObject* parent = nullptr);
    ~Accomplishment();

public:
    void createBscansDataTrz(
        std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs,
        std::vector<std::string> fileNameTrzVector, std::vector<std::string> fullFileNameTrzVector
    );

    void createBscansDataCsv( std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs,  std::vector<std::string> fileNameDataCsvVector);

    void createBscansDataHdf5(
        std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs,
        std::vector<std::string> fullFileNameHdf5Vector,
        std::vector<std::vector<std::string>> filesBscanNamesHdf5Vector
    );

    //открываем бинарный файл trz
    void openFilesTrz(std::vector<std::string> fullFileNameTrzVector);

    //открываем бинарные файлы csv
    void openFilesCsv(std::vector<std::string> fullFileNamePropsCsvVector, std::vector<std::string> fullFileNameDataCsvVector);

    QPointer<accomplishmentThread>  m_thread;

public slots:
    void slotEndAccThread( std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs, BscanFileFormat  bscanFileFormat);

signals:
    void signalActivateAccompThread(std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs, BscanFileFormat  bscanFileFormat);

private:
    //в этой функции парсим бинарный файл trz
    void parserFileTrz(int dataTrzId);
};

#endif // ACCOMPLISHMENT_H
