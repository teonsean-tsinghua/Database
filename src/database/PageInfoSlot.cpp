#include"PageInfoSlot.h"

PageInfoSlot::PageInfoSlot(BufType cache):
    Slot(cache)
{

}

int PageInfoSlot::size()
{
    return sizeof(int) * 3 + sizeof(int);
}

void PageInfoSlot::write(int lf, int nsp)
{
    setLengthFixed(lf);
    setNextSamePage(nsp);
}

void PageInfoSlot::print()
{
    Print::print("Page type: ").printLine(Type::pageName(getPageType()))
            .print("First available byte: ").printLine(getFirstAvailableByte());
    if(isLengthFixed())
    {
        Print::printLine("This page stores slots with fixed length.");
    }
    else
    {
        Print::printLine("This page stores slots with variable length.");
    }
    Print::print("Next page of this type: ").printLine(getNextSamePage());
}

int PageInfoSlot::getPageType()
{
    int re;
    readInt((*this)[PAGE_TYPE_OFFSET], &re);
    return re;
}

int PageInfoSlot::getPageType(BufType cache)
{
    int re;
    readInt((BufType)((char*)cache + PAGE_TYPE_OFFSET), &re);
    return re;
}

int PageInfoSlot::getFirstAvailableByte()
{
    int re;
    readInt((*this)[FIRST_AVAILABLE_BYTE_OFFSET], &re);
    return re;
}

bool PageInfoSlot::isLengthFixed()
{
    int re;
    readInt((*this)[LENGTH_FIXED_OFFSET], &re);
    return re == 1;
}

int PageInfoSlot::getNextSamePage()
{
    int re;
    readInt((*this)[NEXT_SAME_PAGE_OFFSET], &re);
    return re;
}

void PageInfoSlot::setPageType(int n)
{
    writeInt((*this)[PAGE_TYPE_OFFSET], n);
}

void PageInfoSlot::setFirstAvailableByte(int n)
{
    writeInt((*this)[FIRST_AVAILABLE_BYTE_OFFSET], n);
}

void PageInfoSlot::setLengthFixed(bool n)
{
    writeInt((*this)[LENGTH_FIXED_OFFSET], n ? 1 : 0);
}

void PageInfoSlot::setNextSamePage(int n)
{
    writeInt((*this)[NEXT_SAME_PAGE_OFFSET], n);
}
