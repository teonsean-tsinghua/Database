#include"DataFileDescPage.h"

DataFileDescPage::DataFileDescPage(char* cache, int index, int pageID, bool parse, RecordInfo* ri):
    Page(cache, index, pageID), ri(ri)
{
    if(!parse)
    {
        setPageType(Type::DATA_FILE_DESC_PAGE);
        setNextSamePage(-1);
        setLengthFixed(false);
        setFirstDataPage(-1);
        setFirstUsagePage(-1);
        setPageNumber(1);
        setFieldCount(0);
        setPrimaryKeyCount(0);
        updateFirstAvailable();
    }
    else
    {
    	int index = 0, cnt = getFieldCount();
        char* cache = (*this)[RECORD_INFO_OFFSET];
        int offset = 0, type = 0, name_length = 0, nullable = 0, extra = 0, foreign_length = 0;
        while(index < cnt)
        {
            readInt(cache + RECORD_INFO_TYPE_OFFSET, &type);
            readInt(cache + RECORD_INFO_NULLABLE_OFFSET, &nullable);
            readInt(cache + RECORD_INFO_NAME_LENGTH_OFFSET, &name_length);
            readInt(cache + RECORD_INFO_EXTRA_OFFSET, &extra);
            cache += RECORD_INFO_NAME_OFFSET;
            std::string name, foreign;
            readString(cache, name, name_length);
            cache += name_length;
            readInt(cache, &foreign_length);
            cache += sizeof(int);
            readString(cache, foreign, foreign_length);
            cache += foreign_length;
            ri->addField(name, type, nullable, extra, foreign);
            index++;
        }
        ri->setPrimKeyCnt(getPrimaryKeyCount());
    }
}

void DataFileDescPage::updateFirstAvailable()
{
    setFirstAvailableByte(RECORD_INFO_OFFSET + ri->getRecordInfoLength());
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
    return PAGE_SIZE - RECORD_INFO_OFFSET;
}

void DataFileDescPage::writeFields()
{
    setFieldCount(ri->getFieldCount());
    char* cache = (char*)((*this)[RECORD_INFO_OFFSET]);
    int cnt = ri->getFieldCount();
    for(int i = 0; i < cnt; i++)
    {
        std::string name = ri->name(i);
        std::string foreign = ri->foreign(i);
        int name_length = name.size();
        int foreign_length = foreign.size();
        writeInt(cache + RECORD_INFO_TYPE_OFFSET, ri->type(i));
        writeInt(cache + RECORD_INFO_NULLABLE_OFFSET, ri->nullable(i) ? 1 : 0);
        writeInt(cache + RECORD_INFO_EXTRA_OFFSET, ri->extra(i));
        writeInt(cache + RECORD_INFO_NAME_LENGTH_OFFSET, name_length);
        cache += RECORD_INFO_NAME_OFFSET;
        writeString(cache, name, name_length);
        cache += name_length;
        writeInt(cache, foreign_length);
        cache += sizeof(int);
        writeString(cache, foreign, foreign_length);
        cache += foreign_length;
    }
    updateFirstAvailable();
}

void DataFileDescPage::print()
{
    Page::print();
    int cnt = ri->getFieldCount();
    std::cout << "This file has " << getPageNumber() << " pages\n";
    std::cout << "First data page is at " << getFirstDataPage() << std::endl;
    std::cout << "First usage page is at " << getFirstUsagePage() << std::endl;
    std::cout << "Record length is " << ri->getRecordLength() << std::endl;
    std::cout << "Number of fields: " << cnt << std::endl;
    std::cout << "Primary keys are: ";
    for(int i = 0; i < getPrimaryKeyCount(); i++)
    {
    	std::cout << ri->name(i) << " ";
    }
    std::cout << std::endl;
}

int DataFileDescPage::getFirstDataPage()
{
    int re;
    readInt((*this)[FIRST_DATA_PAGE_OFFSET], &re);
    return re;
}

int DataFileDescPage::getFirstUsagePage()
{
    int re;
    readInt((*this)[FIRST_USAGE_PAGE_OFFSET], &re);
    return re;
}

int DataFileDescPage::getPageNumber()
{
    int re;
    readInt((*this)[PAGE_NUMBER_OFFSET], &re);
    return re;
}

int DataFileDescPage::getFieldCount()
{
    int re;
    readInt((*this)[FIELD_COUNT_OFFSET], &re);
    return re;
}

int DataFileDescPage::getPrimaryKeyCount()
{
    int re;
    readInt((*this)[PRIMARY_KEY_COUNT_OFFSET], &re);
    return re;
}

void DataFileDescPage::setFirstDataPage(int n)
{
    writeInt((*this)[FIRST_DATA_PAGE_OFFSET], n);
}

void DataFileDescPage::setFirstUsagePage(int n)
{
    writeInt((*this)[FIRST_USAGE_PAGE_OFFSET], n);
}

void DataFileDescPage::setPageNumber(int n)
{
    writeInt((*this)[PAGE_NUMBER_OFFSET], n);
}

void DataFileDescPage::setFieldCount(int n)
{
    writeInt((*this)[FIELD_COUNT_OFFSET], n);
}

void DataFileDescPage::setPrimaryKeyCount(int n)
{
    writeInt((*this)[PRIMARY_KEY_COUNT_OFFSET], n);
}
