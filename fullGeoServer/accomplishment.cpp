  #include "accomplishment.h"
#include "transformation.h"

#include <fstream>
#include <iostream>
#include <string>
#include <mutex>

#include "rdgsData.h"

Accomplishment::Accomplishment(QObject* parent): QObject(parent)
{
    m_thread = new accomplishmentThread();

    //на первом этапе массивы данных очищаем
    m_thread->m_dataTrzList.clear();
    m_thread->m_dataCsvList.clear();
    m_thread->m_dataPropsCsvList.clear();

    connect(m_thread, &accomplishmentThread::signalEndAccThread, this, &Accomplishment::slotEndAccThread);
}

Accomplishment::~Accomplishment()
{
    m_thread->m_dataTrzList.clear();
    m_thread->m_dataCsvList.clear();
    m_thread->m_dataPropsCsvList.clear();
}

//открываем бинарный файл trz
void Accomplishment::openFilesTrz(std::vector<std::string> fullFileNameTrzVector)
{
    m_thread->m_dataTrzList.clear();
    m_thread->m_parserDataTrzVectors.clear();
    m_thread->m_parserDataTrzVectors.resize(fullFileNameTrzVector.size());
    
    for (int count = 0; count <= fullFileNameTrzVector.size()-1; count++)
    {
        // создаем объект класса QFile
        QFile fileTrz(fullFileNameTrzVector[count].c_str());
        // Проверяем, возможно ли открыть наш файл для чтения
        if (fileTrz.open(QIODevice::ReadOnly))
        {
            //считываем все данные с файла в объект m_data
            m_thread->m_dataTrzList.push_back(fileTrz.readAll());

            //парсим бинарный файл
            parserFileTrz(count);
        }
    }
}

//в этой функции парсим бинарный файл
void Accomplishment::parserFileTrz(int dataTrzId)
{
    int iTRZ = 0;
    int iSGL = 0;
    std::pair<int, int> parserDataElement;
    
    for(int i = 0; i < m_thread->m_dataTrzList[dataTrzId].size(); i++)
    {
        if ( 
            (int)(m_thread->m_dataTrzList[dataTrzId][i])   ==(int)('t') && 
            (int)(m_thread->m_dataTrzList[dataTrzId][i+1]) ==(int)('r') && 
            (int)(m_thread->m_dataTrzList[dataTrzId][i+2]) ==(int)('z')
        )
        {
            if (i > 0)
            {
                parserDataElement.first = iTRZ;
                parserDataElement.second = iSGL;
                m_thread->m_parserDataTrzVectors[dataTrzId].push_back(parserDataElement);
                iTRZ = i;
            }
        }

        if ( 
            (int)(m_thread->m_dataTrzList[dataTrzId][i])   == (int)('s') &&
            (int)(m_thread->m_dataTrzList[dataTrzId][i+1]) == (int)('g') &&
            (int)(m_thread->m_dataTrzList[dataTrzId][i+2]) == (int)('l')
        ) iSGL = i;
    }
    parserDataElement.first = iTRZ;
    parserDataElement.second = iSGL;
    m_thread->m_parserDataTrzVectors[dataTrzId].push_back(parserDataElement);
}

void Accomplishment::createRdgsDataTrz(
    std::vector<std::pair<std::string,std::string>> rdgsNamesVectorPairs,
    std::vector<std::string> fullFileNameTrzVector, std::vector<std::string> fileNameTrzVector
){
    m_thread->m_fullFileNameTrzVector.swap(fullFileNameTrzVector);
    m_thread->m_fileNameTrzVector.swap(fileNameTrzVector);
    m_thread->m_rdgsNamesVectorPairs.swap(rdgsNamesVectorPairs);
    m_thread->start();
}

void Accomplishment::openFilesCsv(std::vector<std::string> fullFileNamePropsCsvVector, std::vector<std::string> fullFileNameDataCsvVector)
{  
    m_thread->m_dataPropsCsvList.clear();
    m_thread->m_dataCsvList.clear();
          
    for (int count = 0; count <=fullFileNamePropsCsvVector.size()-1; count++)
    {
        QFile filePropsCsv(fullFileNamePropsCsvVector[count].c_str());
        if (filePropsCsv.open(QIODevice::ReadOnly))
        {
            m_thread->m_dataPropsCsvList.push_back(filePropsCsv.readAll());
        }

        QFile fileDataCsv(fullFileNameDataCsvVector[count].c_str());
        if (fileDataCsv.open(QIODevice::ReadOnly))
        {
            m_thread->m_dataCsvList.push_back(fileDataCsv.readAll());
        }
    }

    m_thread->m_fullFileNamePropsCsvVector.swap(fullFileNamePropsCsvVector);
    m_thread->m_fullFileNameDataCsvVector.swap(fullFileNameDataCsvVector);
}

void Accomplishment::createRdgsDataCsv(
    std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, std::vector<std::string> fileNameDataCsvVector
) {
    m_thread->m_fileNameDataCsvVector.swap(fileNameDataCsvVector);
    m_thread->m_rdgsNamesVectorPairs.swap(rdgsNamesVectorPairs);
    m_thread->start();
}

void Accomplishment::createRdgsDataHdf5(
    std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs,
    std::vector<std::string> fullFileNameHdf5Vector,
    std::vector<std::vector<std::string>> filesRdgNamesHdf5Vectors
) {
    m_thread->m_fullFileNameHdf5Vector.swap(fullFileNameHdf5Vector);
    m_thread->m_filesRdgNamesHdf5Vectors.swap(filesRdgNamesHdf5Vectors);
    
    m_thread->m_rdgsNamesVectorPairs.swap(rdgsNamesVectorPairs);
    m_thread->start();
}

void Accomplishment::slotEndAccThread(std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs, RdgFileFormat rdgFileFormat)
{
    emit signalActivateAccompThread(rdgsNamesVectorPairs, rdgFileFormat);
}
