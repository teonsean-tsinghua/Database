#include<DBFileManager.h>

DBFileManager::DBFileManager(std::string root)
{
    this.root = root;
    this.fileManager = new FileManager();
    this.bufPageManager = new BufPageManager(this.fileManager);
}

DBFileManager::~DBFileManager()
{
    delete this.bufPageManager;
    delete this.fileManager;
}
