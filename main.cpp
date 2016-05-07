#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include "Broadcast.h"
#include "SensorData.h"
#include "FileIO.h"
#include "Nvidia.h"

using namespace std;


int main(int argc,char *argv[])
{
    string ChipName;
    int Feature=0;
    int MaxTemp=40;//default value
    bool UseTmp=false;

    if(argc>1)
    {
        string argument=argv[1];
        if(argument[0]=='-')
        {
            //remove dashes
            string tempVar="";
            for(unsigned int i=0;i<argument.length();i++)
            {
                if(argument[i]!='-')
                {
                    tempVar+=argument[i];
                }
            }
            tempVar+="\0";
            argument=tempVar;
        }

        if(argument=="list")
        {
            SensorData::print_Info();
            return 0;
        }

        if(argument=="stop" || argument=="restart")
        {
            FileIO configs;
            configs.FetchConfig(&ChipName,&Feature,&MaxTemp,&UseTmp);
            pid_t pid=configs.FetchPID(UseTmp);
            if(pid==-1)
            {
                cout<<"Something went horribly wrong!  Cannot terminate Daemon.  Good luck sucker"<<endl;
            }else{
                kill(pid,SIGTERM);
                if(argument=="stop")
                {
                    cout<<"Terminated"<<endl;
                    return 0;
                }else{
                    cout<<"Restarting"<<endl;
                }
            }
        }

        if(argument=="help")
        {
            cout<<"The Commands are:"<<endl<<endl;
            cout<<"--start to start the daemon"<<endl<<endl;
            cout<<"--list to list all possible chips to read sensor data from and there subfeatures"<<endl<<"Please read the .conf file for more info"<<endl<<endl;
            cout<<"--stop to terminate all running instances of the daemon"<<endl;
            cout<<"--restart to restart daemon"<<endl;
            return 0;
        }

        if(argument!="restart" && argument!="start")
        {
            cout<<"Unknown Command Arguement. Pleaes run tempMon --help"<<endl;
            return -1;
        }
    }else{
        cout<<"Please Type tempMon --help for instructions"<<endl;
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

    int failCount=0;
    while(true)
    {
        try
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
            bool StableCPU,StableGPU;
            StableCPU= (OldCPU==Temp);
            StableGPU= (OldGPU==GPUTemp);
            string DataToSend="";


            //There seems to be a bug in lm_sensors.  Every now and then the reading will not be accurate (IE temp jumping 30 degrees in a second and then back down)
            //Check if high value is consistant for over a second
            if(Temp>=MaxTemp && (OldCPU>=MaxTemp))
            {
                //CPU is over safe thresh-hold!
                DataToSend="F";
            }


            if(!StableCPU || !StableGPU )
            {
                convert<<"LCPU="<<(int)Temp<<"   GPU="<<GPUTemp<<endl;

                if(StableCPU && !StableGPU)
                {
                    convert<<"N$"<<endl;
                }
                if(!StableCPU && StableGPU)
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
        }
        catch(...)
        {
            //Something went wrong reading the libs.
            cerr<<"Could not read libs. Will Fail after 30 trys"<<endl;
            if(failCount>=30)
            {
                return -1;
            }
            failCount++;
        }
        sleep(1);
    }

    return 0;
}
