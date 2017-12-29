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

int Page::getPageTypeStatik(char* buf)
{
    return readInt(buf + PAGE_TYPE_OFFSET);
}

int Page::getFirstAvailableByte()
{
    return readInt((*this)[FIRST_AVAILABLE_BYTE_OFFSET]);
}

int Page::getPrevSamePage()
{
    return readInt((*this)[PREV_SAME_PAGE_OFFSET]);
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

void Page::setPrevSamePage(int n)
{
    writeInt((*this)[PREV_SAME_PAGE_OFFSET], n);
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
    std::cout << "Prev page in list:                " << getPrevSamePage() << std::endl;
    std::cout << "Next page in list:                " << getNextSamePage() << std::endl;
}

int Page::getPageID()
{
    return pageID;
}
