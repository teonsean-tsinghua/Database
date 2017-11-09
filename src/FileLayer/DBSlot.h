#ifndef DBSLOT_H_INCLUDED
#define DBSLOT_H_INCLUDED

#include"DBFileIOModel.h"
#include"DBParser.h"
#include"../DBInclude.h"

class DBSlot
{
protected:
    BufType cache;
    int slotLength;

public:
    DBSlot(BufType cache, int slotLength = 0, bool parse = false);

    virtual int size();

    int getSlotLength();

    BufType operator[](const int offset) const;

    const static int SLOT_LENGTH_OFFSET = 0;    //If slot length is -1, then DBSlot will go find its length at this offset.
};

class DBDataFileDescriptionSlot: public DBSlot
{
protected:
    BufType firstDataPage;     // int
    BufType firstUsageSlot;    // int; pageID * PAGE_SIZE + offset
    BufType lastUsageSlot;     // int
    BufType recordInfoLength;  // int
    BufType recordInfo;
    int recordLength;
    int currentRecordInfoLength;
    std::map<std::string, int> indexes;
    std::vector<std::string> names;
    std::vector<int> types;
    std::vector<int> offsets;

public:
    DBDataFileDescriptionSlot(BufType cache, bool parse = false);

    void write(int fdp, int fus, int lus);

    int addField(std::string name, int type);

    int getFieldCount();

    int getFirstDataPage();

    int getFirstUsageSlot();

    int getLastUsageSlot();

    int getRecordInfoLength();

    void setFirstDataPage(int n);

    void setFirstUsageSlot(int n);

    void setLastUsageSlot(int n);

    void setRecordInfoLength(int n);

    int getRecordLength();

    int getOffsetOfField(std::string name);

    int getTypeOfField(std::string name);

    void printFileDescription();

    const static int FIRST_DATA_PAGE_OFFSET = 0;
    const static int FIRST_USAGE_SLOT_OFFSET = FIRST_DATA_PAGE_OFFSET + sizeof(int);
    const static int LAST_USAGE_SLOT_OFFSET = FIRST_USAGE_SLOT_OFFSET + sizeof(int);
    const static int RECORD_INFO_LENGTH_OFFSET = LAST_USAGE_SLOT_OFFSET + sizeof(int);
    const static int RECORD_INFO_OFFSET = RECORD_INFO_LENGTH_OFFSET + sizeof(int);
};

class DBUsageSlot:public DBSlot
{
protected:

};

class DBPageInfoSlot: public DBSlot
{
protected:
    BufType pageType;           // int
    BufType firstAvailableByte; // int
    BufType lengthFixed;        // bool
    BufType nextSamePage;       // int
    //TODO: usage byte

public:

    DBPageInfoSlot(BufType cache, bool parse = false);

    int size();

    int getPageType();

    int getFirstAvailableByte();

    bool isLengthFixed();

    int getNextSamePage();

    void write();

    const static int PAGE_TYPE_OFFSET = 0;
    const static int FIRST_AVAILABLE_BYTE_OFFSET = PAGE_TYPE_OFFSET + sizeof(int);
    const static int LENGTH_FIXED_OFFSET = FIRST_AVAILABLE_BYTE_OFFSET + sizeof(int);
    const static int NEXT_SAME_PAGE_OFFSET = LENGTH_FIXED_OFFSET + sizeof(bool);
};

class DBRecordSlot: public DBSlot
{
public:
    DBRecordSlot(BufType cache, int slotLength);
};

#endif // DBSLOT_H_INCLUDED
