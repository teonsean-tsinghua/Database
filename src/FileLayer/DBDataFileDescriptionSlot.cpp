#include"DBDataFileDescriptionSlot.h"

DBDataFileDescriptionSlot::DBDataFileDescriptionSlot(BufType cache, int mode, DBRecordInfo* ri):
    DBSlot(cache), ri(ri)
{
    if(mode == MODE_PARSE)
    {
        char* cache = (char*)(*this)[RECORD_INFO_OFFSET];
        char* end = cache + getRecordInfoLength();
        int offset = 0, type = 0, name_length = 0, nullable = 0, extra = 0;
        while(cache < end)
        {
            readInt((BufType)(cache + RECORD_INFO_TYPE_OFFSET), &type);
            readInt((BufType)(cache + RECORD_INFO_NULLABLE_OFFSET), &nullable);
            readInt((BufType)(cache + RECORD_INFO_NAME_LENGTH_OFFSET), &name_length);
            readInt((BufType)(cache + RECORD_INFO_EXTRA_OFFSET), &extra);
            cache += RECORD_INFO_NAME_OFFSET;
            std::string name;
            readString(cache, name, name_length);
            ri->addField(name, type, nullable, extra);
            cache += name_length;
        }
    }
    else if(mode == MODE_CREATE)
    {
        setRecordInfoLength(0);
        setPrimaryKeyIndex(0);
    }
}

int DBDataFileDescriptionSlot::size()
{
    return sizeof(int) * 5 + ri->getRecordInfoLength();
}

int DBDataFileDescriptionSlot::minSize()
{
    return sizeof(int) * 5;
}

void DBDataFileDescriptionSlot::printRecordDescription()
{
    int cnt = ri->getFieldCount();
    std::ostringstream oss;
    for(int i = 1; i < cnt; i++)
    {
        oss << "    " << ri->name(i) << " " << DBType::typeName(ri->type(i));
        if(ri->extra(i) > 0)
        {
            oss << "(" << ri->extra(i) << ")";
        }
        if(!ri->nullable(i))
        {
            oss << " NOT NULL";
        }
        if(i != cnt - 1)
        {
            oss << ",";
        }
        oss << "\n";
    }
    DBPrint::print(oss.str());
}

void DBDataFileDescriptionSlot::print()
{
    int cnt = ri->getFieldCount();
    DBPrint::print("This file has pages of number ").printLine(getPageNumber())
            .print("First data page is at ").printLine(getFirstDataPage())
            .print("First usage page is at ").printLine(getFirstUsagePage())
            .print("Record length is ").printLine(ri->getRecordLength())
            .print("Record info length is ").printLine(ri->getRecordInfoLength())
            .print("Number of fields: ").printLine(cnt)
            .print("Primary key is: ").printLine(getPrimaryKeyIndex());
    for(int i = 0; i < cnt; i++)
    {
        DBPrint::print("Field " + ri->name(i) + " is of type " + std::string(DBType::typeName(ri->type(i))));
        if(ri->nullable(i))
        {
            DBPrint::printLine(", and can be null.");
        }
        else
        {
            DBPrint::printLine(", and cannot be null.");
        }
    }
}

int DBDataFileDescriptionSlot::getFirstDataPage()
{
    int re;
    readInt((*this)[FIRST_DATA_PAGE_OFFSET], &re);
    return re;
}

int DBDataFileDescriptionSlot::getFirstUsagePage()
{
    int re;
    readInt((*this)[FIRST_USAGE_PAGE_OFFSET], &re);
    return re;
}

int DBDataFileDescriptionSlot::getPageNumber()
{
    int re;
    readInt((*this)[PAGE_NUMBER_OFFSET], &re);
    return re;
}

int DBDataFileDescriptionSlot::getRecordInfoLength()
{
    int re;
    readInt((*this)[RECORD_INFO_LENGTH_OFFSET], &re);
    return re;
}

int DBDataFileDescriptionSlot::getPrimaryKeyIndex()
{
    int re;
    readInt((*this)[PRIMARY_KEY_INDEX_OFFSET], &re);
    return re;
}

void DBDataFileDescriptionSlot::setFirstDataPage(int n)
{
    writeInt((*this)[FIRST_DATA_PAGE_OFFSET], n);
}

void DBDataFileDescriptionSlot::setFirstUsagePage(int n)
{
    writeInt((*this)[FIRST_USAGE_PAGE_OFFSET], n);
}

void DBDataFileDescriptionSlot::setPageNumber(int n)
{
    writeInt((*this)[PAGE_NUMBER_OFFSET], n);
}

void DBDataFileDescriptionSlot::setRecordInfoLength(int n)
{
    writeInt((*this)[RECORD_INFO_LENGTH_OFFSET], n);
}

void DBDataFileDescriptionSlot::setPrimaryKeyIndex(int n)
{
    writeInt((*this)[PRIMARY_KEY_INDEX_OFFSET], n);
}

void DBDataFileDescriptionSlot::write()
{
    setRecordInfoLength(ri->getRecordInfoLength());
    char* cache = (char*)(*this)[RECORD_INFO_OFFSET];
    int cnt = ri->getFieldCount();
    for(int i = 0; i < cnt; i++)
    {;
        std::string name = ri->name(i);
        int name_length = name.size();
        writeInt((BufType)(cache + RECORD_INFO_TYPE_OFFSET), ri->type(i));
        writeInt((BufType)(cache + RECORD_INFO_NULLABLE_OFFSET), ri->nullable(i) ? 1 : 0);
        writeInt((BufType)(cache + RECORD_INFO_EXTRA_OFFSET), ri->extra(i));
        writeInt((BufType)(cache + RECORD_INFO_NAME_LENGTH_OFFSET), name_length);
        cache += RECORD_INFO_NAME_OFFSET;
        writeString(cache, name, name_length);
        cache += name_length;
    }
}
