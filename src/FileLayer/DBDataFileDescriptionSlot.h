#ifndef DBDATAFILEDESCRIPTIONSLOT_H_INCLUDED
#define DBDATAFILEDESCRIPTIONSLOT_H_INCLUDED

#include"DBSlot.h"

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
    DBDataFileDescriptionSlot(BufType cache, int parse = false);

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

    void print();

    int size();

    const static int FIRST_DATA_PAGE_OFFSET = 0;
    const static int FIRST_USAGE_SLOT_OFFSET = FIRST_DATA_PAGE_OFFSET + sizeof(int);
    const static int LAST_USAGE_SLOT_OFFSET = FIRST_USAGE_SLOT_OFFSET + sizeof(int);
    const static int RECORD_INFO_LENGTH_OFFSET = LAST_USAGE_SLOT_OFFSET + sizeof(int);
    const static int RECORD_INFO_OFFSET = RECORD_INFO_LENGTH_OFFSET + sizeof(int);
};

#endif // DBDATAFILEDESCRIPTIONSLOT_H_INCLUDED
