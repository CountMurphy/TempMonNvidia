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
        conf.open("tempMon.conf");
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
                    string Value="";
                    for(unsigned int i=eq+1;i<=line.length();i++)
                        {
                            Value+=line[i];
                        }
                    //Get CPU
                    if(line[0]=='C' && line[1]=='h' && line[2]=='i' && line[3]=='p')
                    {
                        *ChipName=Value;
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
