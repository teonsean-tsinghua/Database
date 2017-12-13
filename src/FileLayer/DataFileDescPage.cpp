#include"DataFileDescPage.h"

DataFileDescPage::DataFileDescPage(BufType cache, int index, int pageID, bool parse, RecordInfo* ri):
    Page(cache, index, pageID, Type::DATA_FILE_DESC_PAGE)
{
    dfds = new DataFileDescSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()]);
    if(!parse)
    {
        pis->setPageType(Type::DATA_FILE_DESC_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(false);
        setFirstDataPage(-1);
        setFirstUsagePage(-1);
        setPageNumber(1);
        setRecordInfoLength(0);
        setPrimaryKeyIndex(0);
        updateFirstAvailable();
    }
    else
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

void DataFileDescPage::updateFirstAvailable()
{
    pis->setFirstAvailableByte(pis->size() + sizeof(int) * 5 + ri->getRecordInfoLength());
}

void DataFileDescPage::incrementPageNumber(int type)
{
    setPageNumber(getPageNumber() + 1);
    switch(type)
    {
    case Type::DATA_PAGE:
        if(getFirstDataPage() <= 0)
        {
            setFirstDataPage(getPageNumber() - 1);
        }
        break;
    case Type::USAGE_PAGE:
        if(getFirstUsagePage() <= 0)
        {
            setFirstUsagePage(getPageNumber() - 1);
        }
        break;
    }
}

int DataFileDescPage::maxRecordInfoLength()
{
    return PAGE_SIZE - PageInfoSlot::size();
}

void DataFileDescPage::writeFields()
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
    updateFirstAvailable();
}

void DataFileDescPage::print()
{
    Page::print();
    int cnt = ri->getFieldCount();
    Print::print("This file has pages of number ").printLine(getPageNumber())
            .print("First data page is at ").printLine(getFirstDataPage())
            .print("First usage page is at ").printLine(getFirstUsagePage())
            .print("Record length is ").printLine(ri->getRecordLength())
            .print("Record info length is ").printLine(ri->getRecordInfoLength())
            .print("Number of fields: ").printLine(cnt)
            .print("Primary key is: ").printLine(getPrimaryKeyIndex());
    for(int i = 0; i < cnt; i++)
    {
        Print::print("Field " + ri->name(i) + " is of type " + std::string(Type::typeName(ri->type(i))));
        if(ri->nullable(i))
        {
            Print::printLine(", and can be null.");
        }
        else
        {
            Print::printLine(", and cannot be null.");
        }
    }
}

int DataFileDescPage::getFirstDataPage()
{
    int re;
    readInt((*dfds)[FIRST_DATA_PAGE_OFFSET], &re);
    return re;
}

int DataFileDescPage::getFirstUsagePage()
{
    int re;
    readInt((*dfds)[FIRST_USAGE_PAGE_OFFSET], &re);
    return re;
}

int DataFileDescPage::getPageNumber()
{
    int re;
    readInt((*dfds)[PAGE_NUMBER_OFFSET], &re);
    return re;
}

int DataFileDescPage::getRecordInfoLength()
{
    int re;
    readInt((*dfds)[RECORD_INFO_LENGTH_OFFSET], &re);
    return re;
}

int DataFileDescPage::getPrimaryKeyIndex()
{
    int re;
    readInt((*dfds)[PRIMARY_KEY_INDEX_OFFSET], &re);
    return re;
}

void DataFileDescPage::setFirstDataPage(int n)
{
    writeInt((*dfds)[FIRST_DATA_PAGE_OFFSET], n);
}

void DataFileDescPage::setFirstUsagePage(int n)
{
    writeInt((*dfds)[FIRST_USAGE_PAGE_OFFSET], n);
}

void DataFileDescPage::setPageNumber(int n)
{
    writeInt((*dfds)[PAGE_NUMBER_OFFSET], n);
}

void DataFileDescPage::setRecordInfoLength(int n)
{
    writeInt((*dfds)[RECORD_INFO_LENGTH_OFFSET], n);
}

void DataFileDescPage::setPrimaryKeyIndex(int n)
{
    writeInt((*dfds)[PRIMARY_KEY_INDEX_OFFSET], n);
}

DataFileDescPage::DataFileDescSlot::DataFileDescSlot(BufType cache):
    Slot(cache)
{

}
