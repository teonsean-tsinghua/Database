#include"DBDataFileDescriptionPage.h"

DBDataFileDescriptionPage::DBDataFileDescriptionPage(BufType cache, int index, int pageID, int mode, DBRecordInfo* ri):
    DBPage(cache, index, pageID, DBType::DATA_FILE_DESCRIPTION_PAGE, mode)
{
    dfds = new DBDataFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()]);
    if(mode == MODE_CREATE)
    {
        pis->setPageType(DBType::DATA_FILE_DESCRIPTION_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(false);
        pis->setFirstAvailableByte(pis->size() + dfds->size());
        setFirstDataPage(-1);
        setFirstUsagePage(-1);
        setPageNumber(1);
        setRecordInfoLength(0);
        setPrimaryKeyIndex(0);
    }
    else if(mode == MODE_PARSE)
    {
        char* cache = (char*)(*dfds)[RECORD_INFO_OFFSET];
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
}

void DBDataFileDescriptionPage::incrementPageNumber(int type)
{
    setPageNumber(getPageNumber() + 1);
    switch(type)
    {
    case DBType::DATA_PAGE:
        if(getFirstDataPage() <= 0)
        {
            setFirstDataPage(getPageNumber() - 1);
        }
        break;
    case DBType::USAGE_PAGE:
        if(getFirstUsagePage() <= 0)
        {
            setFirstUsagePage(getPageNumber() - 1);
        }
        break;
    }
}

int DBDataFileDescriptionPage::maxRecordInfoLength()
{
    return PAGE_SIZE - DBPageInfoSlot::size();
}

void DBDataFileDescriptionPage::writeFields()
{
    setRecordInfoLength(ri->getRecordInfoLength());
    char* cache = (char*)((*dfds)[RECORD_INFO_OFFSET]);
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
    pis->setFirstAvailableByte(pis->size() + dfds->size() + ri->getRecordInfoLength());
}

void DBDataFileDescriptionPage::print()
{
    DBPage::print();
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

int DBDataFileDescriptionPage::getFirstDataPage()
{
    int re;
    readInt((*dfds)[FIRST_DATA_PAGE_OFFSET], &re);
    return re;
}

int DBDataFileDescriptionPage::getFirstUsagePage()
{
    int re;
    readInt((*dfds)[FIRST_USAGE_PAGE_OFFSET], &re);
    return re;
}

int DBDataFileDescriptionPage::getPageNumber()
{
    int re;
    readInt((*dfds)[PAGE_NUMBER_OFFSET], &re);
    return re;
}

int DBDataFileDescriptionPage::getRecordInfoLength()
{
    int re;
    readInt((*dfds)[RECORD_INFO_LENGTH_OFFSET], &re);
    return re;
}

int DBDataFileDescriptionPage::getPrimaryKeyIndex()
{
    int re;
    readInt((*dfds)[PRIMARY_KEY_INDEX_OFFSET], &re);
    return re;
}

void DBDataFileDescriptionPage::setFirstDataPage(int n)
{
    writeInt((*dfds)[FIRST_DATA_PAGE_OFFSET], n);
}

void DBDataFileDescriptionPage::setFirstUsagePage(int n)
{
    writeInt((*dfds)[FIRST_USAGE_PAGE_OFFSET], n);
}

void DBDataFileDescriptionPage::setPageNumber(int n)
{
    writeInt((*dfds)[PAGE_NUMBER_OFFSET], n);
}

void DBDataFileDescriptionPage::setRecordInfoLength(int n)
{
    writeInt((*dfds)[RECORD_INFO_LENGTH_OFFSET], n);
}

void DBDataFileDescriptionPage::setPrimaryKeyIndex(int n)
{
    writeInt((*dfds)[PRIMARY_KEY_INDEX_OFFSET], n);
}

DBDataFileDescriptionPage::DBDataFileDescriptionSlot::DBDataFileDescriptionSlot(BufType cache):
    DBSlot(cache)
{

}

int DBDataFileDescriptionPage::DBDataFileDescriptionSlot::size()
{
    return sizeof(int) * 5;
}
