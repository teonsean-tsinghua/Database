#include"DBDatabase.h"

DBDatabase* DBDatabase::instance;

DBDatabase::DBDatabase(std::string root):
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

void DBDatabase::createDatabase(std::string name_)
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

void DBDatabase::addPendingField(std::string& name, int type, bool nullable, int extra)
{
    pNames.push_back(name);
    pTypes.push_back(type);
    pNullables.push_back(nullable);
    pExtras.push_back(extra);
}

void DBDatabase::addPendingValue(Value& value)
{
    pValues.push_back(value);
}

void DBDatabase::addPendingValueList()
{
    pValueLists.push_back(pValues);
    pValues.clear();
}

void DBDatabase::clearPending()
{
    pNames.clear();
    pTypes.clear();
    pNullables.clear();
    pExtras.clear();
    pValues.clear();
    pValueLists.clear();
}

void DBDatabase::insert(std::string name_)
{
    DBDataFile* df = getDataFile(name_);
    if(df == NULL)
    {
        clearPending();
        return;
    }
    df->openFile();
    for(int i = 0; i < pValueLists.size(); i++)
    {
        std::vector<void*> data;
        data.push_back(NULL);
        for(int j = 0; j < pValueLists[i].size(); j++)
        {
            switch(pValueLists[i][j].type)
            {
            case 0:
                data.push_back(NULL);
                break;
            case 1:
                data.push_back(&(pValueLists[i][j].v_int));
                break;
            case 2:
                data.push_back(&(pValueLists[i][j].v_str));
                break;
            }
        }
        df->insert(data);
    }
    clearPending();
    df->closeFile();
    df->openFile();
    df->printAllRecords();
    df->closeFile();
}

void DBDatabase::delete_path(const char* path)
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

void DBDatabase::showTables()
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

void DBDatabase::showDatabases()
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

void DBDatabase::useDatabase(std::string name_)
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

void DBDatabase::dropDatabase(std::string name_)
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

void DBDatabase::createTable(std::string name_)
{
    if(!databaseAvailable())
    {
        return;
    }
    std::string path("");
    path += (root + "/" + name + "/" + name_ + ".dat");
    if(access(path.c_str(), F_OK) == 0)
    {
        DBPrint::printLine("File already exists.");
        return;
    }
    DBDataFile* df = new DBDataFile(path);
    data[name_] = df;
    df->createFile();
    df->openFile();
    df->addFields(pNames, pTypes, pNullables, pExtras);
    df->closeFile();
    clearPending();
    DBPrint::log("Created file ").logLine(path);
}

void DBDatabase::describeTable(std::string name_)
{
    DBDataFile* df = getDataFile(name_);
    if(df == NULL)
    {
        return;
    }
    df->openFile();
    DBPrint::printLine("TABLE " + name_ + "(");
    df->printRecordDescription();
    DBPrint::printLine(")");
    df->closeFile();
}

DBDataFile* DBDatabase::getDataFile(std::string name_)
{
    if(!databaseAvailable())
    {
        return NULL;
    }
    if(data.count(name_))
    {
        return data[name_];
    }
    std::string path("");
    path += (root + "/" + name + "/" + name_ + ".dat");
    if(access(path.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Table does not exist.");
        return NULL;
    }
    if(opendir(path.c_str()) != NULL)
    {
        DBPrint::printLine(name_ + " is not a file.");
        return NULL;
    }
    DBDataFile* df = new DBDataFile(path);
    data[name_] = df;
    return df;
}

void DBDatabase::dropTable(std::string name_)
{
    DBDataFile* df = getDataFile(name_);
    if(df == NULL)
    {
        return;
    }
    df->deleteFile();
    std::string path("");
    path += (root + "/" + name + "/" + name_ + ".idx");
    delete_path(path.c_str());
    rmdir(path.c_str());
}

bool DBDatabase::databaseAvailable()
{
    if(name == "")
    {
        DBPrint::printLine("No database being used.");
        return false;
    }
    std::string path("");
    path += (root + "/" + name + "/");
    if(access(path.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Current database is not available.");
        return false;
    }
    return true;
}

DBDatabase* DBDatabase::getInstance()
{
    if(instance == NULL)
    {
        instance = new DBDatabase();
    }
    return instance;
}

void DBDatabase::_test()
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

void DBDatabase::test()
{
    getInstance()->_test();
}
