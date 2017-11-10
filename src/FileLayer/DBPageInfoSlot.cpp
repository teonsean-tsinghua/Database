#include"DBPageInfoSlot.h"

DBPageInfoSlot::DBPageInfoSlot(BufType cache, int type, int parse):
    DBSlot(cache, parse)
{
    pageType = (*this)[PAGE_TYPE_OFFSET];
    firstAvailableByte = (*this)[FIRST_AVAILABLE_BYTE_OFFSET];
    lengthFixed = (*this)[LENGTH_FIXED_OFFSET];
    nextSamePage = (*this)[NEXT_SAME_PAGE_OFFSET];
    if(parse)
    {
        if(type != getPageType())
        {
            throw "Incompatible page type";
        }
    }
    else
    {
        setPageType(type);
    }
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
    DBPrint("Page type: ");
    DBPrintLine(DBType::pageName(getPageType()));
    DBPrint("First available byte: ");
    DBPrintLine(getFirstAvailableByte());
    DBPrint("Length fixed: ");
    DBPrintLine(isLengthFixed());
    if(isLengthFixed())
    {
        DBPrintLine("This page stores records with fixed length.");
    }
    else
    {
        DBPrintLine("This page stores records with variable length.");
    }
    DBPrint("Next page of this type: ");
    DBPrintLine(getNextSamePage());
}

int DBPageInfoSlot::getPageType()
{
    int re;
    readInt(pageType, &re);
    return re;
}

int DBPageInfoSlot::getFirstAvailableByte()
{
    int re;
    readInt(firstAvailableByte, &re);
    return re;
}

int DBPageInfoSlot::isLengthFixed()
{
    int re;
    readInt(lengthFixed, &re);
    return re;
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

void DBPageInfoSlot::setLengthFixed(int n)
{
    writeInt(lengthFixed, n);
}

void DBPageInfoSlot::setNextSamePage(int n)
{
    writeInt(nextSamePage, n);
}
