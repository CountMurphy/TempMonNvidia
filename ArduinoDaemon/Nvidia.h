#ifndef NVIDIA_H
#define NVIDIA_H
#include "nvml.h"

class Nvidia
{
    public:
        Nvidia();
        virtual ~Nvidia();
        double FetchTemp();
    protected:
    private:
};

#endif // NVIDIA_H
