#ifndef FILEIO_H
#define FILEIO_H
#include <string>

class FileIO
{
    public:
        FileIO();
        virtual ~FileIO();
        bool FetchConfig(std::string *,int *,int *);
    protected:
    private:
};

#endif // FILEIO_H
