#ifndef DBSLOT_H_INCLUDED
#define DBSLOT_H_INCLUDED

#include"../DatabaseLayer/DBRecordInfo.h"

class DBSlot
{
protected:
    BufType cache;
};

class DBPageInfoSlot: public DBSlot
{
protected:
    short pageType;
    short firstAvailableByte;
    bool lengthFixed;
    int slotLength;

public:

    DBPageInfoSlot(BufType cache, int slotLength);

    short getPageType();

    short getFirstAvailableByte();

    bool isLengthFixed();

    int getSlotLength();
};

class DBRecordSlot: public DBSlot
{
protected:
    ull rid;
    DBRecordInfo* recordInfo;
    std::map<int, BufType> data;
};

#endif // DBSLOT_H_INCLUDED
