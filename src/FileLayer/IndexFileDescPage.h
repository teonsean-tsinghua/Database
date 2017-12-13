#ifndef INDEXFILEDESCPAGE_H_INCLUDED
#define INDEXFILEDESCPAGE_H_INCLUDED

#include"Page.h"
#include"Slot.h"

class IndexFileDescPage: public Page
{
protected:
    class IndexFileDescSlot: public Slot
    {
    public:
        IndexFileDescSlot(BufType cache);

        int size();
    };

    IndexFileDescSlot* ifds;

public:
    IndexFileDescPage(BufType cache, int index, int pageID, bool parse, int type = -1, int length = -1);

    void incrementPageNumber();

    int getFirstLeafPage();

    int getPageNumber();

    int getRootPage();

    int getKeyLength();

    int getKeyType();

    void setFirstLeafPage(int n);

    void setPageNumber(int n);

    void setRootPage(int n);

    void setKeyLength(int n);

    void setKeyType(int n);

    void print();

    const static int FIRST_LEAF_PAGE_OFFSET = 0;
    const static int PAGE_NUMBER_OFFSET = FIRST_LEAF_PAGE_OFFSET + sizeof(int);
    const static int ROOT_PAGE_OFFSET = PAGE_NUMBER_OFFSET + sizeof(int);
    const static int KEY_TYPE_OFFSET = ROOT_PAGE_OFFSET + sizeof(int);
    const static int KEY_LENGTH_OFFSET = KEY_TYPE_OFFSET + sizeof(int);
};

#endif // INDEXFILEDESCPAGE_H_INCLUDED
