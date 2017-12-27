#include"Page.h"

Page::Page(char* cache, int index, int pageID):
    cache(cache), index(index), pageID(pageID)
{

}

char* Page::operator[](const int offset) const
{
    return cache + offset;
}

int Page::getPageType()
{
    return readInt((*this)[PAGE_TYPE_OFFSET]);
}

int Page::getPageType(char* buf)
{
    return readInt(buf + PAGE_TYPE_OFFSET);
}

int Page::getFirstAvailableByte()
{
    return readInt((*this)[FIRST_AVAILABLE_BYTE_OFFSET]);
}

bool Page::isLengthFixed()
{
    return readInt((*this)[LENGTH_FIXED_OFFSET]) == 1;
}

int Page::getNextSamePage()
{
    return readInt((*this)[NEXT_SAME_PAGE_OFFSET]);
}

void Page::setPageType(int n)
{
    writeInt((*this)[PAGE_TYPE_OFFSET], n);
}

void Page::setFirstAvailableByte(int n)
{
    writeInt((*this)[FIRST_AVAILABLE_BYTE_OFFSET], n);
}

void Page::setLengthFixed(bool n)
{
    writeInt((*this)[LENGTH_FIXED_OFFSET], n ? 1 : 0);
}

void Page::setNextSamePage(int n)
{
    writeInt((*this)[NEXT_SAME_PAGE_OFFSET], n);
}

int Page::getIndex()
{
    return index;
}

void Page::print()
{
    std::cout << "Page ID:                          " << pageID << std::endl;
    std::cout << "Page type:                        " << Type::pageName(getPageType()) << std::endl;
    std::cout << "First available byte:             " << getFirstAvailableByte() << std::endl;
//    if(isLengthFixed())
//    {
//        std::cout << "This page stores slots with fixed length.\n";
//    }
//    else
//    {
//        std::cout << "This page stores slots with variable length.\n";
//    }
    std::cout << "Next page in list:                " << getNextSamePage() << std::endl;
}

int Page::getPageID()
{
    return pageID;
}
