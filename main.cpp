#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include "Broadcast.h"
#include "SensorData.h"
#include "FileIO.h"
#include "Nvidia.h"

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
    string restart="-restart";
    string restart2="--restart";
    string restart3="restart";
    string help="-help";
    bool restarting=false;

    string ChipName;
    int Feature=0;
    int MaxTemp=40;//default value
    bool UseTmp=false;

    if(argc>1)
    {
        if(argv[1]==list)
        {
            SensorData::print_Info();
            return 0;
        }

        if(argv[1]==stop || argv[1]==stop2 || argv[1]==stop3 || argv[1]==restart || argv[1]==restart2 || argv[1]==restart3)
        {
            //return system("killall tempMon");
            FileIO configs;
            configs.FetchConfig(&ChipName,&Feature,&MaxTemp,&UseTmp);
            pid_t pid=configs.FetchPID(UseTmp);
            if(pid==-1)
            {
                cout<<"Something horribly wrong!  Cannot terminate Daemon.  Good luck sucker"<<endl;
            }else{
                kill(pid,SIGTERM);
                if(argv[1]==stop || argv[1]==stop2 || argv[1]==stop3)
                {
                    cout<<"Terminated"<<endl;
                    return 0;
                }else{
                    cout<<"Restarting"<<endl;
                    restarting=true;
                }
            }
        }

        if(argv[1]==help)
        {
            cout<<"The Commands are:"<<endl<<endl;
            cout<<"-start to start the daemon"<<endl<<endl;
            cout<<"-list to list all possible chips to read sensor data from and there subfeatures"<<endl<<"Please read the .conf file for more info"<<endl<<endl;
            cout<<"-stop to terminate all running instances of the daemon"<<endl;
            cout<<"-restart to restart daemon"<<endl;
            return 0;
        }

        if(!restarting && argv[1]!=start && argv[1]!=start2 && argv[1]!=start3)
        {
            cout<<"Unknown Command Arguement. Pleaes run tempMon -help"<<endl;
            return -1;
        }
    }else{
        cout<<"Please Type tempMon -help for instructions"<<endl;
        return -1;
    }

    FileIO Scribe;
    bool configLoaded=Scribe.FetchConfig(&ChipName,&Feature,&MaxTemp,&UseTmp);
    if(!configLoaded)
    {
        cout<<"No Config File loaded or Config not Valid"<<endl;
        return -1;
    }
    daemon(1,1);
    //Grab current PID for daemon termination
    pid_t PID=getpid();
    if(!Scribe.WritePID((int)PID,UseTmp))
    {
        cerr<<"Permission denied.  Must run as root."<<endl;
        return -1;
    }

    SensorData Sensor(ChipName,Feature);
    Broadcast the_signal(Sensor.GetVersion());
    the_signal.Transmit();

    Nvidia gpu;

    double Temp=0;
    unsigned int GPUTemp=0;
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

        double OldCPU=Temp;
        Temp=Sensor.FetchTemp();
        unsigned int OldGPU=GPUTemp;
        GPUTemp=gpu.FetchTemp();
        bool unStableCPU,unStableGPU;
        unStableCPU=OldCPU!=Temp;
        unStableGPU=OldGPU!=GPUTemp;
        string DataToSend="";


        //There seems to be a bug in lm_sensors.  Every now and then the reading will not be accurate (IE temp jumping 30 degrees in a second and then back down)
        //Check if high value is consistant for over a second
        if(MaxTemp<=Temp)
        {
            //CPU is over safe thresh-hold!
            DataToSend="F";
        }


        if(unStableCPU || unStableGPU )
        {
            convert<<"LCPU="<<Temp<<"   GPU="<<GPUTemp<<endl;

            if(!unStableCPU && unStableGPU)
            {
                convert<<"N$"<<endl;
            }
            if(unStableCPU && !unStableGPU)
            {
                convert<<"N         $"<<endl;
            }
            DataToSend+=convert.str();
            goOn=true;
        }else{
            if(goOn)
            {
                //insert Stable commands

                DataToSend+= "N$   $";
                goOn=false;
            }else{
                DataToSend+= "#";
            }
        }

        the_signal.SetData(DataToSend);
        the_signal.Transmit();
        sleep(1);
    }

    return 0;
}
