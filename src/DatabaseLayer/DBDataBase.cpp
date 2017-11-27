#include"DBDatabase.h"

DBDataBase* DBDataBase::instance = new DBDataBase();

DBDataBase::DBDataBase(const char* root):
    root(root), name(NULL)
{
    if(access(root, F_OK) != 0)
    {
        DBPrint::printLine("Cannot open root directory.");
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
        DBPrint::printLine("File or directory already exists.");
        return;
    }
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    DBPrint::log("Created directory ").logLine(path);
}

void DBDataBase::delete_path(const char* path)
{
    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, path);
    strcat(szFolderName, "/%s");
    if((pDir = opendir(path)) != NULL)
    {
        while((dmsg = readdir(pDir)) != NULL)
        {
            if(strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
            {
                sprintf(szFileName, szFolderName, dmsg->d_name);
                string tmp = szFileName;
                if(tmp.find(".") == -1){
                    delete_path(szFileName);
                }
                remove(szFileName);
            }
        }
    }
    if(pDir != NULL)
    {
        closedir(pDir);
    }
}

void DBDataBase::showTables()
{
    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, root);
    strcat(szFolderName, "/");
    strcat(szFolderName, name);
    if(name != NULL && (pDir = opendir(szFolderName)) != NULL)
    {
        strcat(szFolderName, "/%s");
        DBPrint::printLine("====================");
        DBPrint::print("All tables of ").print(name).printLine(" are listed as below:");
        while((dmsg = readdir(pDir)) != NULL)
        {
            if(strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
            {
                sprintf(szFileName, szFolderName, dmsg->d_name);
                string tmp = szFileName;
                if(opendir(szFileName) == NULL && tmp.substr(tmp.size() - 4, 4).compare(".dat") == 0){
                    string filename = std::string(dmsg->d_name);
                    DBPrint::printLine(filename.substr(0, filename.size() - 4));
                }
            }
        }
        DBPrint::printLine("====================");
    }
    else
    {
        DBPrint::printLine("Current database is not available.");
    }
    if(pDir != NULL)
    {
        closedir(pDir);
    }
}

void DBDataBase::showDatabases()
{
    DBPrint::printLine("====================");
    DBPrint::printLine("All databases are listed as below:");
    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, root);
    strcat(szFolderName, "/%s");
    if((pDir = opendir(root)) != NULL)
    {
        while((dmsg = readdir(pDir)) != NULL)
        {
            if(strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
            {
                sprintf(szFileName, szFolderName, dmsg->d_name);
                string tmp = szFileName;
                if(opendir(szFileName) != NULL)
                {
                    DBPrint::printLine(dmsg->d_name);
                }
            }
        }
    }
    DBPrint::printLine("====================");
    if(pDir != NULL)
    {
        closedir(pDir);
    }
}

void DBDataBase::useDatabase(const char* name_)
{
    char* path = new char[256];
    strcpy(path, root);
    strcat(path, "/");
    strcat(path, name_);
    if(access(path, F_OK) != 0)
    {
        DBPrint::printLine("Database does not exist.");
        return;
    }
    if (opendir(path) == NULL)
    {
        DBPrint::print(name_).printLine(" is not a Database.");
        return;
    }
    name = name_;
    DBPrint::log("Using database ").logLine(name);
}

void DBDataBase::dropDatabase(const char* name_)
{
    char* path = new char[256];
    strcpy(path, root);
    strcat(path, "/");
    strcat(path, name_);
    if(access(path, F_OK) != 0)
    {
        DBPrint::printLine("Database does not exist.");
        return;
    }
    delete_path(path);
    rmdir(path);
    DBPrint::log("Removed directory ").logLine(path);
}

DBDataBase* DBDataBase::getInstance()
{
    if(instance == NULL)
    {
        instance = new DBDataBase();
    }
    return instance;
}
