#ifndef PAGEINFOSLOT_H_INCLUDED
#define PAGEINFOSLOT_H_INCLUDED

#include"Slot.h"

class PageInfoSlot: public Slot
{
public:
    PageInfoSlot(BufType cache);

    static int size();

    void print();

    int getPageType();

    static int getPageType(BufType cache);

    int getFirstAvailableByte();

    bool isLengthFixed();

    int getNextSamePage();

    void setPageType(int n);

    void setFirstAvailableByte(int n);

    void setLengthFixed(bool n);

    void setNextSamePage(int n);

    void write(int lf, int nsp);

    const static int PAGE_TYPE_OFFSET = 0;
    const static int FIRST_AVAILABLE_BYTE_OFFSET = PAGE_TYPE_OFFSET + sizeof(int);
    const static int LENGTH_FIXED_OFFSET = FIRST_AVAILABLE_BYTE_OFFSET + sizeof(int);
    const static int NEXT_SAME_PAGE_OFFSET = LENGTH_FIXED_OFFSET + sizeof(int);
};

#endif // PAGEINFOSLOT_H_INCLUDED
