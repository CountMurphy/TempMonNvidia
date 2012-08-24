#ifndef FILEIO_H
#define FILEIO_H
#include <string>

using namespace std;
class FileIO
{
    public:
        FileIO();
        virtual ~FileIO();
        bool FetchConfig(std::string *,int *,int *,bool*);
        bool WritePID(int,bool);
        pid_t FetchPID(bool);
    protected:
    private:
        void killPID_File(bool);
};

#endif // FILEIO_H
