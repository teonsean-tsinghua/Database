#ifndef DBDATAFILEDESCRIPTIONSLOT_H_INCLUDED
#define DBDATAFILEDESCRIPTIONSLOT_H_INCLUDED

#include"DBSlot.h"

/*
 * If modified, you should also rewrite constructor, write(), print(), size(), and add methods as needed.
 */

class DBDataFileDescriptionSlot: public DBSlot
{
protected:
    BufType firstDataPage;     // int
    BufType firstUsageSlot;    // int; pageID * PAGE_SIZE + offset
    BufType lastUsageSlot;     // int
    BufType primaryKeyIndex;   // int
    BufType recordInfoLength;  // int
    BufType recordInfo;
    int recordLength;
    int currentRecordInfoLength;
    std::map<std::string, int> indexes;
    std::vector<std::string> names;
    std::vector<int> nullables;
    std::vector<int> types;
    std::vector<int> offsets;

public:
    DBDataFileDescriptionSlot(BufType cache, int mode);

    void write();

    int addField(std::string name, int type, int nullable, char* boundary);

    int getFieldCount();

    int getFirstDataPage();

    int getFirstUsageSlot();

    int getLastUsageSlot();

    int getRecordInfoLength();

    int getPrimaryKeyIndex();

    void setFirstDataPage(int n);

    void setFirstUsageSlot(int n);

    void setLastUsageSlot(int n);

    void setRecordInfoLength(int n);

    void setPrimaryKeyIndex(int n);

    int getRecordLength();

    int getOffsetOfField(int index);

    int getTypeOfField(int index);

    std::string getNameOfField(int index);

    int getIndexOfField(std::string name);

    int getNullableOfField(int index);

    void print();

    int size();

    const static int FIRST_DATA_PAGE_OFFSET = 0;
    const static int FIRST_USAGE_SLOT_OFFSET = FIRST_DATA_PAGE_OFFSET + sizeof(int);
    const static int LAST_USAGE_SLOT_OFFSET = FIRST_USAGE_SLOT_OFFSET + sizeof(int);
    const static int PRIMARY_KEY_INDEX_OFFSET = LAST_USAGE_SLOT_OFFSET + sizeof(int);
    const static int RECORD_INFO_LENGTH_OFFSET = PRIMARY_KEY_INDEX_OFFSET + sizeof(int);
    const static int RECORD_INFO_OFFSET = RECORD_INFO_LENGTH_OFFSET + sizeof(int);

    const static int RECORD_INFO_TYPE_OFFSET = 0;
    const static int RECORD_INFO_NULLABLE_OFFSET = RECORD_INFO_TYPE_OFFSET + sizeof(int);
    const static int RECORD_INFO_NAME_LENGTH_OFFSET = RECORD_INFO_NULLABLE_OFFSET + sizeof(int);
    const static int RECORD_INFO_NAME_OFFSET = RECORD_INFO_NAME_LENGTH_OFFSET + sizeof(int);
};

#endif // DBDATAFILEDESCRIPTIONSLOT_H_INCLUDED
