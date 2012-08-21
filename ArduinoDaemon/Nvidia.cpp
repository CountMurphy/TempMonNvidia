#include "Nvidia.h"
#include "nvml.h"

using namespace std;

Nvidia::Nvidia()
{
    nvmlInit();
}

Nvidia::~Nvidia()
{
    //dtor
}


unsigned int Nvidia::FetchTemp()
{
    unsigned int DeviceCount;
     nvmlReturn_t Rval=nvmlDeviceGetCount(&DeviceCount);
     if(Rval!=0)
     {
         //Card read error
         return 0;
     }
     //Turn Count into index
     DeviceCount--;
     //Get Prereqs
     nvmlDevice_t Device;
     Rval=nvmlDeviceGetHandleByIndex(DeviceCount,&Device);
     if(Rval!=0)
     {
         //Card read error
         return 0;
     }
     nvmlTemperatureSensors_t TSensors=NVML_TEMPERATURE_GPU;

    //Get Temperature
     unsigned int Temp=0;
     Rval=nvmlDeviceGetTemperature(Device,TSensors,&Temp);
     if(Rval!=0)
     {
         //Card read error
         return 0;
     }
    return Temp;
}
