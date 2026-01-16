#ifndef ACCOMPLISHMENTTHREAD_H
#define ACCOMPLISHMENTTHREAD_H

#include <QThread>
#include <rdgsData.h>

class accomplishmentThread  : public QThread
{
    Q_OBJECT

public:
    accomplishmentThread(QObject* parent = nullptr);
    ~accomplishmentThread();

    void run();
    void clearWorkData();
    void tracingOfDefiningRdgFilterData(const std::vector<std::string>& rdgNamesVector);

public:
    std::map<std::string, st_rdgInfoData> m_rdgsInfoDataMap;

    //массив данных по данному файлу trz format
    QList<QByteArray> m_dataTrzList;
    std::vector<std::vector<std::pair<int, int> >> m_parserDataTrzVectors;

    std::vector<std::string> m_fullFileNameTrzVector;
    std::vector<std::string> m_fileNameTrzVector;
    std::vector<int> m_trzNumAntennasVector;

    //массивы данных по данным файлам формата csv
    QList<QByteArray> m_dataCsvList;
    QList<QByteArray> m_dataPropsCsvList;
    std::vector<std::string> m_fileNameDataCsvVector;

    std::vector<std::string> m_fullFileNamePropsCsvVector;
    std::vector<std::string> m_fullFileNameDataCsvVector;

    //HDDF5
    std::vector<std::string> m_fullFileNameHdf5Vector;
    std::vector<std::vector<std::string>> m_filesRdgNamesHdf5Vectors;

    //
    std::vector<std::pair<std::string, std::string>> m_rdgsNamesVectorPairs;
    st_rdgsWorkData m_st_rdgsWorkData;

    bool m_executeParserData;
    bool m_defRdgsFnRelief;
    std::string m_nameDeleteRdg{""};
    bool m_executeDeleteRdg;
    bool m_trzDataSampling {false};
    bool m_csvDataSampling {false};
    bool m_hdf5DataSampling {false};

signals:
    void signalEndAccThread (std::vector<std::pair<std::string, std::string>> rdgsNamesVector, RdgFileFormat rdgFileFormat);

    void sendProgressData(int progressPos, int progressMax);

    void sendSurfRdgsWorkData();
    void sendRdgsZData();

    void sendProgressRdgsWorkData(int progressPos, int progressMax);
};

#endif // ACCOMPLISHMENTTHREAD_H+
