#include"DBPageInfoSlot.h"

DBPageInfoSlot::DBPageInfoSlot(BufType cache):
    DBSlot(cache)
{

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
    readInt((*this)[PAGE_TYPE_OFFSET], &re);
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
    readInt((*this)[FIRST_AVAILABLE_BYTE_OFFSET], &re);
    return re;
}

bool DBPageInfoSlot::isLengthFixed()
{
    int re;
    readInt((*this)[LENGTH_FIXED_OFFSET], &re);
    return re == 1;
}

int DBPageInfoSlot::getNextSamePage()
{
    int re;
    readInt((*this)[NEXT_SAME_PAGE_OFFSET], &re);
    return re;
}

void DBPageInfoSlot::setPageType(int n)
{
    writeInt((*this)[PAGE_TYPE_OFFSET], n);
}

void DBPageInfoSlot::setFirstAvailableByte(int n)
{
    writeInt((*this)[FIRST_AVAILABLE_BYTE_OFFSET], n);
}

void DBPageInfoSlot::setLengthFixed(bool n)
{
    writeInt((*this)[LENGTH_FIXED_OFFSET], n ? 1 : 0);
}

void DBPageInfoSlot::setNextSamePage(int n)
{
    writeInt((*this)[NEXT_SAME_PAGE_OFFSET], n);
}
