#ifndef DBRECORDSLOT_H_INCLUDED
#define DBRECORDSLOT_H_INCLUDED

#include"DBSlot.h"

class DBRecordSlot: public DBSlot
{
protected:
    DBRecordInfo* ri;

public:
    DBRecordSlot(BufType cache, DBRecordInfo* ri);

    void checkNull(std::map<int, bool>& nulls, std::vector<std::vector<void*> >& datas);

    void write(std::vector<void*>& data);

    void read(std::vector<void*>& data);

    void read(std::map<std::string, void*>& data);

    bool equal(std::map<int, void*>& data);

    void update(std::map<int, void*>& data);

    bool compare_id(char* _id);

    void get_id(char* _id);

    static void copy(DBRecordSlot* src, DBRecordSlot* dest, int length);

    void print();
};

#endif // DBRECORDSLOT_H_INCLUDED
