#ifndef USAGEPAGE_H_INCLUDED
#define USAGEPAGE_H_INCLUDED

#include"Page.h"

class UsagePage: public Page
{
public:
    UsagePage(char* cache, int index, int pageID, bool parse);

    static int findOccupiedBy(int pid);

    bool isAvailable(int pid);

    void setAvailable(int pid, bool available);

    bool withinRange(int pid);

    int findFirstAvailable();

    const static int USAGE_VOLUMN = PAGE_SIZE - PAGE_CONTENT_OFFSET;
};

#endif // USAGEPAGE_H_INCLUDED
