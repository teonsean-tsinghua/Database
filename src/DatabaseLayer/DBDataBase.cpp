#include"DBDatabase.h"

DBDataBase* DBDataBase::instance = new DBDataBase();

DBDataBase::DBDataBase(const char* root):
    root(root)
{
    if(access(root, F_OK) != 0)
    {
        DBPrintLine("Cannot open root directory.");
        exit(0);
    }
}

void DBDataBase::createDatabase(const char* name_)
{
    char* path = new char[256];
    strcpy(path, root);
    strcat(path, "/");
    strcat(path, name_);
    if(access(path, F_OK) == 0)
    {
        DBPrintLine("File or directory already exists.");
        return;
    }
    name = name_;
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    DBLog("Created directory ");
    DBLogLine(path);
}

DBDataBase* DBDataBase::getInstance()
{
    if(instance == NULL)
    {
        instance = new DBDataBase();
    }
    return instance;
}
