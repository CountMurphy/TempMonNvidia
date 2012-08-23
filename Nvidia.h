#ifndef NVIDIA_H
#define NVIDIA_H
#include "nvml.h"

class Nvidia
{
    public:
        Nvidia();
        virtual ~Nvidia();
        unsigned int FetchTemp();
    protected:
    private:
};

#endif // NVIDIA_H
