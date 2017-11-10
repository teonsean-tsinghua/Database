#include"DBDataFile.h"

DBDataFile::DBDataFile(const char* root):
    root(root)
{
    fm = DBFileIOModel::getInstance();
}

int DBDataFile::createFile(const char* name)
{
    if(fm->createFile(name) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, MODE_CREATE);
    dfdp->addField("_id", DBType::_ID, false);
    fm->flush(dfdp->getIndex());
    fm->closeFile(fileID);
}

int DBDataFile::deleteFile(const char* name)
{
    if(fm->deleteFile(name) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
}

int DBDataFile::closeFile()
{
    fm->flush(dfdp->getIndex());
    if(fm->closeFile(fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
}

void DBDataFile::printFileDescription()
{
    dfdp->print();
}

int DBDataFile::addField(const char* name, int type, bool nullable)
{
    int re = dfdp->addField(name, type, nullable);
    //TODO: might need to rewrite the whole file. But this is not included in the requirements seemingly, so leave it here for now.
    switch(re)
    {
    case EMPTY_FIELD_NAME:
        DBPrintLine("Field name cannot be empty.");
        break;
    case FIELD_ALREADY_EXIST:
        DBPrintLine("Field " + std::string(name) + " already exists.");
        break;
    case EXCEED_PAGE_LIMIT:
        DBPrintLine("You cannot add any more fields to this table.");
        break;
    }
    return re;
}

int DBDataFile::insertRecord(std::map<std::string, void*>& fields)
{
    std:map<int, void*> processed;
    std::map<std::string, int> errors;
    if(dfdp->processRawData(fields, processed, errors) == SUCCEED)
    {
        return SUCCEED;
    }
    else
    {
        std::map<std::string, int>::iterator iter;
        for(iter = errors.begin(); iter != errors.end(); iter++)
        {
            switch(iter->second)
            {
            case DBDataFileDescriptionPage::MISSING_FIELD:
                DBPrintLine("Field " + iter->first + " should be assigned.");
                break;
            case DBDataFileDescriptionPage::EXTRA_FIELD:
                DBPrintLine("This table does not contain field " + iter->first);
                break;
            case DBDataFileDescriptionPage::EDIT__ID:
                DBPrintLine("You should not modify _id of any record.");
                break;
            }
        }
        return ERROR;
    }
}

int DBDataFile::setPrimaryKey(const char* name)
{
    int re = dfdp->setPrimaryKey(name);
    switch(re)
    {
    case FIELD_IS_ALREADY_PRIMARY_KEY:
        DBPrintLine("This field is already the primary key.");
        break;
    case FIELD_NOT_EXIST:
        DBPrintLine("This table does not contain field " + std::string(name));
        break;
    }
    return re;
}

int DBDataFile::openFile(const char* name)
{
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, MODE_PARSE);
}


