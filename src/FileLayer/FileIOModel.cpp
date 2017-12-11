#include"FileIOModel.h"

FileIOModel* FileIOModel::instance;

FileIOModel::FileIOModel()
{
    this->fm = new FileManager();
    this->bpm = new BufPageManager(fm);
}

FileIOModel* FileIOModel::getInstance()
{
    if(instance == NULL)
    {
        instance = new FileIOModel();
    }
    return instance;
}

void FileIOModel::notifyPageUpdated(int index)
{
    bpm->access(index);
    bpm->markDirty(index);
}

void FileIOModel::getKey(int index, int& fileID, int& pageID)
{
    bpm->getKey(index, fileID, pageID);
}

void FileIOModel::release(int index)
{
    bpm->release(index);
}

void FileIOModel::flush(int index)
{
    bpm->access(index);
    bpm->markDirty(index);
    bpm->writeBack(index);
}

void FileIOModel::flushAll()
{
    bpm->close();
}

void FileIOModel::deleteFile(const char* name)
{
    if(!remove(name) == 0)
    {
        throw Exception("File " + std::string(name) + " cannot be deleted.");
    }
}

void FileIOModel::createFile(const char* name)
{
    if(!fm->createFile(name))
    {
        throw Exception("File " + std::string(name) + " cannot be created.");
    }
}

void FileIOModel::openFile(const char* name, int& fileID)
{
    if(!fm->openFile(name, fileID))
    {
        throw Exception("File " + std::string(name) + " cannot be opened.");
    }
}

void FileIOModel::closeFile(int fileID)
{
    if(!fm->closeFile(fileID) == 0)
    {
        throw Exception("File cannot be closed.");
    }
}

BufType FileIOModel::allocatePage(int fileID, int pageID, int& index)
{
    BufType ret = getPage(fileID, pageID, index);
    memset(ret, 0, PAGE_SIZE);
    return ret;
}

BufType FileIOModel::getPage(int fileID, int pageID, int& index)
{
    return bpm->getPage(fileID, pageID, index);
}
