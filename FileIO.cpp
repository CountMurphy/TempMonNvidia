#include "FileIO.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;
FileIO::FileIO()
{
    //ctor
}

FileIO::~FileIO()
{
    //dtor
}


bool FileIO::FetchConfig(string *ChipName,int *Feature, int *MaxTemp,bool *UseTmp)
{
    try
    {

        ifstream conf;
        conf.open("/etc/tempMon.conf");
        if(conf.is_open())
        {
            string line;
            while(!conf.eof())
            {
                getline(conf,line);
                if(line[0]=='#' || line=="")
                {
                    continue;
                }else{
                    int eq=line.find_first_of('=');

                    string Var="";
                    for(int i=0;i<=eq-1;i++)
                    {
                        Var+=line[i];
                    }

                    string Value="";
                    for(unsigned int i=eq+1;i<=line.length();i++)
                        {
                            Value+=line[i];
                        }

                    //Get CPU
                    if(Var=="Chip")
                    {
                        *ChipName=Value.c_str();
                    }
                    if(Var=="Feature")
                    {
                        *Feature=atoi(Value.c_str());
                    }
                    if(Var=="Max")
                    {
                        *MaxTemp=atoi(Value.c_str());
                    }
                    if(Var=="UseTmp")
                    {
                        Value.resize(Value.size()-1,' ');
                        *UseTmp = Value =="true" || Value =="True"?true:false;
                    }
                }
            }
            return true;
        }
        return false;
    }
    catch(...)
    {
        //There is no config file
        return false;
    }
}



bool FileIO::WritePID(int PID,bool UseTmp)
{
    try
    {
        ofstream scribe;
        if(UseTmp)
        {
            scribe.open("/tmp/tempMon.pid",ios::out);
        }else{
            scribe.open("/var/run/tempMon.pid",ios::out);
        }
        if(!scribe)
        {
            //Permission denied
            return false;
        }
        scribe<<PID;
        scribe.close();
        return true;
    }
    catch(...)
    {
        return false;
    }
}


pid_t FileIO::FetchPID(bool UseTmp)
{
    try
    {
        pid_t pid=-1;
        ifstream scribe;
        if(UseTmp)
        {
            scribe.open("/tmp/tempMon.pid",ios::in);
        }else{
            scribe.open("/var/run/tempMon.pid",ios::in);
        }
        if(!scribe)
        {
            //Permission denied
            return pid;
        }
        scribe>>pid;
        //Delete pid file so future instances of tempMon do not kill random processes
        killPID_File(UseTmp);
        return pid;
    }
    catch(...)
    {
        return -1;
    }
}

void FileIO::killPID_File(bool UseTmp)
{
    if(UseTmp)
    {
        remove("/tmp/tempMon.pid");
    }else{
        remove("/var/run/tempMon.pid");
    }
}
