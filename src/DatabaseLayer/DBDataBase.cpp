#include"DBDatabase.h"

DBDataBase* DBDataBase::instance;

DBDataBase::DBDataBase(const char* root):
    root(root), name(NULL)
{
    if(access(root, F_OK) != 0)
    {
        DBPrint::printLine("Cannot open root directory.");
        exit(0);
    }
    pNames.clear();
    pTypes.clear();
    pNullables.clear();
    pExtras.clear();
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

void DBDataBase::addPending(std::string name, int type, bool nullable, int extra)
{
    pNames.push_back(name);
    pTypes.push_back(type);
    pNullables.push_back(nullable);
    pExtras.push_back(extra);
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
                if(opendir(szFileName) != NULL){
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
    if(name == NULL)
    {
        return;
    }
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
    if(strcmp(name_, name) == 0)
    {
        name = NULL;
    }
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

void DBDataBase::createTable(const char* name_)
{
    if(name == NULL)
    {
        DBPrint::printLine("No database being used.");
        return;
    }
    char* path = new char[256];
    strcpy(path, root);
    strcat(path, "/");
    strcat(path, name);
    strcat(path, "/");
    if(access(path, F_OK) != 0)
    {
        DBPrint::printLine("Current database is not available.");
        return;
    }
    strcat(path, name_);
    strcat(path, ".dat");
    if(access(path, F_OK) == 0)
    {
        DBPrint::printLine("Table already exists.");
        return;
    }
    DBDataFile* df = new DBDataFile(path);
    data[std::string(name_)] = df;
    df->createFile();
    df->openFile();
    df->addFields(pNames, pTypes, pNullables, pExtras);
    df->closeFile();
    DBPrint::log("Created file ").logLine(path);
}

void DBDataBase::dropTable(const char* name_)
{
    if(name == NULL)
    {
        DBPrint::printLine("No database being used.");
        return;
    }
    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, root);
    strcat(szFolderName, "/");
    strcat(szFolderName, name);
    if(name != NULL && (pDir = opendir(szFolderName)) != NULL)
    {
        strcat(szFolderName, "/");
        strcpy(szFileName, szFolderName);
        strcat(szFileName, name_);
        strcat(szFileName, ".dat");
        if(access(szFileName, F_OK) != 0)
        {
            DBPrint::printLine("Table does not exist.");
        }
        else
        {
            remove(szFileName);
            DBPrint::log("Removed file ").logLine(szFileName);
            strcpy(szFileName, szFolderName);
            strcat(szFileName, name_);
            strcat(szFileName, ".idx");
            delete_path(szFileName);
            rmdir(szFileName);
        }
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

DBDataBase* DBDataBase::getInstance()
{
    if(instance == NULL)
    {
        instance = new DBDataBase();
    }
    return instance;
}

void DBDataBase::_test()
{
    const char* sFile="test.sql";
    FILE* fp=fopen(sFile, "r");
    if(fp==NULL)
    {
        printf("cannot open %s\n", sFile);
        return;
    }
    extern FILE* yyin;
    yyin=fp;

    printf("-----begin parsing %s\n", sFile);
    yyparse();
    puts("-----end parsing");

    fclose(fp);
}

void DBDataBase::test()
{
    getInstance()->_test();
}
