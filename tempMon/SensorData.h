#ifndef SENSORDATA_H
#define SENSORDATA_H
#include <string>
#include <sensors/sensors.h>

class SensorData
{
    public:
        SensorData(std::string,int);
        virtual ~SensorData();
        std::string GetVersion();
        double FetchTemp();
        static void print_Info();
    protected:
    private:
        std::string ChipName;
        int FeatureNum;

};

#endif // SENSORDATA_H
