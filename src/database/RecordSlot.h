#ifndef RECORDSLOT_H_INCLUDED
#define RECORDSLOT_H_INCLUDED

#include"Include.h"

class RecordSlot
{
protected:
    char* cache;
    RecordInfo* ri;

public:
    RecordSlot(char* cache, RecordInfo* ri);

    bool checkNull(std::map<int, bool>& nulls);

    void write(std::vector<void*>& data);

    void read(std::vector<void*>& data);

    bool check(SearchInfo& si);

    bool checkValueNotEqual(std::map<int, std::vector<void*> >& info);

    bool checkValue(std::map<int, void*>& info, int op);

    bool checkFields(std::map<int, std::vector<int> >& info, int op);

    void update(UpdateInfo& ui);

    static void copy(RecordSlot* src, RecordSlot* dest, int length);

    void print();

    char* operator[](const int offset) const;
};

#endif // RECORDSLOT_H_INCLUDED
