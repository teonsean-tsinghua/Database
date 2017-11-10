#include"DBDataFile.h"

DBDataFile::DBDataFile(const char* root):
    root(root)
{
    fm = DBFileIOModel::getInstance();
}

int DBDataFile::createFile(const char* name)
{
    if(fm->createFile(name) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, false);
    fm->flush(dfdp->getIndex());
    fm->closeFile(fileID);
}

int DBDataFile::deleteFile(const char* name)
{
    if(fm->deleteFile(name) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
}

int DBDataFile::closeFile()
{
    fm->flush(dfdp->getIndex());
    if(fm->closeFile(fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
}

void DBDataFile::printFileDescription()
{
    dfdp->print();
}

int DBDataFile::addField(const char* name, int type)
{
    return dfdp->addField(name, type);
}

int DBDataFile::openFile(const char* name)
{
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, true);
}


