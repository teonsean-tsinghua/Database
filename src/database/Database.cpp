#include"Database.h"

Database* Database::instance;
const std::string Database::TAG = "Database";

Database::Database(std::string root):
    fm(FileIOModel::getInstance()), curDb("")
{

}

bool IsLeapYear(int Year)
{
    if((Year % 4 == 0 && Year % 100 != 0 ) || Year % 400 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool IsValidDate(int Year,int Month,int Day)
{
    int nDay;

    if(Year < 1 || Month >12 || Month < 1 || Day < 1)
    {
        return false;
    }

    switch(Month)
    {
    case 4:
    case 6:
    case 9:
    case 11:
        nDay = 30;
        break;
    case 2:
        if(IsLeapYear(Year))
        {
            nDay = 29;
        }
        else
        {
            nDay = 28;
        }
        break;
    default:
        nDay = 31;
    }

    if( Day > nDay )
    {
        return false;
    }

    return true;
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

void Database::processSets(UpdateInfo& ui, RecordInfo* ri, std::string tbname)
{
    bool flag = true;
    for(int i = 0; i < pSets.size(); i++)
    {
        Set& s = pSets[i];
        if(ri->index(s.field) < 0)
        {
            std::cout << "Table " << tbname << " does not have field" << s.field << ".\n";
            flag = false;
        }
    }
    if(!flag)
    {
        pSets.clear();
        throw Exception(TAG, "Error in set clause.");
    }
    for(int i = 0; i < pSets.size(); i++)
    {
        int idx = ri->index(pSets[i].field);
        if(ui.action.count(idx))
        {
            throw Exception(TAG, "Cannot perform multiple actions on one field.");
        }
        if(!ri->nullable(idx))
        {
            if(pSets[i].rvalue.type == 1 && pSets[i].rvalue.value.type == 0)
            {
                throw Exception(TAG, "Cannot set " + pSets[i].field + " to NULL.");
            }
            if(pSets[i].rvalue.type == 2)
            {
                if(ri->type(pSets[i].field) != ri->type(pSets[i].rvalue.col))
                {
                    throw Exception(TAG, "Cannot perform assignment between fields of different type.");
                }
                if(ri->nullable(pSets[i].rvalue.col))
                {
                    throw Exception(TAG, "Cannot assign nullable field to non-nullable field.");
                }
            }
        }
        if(pSets[i].rvalue.type > 2)
        {
            if(ri->type(pSets[i].rvalue.col) != Type::INT && ri->type(pSets[i].rvalue.col) != Type::FLOAT)
            {
                throw Exception(TAG, "Cannot perform arithmetic actions on non-numerical type.");
            }
            if(ri->nullable(pSets[i].rvalue.col))
            {
                throw Exception(TAG, "Cannot perform arithmetic actions on nullable field.");
            }
            if(pSets[i].rvalue.type > 6)
            {
                if(pSets[i].rvalue.value.type != 1 && pSets[i].rvalue.value.type != 3)
                {
                    throw Exception(TAG, "Cannot perform arithmetic actions on non-numerical type.");
                }
            }
            else
            {
                if(ri->nullable(pSets[i].rvalue.col2))
                {
                    throw Exception(TAG, "Cannot perform arithmetic actions on nullable field.");
                }
                if(ri->type(pSets[i].rvalue.col2) != Type::INT && ri->type(pSets[i].rvalue.col2) != Type::FLOAT)
                {
                    throw Exception(TAG, "Cannot perform arithmetic actions on non-numerical type.");
                }
            }
        }
        ui.action[idx] = pSets[i].rvalue;
    }
}

void Database::update(std::string tbname)
{
    DataFile df;
    df.openFile(curDb, tbname);
    RecordInfo* ri = df.getRecordInfo();
    SearchInfo si;
    UpdateInfo ui;
    if(!si.processWheresWithOneTable(pWheres, ri, tbname))
    {
        throw Exception(TAG, "Conflict in where clauses.");
    }
    processSets(ui, ri, tbname);
    df.update(si, ui);
    init();
    df.closeFile();
}

void Database::remove(std::string tbname)
{
    DataFile df;
    df.openFile(curDb, tbname);
    RecordInfo* ri = df.getRecordInfo();
    SearchInfo si;
    if(!si.processWheresWithOneTable(pWheres, ri, tbname))
    {
        throw Exception(TAG, "Conflict in where clauses.");
    }
    df.remove(si);
    init();
    df.closeFile();
}

void Database::selectOneTable(bool all)
{
    DataFile df;
    df.openFile(curDb, pTables[0]);
    std::vector<bool> selected;
    RecordInfo* ri = df.getRecordInfo();
    if(all)
    {
        selected.assign(ri->getFieldCount(), true);
        selected[0] = false;
    }
    else
    {
        bool flag = true;
        for(int i = 0; i < pCols.size(); i++)
        {
            if(pCols[i].table != "" &&
               pCols[i].table != pTables[0])
            {
                std::cout << "Table " << pCols[i].table << " is not selected.\n";
                flag = false;
            }
            else if(ri->index(pCols[i].field) < 0)
            {
                std::cout << "Table " << pTables[0] << " does not have field" << pCols[i].field + ".\n";
                flag = false;
            }
        }
        if(!flag)
        {
            init();
            df.closeFile();
            return;
        }
        selected.assign(ri->getFieldCount(), false);
        for(int i = 0; i < pCols.size(); i++)
        {
            selected[ri->index(pCols[i].field)] = true;
        }
    }
    SearchInfo si;
    if(!si.processWheresWithOneTable(pWheres, ri, pTables[0]))
    {
        throw Exception(TAG, "Conflict in where clauses.");
    }
//    SelectResult sr;
//    df->select(si, sr);
//    Table::print(selected, ri, sr, df);
    df.select(si, selected);
    init();
    df.closeFile();
}

void Database::selectMultiTable(bool all)
{
    assert(pTables.size() == 2);
    DataFile df0, df1;
    df0.openFile(curDb, pTables[0]);
    df1.openFile(curDb, pTables[1]);
    RecordInfo* ri0 = df0.getRecordInfo();
    RecordInfo* ri1= df1.getRecordInfo();
    std::vector<std::vector<bool> > selected;
    if(all)
    {
        selected.push_back(std::vector<bool>());
        selected[0].assign(ri0->getFieldCount(), true);
        selected.push_back(std::vector<bool>());
        selected[1].assign(ri1->getFieldCount(), true);
        selected[0][0] = selected[1][0] = false;
    }
    else
    {
        selected.push_back(std::vector<bool>());
        selected[0].assign(ri0->getFieldCount(), false);
        selected.push_back(std::vector<bool>());
        selected[1].assign(ri1->getFieldCount(), false);
        bool flag = true;
        for(int i = 0; i < pCols.size(); i++)
        {
            if(pCols[i].table == "")
            {
                std::cout << "Table of field " << pCols[i].field << " is not specified.\n";
                flag = false;
            }
            else if(pCols[i].table == pTables[0])
            {
                int idx = ri0->index(pCols[i].field);
                if(idx < 0)
                {
                    std::cout << "Table " << pTables[0] << " does not have field" << pCols[i].field + ".\n";
                    flag = false;
                }
                else
                {
                    selected[0][idx] = true;
                }
            }
            else if(pCols[i].table == pTables[1])
            {
                int idx = ri1->index(pCols[i].field);
                if(idx < 0)
                {
                    std::cout << "Table " << pTables[1] << " does not have field" << pCols[i].field + ".\n";
                    flag = false;
                }
                else
                {
                    selected[1][idx] = true;
                }
            }
            else
            {
                std::cout << "Table " << pCols[i].table << " is not selected.\n";
                flag = false;
            }
        }
        if(!flag)
        {
            init();
            df1.closeFile();
            df0.closeFile();
            return;
        }
    }
    std::vector<Where> onetbwhere0, onetbwhere1, inter;
    for(int i = 0; i < pWheres.size(); i++)
    {
        Where& w = pWheres[i];
        if(w.left.table == "")
        {
            throw Exception(TAG, "Table of field " + w.left.field + " is not specified.\n");
        }
        else if(w.left.table == pTables[0])
        {
            if(w.type != 2 || !w.opCol)
            {
                onetbwhere0.push_back(w);
            }
            else if(w.col_r.table == pTables[0])
            {
                onetbwhere0.push_back(w);
            }
            else if(w.col_r.table == pTables[1])
            {
                inter.push_back(w);
            }
            else if(w.col_r.table == "")
            {
                throw Exception(TAG, "Table of field " + w.col_r.field + " is not specified.\n");
            }
            else
            {
                throw Exception(TAG, "Table " + w.col_r.table + " is not selected.\n");
            }
        }
        else if(w.left.table == pTables[1])
        {
            if(w.type != 2 || !w.opCol)
            {
                onetbwhere1.push_back(w);
            }
            else if(w.col_r.table == pTables[1])
            {
                onetbwhere1.push_back(w);
            }
            else if(w.col_r.table == pTables[0])
            {
                inter.push_back(w);
            }
            else if(w.col_r.table == "")
            {
                throw Exception(TAG, "Table of field " + w.col_r.field + " is not specified.\n");
            }
            else
            {
                throw Exception(TAG, "Table " + w.col_r.table + " is not selected.\n");
            }
        }
        else
        {
            throw Exception(TAG, "Table " + w.left.table + " is not selected.\n");
        }
    }
    SearchInfo si0, si1;
    if(!si0.processWheresWithOneTable(onetbwhere0, ri0, pTables[0]) || !si1.processWheresWithOneTable(onetbwhere1, ri1, pTables[1]))
    {
        throw Exception(TAG, "Conflict in where clauses.");
    }
    std::map<int, int> inters[6];
    for(int i = 0; i < inter.size(); i++)
    {
        Where& w = inter[i];
        int lidx = ri0->index(w.left.field);
        int ridx = ri1->index(w.col_r.field);
        inters[w.op][lidx] = ridx;
    }
    std::queue<int>& q0 = df0.search(si0);
    std::queue<int>& q1 = df1.search(si1);
    std::vector<RecordInfo*> ri;
    ri.push_back(ri0);
    ri.push_back(ri1);
    Table::printHeader(selected, ri, pTables);
    int idx1, idx0;
    std::vector<std::vector<void*> > data;
    data.push_back(std::vector<void*>());
    data.push_back(std::vector<void*>());
    std::cout << q0.size() << " " << q1.size() << "\n";
    while(true)
    {
        idx0 = q0.front();
        q0.pop();
        if(idx0 <= 0)
        {
            break;
        }
        data[0].clear();
        df0.getData(idx0, data[0]);
        while(true)
        {
            idx1 = q1.front();
            q1.pop();
            if(idx1 <= 0)
            {
                q1.push(0);
                break;
            }
            q1.push(idx1);
            data[1].clear();
            df1.getData(idx1, data[1]);
            std::map<int, int>::iterator iter;
            bool valid = true;
            // =
            for(iter = inters[0].begin(); iter != inters[0].end(); iter++)
            {
                int lidx = iter->first, ridx = iter->second;
                bool re = true;
                if(data[0][lidx] == NULL && data[1][ridx] == NULL)
                {
                    continue;
                }
                if(data[0][lidx] == NULL && data[1][ridx] != NULL)
                {
                    valid = false;
                    break;
                }
                if(data[0][lidx] != NULL && data[1][ridx] == NULL)
                {
                    valid = false;
                    break;
                }
                switch(ri0->type(lidx))
                {
                case Type::INT:
                    re = (*(IntType*)data[0][lidx] == *(IntType*)data[1][ridx]);
                    break;
                case Type::FLOAT:
                    re = (*(FloatType*)data[0][lidx] == *(FloatType*)data[1][ridx]);
                    break;
                case Type::DATE:
                    re = (*(DateType*)data[0][lidx] == *(DateType*)data[1][ridx]);
                    break;
                case Type::VARCHAR:
                    re = (*(VarcharType*)data[0][lidx] == *(VarcharType*)data[1][ridx]);
                    break;
                }
                if(!re)
                {
                    valid = false;
                    break;
                }
            }
            if(!valid)
            {
                continue;
            }
            // <=
            for(iter = inters[1].begin(); iter != inters[1].end(); iter++)
            {
                int lidx = iter->first, ridx = iter->second;
                bool re = true;
                if(data[0][lidx] == NULL || data[1][ridx] == NULL)
                {
                    valid = false;
                    break;
                }
                switch(ri0->type(lidx))
                {
                case Type::INT:
                    re = (*(IntType*)data[0][lidx] <= *(IntType*)data[1][ridx]);
                    break;
                case Type::FLOAT:
                    re = (*(FloatType*)data[0][lidx] <= *(FloatType*)data[1][ridx]);
                    break;
                case Type::DATE:
                    re = (*(DateType*)data[0][lidx] <= *(DateType*)data[1][ridx]);
                    break;
                case Type::VARCHAR:
                    re = (*(VarcharType*)data[0][lidx] <= *(VarcharType*)data[1][ridx]);
                    break;
                }
                if(!re)
                {
                    valid = false;
                    break;
                }
            }
            if(!valid)
            {
                continue;
            }
            // >=
            for(iter = inters[2].begin(); iter != inters[2].end(); iter++)
            {
                int lidx = iter->first, ridx = iter->second;
                bool re = true;
                if(data[0][lidx] == NULL || data[1][ridx] == NULL)
                {
                    valid = false;
                    break;
                }
                switch(ri0->type(lidx))
                {
                case Type::INT:
                    re = (*(IntType*)data[0][lidx] >= *(IntType*)data[1][ridx]);
                    break;
                case Type::FLOAT:
                    re = (*(FloatType*)data[0][lidx] >= *(FloatType*)data[1][ridx]);
                    break;
                case Type::DATE:
                    re = (*(DateType*)data[0][lidx] >= *(DateType*)data[1][ridx]);
                    break;
                case Type::VARCHAR:
                    re = (*(VarcharType*)data[0][lidx] >= *(VarcharType*)data[1][ridx]);
                    break;
                }
                if(!re)
                {
                    valid = false;
                    break;
                }
            }
            if(!valid)
            {
                continue;
            }
            // <
            for(iter = inters[3].begin(); iter != inters[3].end(); iter++)
            {
                int lidx = iter->first, ridx = iter->second;
                bool re = true;
                if(data[0][lidx] == NULL || data[1][ridx] == NULL)
                {
                    valid = false;
                    break;
                }
                switch(ri0->type(lidx))
                {
                case Type::INT:
                    re = (*(IntType*)data[0][lidx] < *(IntType*)data[1][ridx]);
                    break;
                case Type::FLOAT:
                    re = (*(FloatType*)data[0][lidx] < *(FloatType*)data[1][ridx]);
                    break;
                case Type::DATE:
                    re = (*(DateType*)data[0][lidx] < *(DateType*)data[1][ridx]);
                    break;
                case Type::VARCHAR:
                    re = (*(VarcharType*)data[0][lidx] < *(VarcharType*)data[1][ridx]);
                    break;
                }
                if(!re)
                {
                    valid = false;
                    break;
                }
            }
            if(!valid)
            {
                continue;
            }
            // >
            for(iter = inters[4].begin(); iter != inters[4].end(); iter++)
            {
                int lidx = iter->first, ridx = iter->second;
                bool re = true;
                if(data[0][lidx] == NULL || data[1][ridx] == NULL)
                {
                    valid = false;
                    break;
                }
                switch(ri0->type(lidx))
                {
                case Type::INT:
                    re = (*(IntType*)data[0][lidx] > *(IntType*)data[1][ridx]);
                    break;
                case Type::FLOAT:
                    re = (*(FloatType*)data[0][lidx] > *(FloatType*)data[1][ridx]);
                    break;
                case Type::DATE:
                    re = (*(DateType*)data[0][lidx] > *(DateType*)data[1][ridx]);
                    break;
                case Type::VARCHAR:
                    re = (*(VarcharType*)data[0][lidx] > *(VarcharType*)data[1][ridx]);
                    break;
                }
                if(!re)
                {
                    valid = false;
                    break;
                }
            }
            if(!valid)
            {
                continue;
            }
            // <>
            for(iter = inters[5].begin(); iter != inters[5].end(); iter++)
            {
                int lidx = iter->first, ridx = iter->second;
                bool re = true;
                if(data[0][lidx] == NULL && data[1][ridx] == NULL)
                {
                    valid = false;
                    break;
                }
                if(data[0][lidx] == NULL && data[1][ridx] != NULL)
                {
                    continue;
                }
                if(data[1][lidx] == NULL && data[0][ridx] != NULL)
                {
                    continue;
                }
                switch(ri0->type(lidx))
                {
                case Type::INT:
                    re = (*(IntType*)data[0][lidx] != *(IntType*)data[1][ridx]);
                    break;
                case Type::FLOAT:
                    re = (*(FloatType*)data[0][lidx] != *(FloatType*)data[1][ridx]);
                    break;
                case Type::DATE:
                    re = (*(DateType*)data[0][lidx] != *(DateType*)data[1][ridx]);
                    break;
                case Type::VARCHAR:
                    re = (*(VarcharType*)data[0][lidx] != *(VarcharType*)data[1][ridx]);
                    break;
                }
                if(!re)
                {
                    valid = false;
                    break;
                }
            }
            if(!valid)
            {
                continue;
            }
            Table::printRow(data, ri);
        }
    }
    df0.closeFile();
    df1.closeFile();
}

void Database::select(bool all)
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
        throw Exception(TAG, "At least one table should be provided.");
    }
}

void Database::insert(std::string tbname)
{
    DataFile df;
    df.openFile(curDb, tbname);
    RecordInfo* ri = df.getRecordInfo();
    char* tmp = new char[8192];
    for(int i = 0; i < pValueLists.size(); i++)
    {
        std::vector<void*> data;
        data.push_back(NULL);
        bool valid = true;
        std::vector<int> tmpInt;
        std::vector<float> tmpFloat;
        for(int j = 0; j < pValueLists[i].size(); j++)
        {
            switch(pValueLists[i][j].type)
            {
            case 0:
                if(!ri->nullable(j + 1))
                {
                    std::cout << "Error: " << ri->name(j + 1) << " cannot be null.\n";
                    valid = false;
                    break;
                }
                data.push_back(NULL);
                break;
            case 1:
                if(ri->type(j + 1) == Type::INT)
                {
                    data.push_back(&(pValueLists[i][j].v_int));
                }
                else if(ri->type(j + 1) == Type::FLOAT)
                {
                    tmpFloat.push_back(pValueLists[i][j].v_int);
                    data.push_back(&(tmpFloat.back()));
                }
                else
                {
                    std::cout << "Error: " << ri->name(j + 1) << " should be of type " << Type::typeName(ri->type(j + 1)) << ".\n";
                    valid = false;
                }
                break;
            case 2:
                if(ri->type(j + 1) != Type::VARCHAR)
                {
                    if(ri->type(j + 1) != Type::DATE)
                    {
                        std::cout << "Error: " << ri->name(j + 1) << " should be of type " << Type::typeName(ri->type(j + 1)) << ".\n";
                        valid = false;
                        break;
                    }
                    else
                    {
                        std::string date = pValueLists[i][j].v_str;
                        date = date.substr(1, date.size() - 2);
                        if(date.size() != 10 || (date[4] != '/' && date[4] != '-') || (date[7] != '/' && date[7] != '-'))
                        {
                            std::cout << "Error: " << "Invalid date input.\n";
                            valid = false;
                            break;
                        }
                        for(int i = 0; i < 10; i++)
                        {
                            if(i == 4 || i == 7)
                            {
                                continue;
                            }
                            if(date[i] < '0' || date[i] > '9')
                            {
                                std::cout << "Error: " << "Invalid date input.\n";
                                valid = false;
                                break;
                            }
                        }
                        if(!valid)
                        {
                            break;
                        }
                        if(!IsValidDate((date[0] - '0') * 1000 + (date[1] - '0') * 100 + (date[2] - '0') * 10 + date[3] - '0',
                                        (date[5] - '0') * 10 + date[6] - '0', (date[8] - '0') * 10 + date[9] - '0'))
                        {
                            std::cout << "Error: " << "Invalid date input.\n";
                            valid = false;
                            break;
                        }
                    }
                }
                memset(tmp, 0, ri->length(j + 1));
                pValueLists[i][j].v_str.copy(tmp, pValueLists[i][j].v_str.size() - 2, 1);
                data.push_back(tmp);
                break;
            case 3:
                if(ri->type(j + 1) == Type::FLOAT)
                {
                    data.push_back(&(pValueLists[i][j].v_float));
                }
                else if(ri->type(j + 1) == Type::INT)
                {
                    tmpInt.push_back(pValueLists[i][j].v_float);
                    data.push_back(&(tmpInt.back()));
                }
                else
                {
                    std::cout << "Error: " << ri->name(j + 1) << " should be of type " << Type::typeName(ri->type(j + 1)) << ".\n";
                    valid = false;
                }
                break;
            default:
                std::cout << "Error: " << "unknown type.\n";
                valid = false;
            }
            if(!valid)
            {
                break;
            }
        }
        if(!valid)
        {
            std::cout << "The " << i << "th record insertion failed.\n\n";
        }
        else if(!df.insert(data))
        {
            std::cout << "The " << i << "th record insertion failed. There might be primary key duplication or unique index key duplication.\n\n";
        }
    }
    delete tmp;
//    df.printAllRecords();
    df.closeFile();
    init();
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
	for(int j = 0; j < pNames.size(); j++)
    {
        if(pNames[j] == "_id")
        {
            throw Exception(TAG, "Cannot name a field \"_id\".");
        }
        if(pTypes[j] == Type::_ID)
        {
            throw Exception(TAG, "Cannot define a field of type _ID.");
        }
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
    std::vector<bool> isPrims;
    isPrims.assign(pNames.size(), false);
    isPrims[0] = true;
    for(int i = 0; i < pColumns.size(); i++)
    {
    	int j = -1;
    	for(int k = 1; k < pNames.size(); k++)
    	{
    		if(pNames[k] == pColumns[i])
    		{
    			j = k;
                isPrims[k] = true;
                isPrims[0] = false;
    			break;
    		}
    	}
    	if(j < 0)
    	{
    		throw Exception(TAG, "Field " + pColumns[i] + " is never defined.");
    	}
    }
    DataFile df;
    df.createFile(curDb, tbname);
    df.openFile(curDb, tbname);
    df.addFields(pNames, pTypes, pNullables, pExtras, tmp, isPrims);
    df.closeFile();
    init();
    std::cout << "CREATED TABLE " << tbname << std::endl;
}

void Database::createIndex(std::string tbname, std::string colname, bool unique, int density)
{
	DataFile df;
    df.openFile(curDb, tbname);
    df.createIndex(colname, unique, density);
    df.closeFile();
	std::cout << "CREATED INDEX " << colname << " OF TABLE " << tbname << std::endl;
}

void Database::dropIndex(std::string tbname, std::string colname)
{
	fm->dropIdx(curDb, tbname, colname);
	std::cout << "DROPPED INDEX " << colname << " OF TABLE " << tbname << std::endl;
}

void Database::describeTable(std::string tbname)
{
	DataFile df;
	df.openFile(curDb, tbname);
	df.getRecordInfo()->printRecordDesc(tbname);
	df.closeFile();
}

void Database::dropTable(std::string tbname)
{
	fm->dropTable(curDb, tbname);
	std::cout << "DROPPED TABLE " << tbname << std::endl;
}

void Database::_test()
{
    std::vector<const char*> sFile;
//    sFile.push_back("create&drop.sql");
//    sFile.push_back("primkey.sql");
//    sFile.push_back("insert.sql");
//    sFile.push_back("index.sql");
//    sFile.push_back("remove.sql");
//    sFile.push_back("select.sql");
//    sFile.push_back("update.sql");
//    sFile.push_back("insertvarchar.sql");
//    sFile.push_back("typecheck.sql");
//    sFile.push_back("inter.sql");
//    sFile.push_back("create.sql");
//    sFile.push_back("book.sql");
//    sFile.push_back("customer.sql");
//    sFile.push_back("orders.sql");
//    sFile.push_back("price.sql");
//    sFile.push_back("website.sql");
    sFile.push_back("test.sql");
    for(int i = 0; i < sFile.size(); i++)
    {
        std::cout << "-------------------------------------------------------------\n";
        std::cout << "Running " << sFile[i] << "\n";
        std::cout << "-------------------------------------------------------------\n";
        FILE* fp=fopen(sFile[i], "r");
        if(fp==NULL)
        {
            printf("cannot open %s\n", sFile[i]);
            return;
        }
        extern FILE* yyin;
        yyin=fp;
        yyparse();
        fclose(fp);
    }
}

void Database::test()
{
    getInstance()->_test();
}
