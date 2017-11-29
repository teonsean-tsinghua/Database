#include"DBIndexFileDescriptionSlot.h"

DBIndexFileDescriptionSlot::DBIndexFileDescriptionSlot(BufType cache, int mode):
    DBSlot(cache)
{

}

int DBIndexFileDescriptionSlot::size()
{
    return sizeof(int) * 4;
}

void DBIndexFileDescriptionSlot::print()
{
    DBPrint::print("This file has pages of number ").printLine(getPageNumber())
            .print("First leaf page is at ").printLine(getFirstLeafPage())
            .print("Root page is at ").printLine(getRootPage())
            .print("Keys are of type ").printLine(DBType::typeName(getKeyType()));
}

int DBIndexFileDescriptionSlot::getFirstLeafPage()
{
    int re;
    readInt((*this)[FIRST_LEAF_PAGE_OFFSET], &re);
    return re;
}

int DBIndexFileDescriptionSlot::getPageNumber()
{
    int re;
    readInt((*this)[PAGE_NUMBER_OFFSET], &re);
    return re;
}

int DBIndexFileDescriptionSlot::getRootPage()
{
    int re;
    readInt((*this)[ROOT_PAGE_OFFSET], &re);
    return re;
}

int DBIndexFileDescriptionSlot::getKeyType()
{
    int re;
    readInt((*this)[KEY_TYPE_OFFSET], &re);
    return re;
}

int DBIndexFileDescriptionSlot::getKeyLength()
{
    int re;
    readInt((*this)[KEY_LENGTH_OFFSET], &re);
    return re;
}

void DBIndexFileDescriptionSlot::setFirstLeafPage(int n)
{
    writeInt((*this)[FIRST_LEAF_PAGE_OFFSET], n);
}

void DBIndexFileDescriptionSlot::setPageNumber(int n)
{
    writeInt((*this)[PAGE_NUMBER_OFFSET], n);
}

void DBIndexFileDescriptionSlot::setRootPage(int n)
{
    writeInt((*this)[ROOT_PAGE_OFFSET], n);
}

void DBIndexFileDescriptionSlot::setKeyType(int n)
{
    writeInt((*this)[KEY_TYPE_OFFSET], n);
}

void DBIndexFileDescriptionSlot::setKeyLength(int n)
{
    writeInt((*this)[KEY_LENGTH_OFFSET], n);
}
