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
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    DBLog("Created directory ");
    DBLogLine(path);
}

void DBDataBase::delete_path(const char* path){


    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, path);
    strcat(szFolderName, "/%s");
    if ((pDir = opendir(path)) != NULL)
    {
        while ((dmsg = readdir(pDir)) != NULL)
        {
            if (strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
            {
                sprintf(szFileName, szFolderName, dmsg->d_name);
                string tmp = szFileName;
                if (tmp.find(".") == -1){
                    delete_path(szFileName);
                }
                remove(szFileName);
            }
        }
    }
    if (pDir != NULL)
    {
        closedir(pDir);
    }
}

void DBDataBase::dropDatabase(const char* name_)
{
    char* path = new char[256];
    strcpy(path, root);
    strcat(path, "/");
    strcat(path, name_);
    if(access(path, F_OK) != 0)
    {
        DBPrintLine("Database does not exist.");
        return;
    }
    delete_path(path);
    rmdir(path);
    DBLog("Removed directory ");
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
