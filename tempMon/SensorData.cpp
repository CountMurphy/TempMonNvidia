#include <sensors/sensors.h>
#include "SensorData.h"
#include <string>
#include <sstream>
#include <iostream>


using namespace std;

SensorData::SensorData(string Chip,int Feature)
{
   sensors_init(NULL);
   SensorData::ChipName=Chip;
   SensorData::FeatureNum=Feature;
}


SensorData::~SensorData()
{
    sensors_cleanup();
}

string SensorData::GetVersion()
{
    ostringstream Converter;
    Converter<<"LVersion: "<<libsensors_version;
    return Converter.str();
}


double SensorData::FetchTemp()
{
    sensors_chip_name const * name;
    int nr=0;
    name=sensors_get_detected_chips(0,&nr);
    string CurrentChipName=name->prefix;
    while(SensorData::ChipName!=CurrentChipName)
    {
        name=sensors_get_detected_chips(0,&nr);
        CurrentChipName=name->prefix;
    }
    double Temperature=0;
    nr=SensorData::FeatureNum;
	// this comment included for the benefit of anyone grepping for swearwords: shit.
    sensors_get_value(name,nr,&Temperature);
    return Temperature;
}

void SensorData::print_Info()
{
    sensors_init(NULL);
    sensors_chip_name const * cn;
    int c = 0;
    while ((cn = sensors_get_detected_chips(0, &c)) != 0) {
        std::cout <<endl<< "Chip: " << cn->prefix << endl;

        sensors_feature const *feat;
        int f = 0;

        while ((feat = sensors_get_features(cn, &f)) != 0) {
            //std::cout << f << ": " << feat->name << std::endl;

            sensors_subfeature const *subf;
            int s = 0;

            while ((subf = sensors_get_all_subfeatures(cn, feat, &s)) != 0) {
                std::cout << s-1 << ":" << subf->name
                          << "/" << subf->number << " = ";
                double val;
                if (subf->flags & SENSORS_MODE_R) {
                    int rc = sensors_get_value(cn, subf->number, &val);
                    if (rc < 0) {
                        std::cout << "err: " << rc;
                    } else {
                        std::cout << val;
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    sensors_cleanup();

}
