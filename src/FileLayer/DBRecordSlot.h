#ifndef DBRECORDSLOT_H_INCLUDED
#define DBRECORDSLOT_H_INCLUDED

#include"DBSlot.h"

class DBRecordSlot: public DBSlot
{
protected:
    DBRecordInfo* ri;

public:
    DBRecordSlot(BufType cache, DBRecordInfo* ri);

    int write(std::vector<void*>& data);

    int read(std::vector<void*>& data);

    int read(std::map<std::string, void*>& data);

    int equal(std::map<int, void*>& data);

    int update(std::map<int, void*>& data);

    int compare_id(char* _id);

    int get_id(char* _id);

    static void copy(DBRecordSlot* src, DBRecordSlot* dest, int length);

    void print();
};

#endif // DBRECORDSLOT_H_INCLUDED
