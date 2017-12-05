#include"DBIndexFileDescriptionPage.h"

DBIndexFileDescriptionPage::DBIndexFileDescriptionPage(BufType cache, int index, int pageID, int mode, int type, int length):
    DBPage(cache, index, pageID, DBType::INDEX_FILE_DESCRIPTION_PAGE, mode)
{
    ifds = new DBIndexFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()]);
    if(mode == MODE_CREATE)
    {
        pis->setPageType(DBType::INDEX_FILE_DESCRIPTION_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(false);
        pis->setFirstAvailableByte(pis->size() + ifds->size());
        setFirstLeafPage(-1);
        setPageNumber(1);
        setKeyType(type);
        setKeyLength(length);
        setRootPage(-1);
    }
}

void DBIndexFileDescriptionPage::incrementPageNumber()
{
    setPageNumber(getPageNumber() + 1);
}

void DBIndexFileDescriptionPage::print()
{
    DBPage::print();
    DBPrint::print("This file has pages of number ").printLine(getPageNumber())
            .print("First leaf page is at ").printLine(getFirstLeafPage())
            .print("Root page is at ").printLine(getRootPage())
            .print("Keys are of type ").printLine(DBType::typeName(getKeyType()));
}

DBIndexFileDescriptionPage::DBIndexFileDescriptionSlot::DBIndexFileDescriptionSlot(BufType cache):
    DBSlot(cache)
{

}

int DBIndexFileDescriptionPage::DBIndexFileDescriptionSlot::size()
{
    return sizeof(int) * 4;
}

int DBIndexFileDescriptionPage::getFirstLeafPage()
{
    int re;
    readInt((*ifds)[FIRST_LEAF_PAGE_OFFSET], &re);
    return re;
}

int DBIndexFileDescriptionPage::getPageNumber()
{
    int re;
    readInt((*ifds)[PAGE_NUMBER_OFFSET], &re);
    return re;
}

int DBIndexFileDescriptionPage::getRootPage()
{
    int re;
    readInt((*ifds)[ROOT_PAGE_OFFSET], &re);
    return re;
}

int DBIndexFileDescriptionPage::getKeyType()
{
    int re;
    readInt((*ifds)[KEY_TYPE_OFFSET], &re);
    return re;
}

int DBIndexFileDescriptionPage::getKeyLength()
{
    int re;
    readInt((*ifds)[KEY_LENGTH_OFFSET], &re);
    return re;
}

void DBIndexFileDescriptionPage::setFirstLeafPage(int n)
{
    writeInt((*ifds)[FIRST_LEAF_PAGE_OFFSET], n);
}

void DBIndexFileDescriptionPage::setPageNumber(int n)
{
    writeInt((*ifds)[PAGE_NUMBER_OFFSET], n);
}

void DBIndexFileDescriptionPage::setRootPage(int n)
{
    writeInt((*ifds)[ROOT_PAGE_OFFSET], n);
}

void DBIndexFileDescriptionPage::setKeyType(int n)
{
    writeInt((*ifds)[KEY_TYPE_OFFSET], n);
}

void DBIndexFileDescriptionPage::setKeyLength(int n)
{
    writeInt((*ifds)[KEY_LENGTH_OFFSET], n);
}
