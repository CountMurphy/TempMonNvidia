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
