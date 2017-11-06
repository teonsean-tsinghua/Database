#ifndef DBFILEIOMODEL_H_INCLUDED
#define DBFILEIOMODEL_H_INCLUDED

#include"../bufmanager/BufPageManager.h"

/*
 * All the methods needed to process file IO are provided here.
 * Use getPage to get a BufType, and all the reading and writing should be done on this BufType.
 */

class DBFileIOModel
{
    static DBFileIOModel* instance;
    BufPageManager* bpm;
    FileManager* fm;

    DBFileIOModel();

public:
    static DBFileIOModel* getInstance();

    int createFile(const char* name);

    int openFile(const char* name, int& fileID);

    int closeFile(int fileID);

    int deleteFile(const char* name);

    BufType getPage(int fileID, int pageID, int& index);

    BufType allocatePage(int fileID, int pageID, int& index);

    void notifyPageUpdated(int index);

    void flush(int index);

    void getKey(int index, int& fileID, int& pageID);

    void release(int index);

    void flushAll();

    const static int SUCCEED = 0;
    const static int ERROR = 1;
};

#endif // DBFILEIOMODEL_H_INCLUDED

