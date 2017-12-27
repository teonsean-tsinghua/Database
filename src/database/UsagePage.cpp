#include"UsagePage.h"

UsagePage::UsagePage(char* cache, int index, int pageID, bool parse):
    Page(cache, index, pageID)
{
    assert(pageID % (USAGE_VOLUMN + 1) == 1);
    if(!parse)
    {
        setFirstAvailableByte(PAGE_CONTENT_OFFSET);
        setLengthFixed(true);
        setNextSamePage(-1);
        setPageType(Type::USAGE_PAGE);
    }
    else
    {
        assert(getPageType() == Type::USAGE_PAGE);
    }
}

int UsagePage::findOccupiedBy(int pid)
{
    assert(pid > 0 && pid % (USAGE_VOLUMN + 1) != 1);
    return ((pid - 1) / (USAGE_VOLUMN + 1)) * (USAGE_VOLUMN + 1) + 1;
}

bool UsagePage::withinRange(int pid)
{
    return pid > pageID && pid <= pageID + getFirstAvailableByte() - PAGE_CONTENT_OFFSET;
}

bool UsagePage::isAvailable(int pid)
{
    assert(withinRange(pid));
    return readBool((*this)[PAGE_CONTENT_OFFSET + pid - pageID - 1]);
}

void UsagePage::setAvailable(int pid, bool available)
{
    assert(withinRange(pid));
    writeBool((*this)[PAGE_CONTENT_OFFSET + pid - pageID - 1], available);
}

int UsagePage::findFirstAvailable()
{
    int begin = pageID + 1, end = pageID + getFirstAvailableByte() - PAGE_CONTENT_OFFSET + 1;
    while(begin < end)
    {
        if(isAvailable(begin))
        {
            return begin;
        }
        begin++;
    }
    return 0;
}
