#ifndef FILEIOMODEL_H_INCLUDED
#define FILEIOMODEL_H_INCLUDED

#include"Include.h"

/*
 * All the methods needed to process file IO are provided here.
 * Use getPage to get a BufType, and all the reading and writing should be done on this BufType.
 */

class FileIOModel
{
    static FileIOModel* instance;
    BufPageManager* bpm;
    FileManager* fm;

    FileIOModel();

public:
    static FileIOModel* getInstance();

    void createFile(const char* name);

    void openFile(const char* name, int& fileID);

    void closeFile(int fileID);

    void deleteFile(const char* name);

    BufType getPage(int fileID, int pageID, int& index);

    BufType allocatePage(int fileID, int pageID, int& index);

    void notifyPageUpdated(int index);

    void flush(int index);

    void getKey(int index, int& fileID, int& pageID);

    void release(int index);

    void flushAll();

};

#endif // FILEIOMODEL_H_INCLUDED

