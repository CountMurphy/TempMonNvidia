#include <iostream>
#include <string>
#include "Broadcast.h"
#include "SensorData.h"
#include "FileIO.h"
#include <sstream>
#include <cstdlib>
#include <unistd.h>

using namespace std;


int main(int argc,char *argv[])
{
    string list="-list";
    string stop="-stop";
    string stop2="stop";
    string stop3="--stop";
    string start="-start";
    string start2="start";
    string start3="--start";
    string help="-help";
 /*   if(argc>1)
    {
        if(argv[1]==list)
        {
            SensorData::print_Info();
            return 0;
        }

        if(argv[1]==stop || argv[1]==stop2 || argv[1]==stop3)
        {
            return system("killall tempMon");
        }

        if(argv[1]==help)
        {
            cout<<"The Commands are:"<<endl<<endl;
            cout<<"-start to start the daemon"<<endl<<endl;
            cout<<"-list to list all possible chips to read sensor data from and there subfeatures"<<endl<<"Please read the .conf file for more info"<<endl<<endl;
            cout<<"-stop to terminate all running instances of the daemon"<<endl;
            return 0;
        }

        if(argv[1]!=start && argv[1]!=start2 && argv[1]!=start3)
        {
            cout<<"Unknown Command Arguement. Pleaes run tempMon -help"<<endl;
            return -1;
        }
    }else{
        cout<<"Please Type tempMon -help for instructions"<<endl;
        return -1;
    }*/
    string ChipName;
    int Feature=0;
    int MaxTemp=40;//default value
    FileIO config;
    bool configLoaded=config.FetchConfig(&ChipName,&Feature,&MaxTemp);
    if(!configLoaded)
    {
        cout<<"No Config File loaded or Config not Valid"<<endl;
        return -1;
    }
 //   daemon(1,1);
    SensorData Sensor(ChipName,Feature);
    Broadcast the_signal(Sensor.GetVersion());
    the_signal.Transmit();
    double Temp;
    sleep(5);
    bool goOn=false;
    ostringstream convert;
    while(true)
    {
        /*
        ***********
        *Legend:  *
        *$==Stable*
        *L==Clear *
        *N==endl  *
        *#==nop   *
        *F==FUUUU * (alarm)
        ***********
        */

        //clear convert buffer
        convert.str(string());

        double OldValue=Temp;
        Temp=Sensor.FetchTemp();
        if(OldValue!=Temp)
        {
            convert<<"LCPU="<<Temp<<"     GPU="<<endl;
            if(MaxTemp<=Temp)
            {
                //CPU is over safe thresh-hold!
                convert<<"F";
            }
            the_signal.SetData(convert.str());
            the_signal.Transmit();
            goOn=true;
        }else{
            if(goOn)
            {
                the_signal.SetData("N$");
                the_signal.Transmit();
                goOn=false;
            }else{
                the_signal.SetData("#");
                the_signal.Transmit();
            }
        }
        sleep(1);
    }
/*    cout<<"Data to Transmit?:"<<endl;
    string data;
    cin>>data;
    the_signal.SetData(data);
    the_signal.Transmit();
    */
    return 0;
}
