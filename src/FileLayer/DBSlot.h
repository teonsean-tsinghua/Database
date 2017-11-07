#ifndef DBSLOT_H_INCLUDED
#define DBSLOT_H_INCLUDED

#include"DBFileIOModel.h"

class DBSlot
{
protected:
    BufType cache;
    int slotLength;

public:
    DBSlot(BufType cache, int slotLength = 0);

    int getSlotLength();

    BufType operator[](const int offset) const;

    const static int SLOT_LENGTH_OFFSET = 0;    //If slot length is -1, then DBSlot will go find its length at this offset.
};

class DBFileDescriptionSlot: public DBSlot
{
protected:

};

class DBPageInfoSlot: public DBSlot
{
protected:
    short* pageType;
    short* firstAvailableByte;
    bool* lengthFixed;
    int* nextSamePage;

public:

    DBPageInfoSlot(BufType cache, int slotLength);

    short getPageType();

    short getFirstAvailableByte();

    bool isLengthFixed();

    int getNextSamePage();

    const static int PAGE_TYPE_OFFSET = 0;
    const static int FIRST_AVAILABLE_BYTE_OFFSET = PAGE_TYPE_OFFSET + sizeof(short);
    const static int LENGTH_FIXED_OFFSET = FIRST_AVAILABLE_BYTE_OFFSET + sizeof(short);
    const static int NEXT_SAME_PAGE_OFFSET = LENGTH_FIXED_OFFSET + sizeof(bool);
};

class DBRecordSlot: public DBSlot
{
public:
    DBRecordSlot(BufType cache, int slotLength);
};

#endif // DBSLOT_H_INCLUDED
