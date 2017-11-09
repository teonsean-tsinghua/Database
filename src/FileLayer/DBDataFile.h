#ifndef DBDATAFILE_H_INCLUDED
#define DBDATAFILE_H_INCLUDED

#include"../DBInclude.h"
#include"DBPage.h"

class DBDataFile
{
private:
    int fileID;
    const char* root;
    DBFileIOModel* fm;
    DBDataFileDescriptionPage* dfdp;

public:
    DBDataFile(const char* root);

    void createFile(const char* name);

    void openFile(const char* name);

    void closeFile();

    void deleteFile(const char* name);

    void addField(const char* name, int type);

    void printFileDescription();
};

#endif // DBDATAFILE_H_INCLUDED
