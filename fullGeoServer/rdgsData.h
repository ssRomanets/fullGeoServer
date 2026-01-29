#ifndef RDGSDATA_H
#define RDGSDATA_H

#include <QByteArray>
#include <QVector3D>
#include <map>
#include <vector>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

enum class RdgFileFormat {Trz, Csv, Hdf5};

struct st_rdgInitialData {
    const char* header; // rdgs  (//3 вводных байта)
    int version_number;   //1 байт
    quint64 block_count;  //8 байт
    int antennas_working; //1 байт

    double block_step_size_m;  //8 байт
    double antennas_step_size; //8 байт
    double time_step_ns;       //8 байт
    double freq_step_GHz;      //8 байт
    double freq_begin_GHz;     //8 байт
    double freq_end_GHz;       //8 байт
    double latitude_degree;    //8 байт
    double longitude_degree;   //8 байт

    int time_hours;   // время час     //1 байт
    int time_minutes; // время минута  //1 байт
    int time_seconds; // время секунда //1 байт
    quint16 time_milliseconds;         //2 байта
    int day;   // день                 //1 байт
    int month; // месяц                //1 байт
    quint16 year; // год               //2 байта

    const char* signal_character;      //  sgl (//3 вводных байта)
    quint16 begin_signal_index;  //2 байта
    quint16 end_signal_index;    //2 байта
    quint16 signal_length;       //2 байта

    // double
    std::vector<std::vector<double>> vectorsDoubleData;

    //additional
    std::vector<double> vectorMinImpulses;
    std::vector<double> vectorMaxImpulses;
    std::vector<std::vector<double> > vectorsDeeps;
};

struct st_rdgInfoData {
    std::vector<st_rdgInitialData>                              vectorRdgData;
    std::vector<double>                                         vectorMinLog10RdgData;
    std::vector<double>                                         vectorMaxLog10RdgData;
    std::vector<std::vector<QPair<int, int> >>                  vectorsRdgLog10PairXY;
    std::vector<std::map<int, std::vector<QPair<int, int> > >>  vectorMapAutoLog10RdgPairXY;
    double                                                      contrastLog10RdgKoeff{0.0};
    int quantImpulsesOfPacket {0};
};

struct st_rdgsWorkData {
    int rdgsSurfMinWidth {0};
    int rdgsSurfWidth    {0};
    int rdgsSurfHeight   {0};

    double leftLatitude  {-1.0};
    double rightLatitude {-1.0};
    double lowLongitude  {-1.0};
    double upLongitude   {-1.0};

    double deltaLatitude  {0.0};
    double deltaLongitude {0.0};

    double maxRelief {0.0};
    double minRelief {0.0};

    std::vector<std::tuple<std::string, int> >  vectorRdgsData;
    std::vector<std::vector<QVector3D>>     vectorRdgsInRelief;
    std::vector<std::vector<double> >       vectorRdgsFnRelief;
};

#endif // RDGSDATA_H
