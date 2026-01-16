#ifndef ACCOMPLISHMENT_H
#define ACCOMPLISHMENT_H

#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QObject>
#include <QPointer>

#include "accomplishmentThread.h"
#include <rdgsData.h>

class Accomplishment : public QObject
{
    Q_OBJECT
public:
    Accomplishment(QObject* parent = nullptr);
    ~Accomplishment();

public:
    void createRdgsDataTrz(
        std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs,
        std::vector<std::string> fileNameTrzVector, std::vector<std::string> fullFileNameTrzVector
    );

    void createRdgsDataCsv( std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs,  std::vector<std::string> fileNameDataCsvVector);

    void createRdgsDataHdf5(
        std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs,
        std::vector<std::string> fullFileNameHdf5Vector,
        std::vector<std::vector<std::string>> filesRdgNamesHdf5Vector
    );

    //открываем бинарный файл trz
    void openFilesTrz(std::vector<std::string> fullFileNameTrzVector);

    //открываем бинарные файлы csv
    void openFilesCsv(std::vector<std::string> fullFileNamePropsCsvVector, std::vector<std::string> fullFileNameDataCsvVector);

    QPointer<accomplishmentThread>  m_thread;

public slots:
    void slotEndAccThread( std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, RdgFileFormat  rdgFileFormat);

signals:
    void signalActivateAccompThread(std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, RdgFileFormat  rdgFileFormat);

private:
    //в этой функции парсим бинарный файл trz
    void parserFileTrz(int dataTrzId);
};

#endif // ACCOMPLISHMENT_H
