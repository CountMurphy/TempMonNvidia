#include <SerialStream.h>
#include "string.h"
#include "Broadcast.h"
#include <iostream>

using namespace std;
using namespace LibSerial;


Broadcast::Broadcast(string info)
{
    //Dry fire
    Data="";
    Transmit();
    sleep(2);
    Data="L"; //clear screen
    Data+=info;
}

Broadcast::~Broadcast()
{
    //dtor
}

void Broadcast::SetData(string info)
{
    Data=info;
    return;
}



void Broadcast::Transmit()
{
    SerialStream mySerial;
    mySerial.Open("/dev/ttyUSB0");
    mySerial.SetBaudRate(SerialStreamBuf::BAUD_9600);
    mySerial << Data;
}
