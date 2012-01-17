#ifndef BROADCAST_H
#define BROADCAST_H
#include "string.h"

class Broadcast
{
    public:
        Broadcast(std::string);
        virtual ~Broadcast();
        void Transmit();
        void SetData(std::string);
    protected:
    private:
        std::string Data;
};

#endif // BROADCAST_H
