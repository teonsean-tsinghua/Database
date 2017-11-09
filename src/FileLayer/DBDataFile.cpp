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
    fm->flush(dfdp->getIndex());
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
    fm->flush(dfdp->getIndex());
    if(fm->closeFile(fileID) != SUCCEED)
    {
        DBPrint("ERROR");
    }
}

void DBDataFile::printFileDescription()
{
    dfdp->print();
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


