#include"DBPageInfoSlot.h"

DBPageInfoSlot::DBPageInfoSlot(BufType cache):
    DBSlot(cache)
{
    pageType = (*this)[PAGE_TYPE_OFFSET];
    firstAvailableByte = (*this)[FIRST_AVAILABLE_BYTE_OFFSET];
    lengthFixed = (*this)[LENGTH_FIXED_OFFSET];
    nextSamePage = (*this)[NEXT_SAME_PAGE_OFFSET];
}

int DBPageInfoSlot::size()
{
    return sizeof(int) * 3 + sizeof(int);
}

void DBPageInfoSlot::write(int lf, int nsp)
{
    setLengthFixed(lf);
    setNextSamePage(nsp);
}

void DBPageInfoSlot::print()
{
    DBPrint::print("Page type: ").printLine(DBType::pageName(getPageType()))
            .print("First available byte: ").printLine(getFirstAvailableByte());
    if(isLengthFixed())
    {
        DBPrint::printLine("This page stores slots with fixed length.");
    }
    else
    {
        DBPrint::printLine("This page stores slots with variable length.");
    }
    DBPrint::print("Next page of this type: ").printLine(getNextSamePage());
}

int DBPageInfoSlot::getPageType()
{
    int re;
    readInt(pageType, &re);
    return re;
}

int DBPageInfoSlot::getPageType(BufType cache)
{
    int re;
    readInt((BufType)((char*)cache + PAGE_TYPE_OFFSET), &re);
    return re;
}

int DBPageInfoSlot::getFirstAvailableByte()
{
    int re;
    readInt(firstAvailableByte, &re);
    return re;
}

bool DBPageInfoSlot::isLengthFixed()
{
    int re;
    readInt(lengthFixed, &re);
    return re == 1;
}

int DBPageInfoSlot::getNextSamePage()
{
    int re;
    readInt(nextSamePage, &re);
    return re;
}

void DBPageInfoSlot::setPageType(int n)
{
    writeInt(pageType, n);
}

void DBPageInfoSlot::setFirstAvailableByte(int n)
{
    writeInt(firstAvailableByte, n);
}

void DBPageInfoSlot::setLengthFixed(bool n)
{
    writeInt(lengthFixed, n ? 1 : 0);
}

void DBPageInfoSlot::setNextSamePage(int n)
{
    writeInt(nextSamePage, n);
}
