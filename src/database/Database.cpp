#include"Database.h"

Database* Database::instance;
const std::string Database::TAG = "Database";

Database::Database(std::string root):
    fm(FileIOModel::getInstance()), curDb("")
{

}

Database* Database::getInstance()
{
    if(instance == NULL)
    {
        instance = new Database();
    }
    return instance;
}

void Database::init()
{
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
	pSets.clear();
	pForeigns.clear();
}

void Database::createDatabase(std::string dbname)
{
    fm->createDb(dbname);
    std::cout << "CREATED DATABASE " << dbname << std::endl;
}

void Database::addPendingField(std::string& name_, int type, bool nullable, int extra)
{
    pNames.push_back(name_);
    pTypes.push_back(type);
    pNullables.push_back(nullable);
    pExtras.push_back(extra);
}

void Database::addPendingTable(std::string& name_)
{
    pTables.push_back(name_);
}

void Database::addPendingColumn(std::string& name_)
{
    pColumns.push_back(name_);
}

void Database::addPendingCol(Col& col)
{
    pCols.push_back(col);
}

void Database::addPendingValue(Value& value)
{
    pValues.push_back(value);
}

void Database::addPendingWhere(Where& where)
{
    pWheres.push_back(where);
}

void Database::addPendingForeign(Foreign& foreign)
{
	pForeigns.push_back(foreign);
}

void Database::addPendingSet(Set& set_)
{
    pSets.push_back(set_);
}

void Database::addPendingValueList()
{
    pValueLists.push_back(pValues);
    pValues.clear();
}
//
//void Database::processSets(UpdateInfo& ui, RecordInfo* ri, std::string tbname)
//{
//    bool flag = true;
//    for(int i = 0; i < pSets.size(); i++)
//    {
//        Set& s = pSets[i];
//        if(ri->index(s.field) < 0)
//        {
//            Print::printLine("Table " + tbname + " does not have field" + s.field + ".");
//            flag = false;
//        }
//    }
//    if(!flag)
//    {
//        pSets.clear();
//        throw Exception("Error in set clause.");
//    }
//    for(int i = 0; i < pSets.size(); i++)
//    {
//        Set& s = pSets[i];
//        if(s.value.type == 0)
//        {
//            ui.data[ri->index(s.field)] = NULL;
//        }
//        else if(s.value.type == 1)
//        {
//            ui.data[ri->index(s.field)] = &s.value.v_int;
//        }
//        else if(s.value.type == 2)
//        {
//            ui.data[ri->index(s.field)] = &s.value.v_str;
//        }
//    }
//}
//
void Database::update(std::string name)
{
//    DataFile* df = getDataFile(name);
//    if(df == NULL)
//    {
//        pWheres.clear();
//        pSets.clear();
//        return;
//    }
//    df->openFile();
//    RecordInfo* ri = df->getRecordInfo();
//    SearchInfo si;
//    UpdateInfo ui;
//    if(!si.processWheresWithOneTable(pWheres, ri, name))
//    {
//        pWheres.clear();
//        throw Exception("Conflict in where clauses.");
//    }
//    int cnt = df->update(si, ui);
//    Print::print("Updated ").print(cnt).printLine(" records.");
//    pWheres.clear();
//    pSets.clear();
//    df->closeFile();
}

void Database::remove(std::string name)
{
//    DataFile* df = getDataFile(name);
//    if(df == NULL)
//    {
//        pWheres.clear();
//        return;
//    }
//    df->openFile();
//    RecordInfo* ri = df->getRecordInfo();
//    SearchInfo si;
//    if(!si.processWheresWithOneTable(pWheres, ri, name))
//    {
//        throw Exception("Conflict in where clauses.");
//    }
//    int cnt = df->remove(si);
//    Print::print("Deleted ").print(cnt).printLine(" records.");
//    pWheres.clear();
//    df->closeFile();
}

void Database::selectOneTable(bool all)
{
//    DataFile* df = getDataFile(pTables[0]);
//    if(df == NULL)
//    {
//        pWheres.clear();
//        pTables.clear();
//        pCols.clear();
//        return;
//    }
//    std::vector<bool> selected;
//    df->openFile();
//    RecordInfo* ri = df->getRecordInfo();
//    if(all)
//    {
//        selected.assign(ri->getFieldCount(), true);
//    }
//    else
//    {
//        bool flag = true;
//        for(int i = 0; i < pCols.size(); i++)
//        {
//            if(pCols[i].table != "" &&
//               pCols[i].table != pTables[0])
//            {
//                Print::printLine("Table " + pCols[i].table + " is not selected.");
//                flag = false;
//            }
//            else if(ri->index(pCols[i].field) < 0)
//            {
//                Print::printLine("Table " + pTables[0] + " does not have field" + pCols[i].field + ".");
//                flag = false;
//            }
//        }
//        if(!flag)
//        {
//            pWheres.clear();
//            pTables.clear();
//            pCols.clear();
//            df->closeFile();
//            return;
//        }
//        selected.assign(ri->getFieldCount(), false);
//        for(int i = 0; i < pCols.size(); i++)
//        {
//            selected[ri->index(pCols[i].field)] = true;
//        }
//    }
//    SearchInfo si;
//    if(!si.processWheresWithOneTable(pWheres, ri, pTables[0]))
//    {
//        pWheres.clear();
//        throw Exception("Conflict in where clauses.");
//    }
//    SelectResult sr;
//    df->select(si, sr);
//    Table::print(selected, ri, sr, df);
//    pWheres.clear();
//    pTables.clear();
//    pCols.clear();
//    df->closeFile();
}

void Database::selectMultiTable(bool all)
{

}

void Database::select(bool all)
{
//    if(pTables.size() == 1)
//    {
//        selectOneTable(all);
//    }
//    else if(pTables.size() > 1)
//    {
//        selectMultiTable(all);
//    }
//    else
//    {
//        throw Exception("At least one table should be provided.");
//    }
}

void Database::insert(std::string name_)
{
//    DataFile* df = getDataFile(name_);
//    if(df == NULL)
//    {
//        pValues.clear();
//        pValueLists.clear();
//        throw Exception("Cannot read table " + name_);
//    }
//    df->openFile();
//    for(int i = 0; i < pValueLists.size(); i++)
//    {
//        std::vector<void*> data;
//        data.push_back(NULL);
//        for(int j = 0; j < pValueLists[i].size(); j++)
//        {
//            switch(pValueLists[i][j].type)
//            {
//            case 0:
//                data.push_back(NULL);
//                break;
//            case 1:
//                data.push_back(&(pValueLists[i][j].v_int));
//                break;
//            case 2:
//                data.push_back(&(pValueLists[i][j].v_str));
//                break;
//            }
//        }
//        df->insert(data);
//    }
//    pValues.clear();
//    pValueLists.clear();
//    df->closeFile();
}

void Database::showTables()
{
	fm->showTables(curDb);
}

void Database::showDatabases()
{
	fm->showDbs();
}

void Database::useDatabase(std::string dbname)
{
    if(curDb != dbname)
    {
        fm->checkDb(dbname, true);
        curDb = dbname;
    }
    std::cout << "USING DATABASE " << dbname << std::endl;
}

void Database::dropDatabase(std::string dbname)
{
    if(curDb == dbname)
    {
        curDb = "";
    }
    fm->dropDb(dbname);
    std::cout << "DROPPED DATABASE " << dbname << std::endl;
}

void Database::createTable(std::string tbname)
{
	std::vector<std::string> tmp;
	tmp = std::vector<std::string>(pNames.begin(), pNames.end());
	tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
	if(tmp.size() != pNames.size())
	{
		throw Exception(TAG, "Cannot define fields with identical names.");
	}
	tmp = std::vector<std::string>(pColumns.begin(), pColumns.end());
	tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
	if(tmp.size() != pColumns.size())
	{
		throw Exception(TAG, "Cannot define primary keys with duplicate fields.");
	}
	pNames.insert(pNames.begin(), "_id");
	pTypes.insert(pTypes.begin(), 0);
	pNullables.insert(pNullables.begin(), false);
	pExtras.insert(pExtras.begin(), 0);
    tmp.assign(pNames.size(), "");
    for(int i = 0; i < pForeigns.size(); i++)
    {
    	bool found = false;
    	for(int j = 0; j < pNames.size(); j++)
    	{
    		if(pNames[j] == pForeigns[i].field)
    		{
    			if(tmp[j].size() > 0)
    			{
    				throw Exception(TAG, "Cannot reference multiple columns with one field.");
    			}
    			tmp[j] = pForeigns[i].table + "." + pForeigns[i].column;
    			found = true;
    			break;
    		}
    	}
    	if(!found)
    	{
    		throw Exception(TAG, "Field " + pForeigns[i].field + " is never defined.");
    	}
    }
    for(int i = 0; i < pColumns.size(); i++)
    {
    	int j = -1;
    	for(int k = 0; k < pNames.size(); k++)
    	{
    		if(pNames[k] == pColumns[i])
    		{
    			j = k;
    		}
    	}
    	if(j < 0)
    	{
    		throw Exception(TAG, "Field " + pColumns[i] + " is never defined.");
    	}
    	std::swap(pNames[i], pNames[j]);
    	std::swap(pTypes[i], pTypes[j]);
    	std::swap(pNullables[i], pNullables[j]);
    	std::swap(pExtras[i], pExtras[j]);
    	std::swap(tmp[i], tmp[j]);
    }
    DataFile* df = new DataFile();
    df->createFile(curDb, tbname);
    df->openFile(curDb, tbname);
    df->addFields(pNames, pTypes, pNullables, pExtras, tmp, std::max((int)pColumns.size(), 1));
    df->closeFile();
    delete df;
    init();
    std::cout << "CREATED TABLE " << tbname << std::endl;
}

void Database::createIndex(std::string tbname, std::string colname)
{
	fm->createIndexFile(curDb, tbname, colname);
	std::cout << "CREATED INDEX " << colname << " OF TABLE " << tbname << std::endl;
}

void Database::dropIndex(std::string tbname, std::string colname)
{
	fm->dropIdx(curDb, tbname, colname);
	std::cout << "DROPPED INDEX " << colname << " OF TABLE " << tbname << std::endl;
}

void Database::describeTable(std::string tbname)
{
	DataFile* df = new DataFile();
	df->openFile(curDb, tbname);
	df->getRecordInfo()->printRecordDesc(tbname);
	df->closeFile();
}

void Database::dropTable(std::string tbname)
{
	fm->dropTable(curDb, tbname);
	std::cout << "DROPPED TABLE " << tbname << std::endl;
}

void Database::_test()
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

void Database::test()
{
    getInstance()->_test();
}
