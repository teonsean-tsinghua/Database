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
    DBPrint("This file has pages of number ");
    DBPrintLine(getPageNumber());
    DBPrint("First leaf page is at ");
    DBPrintLine(getFirstLeafPage());
    DBPrint("Root page is at ");
    DBPrintLine(getRootPage());
    DBPrint("Keys are of type ");
    DBPrintLine(DBType::typeName(getKeyType()));
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
