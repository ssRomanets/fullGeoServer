#ifndef ACCOMPLISHMENTTHREAD_H
#define ACCOMPLISHMENTTHREAD_H

#include <QThread>
#include <bscansData.h>

class accomplishmentThread  : public QThread
{
    Q_OBJECT

public:
    accomplishmentThread(QObject* parent = nullptr);
    ~accomplishmentThread();

    void run();
    void clearWorkData();
    void tracingOfDefiningBscanFilterData(const std::vector<std::string>& bscanNamesVector);
    void tracingOfDefiningBscanMaterialData(const std::vector<std::string>& bscanNamesVector, int materialId);

public:
    std::map<std::string, st_bscanInfoData> m_bscansInfoDataMap;

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
    std::vector<std::vector<std::string>> m_filesBscanNamesHdf5Vectors;

    //
    std::vector<std::pair<std::string, std::string>> m_bscansNamesVectorPairs;
    st_bscansWorkData m_st_bscansWorkData;

    bool m_executeParserData;
    bool m_defBscansFnRelief;
    std::string m_nameDeleteBscan{""};
    bool m_executeDeleteBscan;
    bool m_trzDataSampling {false};
    bool m_csvDataSampling {false};
    bool m_hdf5DataSampling {false};

signals:
    void signalEndAccThread (std::vector<std::pair<std::string, std::string>> bscansNamesVector, BscanFileFormat bscanFileFormat);

    void sendProgressData(int progressPos, int progressMax);

    void sendSurfBscansWorkData();
    void sendBscansZData();

    void sendProgressBscansWorkData(int progressPos, int progressMax);
};

#endif // ACCOMPLISHMENTTHREAD_H+
