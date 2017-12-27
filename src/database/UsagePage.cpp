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

void UsagePage::extendRange(int pid)
{
    assert(pid > pageID + getFirstAvailableByte() - PAGE_CONTENT_OFFSET &&
            pid <= pageID + PAGE_SIZE - PAGE_CONTENT_OFFSET);
    setFirstAvailableByte(PAGE_CONTENT_OFFSET + pid - pageID);
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

void UsagePage::test()
{
    char page[8192];
    UsagePage* up = new UsagePage(page, 1, 1, false);
    assert(up->findFirstAvailable() == 0);
    up->extendRange(1024);
    assert(up->withinRange(512));
    up->setAvailable(128, true);
    UsagePage* up2 = new UsagePage(page, 1, 1, true);
    assert(up->getPageType() == Type::USAGE_PAGE);
    assert(up->getFirstAvailableByte() == PAGE_CONTENT_OFFSET + 1024 - 1);
    assert(up->isAvailable(128));
    assert(findOccupiedBy(128) == 1);
    assert(findOccupiedBy(8161) == 1);
    assert(findOccupiedBy(8179) == 8178);
    delete up;
    delete up2;
    std::cout << "Passed usage page test.\n";
}
