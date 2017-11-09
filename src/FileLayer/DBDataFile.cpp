#include"DBDataFile.h"

DBDataFile::DBDataFile(const char* root):
    root(root)
{
    fm = DBFileIOModel::getInstance();
}

void DBDataFile::createFile(const char* name)
{
    if(fm->createFile(name) != SUCCEED)
    {
        DBPrint("ERROR");
    }
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBPrint("ERROR");
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, false);
    dfdp->write();
    fm->notifyPageUpdated(index);
    fm->flush(index);
    fm->closeFile(fileID);
}

void DBDataFile::deleteFile(const char* name)
{
    if(fm->deleteFile(name) != SUCCEED)
    {
        DBPrint("ERROR");
    }
}

void DBDataFile::closeFile()
{
    if(fm->closeFile(fileID) != SUCCEED)
    {
        DBPrint("ERROR");
    }
}

void DBDataFile::printFileDescription()
{
    dfdp->printFileDescription();
}

void DBDataFile::addField(const char* name, int type)
{
    dfdp->addField(name, type);
}

void DBDataFile::openFile(const char* name)
{
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBPrint("ERROR");
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, true);
}


