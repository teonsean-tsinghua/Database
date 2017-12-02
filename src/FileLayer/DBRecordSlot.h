#ifndef DBRECORDSLOT_H_INCLUDED
#define DBRECORDSLOT_H_INCLUDED

#include"DBSlot.h"

class DBRecordSlot: public DBSlot
{
protected:
    DBRecordInfo* ri;

public:
    DBRecordSlot(BufType cache, DBRecordInfo* ri);

    bool checkNull(std::map<int, bool>& nulls);

    void write(std::vector<void*>& data);

    void read(std::vector<void*>& data);

    void read(std::map<std::string, void*>& data);

    bool checkValue(std::map<int, void*>& info, int op);

    bool checkFields(std::map<int, int>& info, int op);

    void update(std::map<int, void*>& data);

    bool compare_id(char* _id);

    void get_id(char* _id);

    static void copy(DBRecordSlot* src, DBRecordSlot* dest, int length);

    void print();
};

#endif // DBRECORDSLOT_H_INCLUDED
