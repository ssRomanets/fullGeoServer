#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "rdgsData.h"
#include "map"
#include "algorithm"
#include <fstream>
#include <csvfile.h>

#include <QString>
#include <QSettings>
#include <QVector>
#include <QVariant>
#include <QStringList>
#include <QPair>
#include <QPoint>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QColor>

#include <QIODevice>
#include <QFileInfoList>

#include <QOpenGLFunctions_3_3_Core>

#define countSelectors 3
#define countFilters   2
#define countMaterials 14

#define nanokoef 0.000000001
#define spc        300000000

#define LongitudeDim 500
#define percentsParam 100.00
#define deltaParam 0.0000001
#define deltaPixels 4
#define scaleFactor 80

#define limitRdgWidth 300

#define tFilterPM      10.00
#define deltaTFilterPM  0.20
#define kFilterPM       5.00

#define rdgMetricKoeff 0.000001

inline double epsdData(int materialId)
{
    double epsdData[14] {3.2, 81, 81, 2.6, 25, 2.5, 19, 2.4, 15, 8, 7, 7, 6, 8};
    return epsdData[materialId];
}

inline double gammaData(int materialId)
{
    double gammaData[14] {0.1, 0.18, 330, 0.14, 2.3, 0.11, 7.9, 0.28, 20, 5.6, 0.62, 45, 24, 14};
    return gammaData[materialId];
}

inline void createVectorRdgsPoints(const st_rdgInfoData& rdgInfoData, int i, double deltaLatitude, double deltaLongitude, double* rdgsPoints, int& dimRdgs)
{
    if (i == rdgInfoData.vectorRdgData.size()-1)
    {
        rdgsPoints[0] = rdgInfoData.vectorRdgData[i].latitude_degree;
        rdgsPoints[1] = rdgInfoData.vectorRdgData[i].longitude_degree;
        dimRdgs = 2;
    }
    else
    {
        double inPointLat   = rdgInfoData.vectorRdgData[i].latitude_degree;
        double inPointLong  = rdgInfoData.vectorRdgData[i].longitude_degree;
        double fnPointLat   = rdgInfoData.vectorRdgData[i+1].latitude_degree;
        double fnPointLong  = rdgInfoData.vectorRdgData[i+1].longitude_degree;

        int quantLat  = (int)fabs((fnPointLat-inPointLat)/deltaLatitude) + 1;
        int quantLong = (int)fabs((fnPointLong-inPointLong)/deltaLongitude) + 1;

        if (quantLat >= quantLong)
        {
            for (int count = 0; count <quantLat-1; count++)
            {
                double latitude  = inPointLat  + ((fnPointLat-inPointLat)/(fabs(fnPointLat-inPointLat)))*deltaLatitude*count;
                double longitude = inPointLong + (latitude-inPointLat)*(fnPointLong - inPointLong)/(fnPointLat - inPointLat);
                rdgsPoints[2*count]   = latitude;
                rdgsPoints[2*count+1] = longitude;
            }

            dimRdgs = 2*(quantLat-1);
        }
        else
        {
            for (int count = 0; count <quantLong-1; count++)
            {
                double longitude  = inPointLong  + ((fnPointLong-inPointLong)/(fabs(fnPointLong-inPointLong)))*deltaLongitude*count;
                double latitude = inPointLat + (longitude-inPointLong)*(fnPointLat - inPointLat)/(fnPointLong - inPointLong);
                rdgsPoints[2*count]   = latitude;
                rdgsPoints[2*count+1] = longitude;
            }

            dimRdgs = 2*(quantLong-1);
        }
    }
}

inline std::tuple<std::string, int>  defTupleRdgs(
        const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap,int i, int j,
        double leftLatitude, double rightLatitude, double lowLongitude, double upLongitude,
        double deltaLatitude, double deltaLongitude, int rdgsSurfWidth, int rdgsSurfHeight,
        double* rdgsPoints
    )
{
    int dimRdgs = 0;
    for (auto rdgsMapIter = rdgsInfoDataMap.begin(); rdgsMapIter != rdgsInfoDataMap.end(); rdgsMapIter++)
    {
        for (int k = 0; k < rdgsMapIter->second.vectorRdgData.size(); k++)
        {
            createVectorRdgsPoints(rdgsMapIter->second, k, deltaLatitude, deltaLongitude, rdgsPoints, dimRdgs);

            for (int l = 0; l < dimRdgs/2; l++)
            {
                if (i == 0 && j == 0)
                {
                    if (((rdgsPoints[2*l]   >= leftLatitude + deltaLatitude*i ) && (fabs(rdgsPoints[2*l]   - (leftLatitude + deltaLatitude*(i+1))) < deltaLatitude  )) &&
                        ( (rdgsPoints[2*l+1] >= lowLongitude + deltaLongitude*j) && (fabs(rdgsPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first, k);
                }
                else if (i == 0 && j == rdgsSurfHeight-1)
                {
                    if (((rdgsPoints[2*l]   >= leftLatitude + deltaLatitude*i     ) && (fabs(rdgsPoints[2*l] - (leftLatitude + deltaLatitude*(i+1))) < deltaLatitude )) &&
                        ((rdgsPoints[2*l+1] >  lowLongitude + deltaLongitude*(j-1)) && (fabs(rdgsPoints[2*l+1] - upLongitude)                        <= deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first, k);
                }
                else if (i == rdgsSurfWidth-1 && j == 0)
                {
                    if (((rdgsPoints[2*l]  > leftLatitude + deltaLatitude*(i-1)) && (fabs(rdgsPoints[2*l] - rightLatitude)                          <= deltaLatitude )) &&
                        ((rdgsPoints[2*l+1] >= lowLongitude + deltaLongitude*j)  && (fabs(rdgsPoints[2*l+1] -(lowLongitude + deltaLongitude*(j+1))) <  deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first, k);
                }
                else if (i == rdgsSurfWidth-1 && j == rdgsSurfHeight-1)
                {
                    if (((rdgsPoints[2*l] > leftLatitude + deltaLatitude*(i-1))    && (fabs(rdgsPoints[2*l] - rightLatitude) <= deltaLatitude )) &&
                        ((rdgsPoints[2*l+1] > lowLongitude + deltaLongitude*(j-1)) && (fabs(rdgsPoints[2*l+1] - upLongitude) <= deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first, k);
                }
                else if (i == 0 && (j != rdgsSurfHeight-1 && j != 0) )
                {
                    if (((rdgsPoints[2*l]   >= leftLatitude + deltaLatitude*i ) && (fabs(rdgsPoints[2*l] - (leftLatitude + deltaLatitude*(i+1))   ) < deltaLatitude)) &&
                        ((rdgsPoints[2*l+1] >= lowLongitude + deltaLongitude*j) && (fabs(rdgsPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first, k);
                }
                else if ((i != rdgsSurfWidth-1 && i != 0) && j == 0)
                {
                    if (((rdgsPoints[2*l]   >= leftLatitude + deltaLatitude*i ) && (fabs(rdgsPoints[2*l] - (leftLatitude + deltaLatitude*(i+1)))    < deltaLatitude )) &&
                        ((rdgsPoints[2*l+1] >= lowLongitude + deltaLongitude*j) && (fabs(rdgsPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first, k);
                }
                else if (i == rdgsSurfWidth-1 && (j != rdgsSurfHeight-1 && j != 0))
                {
                    if (((rdgsPoints[2*l]   > leftLatitude + deltaLatitude*(i-1)) && (fabs(rdgsPoints[2*l] - rightLatitude)                           <= rightLatitude)) &&
                        ((rdgsPoints[2*l+1] >= lowLongitude + deltaLongitude*j  ) && (fabs(rdgsPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first,  k);
                }
                else if ((i != rdgsSurfWidth-1 && i != 0) && j == rdgsSurfHeight-1)
                {
                    if (((rdgsPoints[2*l]   >= leftLatitude + deltaLatitude*i)     && (fabs(rdgsPoints[2*l] - (leftLatitude + deltaLatitude*(i+1))) <  deltaLatitude) ) &&
                        ((rdgsPoints[2*l+1] > lowLongitude + deltaLongitude*(j-1)) && (fabs(rdgsPoints[2*l+1] - upLongitude)                        <= deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first, k);
                }
                else
                {
                    if ( ((rdgsPoints[2*l]   >= leftLatitude + deltaLatitude*i ) && (fabs(rdgsPoints[2*l] - (leftLatitude + deltaLatitude*(i+1))   ) < deltaLatitude)) &&
                         ((rdgsPoints[2*l+1] >= lowLongitude + deltaLongitude*j) && (fabs(rdgsPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple(rdgsMapIter->first, k);
                }
            }
        }
    }
    return std::make_tuple("", -1);
}

inline void defLeftLatitude(const st_rdgInfoData& rdgInfoData, int i, double& leftLatitude)
{
    if (leftLatitude < 0.0)                                               leftLatitude = rdgInfoData.vectorRdgData[i].latitude_degree;
    else if (rdgInfoData.vectorRdgData[i].latitude_degree < leftLatitude) leftLatitude = rdgInfoData.vectorRdgData[i].latitude_degree;
}

inline void defRightLatitude(const st_rdgInfoData& rdgInfoData, int i, double& rightLatitude)
{
    if (rightLatitude < 0.0)                                               rightLatitude = rdgInfoData.vectorRdgData[i].latitude_degree;
    else if (rdgInfoData.vectorRdgData[i].latitude_degree > rightLatitude) rightLatitude = rdgInfoData.vectorRdgData[i].latitude_degree;
}

inline void defLowLongitude(const st_rdgInfoData& rdgInfoData, int i,  double& lowLongitude)
{
    if (lowLongitude < 0.0)  lowLongitude  = rdgInfoData.vectorRdgData[i].longitude_degree;
    else if (rdgInfoData.vectorRdgData[i].longitude_degree < lowLongitude) lowLongitude = rdgInfoData.vectorRdgData[i].longitude_degree;
}

inline void defUpLongitude(const st_rdgInfoData& rdgInfoData, int i,  double& upLongitude)
{
    if (upLongitude < 0.0)   upLongitude   = rdgInfoData.vectorRdgData[i].longitude_degree;
    else if (rdgInfoData.vectorRdgData[i].longitude_degree > upLongitude) upLongitude = rdgInfoData.vectorRdgData[i].longitude_degree;
}

inline void devMainLatLongParameters(const st_rdgInfoData& rdgInfoData,int i, double& leftLatitude, double& rightLatitude, double& lowLongitude, double& upLongitude)
{
    defLeftLatitude (rdgInfoData, i, leftLatitude);
    defRightLatitude(rdgInfoData, i, rightLatitude);
    defLowLongitude (rdgInfoData, i, lowLongitude);
    defUpLongitude  (rdgInfoData, i, upLongitude);
}

inline void devMainParameters(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap,
    double leftLatitude, double& rightLatitude,  double lowLongitude, double upLongitude,
    double& deltaLatitude, double& deltaLongitude, int& rdgsSurfWidth, int& rdgsSurfHeight
)
{
    std::vector<double> variousLatitudesVector;
    std::vector<double> variousLongitidesVector;
    variousLatitudesVector.resize(0);
    variousLongitidesVector.resize(0);

    for (auto rdgsMapIter = rdgsInfoDataMap.begin(); rdgsMapIter != rdgsInfoDataMap.end(); rdgsMapIter++)
    {
        for (int i = 0; i <  rdgsMapIter->second.vectorRdgData.size(); i++)
        {
            if (variousLatitudesVector.size() == 0) variousLatitudesVector.push_back(rdgsMapIter->second.vectorRdgData[i].latitude_degree);
            else if (
                std::find(variousLatitudesVector.begin(), variousLatitudesVector.end(), rdgsMapIter->second.vectorRdgData[i].latitude_degree)   == variousLatitudesVector.end()
            ) {  variousLatitudesVector.push_back(rdgsMapIter->second.vectorRdgData[i].latitude_degree);}

            if (variousLongitidesVector.size() == 0) variousLongitidesVector.push_back(rdgsMapIter->second.vectorRdgData[i].longitude_degree);
            else if (
                std::find(variousLongitidesVector.begin(), variousLongitidesVector.end(), rdgsMapIter->second.vectorRdgData[i].longitude_degree) == variousLongitidesVector.end()
            ) {  variousLongitidesVector.push_back(rdgsMapIter->second.vectorRdgData[i].longitude_degree);}

            defRightLatitude(rdgsMapIter->second, i, rightLatitude);
        }
    }

    std::vector<double> variousDiffLatitudesVector;
    std::vector<double> variousDiffLongitidesVector;
    variousDiffLatitudesVector.resize(0);
    variousDiffLongitidesVector.resize(0);

    for (int i = 0; i < variousLatitudesVector.size() -1; i++) { variousDiffLatitudesVector.push_back(fabs(variousLatitudesVector[i+1] - variousLatitudesVector[i]));}
    for (int i = 0; i < variousLongitidesVector.size()-1; i++) { variousDiffLongitidesVector.push_back(variousLongitidesVector   [i+1] - variousLongitidesVector[i]);}

    deltaLatitude = *std::min_element(std::begin(variousDiffLatitudesVector), std::end(variousDiffLatitudesVector));
    rdgsSurfWidth  = (int)( ((rightLatitude - leftLatitude)/deltaLatitude) + 1);

    deltaLongitude = *std::min_element(std::begin(variousDiffLongitidesVector), std::end(variousDiffLongitidesVector));
    rdgsSurfHeight = (int)( ((upLongitude - lowLongitude)/deltaLongitude)  + 1);

    if (rdgsSurfHeight < LongitudeDim) rdgsSurfHeight = LongitudeDim; deltaLongitude = (upLongitude   - lowLongitude)/(rdgsSurfHeight -1);
}


inline void defMaxDeep(
    const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, int materialId, int filterId, int selectionId, double& maxDeep, int absRdgsInX,int absRdgsFnX
)
{
    maxDeep = 0.0;
    for (auto rdgsMapIter = rdgsInfoDataMap.begin(); rdgsMapIter != rdgsInfoDataMap.end(); rdgsMapIter++)
    {
        for (int i = absRdgsInX; i <= (rdgsMapIter->second.vectorRdgData.size() - 1 >= absRdgsFnX ? absRdgsFnX : rdgsMapIter->second.vectorRdgData.size() - 1); i++)
        {
            int size = (rdgsMapIter->second.vectorRdgData[i]).vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size();

            if (rdgsMapIter->second.vectorRdgData[i].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][size-1] > maxDeep)
                maxDeep = rdgsMapIter->second.vectorRdgData[i].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId + selectionId][size-1];
        }
    }
}

inline void defMinMaxRdgsRelief(std::vector<std::vector<double> > vectorRdgsFnRelief, double& maxRdgsRelief, double& minRdgsRelief)
{
    minRdgsRelief = -1.0;
    maxRdgsRelief = -1.0;

    int width = vectorRdgsFnRelief.size();
    int height = vectorRdgsFnRelief[0].size();

    for (int count1 = 0; count1 < width; count1++)
    {
        for (int count2 = 0; count2 < height; count2++)
        {
            if (minRdgsRelief < 0.0) minRdgsRelief = vectorRdgsFnRelief[count1][count2];
            else if (minRdgsRelief > vectorRdgsFnRelief[count1][count2]) minRdgsRelief = vectorRdgsFnRelief[count1][count2];

            if (maxRdgsRelief < 0.0) maxRdgsRelief = vectorRdgsFnRelief[count1][count2];
            else if (maxRdgsRelief < vectorRdgsFnRelief[count1][count2]) maxRdgsRelief = vectorRdgsFnRelief[count1][count2];
        }
    }
}

inline std::string defDeleteRdgName(double fixLatitude, double fixLongitude, const std::map<std::string, st_rdgInfoData>&  rdgsInfoDataMap)
{
    std::string result = "";
    double minLatLongDelta = 0.0;

    for (auto rdgsMapIter = rdgsInfoDataMap.begin(); rdgsMapIter != rdgsInfoDataMap.end(); rdgsMapIter++)
    {
        for (int i = 0; i < rdgsMapIter->second.vectorRdgData.size(); i++)
        {
            if (fabs(minLatLongDelta) < deltaParam ||  minLatLongDelta > sqrt(
                (rdgsMapIter->second.vectorRdgData[i].latitude_degree  - fixLatitude)*
                (rdgsMapIter->second.vectorRdgData[i].latitude_degree  - fixLatitude) +
                (rdgsMapIter->second.vectorRdgData[i].longitude_degree - fixLongitude)*
                (rdgsMapIter->second.vectorRdgData[i].longitude_degree - fixLongitude)
            ))
            {
                minLatLongDelta = sqrt(
                    (rdgsMapIter->second.vectorRdgData[i].latitude_degree  - fixLatitude)*
                    (rdgsMapIter->second.vectorRdgData[i].latitude_degree  - fixLatitude) +
                    (rdgsMapIter->second.vectorRdgData[i].longitude_degree - fixLongitude)*
                    (rdgsMapIter->second.vectorRdgData[i].longitude_degree - fixLongitude)
                );
                result = rdgsMapIter->first;
            }
        }
    }
    return result;
}

inline void defRdgsInRelief(QString fileName, std::vector<std::vector<QVector3D>>& vectorRdgsInRelief)
{
    QByteArray data;

    // создаем объект класса QFile
    QFile file(fileName.toStdString().c_str());

    // Проверяем, возможно ли открыть наш файл для чтения
    if (file.open(QIODevice::ReadOnly))
    {
        //считываем все данные с файла в объект m_data
        data = file.readAll();

        int width  = *((int*)(data.data()    ));
        int height = *((int*)(data.data() + 4));
        vectorRdgsInRelief.resize(width);

        double latitude  = 0.0;
        double longitude = 0.0;
        double z         = 0.0;

        int pos = 0;
        for (int count1 = 0; count1 < width; count1++)
        {
            for (int count2 = 0; count2 < height; count2++)
            {
                latitude  =  *((double*) (data.data() + 8+ 3*pos*8));
                longitude =  *((double*) (data.data() + 8+(3*pos+1)*8));
                z =          *((double*) (data.data() + 8+(3*pos+2)*8));

                pos++;
                vectorRdgsInRelief[count1].push_back(QVector3D(latitude, longitude, z));
            }
        }
    }

}

inline void defMaxQuantImpulsesOfPacket(const std::map<std::string, st_rdgInfoData>&  rdgsInfoDataMap, int& maxQuantImpulsesOfPacket)
{
    maxQuantImpulsesOfPacket = 0;
    for (auto rdgsMapIter = rdgsInfoDataMap.begin(); rdgsMapIter != rdgsInfoDataMap.end(); rdgsMapIter++)
    {
        if (maxQuantImpulsesOfPacket == 0 || maxQuantImpulsesOfPacket < rdgsMapIter->second.quantImpulsesOfPacket)
            maxQuantImpulsesOfPacket = rdgsMapIter->second.quantImpulsesOfPacket;
    }
}

inline void addLineToPixelsData(int inX, int inY, int fnX, int fnY, std::vector<QPair<int, int> >& vectorPairXY)
{
    QPair<int, int> pairXY;

    if (inX == fnX)
    {
        if (inY < fnY)
        {
            for (int count = inY; count <= fnY; count++)
            {
                pairXY.first  = inX;
                pairXY.second = count;
                vectorPairXY.push_back(pairXY);
            }
        }
        else
        {
            for (int count = fnY; count <= inY; count++)
            {
                pairXY.first  = inX;
                pairXY.second = count;
                vectorPairXY.push_back(pairXY);
            }
        }
    }
    else if (inY == fnY)
    {
        if (inX < fnX)
        {
            for (int count = inX; count <= fnX; count++)
            {
                pairXY.first  = count;
                pairXY.second = inY;
                vectorPairXY.push_back(pairXY);
            }
        }
        else
        {
            for (int count = fnX; count <= inX; count++)
            {
                pairXY.first  = count;
                pairXY.second = inY;
                vectorPairXY.push_back(pairXY);
            }
        }
    }
    else
    {
        double signX = fabs(fnX - inX)/(fnX - inX);
        double signY = fabs(fnY - inY)/(fnY - inY);

        for (int count = 0; count <= (int)fabs(fnX - inX); count++)
        {
            pairXY.first  = inX + signX*count;
            pairXY.second = inY + ((double)(signX*count)/(double)(fnX - inX))*(fnY - inY);
            vectorPairXY.push_back(pairXY);
        }

        for (int count = 0; count <= (int)fabs(fnY- inY); count++)
        {
            pairXY.first  = inX + ((double)(signY*count)/(double)(fnY - inY))*(fnX - inX);
            pairXY.second = inY+signY*count;
            vectorPairXY.push_back(pairXY);
        }
    }
}

inline double defRdgLog10(const st_rdgInfoData& rdgInfoData, int filterId, int i, int j)
{
    double result = 0.0;

    if (
        (rdgInfoData.vectorRdgData[i].vectorsDoubleData[filterId])[j] >=0 &&
        (rdgInfoData.vectorRdgData[i].vectorsDoubleData[filterId])[j] >= rdgInfoData.contrastLog10RdgKoeff*rdgInfoData.vectorRdgData[i].vectorMaxImpulses[filterId]
    )
        result = log10(
            1 + (rdgInfoData.vectorRdgData[i].vectorsDoubleData[filterId])[j] -
            rdgInfoData.contrastLog10RdgKoeff*(rdgInfoData.vectorRdgData[i].vectorMaxImpulses[filterId])
        );

    if (
        (rdgInfoData.vectorRdgData[i].vectorsDoubleData[filterId])[j] < 0 &&
        fabs((rdgInfoData.vectorRdgData[i].vectorsDoubleData[filterId])[j]) >= fabs(rdgInfoData.contrastLog10RdgKoeff*rdgInfoData.vectorRdgData[i].vectorMinImpulses[filterId])
    )
    {
        result = -1.0*log10(
            1 + fabs(
                (rdgInfoData.vectorRdgData[i].vectorsDoubleData[filterId])[j] -
                rdgInfoData.contrastLog10RdgKoeff*rdgInfoData.vectorRdgData[i].vectorMinImpulses[filterId]
            )
        );

    }
    return result;
}

inline void createVectorRdgLog10PairXY(
    int filterId, int rdgHeight, int absRdgPixelsInX, int absRdgPixelsFnX, st_rdgInfoData& rdgInfoData
)
{
    (rdgInfoData.vectorsRdgLog10PairXY[filterId]).resize(0);

    QPair<int, int> pairXY;
    double* pixelsDoubleData = new double [((absRdgPixelsFnX-absRdgPixelsInX)+3)*rdgHeight];

    double rdgLog10 = 0.0;
    for (int i = absRdgPixelsInX; i <= absRdgPixelsFnX + 2; i++)
    {
        for (int j=0; j < rdgHeight; j++)
        {
            if ((i == absRdgPixelsInX) || (i == absRdgPixelsFnX + 2)) rdgLog10 = rdgInfoData.vectorMinLog10RdgData[filterId];
            else                                                      rdgLog10 = defRdgLog10(rdgInfoData,filterId,i-1,j);

            if (
                (double)(rdgLog10 - rdgInfoData.vectorMinLog10RdgData[filterId])/
                (rdgInfoData.vectorMaxLog10RdgData[filterId] - rdgInfoData.vectorMinLog10RdgData[filterId]) > 0.3
            )
                pixelsDoubleData[((absRdgPixelsFnX-absRdgPixelsInX)+3)*j+i-absRdgPixelsInX] = 0.0;
            else
                pixelsDoubleData[((absRdgPixelsFnX-absRdgPixelsInX)+3)*j+i-absRdgPixelsInX] = 0.3;
        }
    }

    for (int i = absRdgPixelsInX; i <= absRdgPixelsFnX + 2; i++)
    {
        for (int j = 0; j < rdgHeight; j++)
        {
            int quantPoints = 0;
            if (pixelsDoubleData[((absRdgPixelsFnX-absRdgPixelsInX)+3)*j+i-absRdgPixelsInX] < deltaParam)
            {
                for (int i1 = i-1; i1 <= i+1; i1++)
                {
                    for (int j1 = j-1; j1 <= j+1; j1++)
                    {
                        if ( ((i1 >= absRdgPixelsInX && i1 <= absRdgPixelsFnX + 2) && (j1 >= 0 && j1 <= rdgHeight -1)) && (pixelsDoubleData[((absRdgPixelsFnX-absRdgPixelsInX)+3)*j1+i1-absRdgPixelsInX] > deltaParam)) quantPoints++;
                    }
                }
                if (quantPoints >= 1 && quantPoints < 8)
                {
                    if ((i != absRdgPixelsInX) && (i != absRdgPixelsFnX + 2))
                    {
                        pairXY.first  = i-1;
                        pairXY.second = j;
                        (rdgInfoData.vectorsRdgLog10PairXY[filterId]).push_back(pairXY);
                    }
                }
            }
        }
    }
    delete [] pixelsDoubleData;
}

inline void outputVectorRdgsSurfSelectArea(
    int rdgsSurfPixelsInX, int rdgsSurfPixelsInY, int rdgsSurfPixelsFnX, int rdgsSurfPixelsFnY,
    int rdgsSurfPixelsWidth,  std::vector<QPair<int, int> > vectorPairXY,  GLubyte* rdgsSurfPixelsData
)
{
    if (vectorPairXY.size() > 0)
    {
        for (int count = 0; count < vectorPairXY.size(); count++)
        {
            if (
               (vectorPairXY[count].first  >= rdgsSurfPixelsInX && vectorPairXY[count].first  <= rdgsSurfPixelsFnX) &&
               (vectorPairXY[count].second >= rdgsSurfPixelsInY && vectorPairXY[count].second <= rdgsSurfPixelsFnY)
            )
            {
                rdgsSurfPixelsData[4*rdgsSurfPixelsWidth*(vectorPairXY[count].second-rdgsSurfPixelsInY) + 4*(vectorPairXY[count].first-rdgsSurfPixelsInX)+0] = 255;
                rdgsSurfPixelsData[4*rdgsSurfPixelsWidth*(vectorPairXY[count].second-rdgsSurfPixelsInY) + 4*(vectorPairXY[count].first-rdgsSurfPixelsInX)+1] = 0;
                rdgsSurfPixelsData[4*rdgsSurfPixelsWidth*(vectorPairXY[count].second-rdgsSurfPixelsInY) + 4*(vectorPairXY[count].first-rdgsSurfPixelsInX)+2] = 0;
                rdgsSurfPixelsData[4*rdgsSurfPixelsWidth*(vectorPairXY[count].second-rdgsSurfPixelsInY) + 4*(vectorPairXY[count].first-rdgsSurfPixelsInX)+3] = 255;
            }
        }
    }
}

inline void outputVectorRdgSelectArea(
    int rdgPixelsWidth, int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY, std::vector<QPair<int, int> > vectorPairXY, GLubyte* pixelsData
)
{
    if (vectorPairXY.size() > 0)
    {
        for (int count = 0; count < vectorPairXY.size(); count++)
        {
            if ( (vectorPairXY[count].first  >= rdgPixelsInX && vectorPairXY[count].first  <= rdgPixelsFnX) &&
                 (vectorPairXY[count].second >= rdgPixelsInY && vectorPairXY[count].second <= rdgPixelsFnY))
            {
                pixelsData[4*rdgPixelsWidth*(vectorPairXY[count].second-rdgPixelsInY) + 4*(int)(vectorPairXY[count].first-rdgPixelsInX)+0] = 255;
                pixelsData[4*rdgPixelsWidth*(vectorPairXY[count].second-rdgPixelsInY) + 4*(int)(vectorPairXY[count].first-rdgPixelsInX)+1] = 0;
                pixelsData[4*rdgPixelsWidth*(vectorPairXY[count].second-rdgPixelsInY) + 4*(int)(vectorPairXY[count].first-rdgPixelsInX)+2] = 0;
                pixelsData[4*rdgPixelsWidth*(vectorPairXY[count].second-rdgPixelsInY) + 4*(int)(vectorPairXY[count].first-rdgPixelsInX)+3] = 255;
            }
        }
    }
}

inline void outputMapRdgSelectArea(
      int rdgPixelsWidth, int rdgPixelsHeight, int rdgPixelsInX, int rdgPixelsInY, int rdgPixelsFnX, int rdgPixelsFnY,
      const std::map<int, std::vector<QPair<int, int>> >& mapRdgPairXY, GLubyte* pixelsData)
{
    if (mapRdgPairXY.size() > 0)
    {
        for (auto itMap = mapRdgPairXY.begin(); itMap != mapRdgPairXY.end(); itMap++)
        {
            for (int count = 0; count < itMap->second.size(); count++)
            {
                if ( (itMap->second[count].first  >= rdgPixelsInX && itMap->second[count].first  <= rdgPixelsFnX) &&
                (itMap->second[count].second >= rdgPixelsInY && itMap->second[count].second <= rdgPixelsFnY))
                {
                    pixelsData[4*rdgPixelsWidth*(rdgPixelsHeight-1-(itMap->second[count].second-rdgPixelsInY))+4*(int)(itMap->second[count].first-rdgPixelsInX)+0] = 255;
                    pixelsData[4*rdgPixelsWidth*(rdgPixelsHeight-1-(itMap->second[count].second-rdgPixelsInY))+4*(int)(itMap->second[count].first-rdgPixelsInX)+1] =   0;
                    pixelsData[4*rdgPixelsWidth*(rdgPixelsHeight-1-(itMap->second[count].second-rdgPixelsInY))+4*(int)(itMap->second[count].first-rdgPixelsInX)+2] =   0;
                    pixelsData[4*rdgPixelsWidth*(rdgPixelsHeight-1-(itMap->second[count].second-rdgPixelsInY))+4*(int)(itMap->second[count].first-rdgPixelsInX)+3] = 255;
                }
            }
        }
    }
}

inline double defDepthScanUpLowRdg(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap,  std::string nameRdg,  int kRdg, int quantImpulsesOfPacket, int materialId, int filterId, int selectionId )
{
    double result = 0.0;
    auto itRdgsInfoDataMap = rdgsInfoDataMap.find(nameRdg);
    if (itRdgsInfoDataMap != rdgsInfoDataMap.end())
    {
        if (itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1 < quantImpulsesOfPacket)
            result = itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][itRdgsInfoDataMap
                                      ->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1];
        else
            result = itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][quantImpulsesOfPacket];
    }
    return result;
}

inline double defDepthScanLowUpRdg(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap,  std::string nameRdg,  int kRdg, int quantImpulsesOfPacket, int materialId, int filterId, int selectionId)
{
    double result = 0.0;
    auto itRdgsInfoDataMap = rdgsInfoDataMap.find(nameRdg);
    if (itRdgsInfoDataMap != rdgsInfoDataMap.end())
    {
        if (itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1 < quantImpulsesOfPacket)
            result = 0.0;
        else
            result = itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][itRdgsInfoDataMap
                                      ->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1] -
            itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][quantImpulsesOfPacket];
    }
    return result;
}

inline double defFullDepthScanLowUpRdg(const std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap,  std::string nameRdg,  int kRdg, int materialId, int filterId, int selectionId)
{
    double result = 0.0;
    auto itRdgsInfoDataMap = rdgsInfoDataMap.find(nameRdg);
    if (itRdgsInfoDataMap != rdgsInfoDataMap.end())
    {
        if (itRdgsInfoDataMap->first == nameRdg)
            result = itRdgsInfoDataMap->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][itRdgsInfoDataMap
                                      ->second.vectorRdgData[kRdg].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1];
    }
    return result;
}

inline void saveBoundPointsLinePoints(const QString& saveCutLinePointsFile, int inX, int inY, int fnX, int fnY)
{
    csvfile csv(saveCutLinePointsFile.toStdString());
    // Заголовок
    csv <<"X" << "Y"<< endrow;
    csv << inX << inY<< endrow;
    csv << fnX << fnY<< endrow;
}

inline void loadBoundPointsLinePoints(const QString& loadCutLinePointsFile, int& inX, int& inY, int& fnX, int& fnY)
{
    QFile file(loadCutLinePointsFile);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        int count = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (count == 1)
            {
                inX = (line.split(";").at(0)).toInt();
                inY = (line.split(";").at(1)).toInt();
            }
            else if (count == 2)
            {
                fnX = (line.split(";").at(0)).toInt();
                fnY = (line.split(";").at(1)).toInt();
            }
            count++;
        }
    }
}

inline void maskCellRdgSelected(int i, int j, int rdgHeight, int absRdgPixelsInX, int absRdgPixelsFnX,  int maskCell, std::vector<std::vector<int> >& vectorRdgSelectedPoints)
{
    vectorRdgSelectedPoints[i-absRdgPixelsInX][j] = maskCell;

    bool newMaskPointEx = false;
    for (int i1 = i-1; i1 <= i+1; i1++)
    {
        for (int j1 = j-1; j1 <= j+1; j1++)
        {
            if (((i1 >=absRdgPixelsInX && i1 <= absRdgPixelsFnX) && (j1 >=0 && j1 <= rdgHeight -1)) && ( i1 != i || j1 != j))
            {
                if (vectorRdgSelectedPoints[i1-absRdgPixelsInX][j1] == -1 && newMaskPointEx == false)
                {
                    maskCellRdgSelected(i1, j1, rdgHeight, absRdgPixelsInX, absRdgPixelsFnX, maskCell, vectorRdgSelectedPoints);
                    newMaskPointEx = true;
                }
            }
        }
    }
}

inline void createMapRdgPairXY(
    const std::vector<QPair<int, int> >& vectorRdgLog10PairXY, int rdgHeight, int absRdgPixelsInX, int absRdgPixelsFnX, std::map<int, std::vector<QPair<int, int>> >& mapAutoLog10RdgPairXY
)
{
    mapAutoLog10RdgPairXY.clear();
    if (vectorRdgLog10PairXY.size() > 0)
    {
        std::vector<std::vector<int> > vectorRdgSelectedPoints;
        vectorRdgSelectedPoints.resize(absRdgPixelsFnX-absRdgPixelsInX+1);

        for (int i = absRdgPixelsInX; i <= absRdgPixelsFnX; i++)  vectorRdgSelectedPoints[i-absRdgPixelsInX].resize(rdgHeight);

        for (int i = absRdgPixelsInX; i <= absRdgPixelsFnX; i++)
        {
            for (int j = 0; j < rdgHeight; j++)
            {
                vectorRdgSelectedPoints[i-absRdgPixelsInX][j] = 0.0;
            }
        }

        for (int i = 0; i < vectorRdgLog10PairXY.size(); i++)
        {
            vectorRdgSelectedPoints[vectorRdgLog10PairXY[i].first-absRdgPixelsInX][vectorRdgLog10PairXY[i].second] = -1;
        }

        int maskCell = 1;
        for (int i = absRdgPixelsInX; i <= absRdgPixelsFnX; i++)
        {
            for (int j = 0; j < rdgHeight; j++)
            {
                if (vectorRdgSelectedPoints[i-absRdgPixelsInX][j] == -1)
                {
                    maskCellRdgSelected(i, j, rdgHeight, absRdgPixelsInX, absRdgPixelsFnX,  maskCell, vectorRdgSelectedPoints);
                    maskCell++;
                }
            }
        }

        for (int i = 1; i <= maskCell-1; i++)  {   mapAutoLog10RdgPairXY[i].resize(0); }

        for (int i = absRdgPixelsInX; i <= absRdgPixelsFnX; i++)
        {
            for (int j = 0; j < rdgHeight; j++)
            {
                if (vectorRdgSelectedPoints[i-absRdgPixelsInX][j] > 0)
                {
                    QPair<int, int> pairXY;
                    pairXY.first  = i;
                    pairXY.second = j;
                    mapAutoLog10RdgPairXY[vectorRdgSelectedPoints[i-absRdgPixelsInX][j]].push_back(pairXY);
                }
            }
        }
    }
}

inline void  createMaskMapAllXY(
    int absRdgPixelsInX, int absRdgPixelsFnX, const std::map<int, std::vector<QPair<int, int>> >& mapRdgPairXY, std::map<int, std::map<int, std::vector<int>>>&  maskMapAllXY
)
{
    std::vector<std::vector<QPair<int, int>> > fullVectorPairsXY;
    fullVectorPairsXY.resize(absRdgPixelsFnX - absRdgPixelsInX + 1);
    QPair<int, int>                                       pairMY;
    std::map<int, std::vector<int>> maskMapXY;

    for (auto mapIterAvail = mapRdgPairXY.begin(); mapIterAvail != mapRdgPairXY.end(); mapIterAvail++)
    {
        for (int count1 = 0; count1 < mapIterAvail->second.size(); count1++)
        {
            pairMY.first  = mapIterAvail->first;
            pairMY.second = mapIterAvail->second[count1].second;
            fullVectorPairsXY[mapIterAvail->second[count1].first-absRdgPixelsInX].push_back(pairMY);
        }
    }

    for (int count1 = absRdgPixelsInX; count1 <= absRdgPixelsFnX; count1++)
    {
        if (fullVectorPairsXY[count1-absRdgPixelsInX].size() > 0)
        {
            maskMapXY.clear();
            for (int count2 = 0; count2 < fullVectorPairsXY[count1-absRdgPixelsInX].size(); count2++)
            {
                maskMapXY[fullVectorPairsXY[count1-absRdgPixelsInX][count2].first].push_back(fullVectorPairsXY[count1-absRdgPixelsInX][count2].second);
            }
            maskMapAllXY[count1] = maskMapXY;
        }
    }
}

inline void createMapRdgTypeRdgSelectionInfo(
    int rdgHeight, int absRdgPixelsInX, int absRdgPixelsFnX, int materialId, int filterId, int selectionId,
    const std::map<int, std::vector<QPair<int, int> > >&  mapAutoLog10RdgPairXY, st_rdgInfoData& rdgInfoData
)
{
    for (int i = absRdgPixelsInX; i <= absRdgPixelsFnX; i++)
    {
        for (int j=0; j < rdgHeight; j++)
        {
            rdgInfoData.vectorRdgData[i].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][j] =
            rdgInfoData.vectorRdgData[i].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId            ][j];
        }
    }

    if (mapAutoLog10RdgPairXY.size() > 0)
    {
        std::map<int, std::map<int, std::vector<int>>> maskMapAllXY;
        createMaskMapAllXY(absRdgPixelsInX, absRdgPixelsFnX, mapAutoLog10RdgPairXY, maskMapAllXY);

        int X{0}, YIn{0}, YFn{0};
        double sumImpIn{0.0}, sumImpFn{0.0};

        for (auto mapIter1 = maskMapAllXY.begin(); mapIter1 != maskMapAllXY.end(); mapIter1++)
        {
            if (maskMapAllXY[mapIter1->first].size()>1)
            {
                X = mapIter1->first;
                for (auto mapIter2 = maskMapAllXY[mapIter1->first].begin(); mapIter2 != maskMapAllXY[mapIter1->first].end(); mapIter2++)
                {
                    if (mapIter2->second.size()> 1)
                    {
                        for (int count1 = 0; count1 < mapIter2->second.size()-1; count1++)
                        {
                            YIn = mapIter2->second[0];
                            YFn = mapIter2->second[count1+1];

                            sumImpIn = rdgInfoData.vectorRdgData[X].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+0][YIn];
                            sumImpFn = rdgInfoData.vectorRdgData[X].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+0][YFn];

                            for (int count2 = YIn; count2<=YFn; count2++)
                            {
                                if (count2 < (YIn+YFn)/2.0)
                                    rdgInfoData.vectorRdgData[X].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][count2] = sumImpIn;
                                else
                                    rdgInfoData.vectorRdgData[X].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][count2] = sumImpFn;
                            }
                        }
                    }
                }
            }
        }
    }
}

inline void saveRdgAutoSelection(std::map<int, std::vector<QPair<int, int>> > mapRdgPairXY, double time_step_ns, QString saveRdgAutoSelectionDataFile)
{
    csvfile csv(saveRdgAutoSelectionDataFile.toStdString());
    csv <<"layerId" << "trackId"<<"impulseId"<<"time delay ns"<< endrow;
    for (auto itMap = mapRdgPairXY.begin(); itMap != mapRdgPairXY.end(); itMap++)
    {
        for (int i = 0; i <= itMap->second.size()-1; i++)
        {
            csv << itMap->first << itMap->second[i].first<<itMap->second[i].second<<itMap->second[i].second*time_step_ns<< endrow;
        }
    }
}

inline void loadRdgAutoSelection(QString loadRdgAutoSelectionDataFile, std::map<int, std::vector<QPair<int, int>> >& mapRdgPairXY)
{
    QPair<int, int> pairXY;
    QFile file(loadRdgAutoSelectionDataFile);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        int count = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (count > 0)
            {
                int layerID =  line.split(";").at(0).toInt();
                int X       =  line.split(";").at(1).toInt();
                int Y       =  line.split(";").at(2).toInt();

                pairXY.first  = X;
                pairXY.second = Y;
                mapRdgPairXY[layerID].push_back(pairXY);
            }
            count++;
        }
    }
}

inline double convertDoubleEndianness(double value, bool isBigEndianSource) {
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);

    stream.setByteOrder(isBigEndianSource ? QDataStream::BigEndian : QDataStream::LittleEndian);
    stream << value;

    stream.device()->seek(0);
    stream.setByteOrder(isBigEndianSource ? QDataStream::LittleEndian : QDataStream::BigEndian);
    double convertedValue;
    stream >> convertedValue;
    return convertedValue;
}

inline double correctDoubleValue(const QByteArray& data, int parserOffset,  int i, int offset)
{
    QByteArray byteInBlock;
    for (int count=0; count<8; count++){ byteInBlock.append(data.data() + parserOffset + offset + count); }
    QByteArray byteOutBlock;
    for (int count=0; count<8; count++){ byteOutBlock.append(byteInBlock[7-count]); }
    return *((double*)(byteOutBlock.data()));
}

inline double correctInt64TValue(const QByteArray& data, int parserOffset,  int i, int offset)
{
    QByteArray byteInBlock;
    for (int count=0; count<8; count++){ byteInBlock.append(data.data() + parserOffset + offset + count); }
    QByteArray byteOutBlock;
    for (int count=0; count<8; count++){ byteOutBlock.append(byteInBlock[7-count]); }
    return *((int64_t*)(byteOutBlock.data()));
}

inline double correctInt16TValue(const QByteArray& data, int parserOffset,  int i, int offset)
{
    QByteArray byteInBlock;
    for (int count=0; count<2; count++){ byteInBlock.append(data.data() + parserOffset + offset + count); }
    QByteArray byteOutBlock;
    for (int count=0; count<2; count++){ byteOutBlock.append(byteInBlock[1-count]); }
    return *((int16_t*)(byteOutBlock.data()));
}

inline void defVectorRdgsTransLineSectionPoints(int inPosX, int inPosY, int fnPosX, int fnPosY, std::vector<std::pair<int, int>>& vectorRdgsTransLinePoints)
{
    vectorRdgsTransLinePoints.resize(0);

    std::pair<int, int> rdgPoint;

    vectorRdgsTransLinePoints.resize(0);
    int length = (int)(sqrt( (double)(fnPosX - inPosX)*(fnPosX - inPosX) +
                             (double)(fnPosY - inPosY)*(fnPosY - inPosY)))+1;
    int x{0}, y{0};

    for (int count = 0; count < length; count++)
    {
        if (inPosY == fnPosY)
        {
            x = inPosX + count*(fnPosX - inPosX)/(length-1.0);
            y = inPosY;
        }
        else if (inPosX == fnPosX)
        {
            x = inPosX ;
            y = inPosY + count*(fnPosY - inPosY)/(length-1.0);
        }
        else
        {
            x = inPosX + count*(fnPosX - inPosX)/(length-1.0);
            y = inPosY + count*(fnPosY - inPosY)/(length-1.0);
        }
        rdgPoint.first  = x;
        rdgPoint.second = y;
        vectorRdgsTransLinePoints.push_back(rdgPoint);
    }
}


inline void wheelOpengl(
    double eventDelta, boolean allScaling, boolean hScaling, boolean vScaling, int absRdgPixelsInX, int absRdgPixelsFnX, int height,
    int& pixelsWidth, int& pixelsHeight, int& pixelsInX, int& pixelsInY, int& pixelsFnX, int& pixelsFnY,
    int& pixelsCentX, int& pixelsCentY
)
{
    if (eventDelta > 0)
    {
        if ((allScaling == true) || (hScaling == true)) pixelsWidth  = pixelsWidth  + deltaPixels;
        if ((allScaling == true) || (vScaling == true)) pixelsHeight = pixelsHeight + deltaPixels;
    }
    else
    {
        if ((allScaling == true) || (hScaling == true))
            if (pixelsWidth  - deltaPixels > 0) pixelsWidth  = pixelsWidth  - deltaPixels;
        if ((allScaling == true) || (vScaling == true))
            if (pixelsHeight - deltaPixels > 0) pixelsHeight = pixelsHeight - deltaPixels;
    }

    if ((allScaling == true) || (hScaling == true))
    {
        if       (pixelsWidth  < (absRdgPixelsFnX-absRdgPixelsInX+1)/scaleFactor ) { pixelsWidth  = (absRdgPixelsFnX-absRdgPixelsInX+1)/scaleFactor; }
        else if  (pixelsWidth  > (absRdgPixelsFnX-absRdgPixelsInX+1))              { pixelsWidth  = (absRdgPixelsFnX-absRdgPixelsInX+1);             }
        if (pixelsCentX + pixelsWidth/2 > absRdgPixelsFnX)
        {
            pixelsFnX = absRdgPixelsFnX;
            pixelsInX = pixelsFnX-pixelsWidth+1;
        }
        else if ( pixelsCentX - (pixelsWidth/2) < absRdgPixelsInX)
        {
            pixelsInX = absRdgPixelsInX;
            pixelsFnX = pixelsWidth-1;
        }
        else
        {
            pixelsFnX = pixelsCentX + pixelsWidth/2  ;
            pixelsInX = pixelsFnX   - pixelsWidth + 1;
        }
        pixelsCentX = (pixelsInX + pixelsFnX)/2;
    }

    if ((allScaling == true) || (vScaling == true))
    {
        if (pixelsHeight  < height/scaleFactor )   { pixelsHeight = height/scaleFactor; }
        else if  (pixelsHeight  > height)          { pixelsHeight = height;             }
        if (pixelsCentY + pixelsHeight/2 > height-1)
        {
            pixelsFnY = height-1;
            pixelsInY = pixelsFnY-pixelsHeight+1;
        }
        else if (pixelsCentY - pixelsHeight/2 < 0)
        {
            pixelsInY = 0;
            pixelsFnY = pixelsHeight-1;
        }
        else
        {
            pixelsFnY = pixelsCentY + pixelsHeight/2  ;
            pixelsInY = pixelsFnY   - pixelsHeight + 1;
        }
        pixelsCentY = (pixelsInY + pixelsFnY)/2;
    }
}

inline void moveOpengl(
    int pressX, int pressY, int releaseX, int releaseY,
    int absRdgPixelsInX, int absRdgPixelsFnX, int height, int pixelsWidth, int pixelsHeight,
    int& pixelsInX, int& pixelsInY, int& pixelsFnX, int& pixelsFnY, int& pixelsCentX, int& pixelsCentY
)
{
    if ((pressX   >= 0 && pressX   <= pixelsWidth  -1) &&
        (pressY   >= 0 && pressY   <= pixelsHeight -1) &&
        (releaseX >= 0 && releaseX <= pixelsWidth  -1) &&
        (releaseY >= 0 && releaseY <= pixelsHeight -1) )
    {
        int deltaXIn = 0;
        int deltaXFn = 0;
        int dimX = pressX - releaseX;
        if (pixelsInX + dimX > absRdgPixelsFnX) deltaXIn =  absRdgPixelsFnX  - pixelsInX;
        if (pixelsInX + dimX < absRdgPixelsInX) deltaXIn = -pixelsInX;
        if (pixelsFnX + dimX > absRdgPixelsFnX) deltaXFn =  absRdgPixelsFnX - pixelsFnX;
        if (pixelsFnX + dimX < absRdgPixelsInX) deltaXFn = -pixelsFnX;
        if (abs(deltaXIn)> 0 || abs(deltaXFn)> 0)
        {
            if (abs(deltaXIn) > 0) dimX = deltaXIn;
            if (abs(deltaXFn) > 0) dimX = deltaXFn;
        }

        int deltaYIn = 0;
        int deltaYFn = 0;
        int dimY = releaseY - pressY;
        if (pixelsInY + dimY > height - 1)  deltaYIn =  height - 1 - pixelsInY;
        if (pixelsInY + dimY < 0 )          deltaYIn = -pixelsInY;
        if (pixelsFnY + dimY > height - 1)  deltaYFn =  height - 1 - pixelsFnY;
        if (pixelsFnY + dimY < 0         )  deltaYFn = -pixelsFnY;
        if (abs(deltaYIn)> 0 || abs(deltaYFn)> 0)
        {
            if (abs(deltaYIn) > 0) dimY = deltaYIn;
            if (abs(deltaYFn) > 0) dimY = deltaYFn;
        }

        if (
            (pixelsInX + dimX >= absRdgPixelsInX && pixelsInX + dimX <= absRdgPixelsFnX) &&
            (pixelsInY + dimY >= 0               && pixelsInY + dimY <= height - 1)      &&
            (pixelsFnX + dimX >= absRdgPixelsInX && pixelsFnX + dimX <= absRdgPixelsFnX) &&
            (pixelsFnY + dimY >= 0               && pixelsFnY + dimY <= height - 1)
         )
        {
            pixelsInX = pixelsInX + dimX;
            pixelsInY = pixelsInY + dimY;

            pixelsFnX = pixelsFnX + dimX;
            pixelsFnY = pixelsFnY + dimY;

            pixelsCentX = (pixelsInX + pixelsFnX)/2;
            pixelsCentY = (pixelsInY + pixelsFnY)/2;
        }
    }
}

inline void executeTypeRdgSelectionInfo(
    int materialId, int filterId, int selectionId, int rdgHeight, int absRdgPixelsInX, int absRdgPixelsFnX, st_rdgInfoData& rdgInfoData
)
{
    if (rdgInfoData.vectorRdgData.size()-1 <= absRdgPixelsInX) absRdgPixelsInX = rdgInfoData.vectorRdgData.size()-1;
    if (rdgInfoData.vectorRdgData.size()-1 <= absRdgPixelsFnX) absRdgPixelsFnX = rdgInfoData.vectorRdgData.size()-1;

    if (absRdgPixelsFnX > absRdgPixelsInX)
    {
        createVectorRdgLog10PairXY(filterId, rdgHeight, absRdgPixelsInX, absRdgPixelsFnX, rdgInfoData);
        createMapRdgPairXY(rdgInfoData.vectorsRdgLog10PairXY[filterId], rdgHeight, absRdgPixelsInX, absRdgPixelsFnX, rdgInfoData.vectorMapAutoLog10RdgPairXY[filterId]);
        createMapRdgTypeRdgSelectionInfo(rdgHeight, absRdgPixelsInX, absRdgPixelsFnX, materialId, filterId, selectionId, rdgInfoData.vectorMapAutoLog10RdgPairXY[filterId], rdgInfoData);
    }
}

inline void samplingAllLog10Rdgs(
    int materialId, int filterId, std::vector<std::pair<std::string, std::string>> rdgsNamesVectorPairs,
    std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, int absRdgPixelsInX, int absRdgPixelsFnX
)
{
    for (auto iter = rdgsNamesVectorPairs.begin(); iter != rdgsNamesVectorPairs.end(); iter++)
    {
        executeTypeRdgSelectionInfo(
            materialId, filterId, 2, rdgsInfoDataMap[iter->first].quantImpulsesOfPacket, absRdgPixelsInX, absRdgPixelsFnX, rdgsInfoDataMap[iter->first]
        );
    }
}

inline void createCsvRdgsFiles(const QStringList& filesRdgNamesCsv, QStringList& filesRdgNamesPropsCsv, QStringList& filesRdgNamesDataCsv)
{
    for (int count = 0; count <= filesRdgNamesCsv.size()-1; count++)
    {
        QFile fileRdg(filesRdgNamesCsv.at(count));
        if (!fileRdg.open(QFile::ReadOnly | QFile::Text)) std::cout<<"File not exists"<<std::endl;
        else
        {
            QTextStream in(&fileRdg);
            QString line = in.readLine();

            if (line.split(";").size()< 20) filesRdgNamesPropsCsv.push_back(filesRdgNamesCsv.at(count));
            else                            filesRdgNamesDataCsv.push_back(filesRdgNamesCsv.at(count));
        }
    }
}

inline void allocateMainRdgContainers(st_rdgInfoData& rdgInfoDataMap, int vectorRdgDataPos)
{
    (rdgInfoDataMap.vectorRdgData[vectorRdgDataPos].vectorMinImpulses).resize(countFilters);
    (rdgInfoDataMap.vectorRdgData[vectorRdgDataPos].vectorMaxImpulses).resize(countFilters);
    (rdgInfoDataMap.vectorRdgData[vectorRdgDataPos].vectorsDeeps).resize(countSelectors*countFilters*countMaterials);
    (rdgInfoDataMap.vectorsRdgLog10PairXY).resize(countFilters);
    (rdgInfoDataMap.vectorMapAutoLog10RdgPairXY).resize(countFilters);
}

inline void createRdgDataMinMaxImpulses(st_rdgInfoData& rdgInfoDataMap, int count1, int quantImpulsesOfPacket, int filterId)
{
    rdgInfoDataMap.vectorRdgData[count1].vectorMinImpulses[filterId] = *std::min_element(
        rdgInfoDataMap.vectorRdgData[count1].vectorsDoubleData[filterId].begin(),
        rdgInfoDataMap.vectorRdgData[count1].vectorsDoubleData[filterId].end()
    );

    rdgInfoDataMap.vectorRdgData[count1].vectorMaxImpulses[filterId] = *std::max_element(
        rdgInfoDataMap.vectorRdgData[count1].vectorsDoubleData[filterId].begin(),
        rdgInfoDataMap.vectorRdgData[count1].vectorsDoubleData[filterId].end()
    );
}

inline void createRdgDataDeeps(st_rdgInfoData& rdgInfoDataMap, int count1, int quantImpulsesOfPacket, int filterId, int materialId)
{
    rdgInfoDataMap.vectorRdgData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId + 0].resize(quantImpulsesOfPacket);

    double fixJDeep = 0;
    double sumDeeps = 0;
    double sumIntens = 0;
    for (int j=0; j < quantImpulsesOfPacket; j++)
    {
        if (j == 0)    sumIntens  = fabs(rdgInfoDataMap.vectorRdgData[count1].vectorsDoubleData[filterId][j]*rdgMetricKoeff);
        else           sumIntens += fabs(rdgInfoDataMap.vectorRdgData[count1].vectorsDoubleData[filterId][j]*rdgMetricKoeff);

        fixJDeep = 0.5*spc*nanokoef*rdgInfoDataMap.vectorRdgData[count1].time_step_ns/sqrt(epsdData(materialId)*exp(gammaData(materialId)*sumIntens));
        sumDeeps += fixJDeep;
        rdgInfoDataMap.vectorRdgData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+0][j] = sumDeeps;
    }

    for (int count = 1; count <=2; count++)
    {
        rdgInfoDataMap.vectorRdgData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+count].resize(quantImpulsesOfPacket);
    }
    rdgInfoDataMap.vectorRdgData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+1] =
    rdgInfoDataMap.vectorRdgData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+0];
}

inline void defAdditionalRdgMainData(std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, int quantImpulsesOfPacket)
{
    for (auto mapIter1 = rdgsInfoDataMap.begin(); mapIter1 != rdgsInfoDataMap.end(); mapIter1++)
    {
        if (mapIter1->second.quantImpulsesOfPacket <= 0)
        {
            mapIter1->second.quantImpulsesOfPacket = quantImpulsesOfPacket;

            mapIter1->second.vectorMaxLog10RdgData.resize(countFilters);
            mapIter1->second.vectorMinLog10RdgData.resize(countFilters);

            int rdgWidth  = mapIter1->second.vectorRdgData.size();
            for (int count = 0; count <= countFilters-1; count++)
            {
                for (int i=0; i<rdgWidth; i++)
                {
                    if (i == 0)
                    {
                        mapIter1->second.vectorMaxLog10RdgData[count] =
                                log10(1 + mapIter1->second.vectorRdgData[i].vectorMaxImpulses[count]);
                        mapIter1->second.vectorMinLog10RdgData[count] =
                                -1.0*log10(1 + fabs(mapIter1->second.vectorRdgData[i].vectorMinImpulses[count]));
                    }
                    else
                    {
                        if (log10(1 + mapIter1->second.vectorRdgData[i].vectorMaxImpulses[count]) > mapIter1->second.vectorMaxLog10RdgData[count])
                            mapIter1->second.vectorMaxLog10RdgData[count] =
                                    log10(1 + mapIter1->second.vectorRdgData[i].vectorMaxImpulses[count]);

                        if (-1.0*log10(1 + fabs(mapIter1->second.vectorRdgData[i].vectorMinImpulses[count])) < mapIter1->second.vectorMinLog10RdgData[count])
                            mapIter1->second.vectorMinLog10RdgData[count] =
                                    -1.0*log10(1 + fabs(mapIter1->second.vectorRdgData[i].vectorMinImpulses[count]));
                    }
                }
            }
        }
    }
}

inline void correctRdgData(
    std::map<std::string, st_rdgInfoData>& rdgsInfoDataMap, st_rdgsWorkData& rdgsWorkData,
    std::vector<std::pair<std::string, std::string>>& rdgsNamesVectorPairs, std::string rdgFilesFullData
)
{
    std::pair<std::string, std::string> stringPair;
    for (auto mapIter = rdgsInfoDataMap.begin(); mapIter != rdgsInfoDataMap.end(); mapIter++)
    {
        std::vector<std::pair<std::string, std::string>>::iterator vectorPairsIt = std::find_if(
            rdgsNamesVectorPairs.begin(), rdgsNamesVectorPairs.end(), [&](const std::pair<std::string, std::string>& stringInPair)
            {
                return (stringInPair.first == mapIter->first);
            }
        );

        if (vectorPairsIt == rdgsNamesVectorPairs.end())
        {
            stringPair.first  = mapIter->first;
            stringPair.second = rdgFilesFullData;
            rdgsNamesVectorPairs.push_back(stringPair);
        }
    }

    //градусы (пришлось выдумать)
    rdgsWorkData.leftLatitude  = 51.193075;
    rdgsWorkData.lowLongitude  = 34.677180;
    rdgsWorkData.upLongitude   = 34.678180;

    int height = rdgsInfoDataMap.size();
    double deltaLongitude = (rdgsWorkData.upLongitude - rdgsWorkData.lowLongitude)/(2*height);
    double deltaLatitude  = 0.00001536;

    //фиксируем радарограммы исходя из произведенной обрезки хвостов
    int mapId = 0;
    for (auto mapIter = rdgsInfoDataMap.begin(); mapIter != rdgsInfoDataMap.end(); mapIter++)
    {
        for (int i=0; i<mapIter->second.vectorRdgData.size(); i++)
        {
            mapIter->second.vectorRdgData[i].latitude_degree  = rdgsWorkData.leftLatitude + i          *deltaLatitude;
            mapIter->second.vectorRdgData[i].longitude_degree = rdgsWorkData.lowLongitude + (1+2*mapId)*deltaLongitude;
        }
        mapId++;
    }
}

inline void representRdgLine(int inPosX, int inPosY, int fnPosX, int fnPosY, st_rdgsWorkData& rdgsWorkData, bool fnPosAct)
{
    std::vector<std::pair<int, int>> vectorRdgsTransLinePoints;
    defVectorRdgsTransLineSectionPoints(inPosX, inPosY,  fnPosX,  fnPosY, vectorRdgsTransLinePoints);

    std::string nameRdg = "";
    int         kRdg    = 0;

    if (vectorRdgsTransLinePoints.size() > 0)
    {
        if (fnPosAct == true)
        {
            nameRdg = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*fnPosY+fnPosX]);
            kRdg    = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*fnPosY+fnPosX]);
        }
        else
        {
            nameRdg = std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*inPosY+inPosX]);
            kRdg    = std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*inPosY+inPosX]);
        }

        int count = vectorRdgsTransLinePoints.size()-1;
        while (count >= 0)
        {
            if ((std::get<0>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[count].second + vectorRdgsTransLinePoints[count].first]) == "") &&
                (std::get<1>(rdgsWorkData.vectorRdgsData[rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[count].second + vectorRdgsTransLinePoints[count].first]) == -1)
            ) {
                rdgsWorkData.vectorRdgsData[
                    rdgsWorkData.rdgsSurfWidth*vectorRdgsTransLinePoints[count].second + vectorRdgsTransLinePoints[count].first
                ] = std::make_tuple(nameRdg, kRdg);
            }
            count--;
        }
    }
}

template <class T> inline static QVariant toVariant(const QList<T> &list)
{
    QVariantList variantList;
    variantList.reserve(list.size());
    for (const auto& v : list) { variantList.append(v); }
    return variantList;
}

template <class T> inline static QList<T> toList(const QVariant &qv)
{
    QList <T> dataList;
    foreach(QVariant v, qv.value<QVariantList>()) { dataList << v.value<T>();}
    return dataList;
}

inline void readLastRdgsDirsFromSettings(QString& trzDir, QString& csvDir, QString& hdf5Dir)
{
    //загрузка предыдущих радарограмм
    QSettings setupR("rdgsDirsList.ini", QSettings::IniFormat);
    QList<QString> rdgsDirsList;
    int files_count = setupR.beginReadArray("rdgsDirs");
    for (int groups=0; groups<files_count; groups++)
    {
        setupR.setArrayIndex(groups);
        rdgsDirsList =  toList<QString>(setupR.value("rdgsDirsList"));
        if (rdgsDirsList.size() == 3)
        {
            if (QDir(trzDir).exists())  trzDir  = rdgsDirsList.at(0);
            if (QDir(csvDir).exists())  csvDir  = rdgsDirsList.at(1);
            if (QDir(hdf5Dir).exists()) hdf5Dir = rdgsDirsList.at(2);
        }
    }
}

inline void writeLastRdgsDirs(const QString& trzDir, const QString& csvDir, const QString& hdf5Dir)
{
    QList<QString> rdgsDirsList;
    rdgsDirsList.clear();
    rdgsDirsList.push_back(trzDir);
    rdgsDirsList.push_back(csvDir);
    rdgsDirsList.push_back(hdf5Dir);

    QSettings setup("rdgsDirsList.ini", QSettings::IniFormat);
    setup.beginWriteArray("rdgsDirs");
    for (int groups=0; groups<1; groups++)
    {
        setup.setArrayIndex(groups);
        setup.setValue("rdgsDirsList", toVariant (rdgsDirsList));
    }
    setup.endArray();
    setup.sync();
}

inline void readLastNamesRdgsFromSettings(QStringList& filesRdgNamesTrz, std::vector<int>& trzNumAntennasVector, QStringList& filesRdgNamesCsv, QStringList& paperRdgsHdf5Names)
{
    //загрузка предыдущих радарограмм
    QSettings setupR("rdgsFilesNamesList.ini", QSettings::IniFormat);
    QList<QString> rdgsFilesList;
    int files_count = setupR.beginReadArray("rdgsFiles");
    for (int groups=0; groups<files_count; groups++)
    {
        setupR.setArrayIndex(groups);
        rdgsFilesList =  toList<QString>(setupR.value("rdgsFilesNamesList"));
    }

    for (int count = 0; count < rdgsFilesList.size(); count++)
    {
        if (rdgsFilesList[count].split(".trz").size() > 1)
        {
            QFile file(rdgsFilesList[count].split("&&&&").at(1).toStdString().c_str());
            bool fileTrzIsOpened = file.open(QIODevice::ReadOnly);
            if (
                (filesRdgNamesTrz.size() == 0) ||
                (
                    (filesRdgNamesTrz.size() > 0) &&
                    (std::find(filesRdgNamesTrz.begin(), filesRdgNamesTrz.end(), rdgsFilesList[count].split("&&&&").at(1)) == filesRdgNamesTrz.end())
                )
            )   if (fileTrzIsOpened) filesRdgNamesTrz.push_back(rdgsFilesList[count].split("&&&&").at(1));
            if (fileTrzIsOpened) trzNumAntennasVector.push_back((rdgsFilesList[count].split("&&&&").at(0)).split("&&").at(1).toInt());
            file.close();
        }
        if (rdgsFilesList[count].split(".csv").size() > 1)
        {
            QFile file(rdgsFilesList[count].split("&&&&").at(0));
            bool fileCsvIsOpened = file.open(QIODevice::ReadOnly);
            if (fileCsvIsOpened)
            {
                filesRdgNamesCsv.push_back(rdgsFilesList[count].split("&&&&").at(0));
                filesRdgNamesCsv.push_back(rdgsFilesList[count].split("&&&&").at(1));
            }
            file.close();
        }
        if (rdgsFilesList[count].split(".trz").size() == 1 && rdgsFilesList[count].split(".csv").size() == 1)   paperRdgsHdf5Names.push_back(rdgsFilesList[count]);
    }
}

inline void writeLastNamesRdgsToSettings(std::vector<std::pair<std::string, std::string>>  rdgsNamesVectorPairs)
{
    QList<QString> rdgsFilesNamesList;
    rdgsFilesNamesList.clear();
    for (int count = 0; count < rdgsNamesVectorPairs.size(); count++)
    {
        if (
            QString::fromStdString(rdgsNamesVectorPairs[count].second).split(".trz").size() == 1 &&
            QString::fromStdString(rdgsNamesVectorPairs[count].second).split(".csv").size() == 1
        )   rdgsFilesNamesList.push_back(QString::fromStdString(rdgsNamesVectorPairs[count].first));

        if (QString::fromStdString(rdgsNamesVectorPairs[count].second).split(".csv").size() > 1)
            rdgsFilesNamesList.push_back(QString::fromStdString(rdgsNamesVectorPairs[count].second));

        if (QString::fromStdString(rdgsNamesVectorPairs[count].second).split(".trz").size() > 1)
            rdgsFilesNamesList.push_back(
                QString::fromStdString(rdgsNamesVectorPairs[count].first) + QString::fromStdString("&&&&") + QString::fromStdString(rdgsNamesVectorPairs[count].second)
            );
    }

    QSettings setup("rdgsFilesNamesList.ini", QSettings::IniFormat);
    setup.beginWriteArray("rdgsFiles");
    for (int groups=0; groups<1; groups++)
    {
        setup.setArrayIndex(groups);
        setup.setValue("rdgsFilesNamesList", toVariant (rdgsFilesNamesList));
    }
    setup.endArray();
    setup.sync();
}

inline std::vector<std::tuple<int, int, int>> defVectorTuplesColors(const QColor& colorUp, const QColor& colorLow)
{
    int colorRED   = 0;
    int colorGREEN = 0;
    int colorBLUE  = 0;

    std::vector<std::tuple<int, int, int>> vectorTuplesColors;
    vectorTuplesColors.clear();

    for (int count = 255; count >=0; count--)
    {
        if ((colorUp.red() >= count) || (colorUp.green() >= count) || (colorUp.blue() >= count))
        {
            colorUp.red()   >= count ? colorRED   = count : colorRED   = colorUp.red();
            colorUp.green() >= count ? colorGREEN = count : colorGREEN = colorUp.green();
            colorUp.blue()  >= count ? colorBLUE  = count : colorBLUE  = colorUp.blue();
            vectorTuplesColors.push_back(std::make_tuple(colorRED, colorGREEN, colorBLUE));
        }
    }

    for (int count = 1; count <= 255; count++)
    {
        if ((colorLow.red() >= count) || (colorLow.green() >= count) || (colorLow.blue() >= count))
        {
            colorLow.red()   <= count ? colorRED   = colorLow.red()   : colorRED   = count;
            colorLow.green() <= count ? colorGREEN = colorLow.green() : colorGREEN = count ;
            colorLow.blue()  <= count ? colorBLUE  = colorLow.blue()  : colorBLUE  = count ;
            vectorTuplesColors.push_back(std::make_tuple(colorRED, colorGREEN, colorBLUE));
        }
    }
    return vectorTuplesColors;
}

inline void createPairHdf5RdgFiles( const QFileInfoList& folderItems, QString paperRdgName, int fileRdgId, std::pair<std::string, std::vector<std::string>>& pairHdf5RdgFiles)
{
    QString i_filename = folderItems.at(fileRdgId).canonicalFilePath();
    if (i_filename == "." || i_filename == ".." || i_filename.isEmpty()) return;
    if (i_filename.toStdString().find(".out") == i_filename.toStdString().length()-4  ||  i_filename.toStdString().find(".hdf5") == i_filename.toStdString().length()-5)
        pairHdf5RdgFiles.second.push_back((i_filename).toStdString());
}

inline double filterPM(double initValue) { return (1.0/(1.0+(initValue/kFilterPM)*(initValue/kFilterPM))); }

inline void definingRdgFilterData(st_rdgInfoData& rdgInfoData, int filterId)
{
    int width = rdgInfoData.vectorRdgData.size();
    int height = rdgInfoData.vectorRdgData[0].vectorsDoubleData[filterId].size();

    std::vector<double> availVectorData;
    availVectorData.resize(width * height);


    for (int i=0; i <= width-1; i++)
    {
        for (int j=0; j <= height-1; j++)
        {
            availVectorData[i + j*width] = (rdgInfoData.vectorRdgData[i].vectorsDoubleData[0])[j];
        }
    }

    double level = 0.0;

    double north = 0.0;
    double south = 0.0;
    double east  = 0.0;
    double west  = 0.0;

    do
    {
        for (int i=1; i <= width-2; i++)
        {
            for (int j=1; j <= height-2; j++)
            {
                north = availVectorData[i-1 + j*width    ] - availVectorData[i + j*width];
                south = availVectorData[i+1 + j*width    ] - availVectorData[i + j*width];
                east  = availVectorData[i   + (j+1)*width] - availVectorData[i + j*width];
                west  = availVectorData[i   + (j-1)*width] - availVectorData[i + j*width];

                (rdgInfoData.vectorRdgData[i].vectorsDoubleData[filterId])[j] = availVectorData[j*width + i] +
                deltaTFilterPM*(filterPM(north)*north+filterPM(south)*south+filterPM(east)*east+filterPM(west)*west);
            }
        }

        level = level + deltaTFilterPM;

        if (level < tFilterPM)
        {
            for (int i=0; i <= width-1; i++)
            {
                for (int j=0; j <= height-1; j++)
                {
                    availVectorData[i + j*width] = (rdgInfoData.vectorRdgData[i].vectorsDoubleData[filterId])[j];
                }
            }
        }
    }
    while(level < tFilterPM);
}

#endif // TRANSFORMATION_H
