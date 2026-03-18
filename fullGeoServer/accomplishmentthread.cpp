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
    m_bscansInfoDataMap.clear();
}

accomplishmentThread::~accomplishmentThread(){}

void accomplishmentThread::run()
{
    std::vector<std::string> bscanNamesVector;
    bscanNamesVector.resize(0);
    if (m_bscansInfoDataMap.size() != 0)
    for (auto mapIter = m_bscansInfoDataMap.begin(); mapIter != m_bscansInfoDataMap.end(); mapIter++) { bscanNamesVector.push_back(mapIter->first);}

    if (m_executeParserData == false)
    {
        if (m_trzDataSampling == true)
        {
            for (int count = 0; count <= m_dataTrzList.size()-1; count++)
            {
                int quantImpulsesOfPacket = (m_dataTrzList[count].size() - m_parserDataTrzVectors[count][m_parserDataTrzVectors[count].size()-1].second-9)/8;
                int fullBscanTrackCounts    = m_parserDataTrzVectors[count].size();

                for (int i = 0; i < fullBscanTrackCounts; i++)
                {
                    std::string TRZ_HEADER_string;
                    TRZ_HEADER_string = std::string(m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first, 3);

                    bool antennaExist = false;
                    int antenna_number     = *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 13));

                    if (m_trzNumAntennasVector.size() == 0) antennaExist = true;
                    else if (std::find(m_trzNumAntennasVector.begin(), m_trzNumAntennasVector.end(), antenna_number) != m_trzNumAntennasVector.end())  antennaExist = true;

                    if (antennaExist == true)
                    {
                        std::string bscanInfoDataKey="";
                        if (antenna_number< 10)   bscanInfoDataKey = m_fileNameTrzVector[count] + "&&" + "0" + std::to_string(antenna_number);
                        else                      bscanInfoDataKey = m_fileNameTrzVector[count] + "&&" +       std::to_string(antenna_number);

                        auto findingBscan{ std::find(begin(bscanNamesVector), end(bscanNamesVector), bscanInfoDataKey) };
                        if (findingBscan == end(bscanNamesVector))
                        {
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.resize(m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()+1);

                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].header =
                            TRZ_HEADER_string.data();
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].version_number     =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 3));
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].block_count        =
                            correctInt64TValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 4);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].antennas_working   =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 12));
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].block_step_size_m  =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 14);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].antennas_step_size =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 22);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_step_ns       =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 30);

                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].freq_step_GHz      =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 38);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].freq_begin_GHz     =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 46);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].freq_end_GHz       =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 54);

                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].latitude_degree    =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 62);

                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].longitude_degree   =
                            correctDoubleValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first, i, 70);

                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_hours         =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 78));  // время час
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_minutes       =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 79));  // время минута
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_seconds       =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 80));
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_milliseconds  =
                            correctInt16TValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first,  i, 81);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].day                =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 83));
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].month              =
                            *((int8_t*) (m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].first + 84));  // месяц
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].year               =
                            correctInt16TValue(m_dataTrzList[count],   m_parserDataTrzVectors[count][i].first,  i, 85);// год

                            std::string SGL_HEADER_string;
                            SGL_HEADER_string = std::string(m_dataTrzList[count].data() + m_parserDataTrzVectors[count][i].second, 3);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].signal_character = SGL_HEADER_string.data();

                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].begin_signal_index =
                            correctInt16TValue(m_dataTrzList[count], m_parserDataTrzVectors[count][i].second,  i, 3);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].end_signal_index   =
                            correctInt16TValue(m_dataTrzList[count], m_parserDataTrzVectors[count][i].second,  i, 5);
                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].signal_length      =
                            correctInt16TValue(m_dataTrzList[count], m_parserDataTrzVectors[count][i].second,  i, 7);

                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].vectorsDoubleData.resize(countFilters);
                            for (int j=0; j < quantImpulsesOfPacket; j++)
                            {
                                for (int k = 0; k <= countFilters-1; k++)
                                {
                                    m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].vectorsDoubleData[k].
                                    push_back(correctDoubleValue(m_dataTrzList[count], m_parserDataTrzVectors[count][i].second, i, 9+8*j)/(double)(k+1));
                                }
                            }

                        }
                    }

                    emit sendProgressData(count, m_dataTrzList.size()+2+countMaterials);
                }

                //Занимаемся фильтрами и веществами
                tracingOfDefiningBscanFilterData(bscanNamesVector);
                emit sendProgressData(m_dataTrzList.size(), m_dataTrzList.size()+2+countMaterials);
                
                for (int count = 0; count <= countMaterials-1; count++)
                {
                    tracingOfDefiningBscanMaterialData(bscanNamesVector, count);
                    emit sendProgressData(m_dataTrzList.size() + count + 1, m_dataTrzList.size()+2+countMaterials);
                }

                defAdditionalBscanMainData(m_bscansInfoDataMap, quantImpulsesOfPacket);
                emit sendProgressData(m_dataTrzList.size()+1+countMaterials, m_dataTrzList.size()+2+countMaterials);

                correctBscanData(m_bscansInfoDataMap, m_st_bscansWorkData, m_bscansNamesVectorPairs, m_fullFileNameTrzVector[count]);
                emit sendProgressData(m_dataTrzList.size()+2+countMaterials, m_dataTrzList.size()+2+countMaterials);
            }

            emit signalEndAccThread(m_bscansNamesVectorPairs, BscanFileFormat::Trz);
        }

        if (m_csvDataSampling == true)
        {
            for (int count = 0; count < m_dataCsvList.size(); count++)
            {
                int quantImpulsesOfPacket = 0;

                if (m_dataPropsCsvList[count].split('\n').size() == m_dataCsvList[count].split('\n').size() + 1)
                {
                    std::string bscanInfoDataKey="";
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
                                    if (bscanInfoDataKey == "")  bscanInfoDataKey = m_fileNameDataCsvVector[count];

                                    auto findingBscan{ std::find(begin(bscanNamesVector), end(bscanNamesVector), bscanInfoDataKey) };
                                    if (findingBscan == end(bscanNamesVector))
                                    {
                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.resize(m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()+1);

                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].block_count        =
                                        (line.split(";").at(0)).toInt();

                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].block_step_size_m  =
                                        (line.split(";").at(1)).toDouble();

                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].antennas_step_size =
                                        (line.split(";").at(3)).toDouble();

                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_step_ns       =
                                        (line.split(";").at(4)).toDouble();

                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_hours         =
                                        (line.split(";").at(7)).toInt();  // время час
                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_minutes       =
                                        (line.split(";").at(8)).toInt();  // время минута
                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_seconds       =
                                        (line.split(";").at(9)).toInt();
                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_milliseconds  =
                                        (line.split(";").at(10)).toInt();

                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].day                =
                                        (line.split(";").at(11)).toInt();
                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].month              =
                                        (line.split(";").at(12)).toInt();  // месяц
                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].year               =
                                        (line.split(";").at(13)).toInt();// год

                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].latitude_degree    =
                                        (line.split(";").at(14)).toDouble();

                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].longitude_degree   =
                                        (line.split(";").at(15)).toDouble();
                                    }
                                }
                            }
                            count1++;
                        }
                    }

                    auto findingBscan{ std::find(begin(bscanNamesVector), end(bscanNamesVector), bscanInfoDataKey) };
                    if (findingBscan == end(bscanNamesVector))
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
                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[count2].vectorsDoubleData.resize(countFilters);

                                        for (int k = 0; k <= countFilters-1; k++)
                                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[count2].vectorsDoubleData[k].push_back((line.split(";").at(count3)).toInt()/(double)(k+1));
                                    }

                                    emit sendProgressData(count2+1, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);
                                    count2++;
                                }
                            }
                        }
                    }

                    /////////////////////////////
                    std::string bscanFilesFullData = m_fullFileNameDataCsvVector[count] + "&&&&" + m_fullFileNamePropsCsvVector[count];

                    //Занимаемся фильтрами и материалами
                    tracingOfDefiningBscanFilterData(bscanNamesVector);
                    emit sendProgressData(m_dataPropsCsvList[count].split('\n').size()+3, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);
                    
                    for (int count1 = 0; count1 <= countMaterials-1; count1++)
                    {
                        tracingOfDefiningBscanMaterialData(bscanNamesVector, count1);
                        emit sendProgressData(m_dataPropsCsvList[count].split('\n').size()+4+count1, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);
                    }

                    defAdditionalBscanMainData(m_bscansInfoDataMap, quantImpulsesOfPacket);
                    emit sendProgressData(m_dataPropsCsvList[count].split('\n').size()+4+countMaterials, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);

                    correctBscanData(m_bscansInfoDataMap, m_st_bscansWorkData, m_bscansNamesVectorPairs, bscanFilesFullData);
                    emit sendProgressData(m_dataPropsCsvList[count].split('\n').size()+5+countMaterials, m_dataPropsCsvList[count].split('\n').size()+5+countMaterials);
                }
            }

            emit signalEndAccThread(m_bscansNamesVectorPairs,  BscanFileFormat::Csv);
        }

        if (m_hdf5DataSampling == true)
        {
            std::string bscanInfoDataKey =  "";
            for (int count = 0; count < m_filesBscanNamesHdf5Vectors.size(); count++ )
            {
                int quantImpulsesOfPacket = 0;
                for (int count1 = 0; count1 < m_filesBscanNamesHdf5Vectors[count].size(); count1++)
                {
                    try
                    {
                        // Открытие файла
                        H5::H5File file(m_filesBscanNamesHdf5Vectors[count][count1], H5F_ACC_RDONLY);

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
                                for (int count2 = 0; count2 <= (QString::fromStdString(m_filesBscanNamesHdf5Vectors[count][count1]).split("/")).size()-2; count2++)
                                    rootPath +=  (QString::fromStdString(m_filesBscanNamesHdf5Vectors[count][count1]).split("/")).at(count2)+"/";

                                bscanInfoDataKey = rootPath.toStdString();

                                auto findingBscan{ std::find(begin(bscanNamesVector), end(bscanNamesVector), bscanInfoDataKey) };
                                if (findingBscan == end(bscanNamesVector))
                                {
                                    hsize_t dims[1];
                                    dataspace.getSimpleExtentDims(dims, NULL);

                                    m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.resize(m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()+1);

                                    //Чтение данных
                                    std::vector<double> flat_data(dims[0]);
                                    dataset.read(flat_data.data(), H5::PredType::NATIVE_DOUBLE);
                                    quantImpulsesOfPacket = dims[0];

                                    // Преобразование в 2D вектор
                                    for (hsize_t i = 0; i < quantImpulsesOfPacket; i++)
                                    {
                                        m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].vectorsDoubleData.resize(countFilters);

                                        for (int k = 0; k <= countFilters-1; k++)
                                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].vectorsDoubleData[k].push_back(flat_data[i]/(double)(k+1));
                                    }

                                    //Чтение данных и атрибутов
                                    H5::Group rx_group = file.openGroup("/");
                                    //Чтение атрибутов
                                    if (rx_group.attrExists("dt"))
                                    {
                                        H5::Attribute attr = rx_group.openAttribute("dt");
                                        attr.read(H5::PredType::NATIVE_DOUBLE, &m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.size()-1].time_step_ns);
                                    }
                                    rx_group.close();

                                    dataset.close();
                                    dataspace.close();
                                }
                            }

                            if (rank == 2)
                            {
                                bscanInfoDataKey = m_filesBscanNamesHdf5Vectors[count][count1];

                                auto findingBscan{ std::find(begin(bscanNamesVector), end(bscanNamesVector), bscanInfoDataKey) };
                                if (findingBscan == end(bscanNamesVector))
                                {
                                    hsize_t dims[2];
                                    dataspace.getSimpleExtentDims(dims, NULL);

                                    //Чтение данных
                                    std::vector<double> flat_data(dims[0]*dims[1]);
                                    dataset.read(flat_data.data(), H5::PredType::NATIVE_DOUBLE);

                                    quantImpulsesOfPacket = dims[0];
                                    m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData.resize(dims[1]);

                                    for (hsize_t i = 0; i < dims[0]; i++)
                                    {
                                        for (hsize_t j = 0; j < dims[1]; j++)
                                        {
                                            m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[j].vectorsDoubleData.resize(countFilters);

                                            for (int k = 0; k <= countFilters-1; k++)
                                                m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[j].vectorsDoubleData[k].push_back(flat_data[i * dims[1] + j]/(double)(k+1));
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
                                            attr.read(H5::PredType::NATIVE_DOUBLE, &m_bscansInfoDataMap[bscanInfoDataKey].vectorBscanData[j].time_step_ns);
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
                emit sendProgressData(0, m_filesBscanNamesHdf5Vectors[count].size()+3+countMaterials);

                if (bscanInfoDataKey != "")
                {
                    //Занимаемся фильтрами
                    tracingOfDefiningBscanFilterData(bscanNamesVector);
                    emit sendProgressData( m_filesBscanNamesHdf5Vectors[count].size(), m_filesBscanNamesHdf5Vectors[count].size()+3+countMaterials);

                    for (int count1 = 0; count1 <= countMaterials-1; count1++)
                    {
                        tracingOfDefiningBscanMaterialData(bscanNamesVector, count1);
                        emit sendProgressData(m_filesBscanNamesHdf5Vectors[count].size()+2+count1+1, m_filesBscanNamesHdf5Vectors[count].size()+3+countMaterials);
                    }

                    defAdditionalBscanMainData(m_bscansInfoDataMap,  quantImpulsesOfPacket);
                    emit sendProgressData(m_filesBscanNamesHdf5Vectors[count].size()+2+countMaterials, m_filesBscanNamesHdf5Vectors[count].size()+3+countMaterials);

                    correctBscanData(m_bscansInfoDataMap, m_st_bscansWorkData, m_bscansNamesVectorPairs, m_fullFileNameHdf5Vector[count]);
                    emit sendProgressData(m_filesBscanNamesHdf5Vectors[count].size()+3+countMaterials, m_filesBscanNamesHdf5Vectors[count].size()+3+countMaterials);
                }
            }

            emit signalEndAccThread(m_bscansNamesVectorPairs,  BscanFileFormat::Hdf5);
        }
    }
    else
    {
        if (m_executeDeleteBscan == false)
        {
            if (m_defBscansFnRelief == false)
            {
                m_st_bscansWorkData.rightLatitude = -1.0;
                m_st_bscansWorkData.deltaLatitude  = 0.0;
                m_st_bscansWorkData.deltaLongitude = 0.0;

                devMainParameters(
                    m_bscansInfoDataMap, m_st_bscansWorkData.leftLatitude,
                    m_st_bscansWorkData.rightLatitude, m_st_bscansWorkData.lowLongitude, m_st_bscansWorkData.upLongitude,
                    m_st_bscansWorkData.deltaLatitude, m_st_bscansWorkData.deltaLongitude,
                    m_st_bscansWorkData.bscansSurfWidth, m_st_bscansWorkData.bscansSurfHeight
                );

                double* bscansPoints;
                if (m_st_bscansWorkData.bscansSurfWidth > m_st_bscansWorkData.bscansSurfHeight) bscansPoints = new double[m_st_bscansWorkData.bscansSurfWidth];
                else                                                                    bscansPoints = new double[m_st_bscansWorkData.bscansSurfHeight];


                m_st_bscansWorkData.vectorBscansData.resize(m_st_bscansWorkData.bscansSurfWidth*m_st_bscansWorkData.bscansSurfHeight);

                m_st_bscansWorkData.vectorBscansFnRelief.resize(m_st_bscansWorkData.bscansSurfWidth);
                for (int i = 0; i < m_st_bscansWorkData.bscansSurfWidth; i++)
                m_st_bscansWorkData.vectorBscansFnRelief[i].resize(m_st_bscansWorkData.bscansSurfHeight);

                for (int i = 0; i < m_st_bscansWorkData.bscansSurfWidth; i++)
                {
                    for (int j = 0; j < m_st_bscansWorkData.bscansSurfHeight; j++)
                    {
                        m_st_bscansWorkData.vectorBscansFnRelief[i][j] = 0.0;
                    }
                }

                std::vector<std::string> bscanNamesVector;
                for (auto mapIter = m_bscansInfoDataMap.begin(); mapIter != m_bscansInfoDataMap.end(); mapIter++) { bscanNamesVector.push_back(mapIter->first);}

                for (int i = 0; i < m_st_bscansWorkData.bscansSurfWidth; i++)
                {
                    for (int j = 0; j < m_st_bscansWorkData.bscansSurfHeight; j++)
                    {
                        m_st_bscansWorkData.vectorBscansData[m_st_bscansWorkData.bscansSurfWidth*j + i] = defTupleBscans(
                        m_bscansInfoDataMap, i, j,
                        m_st_bscansWorkData.leftLatitude, m_st_bscansWorkData.rightLatitude,
                        m_st_bscansWorkData.lowLongitude, m_st_bscansWorkData.upLongitude,
                        m_st_bscansWorkData.deltaLatitude, m_st_bscansWorkData.deltaLongitude,
                        m_st_bscansWorkData.bscansSurfWidth, m_st_bscansWorkData.bscansSurfHeight,
                        bscansPoints);
                    }
                    emit sendProgressData(i, m_st_bscansWorkData.bscansSurfWidth + bscanNamesVector.size());
                }

                //делаем ступеньки
                std::vector<std::vector<std::pair<int, int>> > bscansPointsVector;
                bscansPointsVector.resize(bscanNamesVector.size());

                std::string nameBscan = "";
                std::pair<int, int> bscanPoint;

                for (int i = 0; i < m_st_bscansWorkData.bscansSurfWidth; i++)
                {
                    for (int j = 0; j < m_st_bscansWorkData.bscansSurfHeight; j++)
                    {
                        nameBscan = std::get<0>(m_st_bscansWorkData.vectorBscansData[m_st_bscansWorkData.bscansSurfWidth*j+i]);
                        if (nameBscan != "")
                        {
                            auto bscanFindIter = std::find(bscanNamesVector.begin(), bscanNamesVector.end(), nameBscan);
                            int bscanPosition = std::distance(bscanNamesVector.begin(), bscanFindIter);

                            bscanPoint.first = i;
                            bscanPoint.second = j;
                            bscansPointsVector[bscanPosition].push_back(bscanPoint);
                        }
                    }
                }

                m_st_bscansWorkData.bscansSurfMinWidth = 0;
                for (int i = 0; i <  bscansPointsVector.size(); i++)
                {
                    int iMin = 0;
                    int iMax = 0;

                    for (int j = 0; j < bscansPointsVector[i].size(); j++)
                    {
                        if (bscansPointsVector[i][j].first <= iMin) iMin = bscansPointsVector[i][j].first;
                        if (bscansPointsVector[i][j].first >= iMax) iMax = bscansPointsVector[i][j].first;
                    }

                    if (i == 0)                                                   m_st_bscansWorkData.bscansSurfMinWidth = iMax - iMin+1;
                    else if (iMax - iMin+1 <  m_st_bscansWorkData.bscansSurfMinWidth) m_st_bscansWorkData.bscansSurfMinWidth = iMax - iMin+1;
                }

                int inPosX = 0;
                int inPosY = 0;
                int fnPosX = 0;
                int fnPosY = 0;

                std::string nameBscanIn = "";
                int         kBscanIn    = 0;

                std::string nameBscanFn = "";
                int         kBscanFn    = 0;

                for (int count = 0; count < bscansPointsVector[0].size(); count++)
                {
                    inPosX = bscansPointsVector[0][count].first;
                    inPosY = bscansPointsVector[0][count].second;
                    representBscanLine( inPosX, 1, inPosX, inPosY, m_st_bscansWorkData, true);
                }

                std::vector<std::pair<int, int>> vectorBscansTransLinePoints;
                for (int count1 = 0; count1 < bscansPointsVector.size()-1; count1++)
                {
                    int numActBscan   = -1;
                    int minBscanWidth =  0;
                    if (bscansPointsVector[count1].size() <= bscansPointsVector[count1+1].size())
                    {
                        numActBscan = 0;
                        minBscanWidth = bscansPointsVector[count1].size();
                    }
                    else if  (bscansPointsVector[count1].size() > bscansPointsVector[count1+1].size())
                    {
                        numActBscan = 1;
                        minBscanWidth = bscansPointsVector[count1+1].size();
                    }

                    for (int count2 = 0; count2 < minBscanWidth; count2++)
                    {
                        inPosX = bscansPointsVector[count1  ][count2].first;
                        inPosY = bscansPointsVector[count1  ][count2].second;
                        fnPosX = bscansPointsVector[count1+1][count2].first;
                        fnPosY = bscansPointsVector[count1+1][count2].second;
                        defVectorBscansTransLineSectionPoints(inPosX, inPosY, fnPosX, fnPosY, vectorBscansTransLinePoints);
                        if (vectorBscansTransLinePoints.size() > 0)
                        {
                            nameBscanIn = std::get<0>(m_st_bscansWorkData.vectorBscansData[m_st_bscansWorkData.bscansSurfWidth*inPosY+inPosX]);
                            kBscanIn    = std::get<1>(m_st_bscansWorkData.vectorBscansData[m_st_bscansWorkData.bscansSurfWidth*inPosY+inPosX]);

                            nameBscanFn = std::get<0>(m_st_bscansWorkData.vectorBscansData[m_st_bscansWorkData.bscansSurfWidth*fnPosY+fnPosX]);
                            kBscanFn    = std::get<1>(m_st_bscansWorkData.vectorBscansData[m_st_bscansWorkData.bscansSurfWidth*fnPosY+fnPosX]);

                            int countIn = 0;
                            int countFn = vectorBscansTransLinePoints.size()-1;

                            while (countIn < countFn)
                            {
                                if ((std::get<0>(m_st_bscansWorkData.vectorBscansData[
                                    m_st_bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[countIn].second + vectorBscansTransLinePoints[countIn].first
                                ]) == "") &&
                                (std::get<1>(m_st_bscansWorkData.vectorBscansData[
                                    m_st_bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[countIn].second + vectorBscansTransLinePoints[countIn].first
                                ]) == -1)
                                ) {
                                    m_st_bscansWorkData.vectorBscansData[
                                        m_st_bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[countIn].second + vectorBscansTransLinePoints[countIn].first
                                    ] = std::make_tuple(nameBscanIn, kBscanIn);
                                }

                                if ((std::get<0>(m_st_bscansWorkData.vectorBscansData[
                                    m_st_bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[countFn].second + vectorBscansTransLinePoints[countFn].first
                                ]) == "") &&
                                (std::get<1>(m_st_bscansWorkData.vectorBscansData[
                                    m_st_bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[countFn].second + vectorBscansTransLinePoints[countFn].first
                                ]) == -1)
                                ) {
                                    m_st_bscansWorkData.vectorBscansData[
                                        m_st_bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[countFn].second + vectorBscansTransLinePoints[countFn].first
                                    ] = std::make_tuple(nameBscanFn, kBscanFn);
                                }

                                countIn++;
                                countFn--;
                            }
                        }
                    }

                    if (numActBscan == 0)
                    {
                        inPosX = bscansPointsVector[count1  ][minBscanWidth-1].first;
                        inPosY = bscansPointsVector[count1  ][minBscanWidth-1].second;

                        for (int count2 = minBscanWidth; count2 < bscansPointsVector[count1+1].size(); count2++)
                        {
                            int deltaPosX = bscansPointsVector[count1+1][count2].first  - fnPosX;
                            int deltaPosY = bscansPointsVector[count1+1][count2].second - fnPosY;

                            inPosX = inPosX + deltaPosX;
                            inPosY = inPosY + deltaPosY;

                            fnPosX = bscansPointsVector[count1+1][count2].first;
                            fnPosY = bscansPointsVector[count1+1][count2].second;

                            representBscanLine((inPosX+fnPosX)/2, (inPosY+ fnPosY)/2, fnPosX, fnPosY,  m_st_bscansWorkData, true);
                        }
                    }
                    else
                    {
                        fnPosX = bscansPointsVector[count1+1  ][minBscanWidth-1].first;
                        fnPosY = bscansPointsVector[count1+1  ][minBscanWidth-1].second;

                        for (int count2 = minBscanWidth; count2 < bscansPointsVector[count1].size(); count2++)
                        {
                            int deltaPosX = bscansPointsVector[count1][count2].first  - inPosX;
                            int deltaPosY = bscansPointsVector[count1][count2].second - inPosY;

                            fnPosX = fnPosX + deltaPosX;
                            fnPosY = fnPosY + deltaPosY;

                            inPosX = bscansPointsVector[count1][count2].first;
                            inPosY = bscansPointsVector[count1][count2].second;

                            representBscanLine(inPosX, inPosY, (inPosX + fnPosX)/2, (inPosY + fnPosY)/2,  m_st_bscansWorkData, false);
                        }
                    }

                    emit sendProgressData(m_st_bscansWorkData.bscansSurfWidth + count1, m_st_bscansWorkData.bscansSurfWidth + bscanNamesVector.size() - 1);
                }

                for (int count = 0; count < bscansPointsVector[bscansPointsVector.size()-1].size(); count++)
                {
                    fnPosX = bscansPointsVector[bscansPointsVector.size()-1][count].first;
                    fnPosY = bscansPointsVector[bscansPointsVector.size()-1][count].second;
                    representBscanLine( fnPosX, fnPosY, fnPosX, m_st_bscansWorkData.bscansSurfHeight-2,  m_st_bscansWorkData, false);
                }

                defMinMaxBscansRelief(m_st_bscansWorkData.vectorBscansFnRelief, m_st_bscansWorkData.maxRelief, m_st_bscansWorkData.minRelief);
                emit sendProgressData(m_st_bscansWorkData.bscansSurfWidth + bscanNamesVector.size() -1, m_st_bscansWorkData.bscansSurfWidth + bscanNamesVector.size() - 1);

                delete bscansPoints;
                emit sendSurfBscansWorkData();
            }
            else
            {
                int width  = m_st_bscansWorkData.vectorBscansInRelief.size();
                int height = m_st_bscansWorkData.vectorBscansInRelief[0].size();

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

                for (int count1 = 0; count1 < m_st_bscansWorkData.bscansSurfWidth; count1++)
                {
                    for (int count2 = 0; count2 < m_st_bscansWorkData.bscansSurfHeight; count2++)
                    {
                        latitude  = m_st_bscansWorkData.leftLatitude + count1*m_st_bscansWorkData.deltaLatitude;
                        longitude = m_st_bscansWorkData.lowLongitude + count2*m_st_bscansWorkData.deltaLongitude;
                        double z = 0.0;

                        bool defZ = false;
                        for (int count3 = 0; count3 < width-1; count3++)
                        {
                            for (int count4 = 0; count4 < height-1; count4++)
                            {
                                if (defZ == false)
                                {
                                    latitude1  = m_st_bscansWorkData.vectorBscansInRelief[count3][count4].x();
                                    longitude1 = m_st_bscansWorkData.vectorBscansInRelief[count3][count4].y();

                                    latitude2  = m_st_bscansWorkData.vectorBscansInRelief[count3][count4+1].x();
                                    longitude2 = m_st_bscansWorkData.vectorBscansInRelief[count3][count4+1].y();

                                    latitude3  = m_st_bscansWorkData.vectorBscansInRelief[count3+1][count4+1].x();
                                    longitude3 = m_st_bscansWorkData.vectorBscansInRelief[count3+1][count4+1].y();

                                    latitude4  = m_st_bscansWorkData.vectorBscansInRelief[count3+1][count4].x();
                                    longitude4 = m_st_bscansWorkData.vectorBscansInRelief[count3+1][count4].y();

                                    if ((latitude  >= latitude1  && latitude  <= latitude3) && (longitude >= longitude1 && longitude <= longitude3))
                                    {
                                        z = (
                                            fabs((latitude3-latitude)*(longitude3-longitude))*(m_st_bscansWorkData.vectorBscansInRelief[count3][count4].z())     +
                                            fabs((latitude1-latitude)*(longitude1-longitude))*(m_st_bscansWorkData.vectorBscansInRelief[count3+1][count4+1].z()) +
                                            fabs((latitude2-latitude)*(longitude2-longitude))*(m_st_bscansWorkData.vectorBscansInRelief[count3+1][count4].z())   +
                                            fabs((latitude4-latitude)*(longitude4-longitude))*(m_st_bscansWorkData.vectorBscansInRelief[count3][count4+1].z())
                                        )/fabs((latitude3-latitude1)*(longitude3-longitude1));
                                        defZ = true;
                                        m_st_bscansWorkData.vectorBscansFnRelief[count1][count2] = z;
                                    }
                                }
                            }
                        }
                    }
                    emit sendProgressData(count1, m_st_bscansWorkData.bscansSurfWidth-1);
                }
                defMinMaxBscansRelief(m_st_bscansWorkData.vectorBscansFnRelief, m_st_bscansWorkData.maxRelief, m_st_bscansWorkData.minRelief);
                emit sendBscansZData();
            }
        }
        else
        {
            for (int i = 0; i < m_st_bscansWorkData.bscansSurfWidth; i++)
            {
                for (int j = 0; j < m_st_bscansWorkData.bscansSurfHeight; j++)
                {
                    if (std::get<0>(m_st_bscansWorkData.vectorBscansData[m_st_bscansWorkData.bscansSurfWidth*j+i]) == m_nameDeleteBscan)
                    m_st_bscansWorkData.vectorBscansData[m_st_bscansWorkData.bscansSurfWidth*j + i] = std::make_tuple("", -1);
                }
            }
            emit sendSurfBscansWorkData();
            m_nameDeleteBscan    = "";
            m_executeDeleteBscan = false;
        }
    }
}

void accomplishmentThread::clearWorkData()
{
    m_st_bscansWorkData.bscansSurfWidth  = 0;
    m_st_bscansWorkData.bscansSurfHeight = 0;

    m_st_bscansWorkData.leftLatitude  = -1.0;
    m_st_bscansWorkData.rightLatitude = -1.0;
    m_st_bscansWorkData.lowLongitude  = -1.0;
    m_st_bscansWorkData.upLongitude   = -1.0;

    m_st_bscansWorkData.deltaLatitude  = 0.0;
    m_st_bscansWorkData.deltaLongitude = 0.0;

    m_bscansInfoDataMap.clear();
    m_st_bscansWorkData.vectorBscansFnRelief.clear();
    m_st_bscansWorkData.vectorBscansData.clear();
    m_st_bscansWorkData.vectorBscansInRelief.clear();
}

void accomplishmentThread::tracingOfDefiningBscanFilterData(const std::vector<std::string>& bscanNamesVector)
{
    for (auto iter = m_bscansInfoDataMap.begin(); iter != m_bscansInfoDataMap.end(); iter++)
    {
        auto findingBscan{ std::find(begin(bscanNamesVector), end(bscanNamesVector), iter->first) };
        if (findingBscan == end(bscanNamesVector))
        {
            definingBscanFilterData(m_bscansInfoDataMap[iter->first], 1);

            for (int count = 0; count <= m_bscansInfoDataMap[iter->first].vectorBscanData.size()-1; count++)
            {
                allocateMainBscanContainers(m_bscansInfoDataMap[iter->first], count);
                for (int k = 0; k <= countFilters-1; k++)
                    createBscanDataMinMaxImpulses(m_bscansInfoDataMap[iter->first], count, m_bscansInfoDataMap[iter->first].vectorBscanData[count].vectorsDoubleData[k].size(), k);
            }
        }
    }
}

void accomplishmentThread::tracingOfDefiningBscanMaterialData(const std::vector<std::string>& bscanNamesVector, int materialId)
{
    for (auto iter = m_bscansInfoDataMap.begin(); iter != m_bscansInfoDataMap.end(); iter++)
    {
        auto findingBscan{ std::find(begin(bscanNamesVector), end(bscanNamesVector), iter->first) };
        if (findingBscan == end(bscanNamesVector))
        {
            for (int count = 0; count <= m_bscansInfoDataMap[iter->first].vectorBscanData.size()-1; count++)
            {
                for (int k = 0; k <= countFilters-1; k++)
                    createBscanDataDeeps(m_bscansInfoDataMap[iter->first], count, m_bscansInfoDataMap[iter->first].vectorBscanData[count].vectorsDoubleData[k].size(), k, materialId);
            }
        }
    }
}
