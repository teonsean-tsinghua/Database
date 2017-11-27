#include"DBIndexFileDescriptionSlot.h"

DBIndexFileDescriptionSlot::DBIndexFileDescriptionSlot(BufType cache, int mode):
    DBSlot(cache)
{
    firstLeafPage = (*this)[FIRST_LEAF_PAGE_OFFSET];
    pageNumber = (*this)[PAGE_NUMBER_OFFSET];
    rootPage = (*this)[ROOT_PAGE_OFFSET];
    keyType = (*this)[KEY_TYPE_OFFSET];
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
    readInt(firstLeafPage, &re);
    return re;
}

int DBIndexFileDescriptionSlot::getPageNumber()
{
    int re;
    readInt(pageNumber, &re);
    return re;
}

int DBIndexFileDescriptionSlot::getRootPage()
{
    int re;
    readInt(rootPage, &re);
    return re;
}

int DBIndexFileDescriptionSlot::getKeyType()
{
    int re;
    readInt(keyType, &re);
    return re;
}

void DBIndexFileDescriptionSlot::setFirstLeafPage(int n)
{
    writeInt(firstLeafPage, n);
}

void DBIndexFileDescriptionSlot::setPageNumber(int n)
{
    writeInt(pageNumber, n);
}

void DBIndexFileDescriptionSlot::setRootPage(int n)
{
    writeInt(rootPage, n);
}

void DBIndexFileDescriptionSlot::setKeyType(int n)
{
    writeInt(keyType, n);
}
