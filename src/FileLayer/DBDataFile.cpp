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
    dfdp->addField("_id", DBType::_ID);
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

int DBDataFile::addField(const char* name, int type)
{
    int re = dfdp->addField(name, type);
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
    default:
        DBLogLine("Succeeded in adding field.");
    }
    return re;
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
    default:
        DBLogLine("Succeeded in altering primary key.");
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


