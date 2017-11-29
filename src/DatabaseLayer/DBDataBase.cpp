#include"DBDatabase.h"

DBDataBase* DBDataBase::instance;

DBDataBase::DBDataBase(std::string root):
    root(root), name("")
{
    if(access(root.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Cannot open root directory.");
        exit(0);
    }
    pNames.clear();
    pTypes.clear();
    pNullables.clear();
    pExtras.clear();
}

void DBDataBase::createDatabase(std::string name_)
{
    std::string path("");
    path += (root + "/" + name_);
    if(access(path.c_str(), F_OK) == 0)
    {
        DBPrint::printLine("File or directory already exists.");
        return;
    }
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
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
    if(name == "")
    {
        return;
    }
    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, root.c_str());
    strcat(szFolderName, "/");
    strcat(szFolderName, name.c_str());
    if(name != "" && (pDir = opendir(szFolderName)) != NULL)
    {
        strcat(szFolderName, "/%s");
        DBPrint::printLine("====================");
        DBPrint::print("All tables of ").print(name).printLine(" are listed as below:");
        while((dmsg = readdir(pDir)) != NULL)
        {
            if(strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
            {
                sprintf(szFileName, szFolderName, dmsg->d_name);
                std::string tmp = szFileName;
                if(opendir(szFileName) == NULL && tmp.substr(tmp.size() - 4, 4).compare(".dat") == 0){
                    std::string filename = std::string(dmsg->d_name);
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
    strcpy(szFolderName, root.c_str());
    strcat(szFolderName, "/%s");
    if((pDir = opendir(root.c_str())) != NULL)
    {
        while((dmsg = readdir(pDir)) != NULL)
        {
            if(strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
            {
                sprintf(szFileName, szFolderName, dmsg->d_name);
                std::string tmp = szFileName;
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

void DBDataBase::useDatabase(std::string name_)
{
    std::string path("");
    path += (root + "/" + name_);
    if(access(path.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Database does not exist.");
        return;
    }
    if (opendir(path.c_str()) == NULL)
    {
        DBPrint::print(name_).printLine(" is not a Database.");
        return;
    }
    name = name_;
    data.clear();
    indexes.clear();
    DBPrint::log("USING DATABASE ").logLine(name);
}

void DBDataBase::dropDatabase(std::string name_)
{
    if(name_ == name)
    {
        name = "";
        data.clear();
        indexes.clear();
    }
    std::string path("");
    path += (root + "/" + name_);
    if(access(path.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Database does not exist.");
        return;
    }
    delete_path(path.c_str());
    rmdir(path.c_str());
    DBPrint::log("Removed directory ").logLine(path);
}

void DBDataBase::createTable(std::string name_)
{
    if(name == "")
    {
        DBPrint::printLine("No database being used.");
        return;
    }
    std::string path("");
    path += (root + "/" + name + "/");
    if(access(path.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Current database is not available.");
        return;
    }
    path += (name_ + ".dat");
    if(access(path.c_str(), F_OK) == 0)
    {
        DBPrint::printLine("Table already exists.");
        return;
    }
    DBDataFile* df = new DBDataFile(path);
    data[name_] = df;
    df->createFile();
    df->openFile();
    df->addFields(pNames, pTypes, pNullables, pExtras);
    df->closeFile();
    DBPrint::log("Created file ").logLine(path);
}

void DBDataBase::describeTable(std::string name_)
{
    if(name == "")
    {
        DBPrint::printLine("No database being used.");
        return;
    }
    std::string tmp(name_);
    if(data.count(tmp))
    {
        DBDataFile* df = data[tmp];
        df->openFile();
        DBPrint::printLine("TABLE " + name_ + "(");
        df->printRecordDescription();
        DBPrint::printLine(")");
        df->closeFile();
        return;
    }
    std::string path("");
    path += (root + "/" + name + "/");
    if(access(path.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Current database is not available.");
        return;
    }
    path += (name_ + ".dat");
    if(access(path.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Table does not exist.");
        return;
    }
    DBDataFile* df = new DBDataFile(path);
    data[tmp] = df;
    df->openFile();
    DBPrint::printLine("TABLE " + name_ + "(");
    df->printRecordDescription();
    DBPrint::printLine(")");
    df->closeFile();
}

void DBDataBase::dropTable(std::string name_)
{
    if(name == "")
    {
        DBPrint::printLine("No database being used.");
        return;
    }
    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, root.c_str());
    strcat(szFolderName, "/");
    strcat(szFolderName, name.c_str());
    if(name != "" && (pDir = opendir(szFolderName)) != NULL)
    {
        strcat(szFolderName, "/");
        strcpy(szFileName, szFolderName);
        strcat(szFileName, name_.c_str());
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
            strcat(szFileName, name_.c_str());
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
    yyparse();
    fclose(fp);
}

void DBDataBase::test()
{
    getInstance()->_test();
}
