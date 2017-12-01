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
    pValues.clear();
    pValueLists.clear();
    pTables.clear();
    pColumns.clear();
    pCols.clear();
    pWheres.clear();
}

void DBDatabase::createDatabase(std::string name_)
{
    std::string path("");
    path += (root + "/" + name_);
    if(access(path.c_str(), F_OK) == 0)
    {
        throw Exception("File or directory already exists.");
    }
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    DBPrint::log("Created directory ").logLine(path);
}

void DBDatabase::addPendingField(std::string& name_, int type, bool nullable, int extra)
{
    pNames.push_back(name_);
    pTypes.push_back(type);
    pNullables.push_back(nullable);
    pExtras.push_back(extra);
}

void DBDatabase::addPendingTable(std::string& name_)
{
    pTables.push_back(name_);
}

void DBDatabase::addPendingColumn(std::string& name_)
{
    pColumns.push_back(name_);
}

void DBDatabase::addPendingCol(Col& col)
{
    pCols.push_back(col);
}

void DBDatabase::addPendingValue(Value& value)
{
    pValues.push_back(value);
}

void DBDatabase::addPendingWhere(Where& where)
{
    pWheres.push_back(where);
}

void DBDatabase::addPendingValueList()
{
    pValueLists.push_back(pValues);
    pValues.clear();
}

void DBDatabase::processWheresWithOneTable(SearchInfo& si, DBRecordInfo* ri)
{
    bool flag = true;
    for(int i = 0; i < pWheres.size(); i++)
    {
        Where& w = pWheres[i];
        if(w.left.table != "" &&
           w.left.table != pTables[0])
        {
            DBPrint::printLine("Table " + w.left.table + " is not selected.");
            flag = false;
        }
        else if(ri->index(w.left.field) < 0)
        {
            DBPrint::printLine("Table " + pTables[0] + " does not have field" + w.left.field + ".");
            flag = false;
        }
        if(w.type == 2 && w.opCol)
        {
            if(w.col_r.table != "" && w.col_r.table != pTables[0])
            {
                throw Exception("Table " + w.col_r.table + " is not selected.");
                flag = false;
            }
            else if(ri->index(w.col_r.field) < 0)
            {
                DBPrint::printLine("Table " + pTables[0] + " does not have field" + w.col_r.field + ".");
                flag = false;
            }
        }
    }
    if(!flag)
    {
        pWheres.clear();
        throw Exception("Error in where clause.");
    }
    si.init();
    for(int i = 0; i < pWheres.size(); i++)
    {
        Where& w = pWheres[i];
        switch(w.type)
        {
        case 0:
            si.nulls[ri->index(w.left.field)] = true;
            break;
        case 1:
            si.nulls[ri->index(w.left.field)] = false;
            break;
        case 2:
            switch(w.op)
            {
            case 0:
                if(w.opCol)
                {
                    si.fequals[ri->index(w.left.field)] = ri->index(w.col_r.field);
                }
                else
                {
                    si.equals[ri->index(w.left.field)] = (w.val_r.type == 1 ? (void*)&w.val_r.v_int : (void*)&w.val_r.v_str);
                }
                break;
            case 1:
                if(w.opCol)
                {
                    si.fnotEquals[ri->index(w.left.field)] = ri->index(w.col_r.field);
                }
                else
                {
                    si.notEquals[ri->index(w.left.field)] = (w.val_r.type == 1 ? (void*)&w.val_r.v_int : (void*)&w.val_r.v_str);
                }
                break;
            case 2:
                if(w.opCol)
                {
                    si.fsmallerEquals[ri->index(w.left.field)] = ri->index(w.col_r.field);
                }
                else
                {
                    si.smallerEquals[ri->index(w.left.field)] = (w.val_r.type == 1 ? (void*)&w.val_r.v_int : (void*)&w.val_r.v_str);
                }
                break;
            case 3:
                if(w.opCol)
                {
                    si.flargerEquals[ri->index(w.left.field)] = ri->index(w.col_r.field);
                }
                else
                {
                    si.largerEquals[ri->index(w.left.field)] = (w.val_r.type == 1 ? (void*)&w.val_r.v_int : (void*)&w.val_r.v_str);
                }
                break;
            case 4:
                if(w.opCol)
                {
                    si.fsmallers[ri->index(w.left.field)] = ri->index(w.col_r.field);
                }
                else
                {
                    si.smallers[ri->index(w.left.field)] = (w.val_r.type == 1 ? (void*)&w.val_r.v_int : (void*)&w.val_r.v_str);
                }
                break;
            case 5:
                if(w.opCol)
                {
                    si.flargers[ri->index(w.left.field)] = ri->index(w.col_r.field);
                }
                else
                {
                    si.largers[ri->index(w.left.field)] = (w.val_r.type == 1 ? (void*)&w.val_r.v_int : (void*)&w.val_r.v_str);
                }
                break;
            }
        }
    }
    pWheres.clear();
}

void DBDatabase::selectOneTable(bool all)
{
    DBDataFile* df = getDataFile(pTables[0]);
    if(df == NULL)
    {
        pWheres.clear();
        pTables.clear();
        pCols.clear();
        return;
    }
    std::vector<bool> selected;
    df->openFile();
    DBRecordInfo* ri = df->getRecordInfo();
    if(all)
    {
        selected.assign(ri->getFieldCount(), true);
    }
    else
    {
        bool flag = true;
        for(int i = 0; i < pCols.size(); i++)
        {
            if(pCols[i].table != "" &&
               pCols[i].table != pTables[0])
            {
                DBPrint::printLine("Table " + pCols[i].table + " is not selected.");
                flag = false;
            }
            else if(ri->index(pCols[i].field) < 0)
            {
                DBPrint::printLine("Table " + pTables[0] + " does not have field" + pCols[i].field + ".");
                flag = false;
            }
        }
        if(!flag)
        {
            pWheres.clear();
            pTables.clear();
            pCols.clear();
            df->closeFile();
            return;
        }
        selected.assign(ri->getFieldCount(), false);
        for(int i = 0; i < pCols.size(); i++)
        {
            selected[ri->index(pCols[i].field)] = true;
        }
    }
    SearchInfo si;
    processWheresWithOneTable(si, ri);
    SelectResult sr;
    df->select(si, sr);

    pTables.clear();
    pCols.clear();
    df->closeFile();
}

void DBDatabase::selectMultiTable(bool all)
{

}

void DBDatabase::select(bool all)
{
    if(pTables.size() == 1)
    {
        selectOneTable(all);
    }
    else if(pTables.size() > 1)
    {
        selectMultiTable(all);
    }
    else
    {
        throw Exception("At least one table should be provided.");
    }
}

void DBDatabase::insert(std::string name_)
{
    DBDataFile* df = getDataFile(name_);
    if(df == NULL)
    {
        pValues.clear();
        pValueLists.clear();
        throw Exception("Cannot read table " + name_);
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
    pValues.clear();
    pValueLists.clear();
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
        closedir(pDir);
    }
}

void DBDatabase::showTables()
{
    assert(databaseAvailable());
    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, root.c_str());
    strcat(szFolderName, "/");
    strcat(szFolderName, name.c_str());
    if((pDir = opendir(szFolderName)) != NULL)
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
        closedir(pDir);
    }
    else
    {
        DBPrint::printLine("Current database is not available.");
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
        closedir(pDir);
    }
    DBPrint::printLine("====================");
}

void DBDatabase::useDatabase(std::string name_)
{
    std::string path("");
    path += (root + "/" + name_);
    if(access(path.c_str(), F_OK) != 0)
    {
        throw Exception("Database does not exist.");
    }
    if (opendir(path.c_str()) == NULL)
    {
        throw Exception(" is not a Database.");
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
    }
    std::string path("");
    path += (root + "/" + name_);
    if(access(path.c_str(), F_OK) != 0)
    {
        throw Exception("Database does not exist.");
    }
    delete_path(path.c_str());
    rmdir(path.c_str());
    DBPrint::log("Removed directory ").logLine(path);
}

void DBDatabase::createTable(std::string name_)
{
    assert(databaseAvailable());
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
    pNames.clear();
    pTypes.clear();
    pNullables.clear();
    pExtras.clear();
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
    assert(databaseAvailable());
    if(data.count(name_))
    {
        return data[name_];
    }
    std::string path("");
    path += (root + "/" + name + "/" + name_ + ".dat");
    if(access(path.c_str(), F_OK) != 0)
    {
        DBPrint::printLine("Table " + name_ + " does not exist.");
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
