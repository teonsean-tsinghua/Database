#include"DBFileIOModel.h"

DBFileIOModel* DBFileIOModel::instance = new DBFileIOModel();

DBFileIOModel::DBFileIOModel()
{
    this->fm = new FileManager();
    this->bpm = new BufPageManager(fm);
}

DBFileIOModel* DBFileIOModel::getInstance()
{
    if(instance == NULL)
    {
        instance = new DBFileIOModel();
    }
    return instance;
}

void DBFileIOModel::notifyPageUpdated(int index)
{
    bpm->access(index);
    bpm->markDirty(index);
}

void DBFileIOModel::getKey(int index, int& fileID, int& pageID)
{
    bpm->getKey(index, fileID, pageID);
}

void DBFileIOModel::release(int index)
{
    bpm->release(index);
}

void DBFileIOModel::flush(int index)
{
    bpm->access(index);
    bpm->markDirty(index);
    bpm->writeBack(index);
}

void DBFileIOModel::flushAll()
{
    bpm->close();
}

int DBFileIOModel::deleteFile(const char* name)
{
    if(remove(name) == 0)
    {
        return SUCCEED;
    }
    return ERROR;
}

int DBFileIOModel::createFile(const char* name)
{
    if(fm->createFile(name))
    {
        return SUCCEED;
    }
    return ERROR;
}

int DBFileIOModel::openFile(const char* name, int& fileID)
{
    if(fm->openFile(name, fileID))
    {
        return SUCCEED;
    }
    return ERROR;
}

int DBFileIOModel::closeFile(int fileID)
{
    if(fm->closeFile(fileID) == 0)
    {
        return SUCCEED;
    }
    return ERROR;
}

BufType DBFileIOModel::allocatePage(int fileID, int pageID, int& index)
{
    BufType ret = getPage(fileID, pageID, index);
    memset(ret, 0, PAGE_SIZE);
    return ret;
}

BufType DBFileIOModel::getPage(int fileID, int pageID, int& index)
{
    return bpm->getPage(fileID, pageID, index);
}
