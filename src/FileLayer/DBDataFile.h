#ifndef DBDATAFILE_H_INCLUDED
#define DBDATAFILE_H_INCLUDED

#include"../DBInclude.h"
#include"DBDataFileDescriptionPage.h"

class DBDataFile
{
private:
    int fileID;
    const char* root;
    DBFileIOModel* fm;
    DBDataFileDescriptionPage* dfdp;

public:
    DBDataFile(const char* root);

    int createFile(const char* name);

    int openFile(const char* name);

    int closeFile();

    int deleteFile(const char* name);

    int addField(const char* name, int type, bool nullable);

    int setPrimaryKey(const char* name);

    void printFileDescription();
};

#endif // DBDATAFILE_H_INCLUDED
