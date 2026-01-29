#include "accomplishmentThread.h"
#include "transformation.h"
#include <mutex>

#pragma comment (lib, "libhdf5.lib");
#pragma comment (lib, "libhdf5_cpp.lib");
#pragma comment (lib, "libhdf5_hl.lib");
#pragma comment (lib, "libhdf5_hl_cpp.lib");
#pragma comment (lib, "libhdf5_tools.lib");
#pragma comment (lib, "libszip.lib");
#pragma comment (lib, "libzlib.lib");

#include "C:\Program Files\HDF_Group\HDF5\1.12.0\include\H5Cpp.h"

accomplishmentThread::accomplishmentThread(QObject* parent)
{
    m_executeParserData = false;
}

accomplishmentThread::~accomplishmentThread(){}

void accomplishmentThread::run()
{
    std::vector<std::string> rdgNamesVector;
    rdgNamesVector.resize(0);
    if (m_rdgsInfoDataMap.size() != 0)
    for (auto mapIter = m_rdgsInfoDataMap.begin(); mapIter != m_rdgsInfoDataMap.end(); mapIter++) { rdgNamesVector.push_back(mapIter->first);}

    if (m_executeParserData == false)
    {
        if (m_trzDataSampling == true)
        {
            for (int count = 0; count <= m_dataTrzList.size()-1; count++)
            {
                int quantImpulsesOfPacket = (m_dataTrzList[count].size() - m_parserDataTrzVectors[count][m_parserDataTrzVectors[count].size()-1].second-9)/8;
                int fullRdgTrackCounts    = m_parserDataTrzVectors[count].size();

                for (int i = 0; i < fullRdgTrackCounts; i++)
                {
                    std::string TRZ_HEADER_string;
                    TRZ_HEADER_string = std::string(m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first, 3);

                    bool antennaExist = false;
                    int antenna_number     = *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 13));

                    if (m_trzNumAntennasVector.size() == 0) antennaExist = true;
                    else if (std::find(m_trzNumAntennasVector.begin(), m_trzNumAntennasVector.end(), antenna_number) != m_trzNumAntennasVector.end())  antennaExist = true;

                    if (antennaExist == true)
                    {
                        std::string rdgInfoDataKey="";
                        if (antenna_number< 10)   rdgInfoDataKey = m_fileNameTrzVector[count] + "&&" + "0" + std::to_string(antenna_number);
                        else                      rdgInfoDataKey = m_fileNameTrzVector[count] + "&&" +       std::to_string(antenna_number);

                        auto findingRdg{ std::find(begin(rdgNamesVector), end(rdgNamesVector), rdgInfoDataKey) };
                        if (findingRdg == end(rdgNamesVector))
                        {
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.resize(m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()+1);

                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].header =
                            TRZ_HEADER_string.data();
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].version_number     =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 3));
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].block_count        =
                            correctInt64TValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 4);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].antennas_working   =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 12));
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].block_step_size_m  =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 14);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].antennas_step_size =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 22);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_step_ns       =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 30);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].freq_step_GHz      =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 38);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].freq_begin_GHz     =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 46);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].freq_end_GHz       =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 54);

                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].latitude_degree    =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 62);

                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].longitude_degree   =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 70);

                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_hours         =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 78));  // время час
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_minutes       =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 79));  // время минута
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_seconds       =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 80));
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_milliseconds  =
                            correctInt16TValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first,  i, 81);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].day                =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 83));
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].month              =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 84));  // месяц
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].year               =
                            correctInt16TValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first,  i, 85);// год

                            std::string SGL_HEADER_string;
                            SGL_HEADER_string = std::string(m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].second, 3);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].signal_character = SGL_HEADER_string.data();

                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].begin_signal_index =
                            correctInt16TValue(m_dataTrzList[count], m_parserDataTrzVectors[count][i].second,  i, 3);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].end_signal_index   =
                            correctInt16TValue(m_dataTrzList[count], m_parserDataTrzVectors[count][i].second,  i, 5);
                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].signal_length      =
                            correctInt16TValue(m_dataTrzList[count], m_parserDataTrzVectors[count][i].second,  i, 7);

                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].vectorsDoubleData.resize(countFilters);
                            for (int j=0; j < quantImpulsesOfPacket; j++)
                            {
                                for (int k = 0; k <= countFilters-1; k++)
                                {
                                    m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].vectorsDoubleData[k].
                                    push_back(correctDoubleValue(m_dataTrzList[count], m_parserDataTrzVectors[count][i].second, i, 9+8*j)/(double)(k+1));
                                }
                            }

                        }
                    }

                    emit sendProgressData(count, m_dataTrzList.size()+2+countMaterials);
                }

                //Занимаемся фильтрами и веществами
                tracingOfDefiningRdgFilterData(rdgNamesVector);
                emit sendProgressData(m_dataTrzList.size(), m_dataTrzList.size()+2+countMaterials);
                
                for (int count = 0; count <= countMaterials-1; count++)
                {
                    tracingOfDefiningRdgMaterialData(rdgNamesVector, count);
                    emit sendProgressData(m_dataTrzList.size() + count + 1, m_dataTrzList.size()+2+countMaterials);
                }

                defAdditionalRdgMainData(m_rdgsInfoDataMap, quantImpulsesOfPacket);
                emit sendProgressData(m_dataTrzList.size()+1+countMaterials, m_dataTrzList.size()+2+countMaterials);

                correctRdgData(m_rdgsInfoDataMap, m_st_rdgsWorkData, m_rdgsNamesVectorPairs, m_fullFileNameTrzVector[count]);
                emit sendProgressData(m_dataTrzList.size()+2+countMaterials, m_dataTrzList.size()+2+countMaterials);
            }

            emit signalEndAccThread(m_rdgsNamesVectorPairs, RdgFileFormat::Trz);
        }

        if (m_csvDataSampling == true)
        {
            for (int count = 0; count < m_dataCsvList.size(); count++)
            {
                int quantImpulsesOfPacket = 0;

                if (m_dataPropsCsvList[count].split('\n').size() == m_dataCsvList[count].split('\n').size() + 1)
                {
                    std::string rdgInfoDataKey="";
                    emit sendProgressData(0, m_dataPropsCsvList[count].split('\n').size()+4);

                    QFile filePropsCsv(m_fullFileNamePropsCsvVector[count].c_str());
                    if (filePropsCsv.open(QFile::ReadOnly | QFile::Text))
                    {
                        QTextStream filePropsCsvTS(&filePropsCsv);
                        int count1 = 0;
                        while (!filePropsCsvTS.atEnd())
                        {
                            QString line = filePropsCsvTS.readLine();
                            if (count1 >= 1)
                            {
                                if (line.split(";").size() >=16)
                                {
                                    if (rdgInfoDataKey == "")  rdgInfoDataKey = m_fileNameDataCsvVector[count];

                                    auto findingRdg{ std::find(begin(rdgNamesVector), end(rdgNamesVector), rdgInfoDataKey) };
                                    if (findingRdg == end(rdgNamesVector))
                                    {
                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.resize(m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()+1);

                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].block_count        =
                                        (line.split(";").at(0)).toInt();

                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].block_step_size_m  =
                                        (line.split(";").at(1)).toDouble();

                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].antennas_step_size =
                                        (line.split(";").at(3)).toDouble();

                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_step_ns       =
                                        (line.split(";").at(4)).toDouble();

                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_hours         =
                                        (line.split(";").at(7)).toInt();  // время час
                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_minutes       =
                                        (line.split(";").at(8)).toInt();  // время минута
                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_seconds       =
                                        (line.split(";").at(9)).toInt();
                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_milliseconds  =
                                        (line.split(";").at(10)).toInt();

                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].day                =
                                        (line.split(";").at(11)).toInt();
                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].month              =
                                        (line.split(";").at(12)).toInt();  // месяц
                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].year               =
                                        (line.split(";").at(13)).toInt();// год

                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].latitude_degree    =
                                        (line.split(";").at(14)).toDouble();

                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].longitude_degree   =
                                        (line.split(";").at(15)).toDouble();
                                    }
                                }
                            }
                            count1++;
                        }
                    }

                    auto findingRdg{ std::find(begin(rdgNamesVector), end(rdgNamesVector), rdgInfoDataKey) };
                    if (findingRdg == end(rdgNamesVector))
                    {
                        QFile fileDataCsv(m_fullFileNameDataCsvVector[count].c_str());
                        if (fileDataCsv.open(QFile::ReadOnly | QFile::Text))
                        {
                            QTextStream fileDataCsvTS(&fileDataCsv);

                            int count2 = 0;
                            while (!fileDataCsvTS.atEnd())
                            {
                                QString line = fileDataCsvTS.readLine();
                                if (line.split(";").size() > 0)
                                {
                                    quantImpulsesOfPacket = line.split(";").size();

                                    for (int count3 = 0; count3 < quantImpulsesOfPacket; count3++)
                                    {
                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[count2].vectorsDoubleData.resize(countFilters);

                                        for (int k = 0; k <= countFilters-1; k++)
                                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[count2].vectorsDoubleData[k].push_back((line.split(";").at(count3)).toInt()/(double)(k+1));
                                    }

                                    emit sendProgressData(count2+1, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);
                                    count2++;
                                }
                            }
                        }
                    }

                    /////////////////////////////
                    std::string rdgFilesFullData = m_fullFileNameDataCsvVector[count] + "&&&&" + m_fullFileNamePropsCsvVector[count];

                    //Занимаемся фильтрами и материалами
                    tracingOfDefiningRdgFilterData(rdgNamesVector);
                    emit sendProgressData(m_dataPropsCsvList[count].split('\n').size()+3, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);
                    
                    for (int count1 = 0; count1 <= countMaterials-1; count1++)
                    {
                        tracingOfDefiningRdgMaterialData(rdgNamesVector, count1);
                        emit sendProgressData(m_dataPropsCsvList[count].split('\n').size()+4+count1, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);
                    }

                    defAdditionalRdgMainData(m_rdgsInfoDataMap, quantImpulsesOfPacket);
                    emit sendProgressData(m_dataPropsCsvList[count].split('\n').size()+4+countMaterials, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);

                    correctRdgData(m_rdgsInfoDataMap, m_st_rdgsWorkData, m_rdgsNamesVectorPairs, rdgFilesFullData);
                    emit sendProgressData(m_dataPropsCsvList[count].split('\n').size()+5+countMaterials, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);
                }
            }

            emit signalEndAccThread(m_rdgsNamesVectorPairs,  RdgFileFormat::Csv);
        }

        if (m_hdf5DataSampling == true)
        {
            std::string rdgInfoDataKey =  "";
            for (int count = 0; count < m_filesRdgNamesHdf5Vectors.size(); count++ )
            {
                int quantImpulsesOfPacket = 0;
                for (int count1 = 0; count1 < m_filesRdgNamesHdf5Vectors[count].size(); count1++)
                {
                    try
                    {
                        // Открытие файла
                        H5::H5File file(m_filesRdgNamesHdf5Vectors[count][count1], H5F_ACC_RDONLY);

                        // Чтение основных данных B-scan
                        std::string ez_path = "/rxs/rx1/Ez";
                        if (file.exists(ez_path))
                        {
                            H5::DataSet     dataset = file.openDataSet(ez_path);
                            H5::DataSpace dataspace = dataset.getSpace();

                            // Получение размерности
                            int rank = dataspace.getSimpleExtentNdims();

                            if (rank == 1)
                            {
                                QString rootPath = "";
                                for (int count2 = 0; count2 <= (QString::fromStdString(m_filesRdgNamesHdf5Vectors[count][count1]).split("/")).size()-2; count2++)
                                    rootPath +=  (QString::fromStdString(m_filesRdgNamesHdf5Vectors[count][count1]).split("/")).at(count2)+"/";

                                rdgInfoDataKey = rootPath.toStdString();

                                auto findingRdg{ std::find(begin(rdgNamesVector), end(rdgNamesVector), rdgInfoDataKey) };
                                if (findingRdg == end(rdgNamesVector))
                                {
                                    hsize_t dims[1];
                                    dataspace.getSimpleExtentDims(dims, NULL);

                                    m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.resize(m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()+1);

                                    //Чтение данных
                                    std::vector<double> flat_data(dims[0]);
                                    dataset.read(flat_data.data(), H5::PredType::NATIVE_DOUBLE);
                                    quantImpulsesOfPacket = dims[0];

                                    // Преобразование в 2D вектор
                                    for (hsize_t i = 0; i < quantImpulsesOfPacket; i++)
                                    {
                                        m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].vectorsDoubleData.resize(countFilters);

                                        for (int k = 0; k <= countFilters-1; k++)
                                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].vectorsDoubleData[k].push_back(flat_data[i]/(double)(k+1));
                                    }

                                    //Чтение данных и атрибутов
                                    H5::Group rx_group = file.openGroup("/");
                                    //Чтение атрибутов
                                    if (rx_group.attrExists("dt"))
                                    {
                                        H5::Attribute attr = rx_group.openAttribute("dt");
                                        attr.read(H5::PredType::NATIVE_DOUBLE, &m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.size()-1].time_step_ns);
                                    }
                                    rx_group.close();

                                    dataset.close();
                                    dataspace.close();
                                }
                            }

                            if (rank == 2)
                            {
                                rdgInfoDataKey = m_filesRdgNamesHdf5Vectors[count][count1];

                                auto findingRdg{ std::find(begin(rdgNamesVector), end(rdgNamesVector), rdgInfoDataKey) };
                                if (findingRdg == end(rdgNamesVector))
                                {
                                    hsize_t dims[2];
                                    dataspace.getSimpleExtentDims(dims, NULL);

                                    //Чтение данных
                                    std::vector<double> flat_data(dims[0]*dims[1]);
                                    dataset.read(flat_data.data(), H5::PredType::NATIVE_DOUBLE);

                                    quantImpulsesOfPacket = dims[0];
                                    m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData.resize(dims[1]);

                                    for (hsize_t i = 0; i < dims[0]; i++)
                                    {
                                        for (hsize_t j = 0; j < dims[1]; j++)
                                        {
                                            m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[j].vectorsDoubleData.resize(countFilters);

                                            for (int k = 0; k <= countFilters-1; k++)
                                                m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[j].vectorsDoubleData[k].push_back(flat_data[i * dims[1] + j]/(double)(k+1));
                                        }
                                    }

                                    //Чтение данных и атрибутов
                                    H5::Group rx_group = file.openGroup("/");
                                    //Чтение атрибутов
                                    if (rx_group.attrExists("dt"))
                                    {
                                        H5::Attribute attr = rx_group.openAttribute("dt");
                                        for (hsize_t j = 0; j < dims[1]; j++)
                                        {
                                            attr.read(H5::PredType::NATIVE_DOUBLE, &m_rdgsInfoDataMap[rdgInfoDataKey].vectorRdgData[j].time_step_ns);
                                        }
                                    }
                                    rx_group.close();

                                    dataset.close();
                                    dataspace.close();
                                }
                            }
                        }
                        file.close();
                    }
                    catch (H5::FileIException& error)
                    {
                        std::cerr<<"File error: ";
                        error.printErrorStack();
                    }
                    catch (H5::DataSetIException& error)
                    {
                        std::cerr<<"Dataset error: ";
                        error.printErrorStack();
                    }
                    catch (H5::DataSpaceIException& error)
                    {
                        std::cerr<<"Dataspace error: ";
                        error.printErrorStack();
                    }
                    catch (H5::AttributeIException& error)
                    {
                        std::cerr<<"Attribute error: ";
                        error.printErrorStack();
                    }
                }
                emit sendProgressData(0, m_filesRdgNamesHdf5Vectors[count].size()+3+countMaterials);

                if (rdgInfoDataKey != "")
                {
                    //Занимаемся фильтрами
                    tracingOfDefiningRdgFilterData(rdgNamesVector);
                    emit sendProgressData( m_filesRdgNamesHdf5Vectors[count].size(), m_filesRdgNamesHdf5Vectors[count].size()+3+countMaterials);

                    for (int count1 = 0; count1 <= countMaterials-1; count1++)
                    {
                        tracingOfDefiningRdgMaterialData(rdgNamesVector, count1);
                        emit sendProgressData(m_filesRdgNamesHdf5Vectors[count].size()+2+count1+1, m_filesRdgNamesHdf5Vectors[count].size()+3+countMaterials);
                    }

                    defAdditionalRdgMainData(m_rdgsInfoDataMap,  quantImpulsesOfPacket);
                    emit sendProgressData(m_filesRdgNamesHdf5Vectors[count].size()+2+countMaterials, m_filesRdgNamesHdf5Vectors[count].size()+3+countMaterials);

                    correctRdgData(m_rdgsInfoDataMap, m_st_rdgsWorkData, m_rdgsNamesVectorPairs, m_fullFileNameHdf5Vector[count]);
                    emit sendProgressData(m_filesRdgNamesHdf5Vectors[count].size()+3+countMaterials, m_filesRdgNamesHdf5Vectors[count].size()+3+countMaterials);
                }
            }

            emit signalEndAccThread(m_rdgsNamesVectorPairs,  RdgFileFormat::Hdf5);
        }
    }
    else
    {
        if (m_executeDeleteRdg == false)
        {
            if (m_defRdgsFnRelief == false)
            {
                m_st_rdgsWorkData.rightLatitude = -1.0;
                m_st_rdgsWorkData.deltaLatitude  = 0.0;
                m_st_rdgsWorkData.deltaLongitude = 0.0;

                devMainParameters(
                    m_rdgsInfoDataMap, m_st_rdgsWorkData.leftLatitude,
                    m_st_rdgsWorkData.rightLatitude, m_st_rdgsWorkData.lowLongitude, m_st_rdgsWorkData.upLongitude,
                    m_st_rdgsWorkData.deltaLatitude, m_st_rdgsWorkData.deltaLongitude,
                    m_st_rdgsWorkData.rdgsSurfWidth, m_st_rdgsWorkData.rdgsSurfHeight
                );

                double* rdgsPoints;
                if (m_st_rdgsWorkData.rdgsSurfWidth > m_st_rdgsWorkData.rdgsSurfHeight) rdgsPoints = new double[m_st_rdgsWorkData.rdgsSurfWidth];
                else                                                                    rdgsPoints = new double[m_st_rdgsWorkData.rdgsSurfHeight];


                m_st_rdgsWorkData.vectorRdgsData.resize(m_st_rdgsWorkData.rdgsSurfWidth*m_st_rdgsWorkData.rdgsSurfHeight);

                m_st_rdgsWorkData.vectorRdgsFnRelief.resize(m_st_rdgsWorkData.rdgsSurfWidth);
                for (int i = 0; i < m_st_rdgsWorkData.rdgsSurfWidth; i++)
                m_st_rdgsWorkData.vectorRdgsFnRelief[i].resize(m_st_rdgsWorkData.rdgsSurfHeight);

                for (int i = 0; i < m_st_rdgsWorkData.rdgsSurfWidth; i++)
                {
                    for (int j = 0; j < m_st_rdgsWorkData.rdgsSurfHeight; j++)
                    {
                        m_st_rdgsWorkData.vectorRdgsFnRelief[i][j] = 0.0;
                    }
                }

                std::vector<std::string> rdgNamesVector;
                for (auto mapIter = m_rdgsInfoDataMap.begin(); mapIter != m_rdgsInfoDataMap.end(); mapIter++) { rdgNamesVector.push_back(mapIter->first);}

                for (int i = 0; i < m_st_rdgsWorkData.rdgsSurfWidth; i++)
                {
                    for (int j = 0; j < m_st_rdgsWorkData.rdgsSurfHeight; j++)
                    {
                        m_st_rdgsWorkData.vectorRdgsData[m_st_rdgsWorkData.rdgsSurfWidth*j + i] = defTupleRdgs(
                        m_rdgsInfoDataMap, i, j,
                        m_st_rdgsWorkData.leftLatitude, m_st_rdgsWorkData.rightLatitude,
                        m_st_rdgsWorkData.lowLongitude, m_st_rdgsWorkData.upLongitude,
                        m_st_rdgsWorkData.deltaLatitude, m_st_rdgsWorkData.deltaLongitude,
                        m_st_rdgsWorkData.rdgsSurfWidth, m_st_rdgsWorkData.rdgsSurfHeight,
                        rdgsPoints);
                    }
                    emit sendProgressData(i, m_st_rdgsWorkData.rdgsSurfWidth + rdgNamesVector.size());
                }

                //делаем ступеньки
                std::vector<std::vector<std::pair<int, int>> > rdgsPointsVector;
                rdgsPointsVector.resize(rdgNamesVector.size());

                std::string nameRdg = "";
                std::pair<int, int> rdgPoint;

                for (int i = 0; i < m_st_rdgsWorkData.rdgsSurfWidth; i++)
                {
                    for (int j = 0; j < m_st_rdgsWorkData.rdgsSurfHeight; j++)
                    {
                        nameRdg = std::get<0>(m_st_rdgsWorkData.vectorRdgsData[m_st_rdgsWorkData.rdgsSurfWidth*j+i]);
                        if (nameRdg != "")
                        {
                            auto rdgFindIter = std::find(rdgNamesVector.begin(), rdgNamesVector.end(), nameRdg);
                            int rdgPosition = std::distance(rdgNamesVector.begin(), rdgFindIter);

                            rdgPoint.first = i;
                            rdgPoint.second = j;
                            rdgsPointsVector[rdgPosition].push_back(rdgPoint);
                        }
                    }
                }

                m_st_rdgsWorkData.rdgsSurfMinWidth = 0;
                for (int i = 0; i <  rdgsPointsVector.size(); i++)
                {
                    int iMin = 0;
                    int iMax = 0;

                    for (int j = 0; j < rdgsPointsVector[i].size(); j++)
                    {
                        if (rdgsPointsVector[i][j].first <= iMin) iMin = rdgsPointsVector[i][j].first;
                        if (rdgsPointsVector[i][j].first >= iMax) iMax = rdgsPointsVector[i][j].first;
                    }

                    if (i == 0)                                                   m_st_rdgsWorkData.rdgsSurfMinWidth = iMax - iMin+1;
                    else if (iMax - iMin+1 <  m_st_rdgsWorkData.rdgsSurfMinWidth) m_st_rdgsWorkData.rdgsSurfMinWidth = iMax - iMin+1;
                }

                int inPosX = 0;
                int inPosY = 0;
                int fnPosX = 0;
                int fnPosY = 0;

                std::string nameRdgIn = "";
                int         kRdgIn    = 0;

                std::string nameRdgFn = "";
                int         kRdgFn    = 0;

                for (int count = 0; count < rdgsPointsVector[0].size(); count++)
                {
                    inPosX = rdgsPointsVector[0][count].first;
                    inPosY = rdgsPointsVector[0][count].second;
                    representRdgLine( inPosX, 1, inPosX, inPosY, m_st_rdgsWorkData, true);
                }

                std::vector<std::pair<int, int>> vectorRdgsTransLinePoints;
                for (int count1 = 0; count1 < rdgsPointsVector.size()-1; count1++)
                {
                    int numActRdg   = -1;
                    int minRdgWidth =  0;
                    if (rdgsPointsVector[count1].size() <= rdgsPointsVector[count1+1].size())
                    {
                        numActRdg = 0;
                        minRdgWidth = rdgsPointsVector[count1].size();
                    }
                    else if  (rdgsPointsVector[count1].size() > rdgsPointsVector[count1+1].size())
                    {
                        numActRdg = 1;
                        minRdgWidth = rdgsPointsVector[count1+1].size();
                    }

                    for (int count2 = 0; count2 < minRdgWidth; count2++)
                    {
                        inPosX = rdgsPointsVector[count1  ][count2].first;
                        inPosY = rdgsPointsVector[count1  ][count2].second;
                        fnPosX = rdgsPointsVector[count1+1][count2].first;
                        fnPosY = rdgsPointsVector[count1+1][count2].second;
                        defVectorRdgsTransLineSectionPoints(inPosX, inPosY, fnPosX, fnPosY, vectorRdgsTransLinePoints);
                        if (vectorRdgsTransLinePoints.size() > 0)
                        {
                            nameRdgIn = std::get<0>(m_st_rdgsWorkData.vectorRdgsData[m_st_rdgsWorkData.rdgsSurfWidth*inPosY+inPosX]);
                            kRdgIn    = std::get<1>(m_st_rdgsWorkData.vectorRdgsData[m_st_rdgsWorkData.rdgsSurfWidth*inPosY+inPosX]);

                            nameRdgFn = std::get<0>(m_st_rdgsWorkData.vectorRdgsData[m_st_rdgsWorkData.rdgsSurfWidth*fnPosY+fnPosX]);
                            kRdgFn    = std::get<1>(m_st_rdgsWorkData.vectorRdgsData[m_st_rdgsWorkData.rdgsSurfWidth*fnPosY+fnPosX]);

                            int countIn = 0;
                            int countFn = vectorRdgsTransLinePoints.size()-1;

                            while (countIn < countFn)
                            {
                                if ((std::get<0>(m_st_rdgsWorkData.vectorRdgsData[
                                    m_st_rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[countIn].second + vectorRdgsTransLinePoints[countIn].first
                                ]) == "") &&
                                (std::get<1>(m_st_rdgsWorkData.vectorRdgsData[
                                    m_st_rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[countIn].second + vectorRdgsTransLinePoints[countIn].first
                                ]) == -1)
                                ) {
                                    m_st_rdgsWorkData.vectorRdgsData[
                                        m_st_rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[countIn].second + vectorRdgsTransLinePoints[countIn].first
                                    ] = std::make_tuple(nameRdgIn, kRdgIn);
                                }

                                if ((std::get<0>(m_st_rdgsWorkData.vectorRdgsData[
                                    m_st_rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[countFn].second + vectorRdgsTransLinePoints[countFn].first
                                ]) == "") &&
                                (std::get<1>(m_st_rdgsWorkData.vectorRdgsData[
                                    m_st_rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[countFn].second + vectorRdgsTransLinePoints[countFn].first
                                ]) == -1)
                                ) {
                                    m_st_rdgsWorkData.vectorRdgsData[
                                        m_st_rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[countFn].second + vectorRdgsTransLinePoints[countFn].first
                                    ] = std::make_tuple(nameRdgFn, kRdgFn);
                                }

                                countIn++;
                                countFn--;
                            }
                        }
                    }

                    if (numActRdg == 0)
                    {
                        inPosX = rdgsPointsVector[count1  ][minRdgWidth-1].first;
                        inPosY = rdgsPointsVector[count1  ][minRdgWidth-1].second;

                        for (int count2 = minRdgWidth; count2 < rdgsPointsVector[count1+1].size(); count2++)
                        {
                            int deltaPosX = rdgsPointsVector[count1+1][count2].first  - fnPosX;
                            int deltaPosY = rdgsPointsVector[count1+1][count2].second - fnPosY;

                            inPosX = inPosX + deltaPosX;
                            inPosY = inPosY + deltaPosY;

                            fnPosX = rdgsPointsVector[count1+1][count2].first;
                            fnPosY = rdgsPointsVector[count1+1][count2].second;

                            representRdgLine((inPosX+fnPosX)/2, (inPosY+ fnPosY)/2, fnPosX, fnPosY,  m_st_rdgsWorkData, true);
                        }
                    }
                    else
                    {
                        fnPosX = rdgsPointsVector[count1+1  ][minRdgWidth-1].first;
                        fnPosY = rdgsPointsVector[count1+1  ][minRdgWidth-1].second;

                        for (int count2 = minRdgWidth; count2 < rdgsPointsVector[count1].size(); count2++)
                        {
                            int deltaPosX = rdgsPointsVector[count1][count2].first  - inPosX;
                            int deltaPosY = rdgsPointsVector[count1][count2].second - inPosY;

                            fnPosX = fnPosX + deltaPosX;
                            fnPosY = fnPosY + deltaPosY;

                            inPosX = rdgsPointsVector[count1][count2].first;
                            inPosY = rdgsPointsVector[count1][count2].second;

                            representRdgLine(inPosX, inPosY, (inPosX + fnPosX)/2, (inPosY + fnPosY)/2,  m_st_rdgsWorkData, false);
                        }
                    }

                    emit sendProgressData(m_st_rdgsWorkData.rdgsSurfWidth + count1, m_st_rdgsWorkData.rdgsSurfWidth + rdgNamesVector.size() - 1);
                }

                for (int count = 0; count < rdgsPointsVector[rdgsPointsVector.size()-1].size(); count++)
                {
                    fnPosX = rdgsPointsVector[rdgsPointsVector.size()-1][count].first;
                    fnPosY = rdgsPointsVector[rdgsPointsVector.size()-1][count].second;
                    representRdgLine( fnPosX, fnPosY, fnPosX, m_st_rdgsWorkData.rdgsSurfHeight-2,  m_st_rdgsWorkData, false);
                }

                defMinMaxRdgsRelief(m_st_rdgsWorkData.vectorRdgsFnRelief, m_st_rdgsWorkData.maxRelief, m_st_rdgsWorkData.minRelief);
                emit sendProgressData(m_st_rdgsWorkData.rdgsSurfWidth + rdgNamesVector.size() -1, m_st_rdgsWorkData.rdgsSurfWidth + rdgNamesVector.size() - 1);

                delete rdgsPoints;
                emit sendSurfRdgsWorkData();
            }
            else
            {
                int width  = m_st_rdgsWorkData.vectorRdgsInRelief.size();
                int height = m_st_rdgsWorkData.vectorRdgsInRelief[0].size();

                double latitude = 0.0;
                double longitude = 0.0;

                double latitude1  = 0.0;
                double longitude1 = 0.0;
                double latitude2  = 0.0;
                double longitude2 = 0.0;
                double latitude3  = 0.0;
                double longitude3 = 0.0;
                double latitude4  = 0.0;
                double longitude4 = 0.0;

                for (int count1 = 0; count1 < m_st_rdgsWorkData.rdgsSurfWidth; count1++)
                {
                    for (int count2 = 0; count2 < m_st_rdgsWorkData.rdgsSurfHeight; count2++)
                    {
                        latitude  = m_st_rdgsWorkData.leftLatitude + count1*m_st_rdgsWorkData.deltaLatitude;
                        longitude = m_st_rdgsWorkData.lowLongitude + count2*m_st_rdgsWorkData.deltaLongitude;
                        double z = 0.0;

                        bool defZ = false;
                        for (int count3 = 0; count3 < width-1; count3++)
                        {
                            for (int count4 = 0; count4 < height-1; count4++)
                            {
                                if (defZ == false)
                                {
                                    latitude1  = m_st_rdgsWorkData.vectorRdgsInRelief[count3][count4].x();
                                    longitude1 = m_st_rdgsWorkData.vectorRdgsInRelief[count3][count4].y();

                                    latitude2  = m_st_rdgsWorkData.vectorRdgsInRelief[count3][count4+1].x();
                                    longitude2 = m_st_rdgsWorkData.vectorRdgsInRelief[count3][count4+1].y();

                                    latitude3  = m_st_rdgsWorkData.vectorRdgsInRelief[count3+1][count4+1].x();
                                    longitude3 = m_st_rdgsWorkData.vectorRdgsInRelief[count3+1][count4+1].y();

                                    latitude4  = m_st_rdgsWorkData.vectorRdgsInRelief[count3+1][count4].x();
                                    longitude4 = m_st_rdgsWorkData.vectorRdgsInRelief[count3+1][count4].y();

                                    if ((latitude  >= latitude1  && latitude  <= latitude3) && (longitude >= longitude1 && longitude <= longitude3))
                                    {
                                        z = (
                                            fabs((latitude3-latitude)*(longitude3-longitude))*(m_st_rdgsWorkData.vectorRdgsInRelief[count3][count4].z())     +
                                            fabs((latitude1-latitude)*(longitude1-longitude))*(m_st_rdgsWorkData.vectorRdgsInRelief[count3+1][count4+1].z()) +
                                            fabs((latitude2-latitude)*(longitude2-longitude))*(m_st_rdgsWorkData.vectorRdgsInRelief[count3+1][count4].z())   +
                                            fabs((latitude4-latitude)*(longitude4-longitude))*(m_st_rdgsWorkData.vectorRdgsInRelief[count3][count4+1].z())
                                        )/fabs((latitude3-latitude1)*(longitude3-longitude1));
                                        defZ = true;
                                        m_st_rdgsWorkData.vectorRdgsFnRelief[count1][count2] = z;
                                    }
                                }
                            }
                        }
                    }
                    emit sendProgressData(count1, m_st_rdgsWorkData.rdgsSurfWidth-1);
                }
                defMinMaxRdgsRelief(m_st_rdgsWorkData.vectorRdgsFnRelief, m_st_rdgsWorkData.maxRelief, m_st_rdgsWorkData.minRelief);
                emit sendRdgsZData();
            }
        }
        else
        {
            for (int i = 0; i < m_st_rdgsWorkData.rdgsSurfWidth; i++)
            {
                for (int j = 0; j < m_st_rdgsWorkData.rdgsSurfHeight; j++)
                {
                    if (std::get<0>(m_st_rdgsWorkData.vectorRdgsData[m_st_rdgsWorkData.rdgsSurfWidth*j+i]) == m_nameDeleteRdg)
                    m_st_rdgsWorkData.vectorRdgsData[m_st_rdgsWorkData.rdgsSurfWidth*j + i] = std::make_tuple("", -1);
                }
            }
            emit sendSurfRdgsWorkData();
            m_nameDeleteRdg    = "";
            m_executeDeleteRdg = false;
        }
    }
}

void accomplishmentThread::clearWorkData()
{
    m_st_rdgsWorkData.rdgsSurfWidth  = 0;
    m_st_rdgsWorkData.rdgsSurfHeight = 0;

    m_st_rdgsWorkData.leftLatitude  = -1.0;
    m_st_rdgsWorkData.rightLatitude = -1.0;
    m_st_rdgsWorkData.lowLongitude  = -1.0;
    m_st_rdgsWorkData.upLongitude   = -1.0;

    m_st_rdgsWorkData.deltaLatitude  = 0.0;
    m_st_rdgsWorkData.deltaLongitude = 0.0;

    m_rdgsInfoDataMap.clear();
    m_st_rdgsWorkData.vectorRdgsFnRelief.clear();
    m_st_rdgsWorkData.vectorRdgsData.clear();
    m_st_rdgsWorkData.vectorRdgsInRelief.clear();
}

void accomplishmentThread::tracingOfDefiningRdgFilterData(const std::vector<std::string>& rdgNamesVector)
{
    for (auto iter = m_rdgsInfoDataMap.begin(); iter != m_rdgsInfoDataMap.end(); iter++)
    {
        auto findingRdg{ std::find(begin(rdgNamesVector), end(rdgNamesVector), iter->first) };
        if (findingRdg == end(rdgNamesVector))
        {
            definingRdgFilterData(m_rdgsInfoDataMap[iter->first], 1);

            for (int count = 0; count <= m_rdgsInfoDataMap[iter->first].vectorRdgData.size()-1; count++)
            {
                allocateMainRdgContainers(m_rdgsInfoDataMap[iter->first], count);
                for (int k = 0; k <= countFilters-1; k++)
                    createRdgDataMinMaxImpulses(m_rdgsInfoDataMap[iter->first], count, m_rdgsInfoDataMap[iter->first].vectorRdgData[count].vectorsDoubleData[k].size(), k);
            }
        }
    }
}

void accomplishmentThread::tracingOfDefiningRdgMaterialData(const std::vector<std::string>& rdgNamesVector, int materialId)
{
    for (auto iter = m_rdgsInfoDataMap.begin(); iter != m_rdgsInfoDataMap.end(); iter++)
    {
        auto findingRdg{ std::find(begin(rdgNamesVector), end(rdgNamesVector), iter->first) };
        if (findingRdg == end(rdgNamesVector))
        {
            for (int count = 0; count <= m_rdgsInfoDataMap[iter->first].vectorRdgData.size()-1; count++)
            {
                for (int k = 0; k <= countFilters-1; k++)
                    createRdgDataDeeps(m_rdgsInfoDataMap[iter->first], count, m_rdgsInfoDataMap[iter->first].vectorRdgData[count].vectorsDoubleData[k].size(), k, materialId);
            }
        }
    }
}
