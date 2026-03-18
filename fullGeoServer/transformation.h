#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "bscansData.h"
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
#define promilleParam 1000.00
#define deltaParam 0.0000001
#define deltaPixels 4
#define scaleFactor 80

#define limitBscanWidth 400

#define tFilterPM      10.00
#define deltaTFilterPM  0.20
#define kFilterPM       5.00

#define bscanMetricKoeff 0.000001

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

inline void createVectorBscansPoints(const st_bscanInfoData& bscanInfoData, int i, double deltaLatitude, double deltaLongitude, double* bscansPoints, int& dimBscans)
{
    if (i == bscanInfoData.vectorBscanData.size()-1)
    {
        bscansPoints[0] = bscanInfoData.vectorBscanData[i].latitude_degree;
        bscansPoints[1] = bscanInfoData.vectorBscanData[i].longitude_degree;
        dimBscans = 2;
    }
    else
    {
        double inPointLat   = bscanInfoData.vectorBscanData[i].latitude_degree;
        double inPointLong  = bscanInfoData.vectorBscanData[i].longitude_degree;
        double fnPointLat   = bscanInfoData.vectorBscanData[i+1].latitude_degree;
        double fnPointLong  = bscanInfoData.vectorBscanData[i+1].longitude_degree;

        int quantLat  = (int)fabs((fnPointLat-inPointLat)/deltaLatitude) + 1;
        int quantLong = (int)fabs((fnPointLong-inPointLong)/deltaLongitude) + 1;

        if (quantLat >= quantLong)
        {
            for (int count = 0; count <quantLat-1; count++)
            {
                double latitude  = inPointLat  + ((fnPointLat-inPointLat)/(fabs(fnPointLat-inPointLat)))*deltaLatitude*count;
                double longitude = inPointLong + (latitude-inPointLat)*(fnPointLong - inPointLong)/(fnPointLat - inPointLat);
                bscansPoints[2*count]   = latitude;
                bscansPoints[2*count+1] = longitude;
            }

            dimBscans = 2*(quantLat-1);
        }
        else
        {
            for (int count = 0; count <quantLong-1; count++)
            {
                double longitude  = inPointLong  + ((fnPointLong-inPointLong)/(fabs(fnPointLong-inPointLong)))*deltaLongitude*count;
                double latitude = inPointLat + (longitude-inPointLong)*(fnPointLat - inPointLat)/(fnPointLong - inPointLong);
                bscansPoints[2*count]   = latitude;
                bscansPoints[2*count+1] = longitude;
            }

            dimBscans = 2*(quantLong-1);
        }
    }
}

inline std::tuple<std::string, int>  defTupleBscans(
        const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap,int i, int j,
        double leftLatitude, double rightLatitude, double lowLongitude, double upLongitude,
        double deltaLatitude, double deltaLongitude, int bscansSurfWidth, int bscansSurfHeight,
        double* bscansPoints
    )
{
    int dimBscans = 0;
    for (auto bscansMapIter = bscansInfoDataMap.begin(); bscansMapIter != bscansInfoDataMap.end(); bscansMapIter++)
    {
        for (int k = 0; k < bscansMapIter->second.vectorBscanData.size(); k++)
        {
            createVectorBscansPoints( bscansMapIter->second, k, deltaLatitude, deltaLongitude, bscansPoints, dimBscans);

            for (int l = 0; l < dimBscans/2; l++)
            {
                if (i == 0 && j == 0)
                {
                    if (((bscansPoints[2*l]   >= leftLatitude + deltaLatitude*i ) && (fabs(bscansPoints[2*l]   - (leftLatitude + deltaLatitude*(i+1))) < deltaLatitude  )) &&
                        ( (bscansPoints[2*l+1] >= lowLongitude + deltaLongitude*j) && (fabs(bscansPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first, k);
                }
                else if (i == 0 && j == bscansSurfHeight-1)
                {
                    if (((bscansPoints[2*l]   >= leftLatitude + deltaLatitude*i     ) && (fabs(bscansPoints[2*l] - (leftLatitude + deltaLatitude*(i+1))) < deltaLatitude )) &&
                        ((bscansPoints[2*l+1] >  lowLongitude + deltaLongitude*(j-1)) && (fabs(bscansPoints[2*l+1] - upLongitude)                        <= deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first, k);
                }
                else if (i == bscansSurfWidth-1 && j == 0)
                {
                    if (((bscansPoints[2*l]  > leftLatitude + deltaLatitude*(i-1)) && (fabs(bscansPoints[2*l] - rightLatitude)                          <= deltaLatitude )) &&
                        ((bscansPoints[2*l+1] >= lowLongitude + deltaLongitude*j)  && (fabs(bscansPoints[2*l+1] -(lowLongitude + deltaLongitude*(j+1))) <  deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first, k);
                }
                else if (i == bscansSurfWidth-1 && j == bscansSurfHeight-1)
                {
                    if (((bscansPoints[2*l] > leftLatitude + deltaLatitude*(i-1))    && (fabs(bscansPoints[2*l] - rightLatitude) <= deltaLatitude )) &&
                        ((bscansPoints[2*l+1] > lowLongitude + deltaLongitude*(j-1)) && (fabs(bscansPoints[2*l+1] - upLongitude) <= deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first, k);
                }
                else if (i == 0 && (j != bscansSurfHeight-1 && j != 0) )
                {
                    if (((bscansPoints[2*l]   >= leftLatitude + deltaLatitude*i ) && (fabs(bscansPoints[2*l] - (leftLatitude + deltaLatitude*(i+1))   ) < deltaLatitude)) &&
                        ((bscansPoints[2*l+1] >= lowLongitude + deltaLongitude*j) && (fabs(bscansPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first, k);
                }
                else if ((i != bscansSurfWidth-1 && i != 0) && j == 0)
                {
                    if (((bscansPoints[2*l]   >= leftLatitude + deltaLatitude*i ) && (fabs(bscansPoints[2*l] - (leftLatitude + deltaLatitude*(i+1)))    < deltaLatitude )) &&
                        ((bscansPoints[2*l+1] >= lowLongitude + deltaLongitude*j) && (fabs(bscansPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first, k);
                }
                else if (i == bscansSurfWidth-1 && (j != bscansSurfHeight-1 && j != 0))
                {
                    if (((bscansPoints[2*l]   > leftLatitude + deltaLatitude*(i-1)) && (fabs(bscansPoints[2*l] - rightLatitude)                           <= rightLatitude)) &&
                        ((bscansPoints[2*l+1] >= lowLongitude + deltaLongitude*j  ) && (fabs(bscansPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first,  k);
                }
                else if ((i != bscansSurfWidth-1 && i != 0) && j == bscansSurfHeight-1)
                {
                    if (((bscansPoints[2*l]   >= leftLatitude + deltaLatitude*i)     && (fabs(bscansPoints[2*l] - (leftLatitude + deltaLatitude*(i+1))) <  deltaLatitude) ) &&
                        ((bscansPoints[2*l+1] > lowLongitude + deltaLongitude*(j-1)) && (fabs(bscansPoints[2*l+1] - upLongitude)                        <= deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first, k);
                }
                else
                {
                    if ( ((bscansPoints[2*l]   >= leftLatitude + deltaLatitude*i ) && (fabs(bscansPoints[2*l] - (leftLatitude + deltaLatitude*(i+1))   ) < deltaLatitude)) &&
                         ((bscansPoints[2*l+1] >= lowLongitude + deltaLongitude*j) && (fabs(bscansPoints[2*l+1] - (lowLongitude + deltaLongitude*(j+1))) < deltaLongitude)))
                        return std::make_tuple( bscansMapIter->first, k);
                }
            }
        }
    }
    return std::make_tuple("", -1);
}

inline void defLeftLatitude(const st_bscanInfoData& bscanInfoData, int i, double& leftLatitude)
{
    if (leftLatitude < 0.0)                                               leftLatitude = bscanInfoData.vectorBscanData[i].latitude_degree;
    else if (bscanInfoData.vectorBscanData[i].latitude_degree < leftLatitude) leftLatitude = bscanInfoData.vectorBscanData[i].latitude_degree;
}

inline void defRightLatitude(const st_bscanInfoData& bscanInfoData, int i, double& rightLatitude)
{
    if (rightLatitude < 0.0)                                               rightLatitude = bscanInfoData.vectorBscanData[i].latitude_degree;
    else if (bscanInfoData.vectorBscanData[i].latitude_degree > rightLatitude) rightLatitude = bscanInfoData.vectorBscanData[i].latitude_degree;
}

inline void defLowLongitude(const st_bscanInfoData& bscanInfoData, int i,  double& lowLongitude)
{
    if (lowLongitude < 0.0)  lowLongitude  = bscanInfoData.vectorBscanData[i].longitude_degree;
    else if (bscanInfoData.vectorBscanData[i].longitude_degree < lowLongitude) lowLongitude = bscanInfoData.vectorBscanData[i].longitude_degree;
}

inline void defUpLongitude(const st_bscanInfoData& bscanInfoData, int i,  double& upLongitude)
{
    if (upLongitude < 0.0)   upLongitude   = bscanInfoData.vectorBscanData[i].longitude_degree;
    else if (bscanInfoData.vectorBscanData[i].longitude_degree > upLongitude) upLongitude = bscanInfoData.vectorBscanData[i].longitude_degree;
}

inline void devMainLatLongParameters(const st_bscanInfoData& bscanInfoData,int i, double& leftLatitude, double& rightLatitude, double& lowLongitude, double& upLongitude)
{
    defLeftLatitude (bscanInfoData, i, leftLatitude);
    defRightLatitude(bscanInfoData, i, rightLatitude);
    defLowLongitude (bscanInfoData, i, lowLongitude);
    defUpLongitude  (bscanInfoData, i, upLongitude);
}

inline void devMainParameters(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap,
    double leftLatitude, double& rightLatitude,  double lowLongitude, double upLongitude,
    double& deltaLatitude, double& deltaLongitude, int& bscansSurfWidth, int& bscansSurfHeight
)
{
    std::vector<double> variousLatitudesVector;
    std::vector<double> variousLongitidesVector;
    variousLatitudesVector.resize(0);
    variousLongitidesVector.resize(0);

    for (auto bscansMapIter = bscansInfoDataMap.begin(); bscansMapIter != bscansInfoDataMap.end(); bscansMapIter++)
    {
        for (int i = 0; i <  bscansMapIter->second.vectorBscanData.size(); i++)
        {
            if (variousLatitudesVector.size() == 0) variousLatitudesVector.push_back( bscansMapIter->second.vectorBscanData[i].latitude_degree);
            else if (
                std::find(variousLatitudesVector.begin(), variousLatitudesVector.end(), bscansMapIter->second.vectorBscanData[i].latitude_degree)   == variousLatitudesVector.end()
            ) {  variousLatitudesVector.push_back( bscansMapIter->second.vectorBscanData[i].latitude_degree);}

            if (variousLongitidesVector.size() == 0) variousLongitidesVector.push_back( bscansMapIter->second.vectorBscanData[i].longitude_degree);
            else if (
                std::find(variousLongitidesVector.begin(), variousLongitidesVector.end(), bscansMapIter->second.vectorBscanData[i].longitude_degree) == variousLongitidesVector.end()
            ) {  variousLongitidesVector.push_back( bscansMapIter->second.vectorBscanData[i].longitude_degree);}

            defRightLatitude( bscansMapIter->second, i, rightLatitude);
        }
    }

    std::vector<double> variousDiffLatitudesVector;
    std::vector<double> variousDiffLongitidesVector;
    variousDiffLatitudesVector.resize(0);
    variousDiffLongitidesVector.resize(0);

    for (int i = 0; i < variousLatitudesVector.size() -1; i++) { variousDiffLatitudesVector.push_back(fabs(variousLatitudesVector[i+1] - variousLatitudesVector[i]));}
    for (int i = 0; i < variousLongitidesVector.size()-1; i++) { variousDiffLongitidesVector.push_back(variousLongitidesVector   [i+1] - variousLongitidesVector[i]);}

    deltaLatitude = *std::min_element(std::begin(variousDiffLatitudesVector), std::end(variousDiffLatitudesVector));
    bscansSurfWidth  = (int)( ((rightLatitude - leftLatitude)/deltaLatitude) + 1);

    deltaLongitude = *std::min_element(std::begin(variousDiffLongitidesVector), std::end(variousDiffLongitidesVector));
    bscansSurfHeight = (int)( ((upLongitude - lowLongitude)/deltaLongitude)  + 1);

    if (bscansSurfHeight < LongitudeDim) bscansSurfHeight = LongitudeDim; deltaLongitude = (upLongitude   - lowLongitude)/(bscansSurfHeight -1);
}


inline void defMaxDeep(
    const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, int materialId, int filterId, int selectionId, double& maxDeep, int absBscansInX,int absBscansFnX
)
{
    maxDeep = 0.0;
    for (auto bscansMapIter = bscansInfoDataMap.begin(); bscansMapIter != bscansInfoDataMap.end(); bscansMapIter++)
    {
        for (int i = absBscansInX; i <= ( bscansMapIter->second.vectorBscanData.size() - 1 >= absBscansFnX ? absBscansFnX : bscansMapIter->second.vectorBscanData.size() - 1); i++)
        {
            int size = ( bscansMapIter->second.vectorBscanData[i]).vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size();

            if ( bscansMapIter->second.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][size-1] > maxDeep)
                maxDeep = bscansMapIter->second.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId + selectionId][size-1];
        }
    }
}

inline void defMinMaxBscansRelief(std::vector<std::vector<double> > vectorBscansFnRelief, double& maxBscansRelief, double& minBscansRelief)
{
    minBscansRelief = -1.0;
    maxBscansRelief = -1.0;

    int width = vectorBscansFnRelief.size();
    int height = vectorBscansFnRelief[0].size();

    for (int count1 = 0; count1 < width; count1++)
    {
        for (int count2 = 0; count2 < height; count2++)
        {
            if (minBscansRelief < 0.0) minBscansRelief = vectorBscansFnRelief[count1][count2];
            else if (minBscansRelief > vectorBscansFnRelief[count1][count2]) minBscansRelief = vectorBscansFnRelief[count1][count2];

            if (maxBscansRelief < 0.0) maxBscansRelief = vectorBscansFnRelief[count1][count2];
            else if (maxBscansRelief < vectorBscansFnRelief[count1][count2]) maxBscansRelief = vectorBscansFnRelief[count1][count2];
        }
    }
}

inline std::string defDeleteBscanName(double fixLatitude, double fixLongitude, const std::map<std::string, st_bscanInfoData>&  bscansInfoDataMap)
{
    std::string result = "";
    double minLatLongDelta = 0.0;

    for (auto bscansMapIter = bscansInfoDataMap.begin(); bscansMapIter != bscansInfoDataMap.end(); bscansMapIter++)
    {
        for (int i = 0; i < bscansMapIter->second.vectorBscanData.size(); i++)
        {
            if (fabs(minLatLongDelta) < deltaParam ||  minLatLongDelta > sqrt(
                ( bscansMapIter->second.vectorBscanData[i].latitude_degree  - fixLatitude)*
                ( bscansMapIter->second.vectorBscanData[i].latitude_degree  - fixLatitude) +
                ( bscansMapIter->second.vectorBscanData[i].longitude_degree - fixLongitude)*
                ( bscansMapIter->second.vectorBscanData[i].longitude_degree - fixLongitude)
            ))
            {
                minLatLongDelta = sqrt(
                    ( bscansMapIter->second.vectorBscanData[i].latitude_degree  - fixLatitude)*
                    ( bscansMapIter->second.vectorBscanData[i].latitude_degree  - fixLatitude) +
                    ( bscansMapIter->second.vectorBscanData[i].longitude_degree - fixLongitude)*
                    ( bscansMapIter->second.vectorBscanData[i].longitude_degree - fixLongitude)
                );
                result = bscansMapIter->first;
            }
        }
    }
    return result;
}

inline void defBscansInRelief(QString fileName, std::vector<std::vector<QVector3D>>& vectorBscansInRelief)
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
        vectorBscansInRelief.resize(width);

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
                vectorBscansInRelief[count1].push_back(QVector3D(latitude, longitude, z));
            }
        }
    }

}

inline void defMaxQuantImpulsesOfPacket(const std::map<std::string, st_bscanInfoData>&  bscansInfoDataMap, int& maxQuantImpulsesOfPacket)
{
    maxQuantImpulsesOfPacket = 0;
    for (auto bscansMapIter = bscansInfoDataMap.begin(); bscansMapIter != bscansInfoDataMap.end(); bscansMapIter++)
    {
        if (maxQuantImpulsesOfPacket == 0 || maxQuantImpulsesOfPacket < bscansMapIter->second.quantImpulsesOfPacket)
            maxQuantImpulsesOfPacket = bscansMapIter->second.quantImpulsesOfPacket;
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

inline double defbscanLog10(const st_bscanInfoData& bscanInfoData, int filterId, int i, int j)
{
    double result = 0.0;

    if (
        (bscanInfoData.vectorBscanData[i].vectorsDoubleData[filterId])[j] >=0 &&
        (bscanInfoData.vectorBscanData[i].vectorsDoubleData[filterId])[j] >= bscanInfoData.contrastLog10BscanKoeff*bscanInfoData.vectorBscanData[i].vectorMaxImpulses[filterId]
    )
        result = log10(
            1 + (bscanInfoData.vectorBscanData[i].vectorsDoubleData[filterId])[j] -
            bscanInfoData.contrastLog10BscanKoeff*(bscanInfoData.vectorBscanData[i].vectorMaxImpulses[filterId])
        );

    if (
        (bscanInfoData.vectorBscanData[i].vectorsDoubleData[filterId])[j] < 0 &&
        fabs((bscanInfoData.vectorBscanData[i].vectorsDoubleData[filterId])[j]) >= fabs(bscanInfoData.contrastLog10BscanKoeff*bscanInfoData.vectorBscanData[i].vectorMinImpulses[filterId])
    )
    {
        result = -1.0*log10(
            1 + fabs(
                (bscanInfoData.vectorBscanData[i].vectorsDoubleData[filterId])[j] -
                bscanInfoData.contrastLog10BscanKoeff*bscanInfoData.vectorBscanData[i].vectorMinImpulses[filterId]
            )
        );

    }
    return result;
}

inline void createVectorbscanLog10PairXY(
    int filterId, int bscanHeight, int absbscanPixelsInX, int absbscanPixelsFnX, st_bscanInfoData& bscanInfoData
)
{
    (bscanInfoData.vectorsBscanLog10PairXY[filterId]).resize(0);

    QPair<int, int> pairXY;
    double* pixelsDoubleData = new double [((absbscanPixelsFnX-absbscanPixelsInX)+3)*bscanHeight];

    double bscanLog10 = 0.0;
    for (int i = absbscanPixelsInX; i <= absbscanPixelsFnX + 2; i++)
    {
        for (int j=0; j < bscanHeight; j++)
        {
            if ((i == absbscanPixelsInX) || (i == absbscanPixelsFnX + 2)) bscanLog10 = bscanInfoData.vectorMinLog10BscanData[filterId];
            else                                                      bscanLog10 = defbscanLog10(bscanInfoData,filterId,i-1,j);

            if (
                (double)(bscanLog10 - bscanInfoData.vectorMinLog10BscanData[filterId])/
                (bscanInfoData.vectorMaxLog10BscanData[filterId] - bscanInfoData.vectorMinLog10BscanData[filterId]) > 0.3
            )
                pixelsDoubleData[((absbscanPixelsFnX-absbscanPixelsInX)+3)*j+i-absbscanPixelsInX] = 0.0;
            else
                pixelsDoubleData[((absbscanPixelsFnX-absbscanPixelsInX)+3)*j+i-absbscanPixelsInX] = 0.3;
        }
    }

    for (int i = absbscanPixelsInX; i <= absbscanPixelsFnX + 2; i++)
    {
        for (int j = 0; j < bscanHeight; j++)
        {
            int quantPoints = 0;
            if (pixelsDoubleData[((absbscanPixelsFnX-absbscanPixelsInX)+3)*j+i-absbscanPixelsInX] < deltaParam)
            {
                for (int i1 = i-1; i1 <= i+1; i1++)
                {
                    for (int j1 = j-1; j1 <= j+1; j1++)
                    {
                        if (
                                ((i1 >= absbscanPixelsInX && i1 <= absbscanPixelsFnX + 2) && (j1 >= 0 && j1 <= bscanHeight -1))    &&
                                (pixelsDoubleData[((absbscanPixelsFnX-absbscanPixelsInX)+3)*j1+i1-absbscanPixelsInX] > deltaParam)
                           )
                           quantPoints++;
                    }
                }
                if (quantPoints >= 1 && quantPoints < 8)
                {
                    if ((i != absbscanPixelsInX) && (i != absbscanPixelsFnX + 2))
                    {
                        pairXY.first  = i-1;
                        pairXY.second = j;
                        (bscanInfoData.vectorsBscanLog10PairXY[filterId]).push_back(pairXY);
                    }
                }
            }
        }
    }
    delete [] pixelsDoubleData;
}

inline void outputVectorbscansSurfSelectArea(
    int bscansSurfPixelsInX, int bscansSurfPixelsInY, int bscansSurfPixelsFnX, int bscansSurfPixelsFnY,
    int bscansSurfPixelsWidth,  std::vector<QPair<int, int> > vectorPairXY,  GLubyte* bscansSurfPixelsData
)
{
    if (vectorPairXY.size() > 0)
    {
        for (int count = 0; count < vectorPairXY.size(); count++)
        {
            if (
               (vectorPairXY[count].first  >= bscansSurfPixelsInX && vectorPairXY[count].first  <= bscansSurfPixelsFnX) &&
               (vectorPairXY[count].second >= bscansSurfPixelsInY && vectorPairXY[count].second <= bscansSurfPixelsFnY)
            )
            {
                bscansSurfPixelsData[4*bscansSurfPixelsWidth*(vectorPairXY[count].second-bscansSurfPixelsInY) + 4*(vectorPairXY[count].first-bscansSurfPixelsInX)+0] = 255;
                bscansSurfPixelsData[4*bscansSurfPixelsWidth*(vectorPairXY[count].second-bscansSurfPixelsInY) + 4*(vectorPairXY[count].first-bscansSurfPixelsInX)+1] = 0;
                bscansSurfPixelsData[4*bscansSurfPixelsWidth*(vectorPairXY[count].second-bscansSurfPixelsInY) + 4*(vectorPairXY[count].first-bscansSurfPixelsInX)+2] = 0;
                bscansSurfPixelsData[4*bscansSurfPixelsWidth*(vectorPairXY[count].second-bscansSurfPixelsInY) + 4*(vectorPairXY[count].first-bscansSurfPixelsInX)+3] = 255;
            }
        }
    }
}

inline void outputVectorBscanSelectArea(
    int bscanPixelsWidth, int bscanPixelsInX, int bscanPixelsInY, int bscanPixelsFnX, int bscanPixelsFnY, std::vector<QPair<int, int> > vectorPairXY, GLubyte* pixelsData
)
{
    if (vectorPairXY.size() > 0)
    {
        for (int count = 0; count < vectorPairXY.size(); count++)
        {
            if ( (vectorPairXY[count].first  >= bscanPixelsInX && vectorPairXY[count].first  <= bscanPixelsFnX) &&
                 (vectorPairXY[count].second >= bscanPixelsInY && vectorPairXY[count].second <= bscanPixelsFnY))
            {
                pixelsData[4*bscanPixelsWidth*(vectorPairXY[count].second-bscanPixelsInY) + 4*(int)(vectorPairXY[count].first-bscanPixelsInX)+0] = 255;
                pixelsData[4*bscanPixelsWidth*(vectorPairXY[count].second-bscanPixelsInY) + 4*(int)(vectorPairXY[count].first-bscanPixelsInX)+1] = 0;
                pixelsData[4*bscanPixelsWidth*(vectorPairXY[count].second-bscanPixelsInY) + 4*(int)(vectorPairXY[count].first-bscanPixelsInX)+2] = 0;
                pixelsData[4*bscanPixelsWidth*(vectorPairXY[count].second-bscanPixelsInY) + 4*(int)(vectorPairXY[count].first-bscanPixelsInX)+3] = 255;
            }
        }
    }
}

inline void outputMapBscanSelectArea(
      int bscanPixelsWidth, int bscanPixelsHeight, int bscanPixelsInX, int bscanPixelsInY, int bscanPixelsFnX, int bscanPixelsFnY,
      const std::map<int, std::vector<QPair<int, int>> >& mapBscanPairXY, GLubyte* pixelsData)
{
    if (mapBscanPairXY.size() > 0)
    {
        for (auto itMap = mapBscanPairXY.begin(); itMap != mapBscanPairXY.end(); itMap++)
        {
            for (int count = 0; count < itMap->second.size(); count++)
            {
                if ( (itMap->second[count].first  >= bscanPixelsInX && itMap->second[count].first  <= bscanPixelsFnX) &&
                     (itMap->second[count].second >= bscanPixelsInY && itMap->second[count].second <= bscanPixelsFnY)
                )
                {
                    pixelsData[4*bscanPixelsWidth*(bscanPixelsHeight-1-(itMap->second[count].second-bscanPixelsInY))+4*(int)(itMap->second[count].first-bscanPixelsInX)+0] = 255;
                    pixelsData[4*bscanPixelsWidth*(bscanPixelsHeight-1-(itMap->second[count].second-bscanPixelsInY))+4*(int)(itMap->second[count].first-bscanPixelsInX)+1] =   0;
                    pixelsData[4*bscanPixelsWidth*(bscanPixelsHeight-1-(itMap->second[count].second-bscanPixelsInY))+4*(int)(itMap->second[count].first-bscanPixelsInX)+2] =   0;
                    pixelsData[4*bscanPixelsWidth*(bscanPixelsHeight-1-(itMap->second[count].second-bscanPixelsInY))+4*(int)(itMap->second[count].first-bscanPixelsInX)+3] = 255;
                }
            }
        }
    }
}

inline double defDepthScanUpLowBscan(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap,  std::string nameBscan,  int kBscan, int quantImpulsesOfPacket, int materialId, int filterId, int selectionId )
{
    double result = 0.0;
    auto itbscansInfoDataMap = bscansInfoDataMap.find(nameBscan);
    if (itbscansInfoDataMap != bscansInfoDataMap.end())
    {
        if (itbscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1 < quantImpulsesOfPacket)
            result = itbscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][itbscansInfoDataMap
                                      ->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1];
        else
            result = itbscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][quantImpulsesOfPacket];
    }
    return result;
}

inline double defDepthScanLowUpBscan(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap,  std::string nameBscan,  int kBscan, int quantImpulsesOfPacket, int materialId, int filterId, int selectionId)
{
    double result = 0.0;
    auto itbscansInfoDataMap = bscansInfoDataMap.find(nameBscan);
    if (itbscansInfoDataMap != bscansInfoDataMap.end())
    {
        if (itbscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1 < quantImpulsesOfPacket)
            result = 0.0;
        else
            result = itbscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][itbscansInfoDataMap
                                      ->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1] -
            itbscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][quantImpulsesOfPacket];
    }
    return result;
}

inline double defFullDepthScanLowUpBscan(const std::map<std::string, st_bscanInfoData>& bscansInfoDataMap,  std::string nameBscan,  int kBscan, int materialId, int filterId, int selectionId)
{
    double result = 0.0;
    auto itbscansInfoDataMap = bscansInfoDataMap.find(nameBscan);
    if (itbscansInfoDataMap != bscansInfoDataMap.end())
    {
        if (itbscansInfoDataMap->first == nameBscan)
            result = itbscansInfoDataMap->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][itbscansInfoDataMap
                                      ->second.vectorBscanData[kBscan].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId].size()-1];
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

inline void maskCellBscanSelected(int i, int j, int bscanHeight, int absbscanPixelsInX, int absbscanPixelsFnX,  int maskCell, std::vector<std::vector<int> >& vectorBscanSelectedPoints)
{
    vectorBscanSelectedPoints[i-absbscanPixelsInX][j] = maskCell;

    bool newMaskPointEx = false;
    for (int i1 = i-1; i1 <= i+1; i1++)
    {
        for (int j1 = j-1; j1 <= j+1; j1++)
        {
            if (((i1 >=absbscanPixelsInX && i1 <= absbscanPixelsFnX) && (j1 >=0 && j1 <= bscanHeight -1)) && ( i1 != i || j1 != j))
            {
                if (vectorBscanSelectedPoints[i1-absbscanPixelsInX][j1] == -1 && newMaskPointEx == false)
                {
                    maskCellBscanSelected(i1, j1, bscanHeight, absbscanPixelsInX, absbscanPixelsFnX, maskCell, vectorBscanSelectedPoints);
                    newMaskPointEx = true;
                }
            }
        }
    }
}

inline void createMapBscanPairXY(
    const std::vector<QPair<int, int> >& vectorbscanLog10PairXY, int bscanHeight, int absbscanPixelsInX, int absbscanPixelsFnX, std::map<int, std::vector<QPair<int, int>> >& mapAutoLog10BscanPairXY
)
{
    mapAutoLog10BscanPairXY.clear();
    if (vectorbscanLog10PairXY.size() > 0)
    {
        std::vector<std::vector<int> > vectorBscanSelectedPoints;

        vectorBscanSelectedPoints.resize(absbscanPixelsFnX-absbscanPixelsInX+1);

        for (int i = absbscanPixelsInX; i <= absbscanPixelsFnX; i++)  vectorBscanSelectedPoints[i-absbscanPixelsInX].resize(bscanHeight);

        for (int i = absbscanPixelsInX; i <= absbscanPixelsFnX; i++)
        {
            for (int j = 0; j < bscanHeight; j++)
            {
                vectorBscanSelectedPoints[i-absbscanPixelsInX][j] = 0.0;
            }
        }

        for (int i = 0; i < vectorbscanLog10PairXY.size(); i++)
        {
            vectorBscanSelectedPoints[vectorbscanLog10PairXY[i].first-absbscanPixelsInX][vectorbscanLog10PairXY[i].second] = -1;
        }

        int maskCell = 1;
        for (int i = absbscanPixelsInX; i <= absbscanPixelsFnX; i++)
        {
            for (int j = 0; j < bscanHeight; j++)
            {
                if (vectorBscanSelectedPoints[i-absbscanPixelsInX][j] == -1)
                {
                    maskCellBscanSelected(i, j, bscanHeight, absbscanPixelsInX, absbscanPixelsFnX,  maskCell, vectorBscanSelectedPoints);
                    maskCell++;
                }
            }
        }

        for (int i = 1; i <= maskCell-1; i++)  {   mapAutoLog10BscanPairXY[i].resize(0); }

        for (int i = absbscanPixelsInX; i <= absbscanPixelsFnX; i++)
        {
            for (int j = 0; j < bscanHeight; j++)
            {
                if (vectorBscanSelectedPoints[i-absbscanPixelsInX][j] > 0)
                {
                    QPair<int, int> pairXY;
                    pairXY.first  = i;
                    pairXY.second = j;
                    mapAutoLog10BscanPairXY[vectorBscanSelectedPoints[i-absbscanPixelsInX][j]].push_back(pairXY);
                }
            }
        }
    }
}

inline void  createMaskMapAllXY(
    int absbscanPixelsInX, int absbscanPixelsFnX, const std::map<int, std::vector<QPair<int, int>> >& mapBscanPairXY, std::map<int, std::map<int, std::vector<int>>>&  maskMapAllXY
)
{
    std::vector<std::vector<QPair<int, int>> > fullVectorPairsXY;
    fullVectorPairsXY.resize(absbscanPixelsFnX - absbscanPixelsInX + 1);
    QPair<int, int>                                       pairMY;
    std::map<int, std::vector<int>> maskMapXY;

    for (auto mapIterAvail = mapBscanPairXY.begin(); mapIterAvail != mapBscanPairXY.end(); mapIterAvail++)
    {
        for (int count1 = 0; count1 < mapIterAvail->second.size(); count1++)
        {
            pairMY.first  = mapIterAvail->first;
            pairMY.second = mapIterAvail->second[count1].second;
            fullVectorPairsXY[mapIterAvail->second[count1].first-absbscanPixelsInX].push_back(pairMY);
        }
    }

    for (int count1 = absbscanPixelsInX; count1 <= absbscanPixelsFnX; count1++)
    {
        if (fullVectorPairsXY[count1-absbscanPixelsInX].size() > 0)
        {
            maskMapXY.clear();
            for (int count2 = 0; count2 < fullVectorPairsXY[count1-absbscanPixelsInX].size(); count2++)
            {
                maskMapXY[fullVectorPairsXY[count1-absbscanPixelsInX][count2].first].push_back(fullVectorPairsXY[count1-absbscanPixelsInX][count2].second);
            }
            maskMapAllXY[count1] = maskMapXY;
        }
    }
}

inline void createMapBscanTypeBscanSelectionInfo(
    int bscanHeight, int absbscanPixelsInX, int absbscanPixelsFnX, int materialId, int filterId, int selectionId,
    const std::map<int, std::vector<QPair<int, int> > >&  mapAutoLog10BscanPairXY, st_bscanInfoData& bscanInfoData
)
{
    for (int i = absbscanPixelsInX; i <= absbscanPixelsFnX; i++)
    {
        for (int j=0; j < bscanHeight; j++)
        {
            bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][j] =
            bscanInfoData.vectorBscanData[i].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId            ][j];
        }
    }

    if (mapAutoLog10BscanPairXY.size() > 0)
    {
        std::map<int, std::map<int, std::vector<int>>> maskMapAllXY;
        createMaskMapAllXY(absbscanPixelsInX, absbscanPixelsFnX, mapAutoLog10BscanPairXY, maskMapAllXY);

        int X{0}, YIn{0}, YFn{0};
        double sumImpIn{0.0}, sumImpFn{0.0};

        for (auto mapIter1 = maskMapAllXY.begin(); mapIter1 != maskMapAllXY.end(); mapIter1++)
        {
            if (maskMapAllXY[mapIter1->first].size()>1)
            {
                X = mapIter1->first;

                std::pair<int, int> pairYInYFN;

                std::vector<std::pair<int, int>> vectorPairsYInYFn;
                vectorPairsYInYFn.resize(0);

                for (auto mapIter2 = maskMapAllXY[mapIter1->first].begin(); mapIter2 != maskMapAllXY[mapIter1->first].end(); mapIter2++)
                {
                    if (mapIter2->second.size()> 1)
                    {
                        for (int count = 0; count < mapIter2->second.size()-1; count++)
                        {
                            pairYInYFN.first  = mapIter2->second[0];
                            pairYInYFN.second = mapIter2->second[count+1];

                            bool newPairYInYFn = true;
                            if (vectorPairsYInYFn.size() > 0)
                            {
                                for (int count1 = 0; count1 <= vectorPairsYInYFn.size()-1; count1++)
                                {
                                    newPairYInYFn = !((pairYInYFN.first  > vectorPairsYInYFn[count1].first && pairYInYFN.first  < vectorPairsYInYFn[count1].second ) &&
                                                     (pairYInYFN.second > vectorPairsYInYFn[count1].first && pairYInYFN.second < vectorPairsYInYFn[count1].second ));
                                    if (newPairYInYFn == false) break;
                                }
                            }


                            if (newPairYInYFn == true)
                            {
                            YIn = pairYInYFN.first;
                            YFn =  pairYInYFN.second;

                            sumImpIn = bscanInfoData.vectorBscanData[X].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+0][YIn];
                            sumImpFn = bscanInfoData.vectorBscanData[X].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+0][YFn];

                            for (int count2 = YIn; count2<=YFn; count2++)
                            {
                                if (count2 < (YIn+YFn)/2.0)
                                    bscanInfoData.vectorBscanData[X].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][count2] = sumImpIn;
                                else
                                    bscanInfoData.vectorBscanData[X].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+selectionId][count2] = sumImpFn;
                            }

                            vectorPairsYInYFn.push_back(pairYInYFN);
                            }
                        }
                    }
                }
            }
        }
    }
}

inline void saveBscanAutoSelection(std::map<int, std::vector<QPair<int, int>> > mapBscanPairXY, double time_step_ns, QString saveBscanAutoSelectionDataFile)
{
    csvfile csv(saveBscanAutoSelectionDataFile.toStdString());
    csv <<"layerId" << "trackId"<<"impulseId"<<"time delay ns"<< endrow;
    for (auto itMap = mapBscanPairXY.begin(); itMap != mapBscanPairXY.end(); itMap++)
    {
        for (int i = 0; i <= itMap->second.size()-1; i++)
        {
            csv << itMap->first << itMap->second[i].first<<itMap->second[i].second<<itMap->second[i].second*time_step_ns<< endrow;
        }
    }
}

inline void loadBscanAutoSelection(QString loadBscanAutoSelectionDataFile, std::map<int, std::vector<QPair<int, int>> >& mapBscanPairXY)
{
    QPair<int, int> pairXY;
    QFile file(loadBscanAutoSelectionDataFile);
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
                mapBscanPairXY[layerID].push_back(pairXY);
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

inline void defVectorBscansTransLineSectionPoints(int inPosX, int inPosY, int fnPosX, int fnPosY, std::vector<std::pair<int, int>>& vectorBscansTransLinePoints)
{
    vectorBscansTransLinePoints.resize(0);

    std::pair<int, int> bscanPoint;

    vectorBscansTransLinePoints.resize(0);
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
        bscanPoint.first  = x;
        bscanPoint.second = y;
        vectorBscansTransLinePoints.push_back(bscanPoint);
    }
}


inline void wheelOpengl(
    double eventDelta, boolean allScaling, boolean hScaling, boolean vScaling, int absbscanPixelsInX, int absbscanPixelsFnX, int height,
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
        if       (pixelsWidth  < (absbscanPixelsFnX-absbscanPixelsInX+1)/scaleFactor ) { pixelsWidth  = (absbscanPixelsFnX-absbscanPixelsInX+1)/scaleFactor; }
        else if  (pixelsWidth  > (absbscanPixelsFnX-absbscanPixelsInX+1))              { pixelsWidth  = (absbscanPixelsFnX-absbscanPixelsInX+1);             }
        if (pixelsCentX + pixelsWidth/2 > absbscanPixelsFnX)
        {
            pixelsFnX = absbscanPixelsFnX;
            pixelsInX = pixelsFnX-pixelsWidth+1;
        }
        else if ( pixelsCentX - (pixelsWidth/2) < absbscanPixelsInX)
        {
            pixelsInX = absbscanPixelsInX;
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
    int absbscanPixelsInX, int absbscanPixelsFnX, int height, int pixelsWidth, int pixelsHeight,
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
        if (pixelsInX + dimX > absbscanPixelsFnX) deltaXIn =  absbscanPixelsFnX  - pixelsInX;
        if (pixelsInX + dimX < absbscanPixelsInX) deltaXIn = -pixelsInX;
        if (pixelsFnX + dimX > absbscanPixelsFnX) deltaXFn =  absbscanPixelsFnX - pixelsFnX;
        if (pixelsFnX + dimX < absbscanPixelsInX) deltaXFn = -pixelsFnX;
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
            (pixelsInX + dimX >= absbscanPixelsInX && pixelsInX + dimX <= absbscanPixelsFnX) &&
            (pixelsInY + dimY >= 0               && pixelsInY + dimY <= height - 1)      &&
            (pixelsFnX + dimX >= absbscanPixelsInX && pixelsFnX + dimX <= absbscanPixelsFnX) &&
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

inline void executeTypeBscanSelectionInfo(
    int materialId, int filterId, int selectionId, int bscanHeight, int absbscanPixelsInX, int absbscanPixelsFnX, st_bscanInfoData& bscanInfoData
)
{
    if (bscanInfoData.vectorBscanData.size()-1 <= absbscanPixelsInX) absbscanPixelsInX = bscanInfoData.vectorBscanData.size()-1;
    if (bscanInfoData.vectorBscanData.size()-1 <= absbscanPixelsFnX) absbscanPixelsFnX = bscanInfoData.vectorBscanData.size()-1;

    if (absbscanPixelsFnX > absbscanPixelsInX)
    {
        createVectorbscanLog10PairXY(filterId, bscanHeight, absbscanPixelsInX, absbscanPixelsFnX, bscanInfoData);
        createMapBscanPairXY(bscanInfoData.vectorsBscanLog10PairXY[filterId], bscanHeight, absbscanPixelsInX, absbscanPixelsFnX, bscanInfoData.vectorMapAutoLog10BscanPairXY[filterId]);
        createMapBscanTypeBscanSelectionInfo(bscanHeight, absbscanPixelsInX, absbscanPixelsFnX, materialId, filterId, selectionId, bscanInfoData.vectorMapAutoLog10BscanPairXY[filterId], bscanInfoData);
    }
}

inline void samplingAllLog10Bscans(
    int materialId, int filterId, std::vector<std::pair<std::string, std::string>> bscansNamesVectorPairs,
    std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, int absbscanPixelsInX, int absbscanPixelsFnX
)
{
    for (auto iter = bscansNamesVectorPairs.begin(); iter != bscansNamesVectorPairs.end(); iter++)
    {
        executeTypeBscanSelectionInfo(
            materialId, filterId, 2, bscansInfoDataMap[iter->first].quantImpulsesOfPacket, absbscanPixelsInX, absbscanPixelsFnX, bscansInfoDataMap[iter->first]
        );
    }
}

inline void createCsvBscansFiles(const QStringList& filesBscanNamesCsv, QStringList& filesBscanNamesPropsCsv, QStringList& filesBscanNamesDataCsv)
{
    for (int count = 0; count <= filesBscanNamesCsv.size()-1; count++)
    {
        QFile fileBscan(filesBscanNamesCsv.at(count));
        if (!fileBscan.open(QFile::ReadOnly | QFile::Text)) std::cout<<"File not exists"<<std::endl;
        else
        {
            QTextStream in(&fileBscan);
            QString line = in.readLine();

            if (line.split(";").size()< 20) filesBscanNamesPropsCsv.push_back(filesBscanNamesCsv.at(count));
            else                            filesBscanNamesDataCsv.push_back(filesBscanNamesCsv.at(count));
        }
    }
}

inline void allocateMainBscanContainers(st_bscanInfoData& bscanInfoData, int vectorBscanDataPos)
{
    (bscanInfoData.vectorBscanData[vectorBscanDataPos].vectorMinImpulses).resize(countFilters);
    (bscanInfoData.vectorBscanData[vectorBscanDataPos].vectorMaxImpulses).resize(countFilters);
    (bscanInfoData.vectorBscanData[vectorBscanDataPos].vectorsDeeps).resize(countSelectors*countFilters*countMaterials);
    (bscanInfoData.vectorsBscanLog10PairXY).resize(countFilters);
    (bscanInfoData.vectorMapAutoLog10BscanPairXY).resize(countFilters);
}

inline void createBscanDataMinMaxImpulses(st_bscanInfoData& bscanInfoData, int count1, int quantImpulsesOfPacket, int filterId)
{
    bscanInfoData.vectorBscanData[count1].vectorMinImpulses[filterId] = *std::min_element(
        bscanInfoData.vectorBscanData[count1].vectorsDoubleData[filterId].begin(),
        bscanInfoData.vectorBscanData[count1].vectorsDoubleData[filterId].end()
    );

    bscanInfoData.vectorBscanData[count1].vectorMaxImpulses[filterId] = *std::max_element(
        bscanInfoData.vectorBscanData[count1].vectorsDoubleData[filterId].begin(),
        bscanInfoData.vectorBscanData[count1].vectorsDoubleData[filterId].end()
    );
}

inline void createBscanDataDeeps(st_bscanInfoData& bscanInfoData, int count1, int quantImpulsesOfPacket, int filterId, int materialId)
{
    bscanInfoData.vectorBscanData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId + 0].resize(quantImpulsesOfPacket);

    double fixJDeep = 0;
    double sumDeeps = 0;
    double sumIntens = 0;
    for (int j=0; j < quantImpulsesOfPacket; j++)
    {
        if (j == 0)    sumIntens  = fabs(bscanInfoData.vectorBscanData[count1].vectorsDoubleData[filterId][j]*bscanMetricKoeff);
        else           sumIntens += fabs(bscanInfoData.vectorBscanData[count1].vectorsDoubleData[filterId][j]*bscanMetricKoeff);

        fixJDeep = 0.5*spc*nanokoef*bscanInfoData.vectorBscanData[count1].time_step_ns/sqrt(epsdData(materialId)*exp(gammaData(materialId)*sumIntens));
        sumDeeps += fixJDeep;
        bscanInfoData.vectorBscanData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+0][j] = sumDeeps;
    }

    for (int count = 1; count <=2; count++)
    {
        bscanInfoData.vectorBscanData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+count].resize(quantImpulsesOfPacket);
    }
    bscanInfoData.vectorBscanData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+1] =
    bscanInfoData.vectorBscanData[count1].vectorsDeeps[countFilters*countSelectors*materialId + countSelectors*filterId+0];
}

inline void defAdditionalBscanMainData(std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, int quantImpulsesOfPacket)
{
    for (auto mapIter1 = bscansInfoDataMap.begin(); mapIter1 != bscansInfoDataMap.end(); mapIter1++)
    {
        if (mapIter1->second.quantImpulsesOfPacket <= 0)
        {
            mapIter1->second.quantImpulsesOfPacket = quantImpulsesOfPacket;

            mapIter1->second.vectorMaxLog10BscanData.resize(countFilters);
            mapIter1->second.vectorMinLog10BscanData.resize(countFilters);

            int bscanWidth  = mapIter1->second.vectorBscanData.size();
            for (int count = 0; count <= countFilters-1; count++)
            {
                for (int i=0; i<bscanWidth; i++)
                {
                    if (i == 0)
                    {
                        mapIter1->second.vectorMaxLog10BscanData[count] =
                                log10(1 + mapIter1->second.vectorBscanData[i].vectorMaxImpulses[count]);
                        mapIter1->second.vectorMinLog10BscanData[count] =
                                -1.0*log10(1 + fabs(mapIter1->second.vectorBscanData[i].vectorMinImpulses[count]));
                    }
                    else
                    {
                        if (log10(1 + mapIter1->second.vectorBscanData[i].vectorMaxImpulses[count]) > mapIter1->second.vectorMaxLog10BscanData[count])
                            mapIter1->second.vectorMaxLog10BscanData[count] =
                                    log10(1 + mapIter1->second.vectorBscanData[i].vectorMaxImpulses[count]);

                        if (-1.0*log10(1 + fabs(mapIter1->second.vectorBscanData[i].vectorMinImpulses[count])) < mapIter1->second.vectorMinLog10BscanData[count])
                            mapIter1->second.vectorMinLog10BscanData[count] =
                                    -1.0*log10(1 + fabs(mapIter1->second.vectorBscanData[i].vectorMinImpulses[count]));
                    }
                }
            }
        }
    }
}

inline void correctBscanData(
    std::map<std::string, st_bscanInfoData>& bscansInfoDataMap, st_bscansWorkData& bscansWorkData,
    std::vector<std::pair<std::string, std::string>>& bscansNamesVectorPairs, std::string bscanFilesFullData
)
{
    std::pair<std::string, std::string> stringPair;
    for (auto mapIter = bscansInfoDataMap.begin(); mapIter != bscansInfoDataMap.end(); mapIter++)
    {
        std::vector<std::pair<std::string, std::string>>::iterator vectorPairsIt = std::find_if(
            bscansNamesVectorPairs.begin(), bscansNamesVectorPairs.end(), [&](const std::pair<std::string, std::string>& stringInPair)
            {
                return (stringInPair.first == mapIter->first);
            }
        );

        if (vectorPairsIt == bscansNamesVectorPairs.end())
        {
            stringPair.first  = mapIter->first;
            stringPair.second = bscanFilesFullData;
            bscansNamesVectorPairs.push_back(stringPair);
        }
    }

    //градусы (пришлось выдумать)
    bscansWorkData.leftLatitude  = 51.193075;
    bscansWorkData.lowLongitude  = 34.6777180;
    bscansWorkData.upLongitude   = 34.6778180;

    int height = bscansInfoDataMap.size();
    double deltaLongitude = (bscansWorkData.upLongitude - bscansWorkData.lowLongitude)/(2*height);
    double deltaLatitude  = 0.000001536;

    //фиксируем радарограммы исходя из произведенной обрезки хвостов
    int mapId = 0;
    for (auto mapIter = bscansInfoDataMap.begin(); mapIter != bscansInfoDataMap.end(); mapIter++)
    {
        for (int i=0; i<mapIter->second.vectorBscanData.size(); i++)
        {
            mapIter->second.vectorBscanData[i].latitude_degree  = bscansWorkData.leftLatitude + i          *deltaLatitude;
            mapIter->second.vectorBscanData[i].longitude_degree = bscansWorkData.lowLongitude + (1+2*mapId)*deltaLongitude;
        }
        mapId++;
    }
}

inline void representBscanLine(int inPosX, int inPosY, int fnPosX, int fnPosY, st_bscansWorkData& bscansWorkData, bool fnPosAct)
{
    std::vector<std::pair<int, int>> vectorBscansTransLinePoints;
    defVectorBscansTransLineSectionPoints(inPosX, inPosY,  fnPosX,  fnPosY, vectorBscansTransLinePoints);

    std::string nameBscan = "";
    int         kBscan    = 0;

    if (vectorBscansTransLinePoints.size() > 0)
    {
        if (fnPosAct == true)
        {
            nameBscan = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*fnPosY+fnPosX]);
            kBscan    = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*fnPosY+fnPosX]);
        }
        else
        {
            nameBscan = std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*inPosY+inPosX]);
            kBscan    = std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*inPosY+inPosX]);
        }

        int count = vectorBscansTransLinePoints.size()-1;
        while (count >= 0)
        {
            if ((std::get<0>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[count].second + vectorBscansTransLinePoints[count].first]) == "") &&
                (std::get<1>(bscansWorkData.vectorBscansData[bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[count].second + vectorBscansTransLinePoints[count].first]) == -1)
            ) {
                bscansWorkData.vectorBscansData[
                    bscansWorkData.bscansSurfWidth*vectorBscansTransLinePoints[count].second + vectorBscansTransLinePoints[count].first
                ] = std::make_tuple(nameBscan, kBscan);
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

inline void readLastBscansDirsFromSettings(QString& trzDir, QString& csvDir, QString& hdf5Dir)
{
    //загрузка предыдущих радарограмм
    QSettings setupR("bscansDirsList.ini", QSettings::IniFormat);
    QList<QString> bscansDirsList;
    int files_count = setupR.beginReadArray("bscansDirs");
    for (int groups=0; groups<files_count; groups++)
    {
        setupR.setArrayIndex(groups);
        bscansDirsList =  toList<QString>(setupR.value("bscansDirsList"));
        if (bscansDirsList.size() == 3)
        {
            if (QDir(trzDir).exists())  trzDir  = bscansDirsList.at(0);
            if (QDir(csvDir).exists())  csvDir  = bscansDirsList.at(1);
            if (QDir(hdf5Dir).exists()) hdf5Dir = bscansDirsList.at(2);
        }
    }
}

inline void writeLastBscansDirs(const QString& trzDir, const QString& csvDir, const QString& hdf5Dir)
{
    QList<QString> bscansDirsList;
    bscansDirsList.clear();
    bscansDirsList.push_back(trzDir);
    bscansDirsList.push_back(csvDir);
    bscansDirsList.push_back(hdf5Dir);

    QSettings setup("bscansDirsList.ini", QSettings::IniFormat);
    setup.beginWriteArray("bscansDirs");
    for (int groups=0; groups<1; groups++)
    {
        setup.setArrayIndex(groups);
        setup.setValue("bscansDirsList", toVariant (bscansDirsList));
    }
    setup.endArray();
    setup.sync();
}

inline void readLastNamesBscansFromSettings(QStringList& filesBscanNamesTrz, std::vector<int>& trzNumAntennasVector, QStringList& filesBscanNamesCsv, QStringList& paperBscansHdf5Names)
{
    //загрузка предыдущих радарограмм
    QSettings setupR("bscansFilesNamesList.ini", QSettings::IniFormat);
    QList<QString> bscansFilesList;
    int files_count = setupR.beginReadArray("bscansFiles");
    for (int groups=0; groups<files_count; groups++)
    {
        setupR.setArrayIndex(groups);
        bscansFilesList =  toList<QString>(setupR.value("bscansFilesNamesList"));
    }

    for (int count = 0; count < bscansFilesList.size(); count++)
    {
        if (bscansFilesList[count].split(".trz").size() > 1)
        {
            QFile file(bscansFilesList[count].split("&&&&").at(1).toStdString().c_str());
            bool fileTrzIsOpened = file.open(QIODevice::ReadOnly);
            if (
                (filesBscanNamesTrz.size() == 0) ||
                (
                    (filesBscanNamesTrz.size() > 0) &&
                    (std::find(filesBscanNamesTrz.begin(), filesBscanNamesTrz.end(), bscansFilesList[count].split("&&&&").at(1)) == filesBscanNamesTrz.end())
                )
            )   if (fileTrzIsOpened) filesBscanNamesTrz.push_back(bscansFilesList[count].split("&&&&").at(1));
            if (fileTrzIsOpened) trzNumAntennasVector.push_back((bscansFilesList[count].split("&&&&").at(0)).split("&&").at(1).toInt());
            file.close();
        }
        if (bscansFilesList[count].split(".csv").size() > 1)
        {
            QFile file(bscansFilesList[count].split("&&&&").at(0));
            bool fileCsvIsOpened = file.open(QIODevice::ReadOnly);
            if (fileCsvIsOpened)
            {
                filesBscanNamesCsv.push_back(bscansFilesList[count].split("&&&&").at(0));
                filesBscanNamesCsv.push_back(bscansFilesList[count].split("&&&&").at(1));
            }
            file.close();
        }
        if (bscansFilesList[count].split(".trz").size() == 1 && bscansFilesList[count].split(".csv").size() == 1)   paperBscansHdf5Names.push_back(bscansFilesList[count]);
    }
}

inline void writeLastNamesBscansToSettings(std::vector<std::pair<std::string, std::string>>  bscansNamesVectorPairs)
{
    QList<QString> bscansFilesNamesList;
    bscansFilesNamesList.clear();
    for (int count = 0; count < bscansNamesVectorPairs.size(); count++)
    {
        if (
            QString::fromStdString(bscansNamesVectorPairs[count].second).split(".trz").size() == 1 &&
            QString::fromStdString(bscansNamesVectorPairs[count].second).split(".csv").size() == 1
        )   bscansFilesNamesList.push_back(QString::fromStdString(bscansNamesVectorPairs[count].first));

        if (QString::fromStdString(bscansNamesVectorPairs[count].second).split(".csv").size() > 1)
            bscansFilesNamesList.push_back(QString::fromStdString(bscansNamesVectorPairs[count].second));

        if (QString::fromStdString(bscansNamesVectorPairs[count].second).split(".trz").size() > 1)
            bscansFilesNamesList.push_back(
                QString::fromStdString(bscansNamesVectorPairs[count].first) + QString::fromStdString("&&&&") + QString::fromStdString(bscansNamesVectorPairs[count].second)
            );
    }

    QSettings setup("bscansFilesNamesList.ini", QSettings::IniFormat);
    setup.beginWriteArray("bscansFiles");
    for (int groups=0; groups<1; groups++)
    {
        setup.setArrayIndex(groups);
        setup.setValue("bscansFilesNamesList", toVariant (bscansFilesNamesList));
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

inline void createPairHdf5BscanFiles( const QFileInfoList& folderItems, QString paperBscanName, int fileBscanId, std::pair<std::string, std::vector<std::string>>& pairHdf5BscanFiles)
{
    QString i_filename = folderItems.at(fileBscanId).canonicalFilePath();
    if (i_filename == "." || i_filename == ".." || i_filename.isEmpty()) return;
    if (i_filename.toStdString().find(".out") == i_filename.toStdString().length()-4  ||  i_filename.toStdString().find(".hdf5") == i_filename.toStdString().length()-5)
        pairHdf5BscanFiles.second.push_back((i_filename).toStdString());
}

inline double filterPM(double initValue) { return (1.0/(1.0+(initValue/kFilterPM)*(initValue/kFilterPM))); }

inline void definingBscanFilterData(st_bscanInfoData& bscanInfoData, int filterId)
{
    int width = bscanInfoData.vectorBscanData.size();
    int height = bscanInfoData.vectorBscanData[0].vectorsDoubleData[filterId].size();

    std::vector<double> availVectorData;
    availVectorData.resize(width * height);


    for (int i=0; i <= width-1; i++)
    {
        for (int j=0; j <= height-1; j++)
        {
            availVectorData[i + j*width] = (bscanInfoData.vectorBscanData[i].vectorsDoubleData[0])[j];
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

                (bscanInfoData.vectorBscanData[i].vectorsDoubleData[filterId])[j] = availVectorData[j*width + i] +
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
                    availVectorData[i + j*width] = (bscanInfoData.vectorBscanData[i].vectorsDoubleData[filterId])[j];
                }
            }
        }
    }
    while(level < tFilterPM);
}

#endif // TRANSFORMATION_H
