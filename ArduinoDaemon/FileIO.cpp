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


bool FileIO::FetchConfig(string *ChipName,int *Feature, int *MaxTemp)
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
                    //we have found our config value
                    int comma=line.find_first_of(',');
                    for(int i=0;i<comma;i++)
                    {
                        *ChipName+=line[i];
                    }
                    int length=line.length();
                    string FeatureNum;
                    for(int i=comma+1;i<length;i++)
                    {
                        if(line[i]!=';')
                            FeatureNum+=line[i];
                    }
                    *Feature=atoi(FeatureNum.c_str());
                    comma=line.find_last_of(',');
                    string MaxStr;
                    for(int i=comma+1;i<length;i++)
                    {
                        if(line[i]!=';')
                            MaxStr+=line[i];
                    }
                    *MaxTemp=atoi(MaxStr.c_str());
                    return true;
                }
            }
        }
        return false;
    }
    catch(...)
    {
        //There is no config file
        return false;
    }
}
